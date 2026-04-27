
#include "api/RpcLibClientBase.hpp"
#include "common/Common.hpp"
#include "common/ClockFactory.hpp"
#include "common/ZenohRpc.hpp"
#include <functional>
#include <vector>
#include <thread>
#include <iostream>

STRICT_MODE_OFF
#include "api/RpcLibAdaptorsBase.hpp"
STRICT_MODE_ON

namespace msr {
namespace airlib {

    struct RpcLibClientBase::impl {
        impl() = default;

        ZenohRpc rpc;
    };

    typedef msr::airlib_rpclib::RpcLibAdaptorsBase RpcLibAdaptorsBase;

    RpcLibClientBase::RpcLibClientBase(const string& ip_address, uint16_t port, float timeout_sec)
    {
        pimpl_.reset(new impl());
    }

    RpcLibClientBase::~RpcLibClientBase() {}

    bool RpcLibClientBase::ping() { return pimpl_->rpc.callResult<bool>("ping"); }

    RpcLibClientBase::ConnectionState RpcLibClientBase::getConnectionState() {
        return ConnectionState::Connected;
    }

    void RpcLibClientBase::enableApiControl(bool is_enabled, const std::string& vehicle_name) {
        pimpl_->rpc.call("enableApiControl", is_enabled, vehicle_name);
    }

    bool RpcLibClientBase::isApiControlEnabled(const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<bool>("isApiControlEnabled", vehicle_name);
    }

    int RpcLibClientBase::getClientVersion() const { return 1; }
    int RpcLibClientBase::getMinRequiredServerVersion() const { return 1; }
    int RpcLibClientBase::getMinRequiredClientVersion() const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<int>("getMinRequiredClientVersion");
    }
    int RpcLibClientBase::getServerVersion() const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<int>("getServerVersion");
    }

    void RpcLibClientBase::reset() { pimpl_->rpc.call("reset"); }

    void RpcLibClientBase::confirmConnection() {
        ClockBase* clock = ClockFactory::get();
        std::cout << "Connected!" << std::endl;
        auto server_ver = getServerVersion();
        auto client_ver = getClientVersion();
        auto server_min_ver = getMinRequiredServerVersion();
        auto client_min_ver = getMinRequiredClientVersion();
        std::string ver_info = Utils::stringf("Client Ver:%i (Min Req:%i), Server Ver:%i (Min Req:%i)",
                                              client_ver, client_min_ver, server_ver, server_min_ver);
        if (server_ver < server_min_ver) {
            std::cerr << std::endl << ver_info << std::endl;
            std::cerr << "AirSim server is of older version and not supported by this client. Please upgrade!" << std::endl;
        } else if (client_ver < client_min_ver) {
            std::cerr << std::endl << ver_info << std::endl;
            std::cerr << "AirSim client is of older version and not supported by this server. Please upgrade!" << std::endl;
        } else
            std::cout << std::endl << ver_info << std::endl;
    }

    bool RpcLibClientBase::armDisarm(bool arm, const std::string& vehicle_name) {
        return pimpl_->rpc.callResult<bool>("armDisarm", arm, vehicle_name);
    }
    msr::airlib::GeoPoint RpcLibClientBase::getHomeGeoPoint(const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::GeoPoint>("getHomeGeoPoint", vehicle_name).to();
    }
    msr::airlib::LidarData RpcLibClientBase::getLidarData(const std::string& lidar_name, const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::LidarData>("getLidarData", lidar_name, vehicle_name).to();
    }
    msr::airlib::ImuBase::Output RpcLibClientBase::getImuData(const std::string& imu_name, const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::ImuData>("getImuData", imu_name, vehicle_name).to();
    }
    msr::airlib::BarometerBase::Output RpcLibClientBase::getBarometerData(const std::string& barometer_name, const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::BarometerData>("getBarometerData", barometer_name, vehicle_name).to();
    }
    msr::airlib::MagnetometerBase::Output RpcLibClientBase::getMagnetometerData(const std::string& magnetometer_name, const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::MagnetometerData>("getMagnetometerData", magnetometer_name, vehicle_name).to();
    }
    msr::airlib::GpsBase::Output RpcLibClientBase::getGpsData(const std::string& gps_name, const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::GpsData>("getGpsData", gps_name, vehicle_name).to();
    }
    msr::airlib::DistanceSensorData RpcLibClientBase::getDistanceSensorData(const std::string& distance_sensor_name, const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::DistanceSensorData>("getDistanceSensorData", distance_sensor_name, vehicle_name).to();
    }

    bool RpcLibClientBase::simSetSegmentationObjectID(const std::string& mesh_name, int object_id, bool is_name_regex) {
        return pimpl_->rpc.callResult<bool>("simSetSegmentationObjectID", mesh_name, object_id, is_name_regex);
    }
    int RpcLibClientBase::simGetSegmentationObjectID(const std::string& mesh_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<int>("simGetSegmentationObjectID", mesh_name);
    }

    void RpcLibClientBase::simAddDetectionFilterMeshName(const std::string& camera_name, ImageCaptureBase::ImageType type, const std::string& mesh_name, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simAddDetectionFilterMeshName", camera_name, type, mesh_name, vehicle_name, external);
    }
    void RpcLibClientBase::simSetDetectionFilterRadius(const std::string& camera_name, ImageCaptureBase::ImageType type, const float radius_cm, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simSetDetectionFilterRadius", camera_name, type, radius_cm, vehicle_name, external);
    }
    void RpcLibClientBase::simClearDetectionMeshNames(const std::string& camera_name, ImageCaptureBase::ImageType type, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simClearDetectionMeshNames", camera_name, type, vehicle_name, external);
    }
    vector<DetectionInfo> RpcLibClientBase::simGetDetections(const std::string& camera_name, ImageCaptureBase::ImageType image_type, const std::string& vehicle_name, bool external) {
        const auto& result = pimpl_->rpc.callResult<vector<RpcLibAdaptorsBase::DetectionInfo>>("simGetDetections", camera_name, image_type, vehicle_name, external);
        return RpcLibAdaptorsBase::DetectionInfo::to(result);
    }

    CollisionInfo RpcLibClientBase::simGetCollisionInfo(const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::CollisionInfo>("simGetCollisionInfo", vehicle_name).to();
    }
    Pose RpcLibClientBase::simGetVehiclePose(const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::Pose>("simGetVehiclePose", vehicle_name).to();
    }
    void RpcLibClientBase::simSetVehiclePose(const Pose& pose, bool ignore_collision, const std::string& vehicle_name) {
        pimpl_->rpc.call("simSetVehiclePose", RpcLibAdaptorsBase::Pose(pose), ignore_collision, vehicle_name);
    }
    void RpcLibClientBase::simSetKinematics(const Kinematics::State& state, bool ignore_collision, const std::string& vehicle_name) {
        pimpl_->rpc.call("simSetKinematics", RpcLibAdaptorsBase::KinematicsState(state), ignore_collision, vehicle_name);
    }
    void RpcLibClientBase::simSetTraceLine(const std::vector<float>& color_rgba, float thickness, const std::string& vehicle_name) {
        pimpl_->rpc.call("simSetTraceLine", color_rgba, thickness, vehicle_name);
    }
    vector<ImageCaptureBase::ImageResponse> RpcLibClientBase::simGetImages(vector<ImageCaptureBase::ImageRequest> request, const std::string& vehicle_name, bool external) {
        const auto& response_adaptor = pimpl_->rpc.callResult<vector<RpcLibAdaptorsBase::ImageResponse>>("simGetImages", RpcLibAdaptorsBase::ImageRequest::from(request), vehicle_name, external);
        return RpcLibAdaptorsBase::ImageResponse::to(response_adaptor);
    }
    vector<uint8_t> RpcLibClientBase::simGetImage(const std::string& camera_name, ImageCaptureBase::ImageType type, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<vector<uint8_t>>("simGetImage", camera_name, type, vehicle_name, external);
    }

    std::vector<std::string> RpcLibClientBase::simGetPresetLensSettings(const std::string& camera_name, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<vector<std::string>>("simGetPresetLensSettings", camera_name, vehicle_name, external);
    }
    std::string RpcLibClientBase::simGetLensSettings(const std::string& camera_name, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<std::string>("simGetLensSettings", camera_name, vehicle_name, external);
    }
    void RpcLibClientBase::simSetPresetLensSettings(const std::string& preset_lens_settings, const std::string& camera_name, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simSetPresetLensSettings", preset_lens_settings, camera_name, vehicle_name, external);
    }
    std::vector<std::string> RpcLibClientBase::simGetPresetFilmbackSettings(const std::string& camera_name, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<vector<std::string>>("simGetPresetFilmbackSettings", camera_name, vehicle_name, external);
    }
    void RpcLibClientBase::simSetPresetFilmbackSettings(const std::string& preset_filmback_settings, const std::string& camera_name, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simSetPresetFilmbackSettings", preset_filmback_settings, camera_name, vehicle_name, external);
    }
    std::string RpcLibClientBase::simGetFilmbackSettings(const std::string& camera_name, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<std::string>("simGetFilmbackSettings", camera_name, vehicle_name, external);
    }
    float RpcLibClientBase::simSetFilmbackSettings(const float sensor_width, const float sensor_height, const std::string& camera_name, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<float>("simSetFilmbackSettings", sensor_width, sensor_height, camera_name, vehicle_name, external);
    }
    float RpcLibClientBase::simGetFocalLength(const std::string& camera_name, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<float>("simGetFocalLength", camera_name, vehicle_name, external);
    }
    void RpcLibClientBase::simSetFocalLength(const float focal_length, const std::string& camera_name, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simSetFocalLength", focal_length, camera_name, vehicle_name, external);
    }
    void RpcLibClientBase::simEnableManualFocus(const bool enable, const std::string& camera_name, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simEnableManualFocus", enable, camera_name, vehicle_name, external);
    }
    float RpcLibClientBase::simGetFocusDistance(const std::string& camera_name, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<float>("simGetFocusDistance", camera_name, vehicle_name, external);
    }
    void RpcLibClientBase::simSetFocusDistance(const float focus_distance, const std::string& camera_name, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simSetFocusDistance", focus_distance, camera_name, vehicle_name, external);
    }
    float RpcLibClientBase::simGetFocusAperture(const std::string& camera_name, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<float>("simGetFocusAperture", camera_name, vehicle_name, external);
    }
    void RpcLibClientBase::simSetFocusAperture(const float focus_aperture, const std::string& camera_name, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simSetFocusAperture", focus_aperture, camera_name, vehicle_name, external);
    }
    void RpcLibClientBase::simEnableFocusPlane(const bool enable, const std::string& camera_name, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simEnableFocusPlane", enable, camera_name, vehicle_name, external);
    }
    std::string RpcLibClientBase::simGetCurrentFieldOfView(const std::string& camera_name, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<std::string>("simGetCurrentFieldOfView", camera_name, vehicle_name, external);
    }

    bool RpcLibClientBase::simTestLineOfSightToPoint(const msr::airlib::GeoPoint& point, const std::string& vehicle_name) {
        return pimpl_->rpc.callResult<bool>("simTestLineOfSightToPoint", RpcLibAdaptorsBase::GeoPoint(point), vehicle_name);
    }
    bool RpcLibClientBase::simTestLineOfSightBetweenPoints(const msr::airlib::GeoPoint& point1, const msr::airlib::GeoPoint& point2) {
        return pimpl_->rpc.callResult<bool>("simTestLineOfSightBetweenPoints", RpcLibAdaptorsBase::GeoPoint(point1), RpcLibAdaptorsBase::GeoPoint(point2));
    }
    vector<msr::airlib::GeoPoint> RpcLibClientBase::simGetWorldExtents() {
        vector<RpcLibAdaptorsBase::GeoPoint> raw_result = pimpl_->rpc.callResult<vector<RpcLibAdaptorsBase::GeoPoint>>("simGetWorldExtents");
        vector<msr::airlib::GeoPoint> final_result;
        final_result.push_back(raw_result[0].to());
        final_result.push_back(raw_result[1].to());
        return final_result;
    }

    vector<MeshPositionVertexBuffersResponse> RpcLibClientBase::simGetMeshPositionVertexBuffers() {
        const auto& response_adaptor = pimpl_->rpc.callResult<vector<RpcLibAdaptorsBase::MeshPositionVertexBuffersResponse>>("simGetMeshPositionVertexBuffers");
        return RpcLibAdaptorsBase::MeshPositionVertexBuffersResponse::to(response_adaptor);
    }

    bool RpcLibClientBase::simAddVehicle(const std::string& vehicle_name, const std::string& vehicle_type, const Pose& pose, const std::string& pawn_path) {
        return pimpl_->rpc.callResult<bool>("simAddVehicle", vehicle_name, vehicle_type, RpcLibAdaptorsBase::Pose(pose), pawn_path);
    }

    void RpcLibClientBase::simPrintLogMessage(const std::string& message, std::string message_param, unsigned char severity) {
        pimpl_->rpc.call("simPrintLogMessage", message, message_param, severity);
    }

    void RpcLibClientBase::simFlushPersistentMarkers() { pimpl_->rpc.call("simFlushPersistentMarkers"); }

    void RpcLibClientBase::simPlotPoints(const vector<Vector3r>& points, const vector<float>& color_rgba, float size, float duration, bool is_persistent) {
        vector<RpcLibAdaptorsBase::Vector3r> conv_points;
        RpcLibAdaptorsBase::from(points, conv_points);
        pimpl_->rpc.call("simPlotPoints", conv_points, color_rgba, size, duration, is_persistent);
    }
    void RpcLibClientBase::simPlotLineStrip(const vector<Vector3r>& points, const vector<float>& color_rgba, float thickness, float duration, bool is_persistent) {
        vector<RpcLibAdaptorsBase::Vector3r> conv_points;
        RpcLibAdaptorsBase::from(points, conv_points);
        pimpl_->rpc.call("simPlotLineStrip", conv_points, color_rgba, thickness, duration, is_persistent);
    }
    void RpcLibClientBase::simPlotLineList(const vector<Vector3r>& points, const vector<float>& color_rgba, float thickness, float duration, bool is_persistent) {
        vector<RpcLibAdaptorsBase::Vector3r> conv_points;
        RpcLibAdaptorsBase::from(points, conv_points);
        pimpl_->rpc.call("simPlotLineList", conv_points, color_rgba, thickness, duration, is_persistent);
    }
    void RpcLibClientBase::simPlotArrows(const vector<Vector3r>& points_start, const vector<Vector3r>& points_end, const vector<float>& color_rgba, float thickness, float arrow_size, float duration, bool is_persistent) {
        vector<RpcLibAdaptorsBase::Vector3r> conv_points_start; RpcLibAdaptorsBase::from(points_start, conv_points_start);
        vector<RpcLibAdaptorsBase::Vector3r> conv_points_end; RpcLibAdaptorsBase::from(points_end, conv_points_end);
        pimpl_->rpc.call("simPlotArrows", conv_points_start, conv_points_end, color_rgba, thickness, arrow_size, duration, is_persistent);
    }
    void RpcLibClientBase::simPlotStrings(const vector<std::string>& strings, const vector<Vector3r>& positions, float scale, const vector<float>& color_rgba, float duration) {
        vector<RpcLibAdaptorsBase::Vector3r> conv_positions;
        RpcLibAdaptorsBase::from(positions, conv_positions);
        pimpl_->rpc.call("simPlotStrings", strings, conv_positions, scale, color_rgba, duration);
    }
    void RpcLibClientBase::simPlotTransforms(const vector<Pose>& poses, float scale, float thickness, float duration, bool is_persistent) {
        vector<RpcLibAdaptorsBase::Pose> conv_poses;
        RpcLibAdaptorsBase::from(poses, conv_poses);
        pimpl_->rpc.call("simPlotTransforms", conv_poses, scale, thickness, duration, is_persistent);
    }
    void RpcLibClientBase::simPlotTransformsWithNames(const vector<Pose>& poses, const vector<std::string>& names, float tf_scale, float tf_thickness, float text_scale, const vector<float>& text_color_rgba, float duration) {
        vector<RpcLibAdaptorsBase::Pose> conv_poses;
        RpcLibAdaptorsBase::from(poses, conv_poses);
        pimpl_->rpc.call("simPlotTransformsWithNames", conv_poses, names, tf_scale, tf_thickness, text_scale, text_color_rgba, duration);
    }

    bool RpcLibClientBase::simIsPaused() const { return const_cast<impl*>(pimpl_.get())->rpc.callResult<bool>("simIsPaused"); }
    void RpcLibClientBase::simPause(bool is_paused) { pimpl_->rpc.call("simPause", is_paused); }
    void RpcLibClientBase::simContinueForTime(double seconds) { pimpl_->rpc.call("simContinueForTime", seconds); }
    void RpcLibClientBase::simContinueForFrames(uint32_t frames) { pimpl_->rpc.call("simContinueForFrames", frames); }
    void RpcLibClientBase::simEnableWeather(bool enable) { pimpl_->rpc.call("simEnableWeather", enable); }
    void RpcLibClientBase::simSetWeatherParameter(WorldSimApiBase::WeatherParameter param, float val) {
        pimpl_->rpc.call("simSetWeatherParameter", param, val);
    }
    void RpcLibClientBase::simSetTimeOfDay(bool is_enabled, const string& start_datetime, bool is_start_datetime_dst, float celestial_clock_speed, float update_interval_secs, bool move_sun) {
        pimpl_->rpc.call("simSetTimeOfDay", is_enabled, start_datetime, is_start_datetime_dst, celestial_clock_speed, update_interval_secs, move_sun);
    }

    vector<string> RpcLibClientBase::simListSceneObjects(const string& name_regex) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<vector<string>>("simListSceneObjects", name_regex);
    }
    vector<string> RpcLibClientBase::simListSceneObjectsByTag(const string& tag_regex) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<vector<string>>("simListSceneObjectsByTag", tag_regex);
    }

    std::vector<std::string> RpcLibClientBase::simSwapTextures(const std::string& tags, int tex_id, int component_id, int material_id) {
        return pimpl_->rpc.callResult<vector<string>>("simSwapTextures", tags, tex_id, component_id, material_id);
    }
    bool RpcLibClientBase::simSetObjectMaterial(const std::string& object_name, const std::string& material_name, const int component_id) {
        return pimpl_->rpc.callResult<bool>("simSetObjectMaterial", object_name, material_name, component_id);
    }
    bool RpcLibClientBase::simSetObjectMaterialFromTexture(const std::string& object_name, const std::string& texture_path, const int component_id) {
        return pimpl_->rpc.callResult<bool>("simSetObjectMaterialFromTexture", object_name, texture_path, component_id);
    }
    bool RpcLibClientBase::simLoadLevel(const string& level_name) {
        return pimpl_->rpc.callResult<bool>("simLoadLevel", level_name);
    }

    std::string RpcLibClientBase::simSpawnObject(const std::string& object_name, const std::string& load_component, const Pose& pose,
                                                   const Vector3r& scale, bool physics_enabled) {
        return pimpl_->rpc.callResult<std::string>("simSpawnObject", object_name, load_component, RpcLibAdaptorsBase::Pose(pose), RpcLibAdaptorsBase::Vector3r(scale), physics_enabled);
    }
    bool RpcLibClientBase::simDestroyObject(const std::string& object_name) {
        return pimpl_->rpc.callResult<bool>("simDestroyObject", object_name);
    }
    msr::airlib::Vector3r RpcLibClientBase::simGetObjectScale(const std::string& object_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::Vector3r>("simGetObjectScale", object_name).to();
    }
    msr::airlib::Pose RpcLibClientBase::simGetObjectPose(const std::string& object_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::Pose>("simGetObjectPose", object_name).to();
    }
    bool RpcLibClientBase::simSetObjectPose(const std::string& object_name, const msr::airlib::Pose& pose, bool teleport) {
        return pimpl_->rpc.callResult<bool>("simSetObjectPose", object_name, RpcLibAdaptorsBase::Pose(pose), teleport);
    }
    bool RpcLibClientBase::simSetObjectScale(const std::string& object_name, const msr::airlib::Vector3r& scale) {
        return pimpl_->rpc.callResult<bool>("simSetObjectScale", object_name, RpcLibAdaptorsBase::Vector3r(scale));
    }

    CameraInfo RpcLibClientBase::simGetCameraInfo(const std::string& camera_name, const std::string& vehicle_name, bool external) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::CameraInfo>("simGetCameraInfo", camera_name, vehicle_name, external).to();
    }
    void RpcLibClientBase::simSetCameraPose(const std::string& camera_name, const Pose& pose, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simSetCameraPose", camera_name, RpcLibAdaptorsBase::Pose(pose), vehicle_name, external);
    }
    void RpcLibClientBase::simSetCameraFov(const std::string& camera_name, float fov_degrees, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simSetCameraFov", camera_name, fov_degrees, vehicle_name, external);
    }
    void RpcLibClientBase::simSetDistortionParam(const std::string& camera_name, const std::string& param_name, float value, const std::string& vehicle_name, bool external) {
        pimpl_->rpc.call("simSetDistortionParam", camera_name, param_name, value, vehicle_name, external);
    }
    std::vector<float> RpcLibClientBase::simGetDistortionParams(const std::string& camera_name, const std::string& vehicle_name, bool external) {
        return pimpl_->rpc.callResult<std::vector<float>>("simGetDistortionParams", camera_name, vehicle_name, external);
    }

    msr::airlib::Kinematics::State RpcLibClientBase::simGetGroundTruthKinematics(const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::KinematicsState>("simGetGroundTruthKinematics", vehicle_name).to();
    }
    msr::airlib::Environment::State RpcLibClientBase::simGetGroundTruthEnvironment(const std::string& vehicle_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::EnvironmentState>("simGetGroundTruthEnvironment", vehicle_name).to();
    }
    bool RpcLibClientBase::simCreateVoxelGrid(const msr::airlib::Vector3r& position, const int& x, const int& y, const int& z, const float& res, const std::string& output_file) {
        return pimpl_->rpc.callResult<bool>("simCreateVoxelGrid", RpcLibAdaptorsBase::Vector3r(position), x, y, z, res, output_file);
    }
    msr::airlib::Vector3r RpcLibClientBase::simFindLookAtRotation(const std::string& vehicle_name, const std::string& object_name) const {
        return const_cast<impl*>(pimpl_.get())->rpc.callResult<RpcLibAdaptorsBase::Vector3r>("simFindLookAtRotation", vehicle_name, object_name).to();
    }
    void RpcLibClientBase::cancelLastTask(const std::string& vehicle_name) {
        pimpl_->rpc.call("cancelLastTask", vehicle_name);
    }
    bool RpcLibClientBase::simRunConsoleCommand(const std::string& command) {
        return pimpl_->rpc.callResult<bool>("simRunConsoleCommand", command);
    }

    RpcLibClientBase* RpcLibClientBase::waitOnLastTask(bool* task_result, float timeout_sec) {
        unused(timeout_sec); unused(task_result);
        return this;
    }

    void RpcLibClientBase::startRecording() { pimpl_->rpc.call("startRecording"); }
    void RpcLibClientBase::stopRecording() { pimpl_->rpc.call("stopRecording"); }
    bool RpcLibClientBase::isRecording() { return pimpl_->rpc.callResult<bool>("isRecording"); }

    void RpcLibClientBase::simSetWind(const Vector3r& wind) const {
        const_cast<impl*>(pimpl_.get())->rpc.call("simSetWind", RpcLibAdaptorsBase::Vector3r(wind));
    }
    void RpcLibClientBase::simSetExtForce(const Vector3r& ext_force) const {
        const_cast<impl*>(pimpl_.get())->rpc.call("simSetExtForce", RpcLibAdaptorsBase::Vector3r(ext_force));
    }
    vector<string> RpcLibClientBase::listVehicles() { return pimpl_->rpc.callResult<vector<string>>("listVehicles"); }
    std::string RpcLibClientBase::getSettingsString() const { return const_cast<impl*>(pimpl_.get())->rpc.callResult<std::string>("getSettingsString"); }
    std::vector<std::string> RpcLibClientBase::simListAssets() const { return const_cast<impl*>(pimpl_.get())->rpc.callResult<std::vector<std::string>>("simListAssets"); }

    void* RpcLibClientBase::getClient() { return nullptr; }
    const void* RpcLibClientBase::getClient() const { return nullptr; }
    ZenohRpc& RpcLibClientBase::getZenohRpc() { return pimpl_->rpc; }
}
} //namespace

