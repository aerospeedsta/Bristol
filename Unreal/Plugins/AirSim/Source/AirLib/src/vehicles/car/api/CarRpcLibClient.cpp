
#include "vehicles/car/api/CarRpcLibClient.hpp"
#include "common/Common.hpp"
#include "common/ZenohRpc.hpp"
#include <thread>

STRICT_MODE_OFF
#include "vehicles/car/api/CarRpcLibAdaptors.hpp"
STRICT_MODE_ON

namespace msr {
namespace airlib {

    typedef msr::airlib_rpclib::CarRpcLibAdaptors CarRpcLibAdaptors;

    CarRpcLibClient::CarRpcLibClient(const string& ip_address, uint16_t port, float timeout_sec)
        : RpcLibClientBase(ip_address, port, timeout_sec) {}
    CarRpcLibClient::~CarRpcLibClient() {}

    void CarRpcLibClient::setCarControls(const CarApiBase::CarControls& controls, const std::string& vehicle_name) {
        getZenohRpc().call("setCarControls", CarRpcLibAdaptors::CarControls(controls), vehicle_name);
    }

    CarApiBase::CarState CarRpcLibClient::getCarState(const std::string& vehicle_name) {
        return getZenohRpc().callResult<CarRpcLibAdaptors::CarState>("getCarState", vehicle_name).to();
    }

    CarApiBase::CarControls CarRpcLibClient::getCarControls(const std::string& vehicle_name) {
        return getZenohRpc().callResult<CarRpcLibAdaptors::CarControls>("getCarControls", vehicle_name).to();
    }
}
} //namespace

