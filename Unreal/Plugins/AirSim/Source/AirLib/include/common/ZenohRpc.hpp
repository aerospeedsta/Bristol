#pragma once

#include "common/Common.hpp"
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <variant>

STRICT_MODE_OFF
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmacro-redefined"
#pragma clang diagnostic ignored "-Wdeprecated-literal-operator"
#undef ALIGN
#undef check
#undef FLOAT
#undef NOUSER
#include <zenoh.hxx>
#include <rpc/msgpack.hpp>
#pragma clang diagnostic pop
#ifndef check
#define check(expr) (static_cast<void>((expr)))
#endif
STRICT_MODE_ON

namespace msr {
namespace airlib {

class ZenohRpc {
public:
    static constexpr const char* KEY_PREFIX = "airsim/rpc/";

    ZenohRpc() {
        zenoh::Config config = zenoh::Config::create_default();
        session_ = std::make_unique<zenoh::Session>(std::move(config));
    }

    ~ZenohRpc() = default;

    template<typename R, typename... Args>
    void bind(const std::string& name, std::function<R(Args...)> handler) {
        zenoh::KeyExpr ke(KEY_PREFIX + name);
        queryables_.push_back(
            session_->declare_queryable(
                std::move(ke),
                [handler = std::move(handler)](zenoh::Query& query) mutable {
                    auto msg = unpackPayload(query);
                    using DecayedArgs = std::tuple<std::decay_t<Args>...>;
                    auto tuple = msg.get().as<DecayedArgs>();
                    auto invoke = [&handler](auto&&... vals) {
                        return handler(std::forward<decltype(vals)>(vals)...);
                    };
                    if constexpr (std::is_same_v<R, void>) {
                        std::apply(invoke, std::move(tuple));
                        query.reply(query.get_keyexpr(), zenoh::Bytes(""));
                    } else {
                        R result = std::apply(invoke, std::move(tuple));
                        clmdep_msgpack::sbuffer buf;
                        clmdep_msgpack::pack(buf, result);
                        auto data = std::vector<uint8_t>(buf.data(), buf.data() + buf.size());
                        query.reply(query.get_keyexpr(), zenoh::Bytes(std::move(data)));
                    }
                },
                []() {}
            )
        );
    }

    void call(const std::string& name) {
        zenoh::KeyExpr ke(KEY_PREFIX + name);
        zenoh::Session::GetOptions opts{};
        auto replies = session_->get(std::move(ke), "", zenoh::channels::FifoChannel(256), std::move(opts));
        while (true) {
            auto res = replies.recv();
            if (auto reply = std::get_if<zenoh::Reply>(&res)) {
                if (reply->is_ok()) break;
            } else break;
        }
    }

    template<typename R>
    R callResult(const std::string& name) {
        zenoh::KeyExpr ke(KEY_PREFIX + name);
        zenoh::Session::GetOptions opts{};
        auto replies = session_->get(std::move(ke), "", zenoh::channels::FifoChannel(256), std::move(opts));
        while (true) {
            auto res = replies.recv();
            if (auto reply = std::get_if<zenoh::Reply>(&res)) {
                if (reply->is_ok()) {
                    auto& sample = reply->get_ok();
                    const auto& payload = sample.get_payload();
                    return unpackSingle<R>(payload.as_vector());
                }
            } else break;
        }
        return R{};
    }

    template<typename... Args>
    void call(const std::string& name, const Args&... args) {
        auto data = packTuple(args...);
        zenoh::KeyExpr ke(KEY_PREFIX + name);
        zenoh::Session::GetOptions opts{};
        zenoh::Bytes b(std::move(data));
        opts.payload = std::move(b);
        auto replies = session_->get(std::move(ke), "", zenoh::channels::FifoChannel(256), std::move(opts));
        while (true) {
            auto res = replies.recv();
            if (auto reply = std::get_if<zenoh::Reply>(&res)) {
                if (reply->is_ok()) break;
            } else break;
        }
    }

    template<typename R, typename... Args>
    R callResult(const std::string& name, const Args&... args) {
        auto data = packTuple(args...);
        zenoh::KeyExpr ke(KEY_PREFIX + name);
        zenoh::Session::GetOptions opts{};
        zenoh::Bytes b(std::move(data));
        opts.payload = std::move(b);
        auto replies = session_->get(std::move(ke), "", zenoh::channels::FifoChannel(256), std::move(opts));
        while (true) {
            auto res = replies.recv();
            if (auto reply = std::get_if<zenoh::Reply>(&res)) {
                if (reply->is_ok()) {
                    auto& sample = reply->get_ok();
                    const auto& payload = sample.get_payload();
                    return unpackSingle<R>(payload.as_vector());
                }
            } else break;
        }
        return R{};
    }

    bool isConnected() const { return true; }

private:
    static clmdep_msgpack::unpacked unpackPayload(zenoh::Query& query) {
        auto payload_opt = query.get_payload();
        if (!payload_opt.has_value()) {
            clmdep_msgpack::unpacked msg;
            return msg;
        }
        auto vec = payload_opt->get().as_vector();
        clmdep_msgpack::unpacked msg;
        clmdep_msgpack::unpack(msg, reinterpret_cast<const char*>(vec.data()), vec.size());
        return msg;
    }

    template<typename T>
    static T unpackSingle(const std::vector<uint8_t>& data) {
        clmdep_msgpack::unpacked msg;
        clmdep_msgpack::unpack(msg, reinterpret_cast<const char*>(data.data()), data.size());
        return msg.get().as<T>();
    }

    template<typename... Args>
    static std::vector<uint8_t> packTuple(const Args&... args) {
        auto t = std::make_tuple(args...);
        clmdep_msgpack::sbuffer buf;
        clmdep_msgpack::pack(buf, t);
        return std::vector<uint8_t>(buf.data(), buf.data() + buf.size());
    }

    std::unique_ptr<zenoh::Session> session_;
    std::vector<zenoh::Queryable<void>> queryables_;
};

}} // namespaces
