
#include "vehicles/multirotor/api/MultirotorRpcLibServer.hpp"
#include "common/Common.hpp"
#include "common/ZenohRpc.hpp"
#include <functional>

STRICT_MODE_OFF
#include "vehicles/multirotor/api/MultirotorRpcLibAdaptors.hpp"
STRICT_MODE_ON

namespace msr {
namespace airlib {

    typedef msr::airlib_rpclib::MultirotorRpcLibAdaptors MultirotorRpcLibAdaptors;

    MultirotorRpcLibServer::MultirotorRpcLibServer(ApiProvider* api_provider, string server_address, uint16_t port)
        : RpcLibServerBase(api_provider, server_address, port)
    {
        auto self = this;
        auto& rpc = getZenohRpc();

        rpc.bind("takeoff", std::function<bool(float, const std::string&)>(
            [self](float timeout_sec, const std::string& vehicle_name) -> bool { return self->getVehicleApi(vehicle_name)->takeoff(timeout_sec); }));
        rpc.bind("land", std::function<bool(float, const std::string&)>(
            [self](float timeout_sec, const std::string& vehicle_name) -> bool { return self->getVehicleApi(vehicle_name)->land(timeout_sec); }));
        rpc.bind("goHome", std::function<bool(float, const std::string&)>(
            [self](float timeout_sec, const std::string& vehicle_name) -> bool { return self->getVehicleApi(vehicle_name)->goHome(timeout_sec); }));

        rpc.bind("moveByVelocityBodyFrame", std::function<bool(float, float, float, float, DrivetrainType, const MultirotorRpcLibAdaptors::YawMode&, const std::string&)>(
            [self](float vx, float vy, float vz, float duration, DrivetrainType drivetrain, const MultirotorRpcLibAdaptors::YawMode& yaw_mode, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByVelocityBodyFrame(vx, vy, vz, duration, drivetrain, yaw_mode.to());
            }));
        rpc.bind("moveByVelocityZBodyFrame", std::function<bool(float, float, float, float, DrivetrainType, const MultirotorRpcLibAdaptors::YawMode&, const std::string&)>(
            [self](float vx, float vy, float z, float duration, DrivetrainType drivetrain, const MultirotorRpcLibAdaptors::YawMode& yaw_mode, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByVelocityZBodyFrame(vx, vy, z, duration, drivetrain, yaw_mode.to());
            }));
        rpc.bind("moveByMotorPWMs", std::function<bool(float, float, float, float, float, const std::string&)>(
            [self](float front_right_pwm, float rear_left_pwm, float front_left_pwm, float rear_right_pwm, float duration, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByMotorPWMs(front_right_pwm, rear_left_pwm, front_left_pwm, rear_right_pwm, duration);
            }));
        rpc.bind("moveByRollPitchYawZ", std::function<bool(float, float, float, float, float, const std::string&)>(
            [self](float roll, float pitch, float yaw, float z, float duration, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByRollPitchYawZ(roll, pitch, yaw, z, duration);
            }));
        rpc.bind("moveByRollPitchYawThrottle", std::function<bool(float, float, float, float, float, const std::string&)>(
            [self](float roll, float pitch, float yaw, float throttle, float duration, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByRollPitchYawThrottle(roll, pitch, yaw, throttle, duration);
            }));
        rpc.bind("moveByRollPitchYawrateThrottle", std::function<bool(float, float, float, float, float, const std::string&)>(
            [self](float roll, float pitch, float yaw_rate, float throttle, float duration, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByRollPitchYawrateThrottle(roll, pitch, yaw_rate, throttle, duration);
            }));
        rpc.bind("moveByRollPitchYawrateZ", std::function<bool(float, float, float, float, float, const std::string&)>(
            [self](float roll, float pitch, float yaw_rate, float z, float duration, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByRollPitchYawrateZ(roll, pitch, yaw_rate, z, duration);
            }));
        rpc.bind("moveByAngleRatesZ", std::function<bool(float, float, float, float, float, const std::string&)>(
            [self](float roll_rate, float pitch_rate, float yaw_rate, float z, float duration, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByAngleRatesZ(roll_rate, pitch_rate, yaw_rate, z, duration);
            }));
        rpc.bind("moveByAngleRatesThrottle", std::function<bool(float, float, float, float, float, const std::string&)>(
            [self](float roll_rate, float pitch_rate, float yaw_rate, float throttle, float duration, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByAngleRatesThrottle(roll_rate, pitch_rate, yaw_rate, throttle, duration);
            }));
        rpc.bind("moveByVelocity", std::function<bool(float, float, float, float, DrivetrainType, const MultirotorRpcLibAdaptors::YawMode&, const std::string&)>(
            [self](float vx, float vy, float vz, float duration, DrivetrainType drivetrain, const MultirotorRpcLibAdaptors::YawMode& yaw_mode, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByVelocity(vx, vy, vz, duration, drivetrain, yaw_mode.to());
            }));
        rpc.bind("moveByVelocityZ", std::function<bool(float, float, float, float, DrivetrainType, const MultirotorRpcLibAdaptors::YawMode&, const std::string&)>(
            [self](float vx, float vy, float z, float duration, DrivetrainType drivetrain, const MultirotorRpcLibAdaptors::YawMode& yaw_mode, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByVelocityZ(vx, vy, z, duration, drivetrain, yaw_mode.to());
            }));

        rpc.bind("moveOnPath", std::function<bool(const std::vector<MultirotorRpcLibAdaptors::Vector3r>&, float, float, DrivetrainType, const MultirotorRpcLibAdaptors::YawMode&, float, float, const std::string&)>(
            [self](const std::vector<MultirotorRpcLibAdaptors::Vector3r>& path, float velocity, float timeout_sec, DrivetrainType drivetrain, const MultirotorRpcLibAdaptors::YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name) -> bool {
                std::vector<Vector3r> conv_path; MultirotorRpcLibAdaptors::to(path, conv_path);
                return self->getVehicleApi(vehicle_name)->moveOnPath(conv_path, velocity, timeout_sec, drivetrain, yaw_mode.to(), lookahead, adaptive_lookahead);
            }));
        rpc.bind("moveToGPS", std::function<bool(float, float, float, float, float, DrivetrainType, const MultirotorRpcLibAdaptors::YawMode&, float, float, const std::string&)>(
            [self](float latitude, float longitude, float altitude, float velocity, float timeout_sec, DrivetrainType drivetrain, const MultirotorRpcLibAdaptors::YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveToGPS(latitude, longitude, altitude, velocity, timeout_sec, drivetrain, yaw_mode.to(), lookahead, adaptive_lookahead);
            }));
        rpc.bind("moveToPosition", std::function<bool(float, float, float, float, float, DrivetrainType, const MultirotorRpcLibAdaptors::YawMode&, float, float, const std::string&)>(
            [self](float x, float y, float z, float velocity, float timeout_sec, DrivetrainType drivetrain, const MultirotorRpcLibAdaptors::YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveToPosition(x, y, z, velocity, timeout_sec, drivetrain, yaw_mode.to(), lookahead, adaptive_lookahead);
            }));
        rpc.bind("moveToZ", std::function<bool(float, float, float, const MultirotorRpcLibAdaptors::YawMode&, float, float, const std::string&)>(
            [self](float z, float velocity, float timeout_sec, const MultirotorRpcLibAdaptors::YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveToZ(z, velocity, timeout_sec, yaw_mode.to(), lookahead, adaptive_lookahead);
            }));
        rpc.bind("moveByManual", std::function<bool(float, float, float, float, DrivetrainType, const MultirotorRpcLibAdaptors::YawMode&, const std::string&)>(
            [self](float vx_max, float vy_max, float z_min, float duration, DrivetrainType drivetrain, const MultirotorRpcLibAdaptors::YawMode& yaw_mode, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->moveByManual(vx_max, vy_max, z_min, duration, drivetrain, yaw_mode.to());
            }));

        rpc.bind("rotateToYaw", std::function<bool(float, float, float, const std::string&)>(
            [self](float yaw, float timeout_sec, float margin, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->rotateToYaw(yaw, timeout_sec, margin);
            }));
        rpc.bind("rotateByYawRate", std::function<bool(float, float, const std::string&)>(
            [self](float yaw_rate, float duration, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->rotateByYawRate(yaw_rate, duration);
            }));
        rpc.bind("hover", std::function<bool(const std::string&)>(
            [self](const std::string& vehicle_name) -> bool { return self->getVehicleApi(vehicle_name)->hover(); }));

        rpc.bind("setAngleLevelControllerGains", std::function<void(const std::vector<float>&, const std::vector<float>&, const std::vector<float>&, const std::string&)>(
            [self](const std::vector<float>& kp, const std::vector<float>& ki, const std::vector<float>& kd, const std::string& vehicle_name) {
                self->getVehicleApi(vehicle_name)->setAngleLevelControllerGains(kp, ki, kd);
            }));
        rpc.bind("setAngleRateControllerGains", std::function<void(const std::vector<float>&, const std::vector<float>&, const std::vector<float>&, const std::string&)>(
            [self](const std::vector<float>& kp, const std::vector<float>& ki, const std::vector<float>& kd, const std::string& vehicle_name) {
                self->getVehicleApi(vehicle_name)->setAngleRateControllerGains(kp, ki, kd);
            }));
        rpc.bind("setVelocityControllerGains", std::function<void(const std::vector<float>&, const std::vector<float>&, const std::vector<float>&, const std::string&)>(
            [self](const std::vector<float>& kp, const std::vector<float>& ki, const std::vector<float>& kd, const std::string& vehicle_name) {
                self->getVehicleApi(vehicle_name)->setVelocityControllerGains(kp, ki, kd);
            }));
        rpc.bind("setPositionControllerGains", std::function<void(const std::vector<float>&, const std::vector<float>&, const std::vector<float>&, const std::string&)>(
            [self](const std::vector<float>& kp, const std::vector<float>& ki, const std::vector<float>& kd, const std::string& vehicle_name) {
                self->getVehicleApi(vehicle_name)->setPositionControllerGains(kp, ki, kd);
            }));
        rpc.bind("moveByRC", std::function<void(const MultirotorRpcLibAdaptors::RCData&, const std::string&)>(
            [self](const MultirotorRpcLibAdaptors::RCData& data, const std::string& vehicle_name) {
                self->getVehicleApi(vehicle_name)->moveByRC(data.to());
            }));
        rpc.bind("setSafety", std::function<bool(uint, float, const SafetyEval::ObsAvoidanceStrategy&, float, const MultirotorRpcLibAdaptors::Vector3r&, float, float, float, const std::string&)>(
            [self](uint enable_reasons, float obs_clearance, const SafetyEval::ObsAvoidanceStrategy& obs_startegy, float obs_avoidance_vel, const MultirotorRpcLibAdaptors::Vector3r& origin, float xy_length, float max_z, float min_z, const std::string& vehicle_name) -> bool {
                return self->getVehicleApi(vehicle_name)->setSafety(SafetyEval::SafetyViolationType(enable_reasons), obs_clearance, obs_startegy, obs_avoidance_vel, origin.to(), xy_length, max_z, min_z);
            }));

        rpc.bind("getRotorStates", std::function<MultirotorRpcLibAdaptors::RotorStates(const std::string&)>(
            [self](const std::string& vehicle_name) -> MultirotorRpcLibAdaptors::RotorStates {
                return MultirotorRpcLibAdaptors::RotorStates(self->getVehicleApi(vehicle_name)->getRotorStates());
            }));
        rpc.bind("getMultirotorState", std::function<MultirotorRpcLibAdaptors::MultirotorState(const std::string&)>(
            [self](const std::string& vehicle_name) -> MultirotorRpcLibAdaptors::MultirotorState {
                return MultirotorRpcLibAdaptors::MultirotorState(self->getVehicleApi(vehicle_name)->getMultirotorState());
            }));
    }

    MultirotorRpcLibServer::~MultirotorRpcLibServer() {}
}
} //namespace

