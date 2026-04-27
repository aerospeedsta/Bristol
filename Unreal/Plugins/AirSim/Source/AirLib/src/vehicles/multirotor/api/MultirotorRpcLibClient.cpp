
#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include "common/Common.hpp"
#include "common/ClockFactory.hpp"
#include "common/ZenohRpc.hpp"
#include <thread>

STRICT_MODE_OFF
#include "vehicles/multirotor/api/MultirotorRpcLibAdaptors.hpp"
STRICT_MODE_ON

namespace msr {
namespace airlib {

    typedef msr::airlib_rpclib::MultirotorRpcLibAdaptors MultirotorRpcLibAdaptors;

    MultirotorRpcLibClient::MultirotorRpcLibClient(const string& ip_address, uint16_t port, float timeout_sec)
        : RpcLibClientBase(ip_address, port, timeout_sec) {}

    void MultirotorRpcLibClient::setAngleLevelControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name) {
        getZenohRpc().call("setAngleLevelControllerGains", kp, ki, kd, vehicle_name);
    }
    void MultirotorRpcLibClient::setAngleRateControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name) {
        getZenohRpc().call("setAngleRateControllerGains", kp, ki, kd, vehicle_name);
    }
    void MultirotorRpcLibClient::setVelocityControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name) {
        getZenohRpc().call("setVelocityControllerGains", kp, ki, kd, vehicle_name);
    }
    void MultirotorRpcLibClient::setPositionControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name) {
        getZenohRpc().call("setPositionControllerGains", kp, ki, kd, vehicle_name);
    }
    void MultirotorRpcLibClient::moveByRC(const RCData& rc_data, const std::string& vehicle_name) {
        getZenohRpc().call("moveByRC", MultirotorRpcLibAdaptors::RCData(rc_data), vehicle_name);
    }
    bool MultirotorRpcLibClient::setSafety(SafetyEval::SafetyViolationType enable_reasons, float obs_clearance, SafetyEval::ObsAvoidanceStrategy obs_startegy,
                       float obs_avoidance_vel, const Vector3r& origin, float xy_length, float max_z, float min_z, const std::string& vehicle_name) {
        return getZenohRpc().callResult<bool>("setSafety", static_cast<uint>(enable_reasons), obs_clearance, obs_startegy, obs_avoidance_vel, MultirotorRpcLibAdaptors::Vector3r(origin), xy_length, max_z, min_z, vehicle_name);
    }

    MultirotorState MultirotorRpcLibClient::getMultirotorState(const std::string& vehicle_name) {
        return getZenohRpc().callResult<MultirotorRpcLibAdaptors::MultirotorState>("getMultirotorState", vehicle_name).to();
    }
    RotorStates MultirotorRpcLibClient::getRotorStates(const std::string& vehicle_name) {
        return getZenohRpc().callResult<MultirotorRpcLibAdaptors::RotorStates>("getRotorStates", vehicle_name).to();
    }

    MultirotorRpcLibClient* MultirotorRpcLibClient::waitOnLastTask(bool* task_result, float timeout_sec) {
        unused(timeout_sec); unused(task_result);
        return this;
    }

    MultirotorRpcLibClient* MultirotorRpcLibClient::takeoffAsync(float timeout_sec, const std::string& vehicle_name) {
        getZenohRpc().call("takeoff", timeout_sec, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::landAsync(float timeout_sec, const std::string& vehicle_name) {
        getZenohRpc().call("land", timeout_sec, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::goHomeAsync(float timeout_sec, const std::string& vehicle_name) {
        getZenohRpc().call("goHome", timeout_sec, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByVelocityBodyFrameAsync(float vx, float vy, float vz, float duration, DrivetrainType drivetrain, const YawMode& yaw_mode, const std::string& vehicle_name) {
        getZenohRpc().call("moveByVelocityBodyFrame", vx, vy, vz, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByVelocityZBodyFrameAsync(float vx, float vy, float z, float duration, DrivetrainType drivetrain, const YawMode& yaw_mode, const std::string& vehicle_name) {
        getZenohRpc().call("moveByVelocityZBodyFrame", vx, vy, z, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByMotorPWMsAsync(float front_right_pwm, float rear_left_pwm, float front_left_pwm, float rear_right_pwm, float duration, const std::string& vehicle_name) {
        getZenohRpc().call("moveByMotorPWMs", front_right_pwm, rear_left_pwm, front_left_pwm, rear_right_pwm, duration, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByRollPitchYawZAsync(float roll, float pitch, float yaw, float z, float duration, const std::string& vehicle_name) {
        getZenohRpc().call("moveByRollPitchYawZ", roll, pitch, yaw, z, duration, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByRollPitchYawThrottleAsync(float roll, float pitch, float yaw, float throttle, float duration, const std::string& vehicle_name) {
        getZenohRpc().call("moveByRollPitchYawThrottle", roll, pitch, yaw, throttle, duration, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByRollPitchYawrateThrottleAsync(float roll, float pitch, float yaw_rate, float throttle, float duration, const std::string& vehicle_name) {
        getZenohRpc().call("moveByRollPitchYawrateThrottle", roll, pitch, yaw_rate, throttle, duration, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByRollPitchYawrateZAsync(float roll, float pitch, float yaw_rate, float z, float duration, const std::string& vehicle_name) {
        getZenohRpc().call("moveByRollPitchYawrateZ", roll, pitch, yaw_rate, z, duration, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByAngleRatesZAsync(float roll_rate, float pitch_rate, float yaw_rate, float z, float duration, const std::string& vehicle_name) {
        getZenohRpc().call("moveByAngleRatesZ", roll_rate, pitch_rate, yaw_rate, z, duration, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByAngleRatesThrottleAsync(float roll_rate, float pitch_rate, float yaw_rate, float throttle, float duration, const std::string& vehicle_name) {
        getZenohRpc().call("moveByAngleRatesThrottle", roll_rate, pitch_rate, yaw_rate, throttle, duration, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByVelocityAsync(float vx, float vy, float vz, float duration, DrivetrainType drivetrain, const YawMode& yaw_mode, const std::string& vehicle_name) {
        getZenohRpc().call("moveByVelocity", vx, vy, vz, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByVelocityZAsync(float vx, float vy, float z, float duration, DrivetrainType drivetrain, const YawMode& yaw_mode, const std::string& vehicle_name) {
        getZenohRpc().call("moveByVelocityZ", vx, vy, z, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveOnPathAsync(const vector<Vector3r>& path, float velocity, float timeout_sec, DrivetrainType drivetrain, const YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name) {
        std::vector<MultirotorRpcLibAdaptors::Vector3r> conv_path;
        MultirotorRpcLibAdaptors::from(path, conv_path);
        getZenohRpc().call("moveOnPath", conv_path, velocity, timeout_sec, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), lookahead, adaptive_lookahead, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveToGPSAsync(float latitude, float longitude, float altitude, float velocity, float timeout_sec, DrivetrainType drivetrain, const YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name) {
        getZenohRpc().call("moveToGPS", latitude, longitude, altitude, velocity, timeout_sec, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), lookahead, adaptive_lookahead, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveToPositionAsync(float x, float y, float z, float velocity, float timeout_sec, DrivetrainType drivetrain, const YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name) {
        getZenohRpc().call("moveToPosition", x, y, z, velocity, timeout_sec, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), lookahead, adaptive_lookahead, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveToZAsync(float z, float velocity, float timeout_sec, const YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name) {
        getZenohRpc().call("moveToZ", z, velocity, timeout_sec, MultirotorRpcLibAdaptors::YawMode(yaw_mode), lookahead, adaptive_lookahead, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::moveByManualAsync(float vx_max, float vy_max, float z_min, float duration, DrivetrainType drivetrain, const YawMode& yaw_mode, const std::string& vehicle_name) {
        getZenohRpc().call("moveByManual", vx_max, vy_max, z_min, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::rotateToYawAsync(float yaw, float timeout_sec, float margin, const std::string& vehicle_name) {
        getZenohRpc().call("rotateToYaw", yaw, timeout_sec, margin, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::rotateByYawRateAsync(float yaw_rate, float duration, const std::string& vehicle_name) {
        getZenohRpc().call("rotateByYawRate", yaw_rate, duration, vehicle_name);
        return this;
    }
    MultirotorRpcLibClient* MultirotorRpcLibClient::hoverAsync(const std::string& vehicle_name) {
        getZenohRpc().call("hover", vehicle_name);
        return this;
    }
}
} //namespace

