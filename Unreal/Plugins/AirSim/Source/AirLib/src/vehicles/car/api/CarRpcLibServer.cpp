
#include "vehicles/car/api/CarRpcLibServer.hpp"
#include "common/Common.hpp"
#include "common/ZenohRpc.hpp"
#include <functional>

STRICT_MODE_OFF
#include "vehicles/car/api/CarRpcLibAdaptors.hpp"
STRICT_MODE_ON

namespace msr {
namespace airlib {

    typedef msr::airlib_rpclib::CarRpcLibAdaptors CarRpcLibAdaptors;

    CarRpcLibServer::CarRpcLibServer(ApiProvider* api_provider, string server_address, uint16_t port)
        : RpcLibServerBase(api_provider, server_address, port)
    {
        auto self = this;
        auto& rpc = getZenohRpc();

        rpc.bind("getCarState", std::function<CarRpcLibAdaptors::CarState(const std::string&)>(
            [self](const std::string& vehicle_name) -> CarRpcLibAdaptors::CarState {
                return CarRpcLibAdaptors::CarState(self->getVehicleApi(vehicle_name)->getCarState());
            }));
        rpc.bind("setCarControls", std::function<void(const CarRpcLibAdaptors::CarControls&, const std::string&)>(
            [self](const CarRpcLibAdaptors::CarControls& controls, const std::string& vehicle_name) {
                self->getVehicleApi(vehicle_name)->setCarControls(controls.to());
            }));
        rpc.bind("getCarControls", std::function<CarRpcLibAdaptors::CarControls(const std::string&)>(
            [self](const std::string& vehicle_name) -> CarRpcLibAdaptors::CarControls {
                return CarRpcLibAdaptors::CarControls(self->getVehicleApi(vehicle_name)->getCarControls());
            }));
    }

    CarRpcLibServer::~CarRpcLibServer() {}
}
} //namespace

