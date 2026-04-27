
#include "api/RpcLibServerBase.hpp"
#include "common/Common.hpp"
#include "common/ZenohRpc.hpp"
#include <functional>
#include <thread>

STRICT_MODE_OFF
#include "api/RpcLibAdaptorsBase.hpp"
STRICT_MODE_ON

namespace msr {
namespace airlib {

    using RpcLibAdaptorsBase = msr::airlib_rpclib::RpcLibAdaptorsBase;

    struct RpcLibServerBase::impl {
        impl(ApiProvider* api_provider)
            : api_provider_(api_provider) {}

        void bindAll(RpcLibServerBase* self) {
            rpc.bind("ping", std::function<bool()>([self]() -> bool { return true; }));
            rpc.bind("getServerVersion", std::function<int()>([]() -> int { return 1; }));
            rpc.bind("getMinRequiredClientVersion", std::function<int()>([]() -> int { return 1; }));

            rpc.bind("simPause", std::function<void(bool)>([self](bool is_paused) { self->getWorldSimApi()->pause(is_paused); }));
            rpc.bind("simIsPaused", std::function<bool()>([self]() -> bool { return self->getWorldSimApi()->isPaused(); }));
            rpc.bind("simContinueForTime", std::function<void(double)>([self](double seconds) { self->getWorldSimApi()->continueForTime(seconds); }));
            rpc.bind("simContinueForFrames", std::function<void(uint32_t)>([self](uint32_t frames) { self->getWorldSimApi()->continueForFrames(frames); }));

            rpc.bind("simSetTimeOfDay", std::function<void(bool, const string&, bool, float, float, bool)>(
                [self](bool is_enabled, const string& start_datetime, bool is_start_datetime_dst, float celestial_clock_speed, float update_interval_secs, bool move_sun) {
                    self->getWorldSimApi()->setTimeOfDay(is_enabled, start_datetime, is_start_datetime_dst, celestial_clock_speed, update_interval_secs, move_sun);
                }));
            rpc.bind("simEnableWeather", std::function<void(bool)>([self](bool enable) { self->getWorldSimApi()->enableWeather(enable); }));
            rpc.bind("simSetWeatherParameter", std::function<void(WorldSimApiBase::WeatherParameter, float)>(
                [self](WorldSimApiBase::WeatherParameter param, float val) { self->getWorldSimApi()->setWeatherParameter(param, val); }));

            rpc.bind("enableApiControl", std::function<void(bool, const std::string&)>(
                [self](bool is_enabled, const std::string& vehicle_name) { self->getVehicleApi(vehicle_name)->enableApiControl(is_enabled); }));
            rpc.bind("isApiControlEnabled", std::function<bool(const std::string&)>(
                [self](const std::string& vehicle_name) -> bool { return self->getVehicleApi(vehicle_name)->isApiControlEnabled(); }));
            rpc.bind("armDisarm", std::function<bool(bool, const std::string&)>(
                [self](bool arm, const std::string& vehicle_name) -> bool { return self->getVehicleApi(vehicle_name)->armDisarm(arm); }));
            rpc.bind("simRunConsoleCommand", std::function<bool(const std::string&)>(
                [self](const std::string& command) -> bool { return self->getWorldSimApi()->runConsoleCommand(command); }));

            rpc.bind("simGetImages", std::function<std::vector<RpcLibAdaptorsBase::ImageResponse>(const std::vector<RpcLibAdaptorsBase::ImageRequest>&, const std::string&, bool)>(
                [self](const std::vector<RpcLibAdaptorsBase::ImageRequest>& request_adapter, const std::string& vehicle_name, bool external) -> std::vector<RpcLibAdaptorsBase::ImageResponse> {
                    const auto& response = self->getWorldSimApi()->getImages(RpcLibAdaptorsBase::ImageRequest::to(request_adapter), vehicle_name, external);
                    return RpcLibAdaptorsBase::ImageResponse::from(response);
                }));
            rpc.bind("simGetImage", std::function<std::vector<uint8_t>(const std::string&, ImageCaptureBase::ImageType, const std::string&, bool)>(
                [self](const std::string& camera_name, ImageCaptureBase::ImageType type, const std::string& vehicle_name, bool external) -> std::vector<uint8_t> {
                    return self->getWorldSimApi()->getImage(type, CameraDetails(camera_name, vehicle_name, external));
                }));

            // CinemAirSim
            rpc.bind("simGetPresetLensSettings", std::function<std::vector<std::string>(const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& vehicle_name, bool external) -> std::vector<std::string> {
                    return self->getWorldSimApi()->getPresetLensSettings(CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simGetLensSettings", std::function<std::string(const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& vehicle_name, bool external) -> std::string {
                    return self->getWorldSimApi()->getLensSettings(CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simSetPresetLensSettings", std::function<void(const std::string, const std::string&, const std::string&, bool)>(
                [self](const std::string preset_lens_settings, const std::string& camera_name, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->setPresetLensSettings(preset_lens_settings, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simGetPresetFilmbackSettings", std::function<std::vector<std::string>(const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& vehicle_name, bool external) -> std::vector<std::string> {
                    return self->getWorldSimApi()->getPresetFilmbackSettings(CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simSetPresetFilmbackSettings", std::function<void(const std::string, const std::string&, const std::string&, bool)>(
                [self](const std::string preset_filmback_settings, const std::string& camera_name, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->setPresetFilmbackSettings(preset_filmback_settings, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simGetFilmbackSettings", std::function<std::string(const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& vehicle_name, bool external) -> std::string {
                    return self->getWorldSimApi()->getFilmbackSettings(CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simSetFilmbackSettings", std::function<float(float, float, const std::string&, const std::string&, bool)>(
                [self](float width, float height, const std::string& camera_name, const std::string& vehicle_name, bool external) -> float {
                    return self->getWorldSimApi()->setFilmbackSettings(width, height, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simGetFocalLength", std::function<float(const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& vehicle_name, bool external) -> float {
                    return self->getWorldSimApi()->getFocalLength(CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simSetFocalLength", std::function<void(float, const std::string&, const std::string&, bool)>(
                [self](float focal_length, const std::string& camera_name, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->setFocalLength(focal_length, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simEnableManualFocus", std::function<void(bool, const std::string&, const std::string&, bool)>(
                [self](bool enable, const std::string& camera_name, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->enableManualFocus(enable, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simGetFocusDistance", std::function<float(const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& vehicle_name, bool external) -> float {
                    return self->getWorldSimApi()->getFocusDistance(CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simSetFocusDistance", std::function<void(float, const std::string&, const std::string&, bool)>(
                [self](float focus_distance, const std::string& camera_name, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->setFocusDistance(focus_distance, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simGetFocusAperture", std::function<float(const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& vehicle_name, bool external) -> float {
                    return self->getWorldSimApi()->getFocusAperture(CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simSetFocusAperture", std::function<void(float, const std::string&, const std::string&, bool)>(
                [self](float focus_aperture, const std::string& camera_name, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->setFocusAperture(focus_aperture, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simEnableFocusPlane", std::function<void(bool, const std::string&, const std::string&, bool)>(
                [self](bool enable, const std::string& camera_name, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->enableFocusPlane(enable, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simGetCurrentFieldOfView", std::function<std::string(const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& vehicle_name, bool external) -> std::string {
                    return self->getWorldSimApi()->getCurrentFieldOfView(CameraDetails(camera_name, vehicle_name, external));
                }));

            rpc.bind("simTestLineOfSightToPoint", std::function<bool(const RpcLibAdaptorsBase::GeoPoint&, const std::string&)>(
                [self](const RpcLibAdaptorsBase::GeoPoint& point, const std::string& vehicle_name) -> bool {
                    return self->getVehicleSimApi(vehicle_name)->testLineOfSightToPoint(point.to());
                }));
            rpc.bind("simTestLineOfSightBetweenPoints", std::function<bool(const RpcLibAdaptorsBase::GeoPoint&, const RpcLibAdaptorsBase::GeoPoint&)>(
                [self](const RpcLibAdaptorsBase::GeoPoint& point1, const RpcLibAdaptorsBase::GeoPoint& point2) -> bool {
                    return self->getWorldSimApi()->testLineOfSightBetweenPoints(point1.to(), point2.to());
                }));
            rpc.bind("simGetWorldExtents", std::function<std::vector<RpcLibAdaptorsBase::GeoPoint>()>(
                [self]() -> std::vector<RpcLibAdaptorsBase::GeoPoint> {
                    std::vector<msr::airlib::GeoPoint> result = self->getWorldSimApi()->getWorldExtents();
                    std::vector<RpcLibAdaptorsBase::GeoPoint> conv_result;
                    RpcLibAdaptorsBase::from(result, conv_result);
                    return conv_result;
                }));
            rpc.bind("simGetMeshPositionVertexBuffers", std::function<std::vector<RpcLibAdaptorsBase::MeshPositionVertexBuffersResponse>()>(
                [self]() -> std::vector<RpcLibAdaptorsBase::MeshPositionVertexBuffersResponse> {
                    const auto& response = self->getWorldSimApi()->getMeshPositionVertexBuffers();
                    return RpcLibAdaptorsBase::MeshPositionVertexBuffersResponse::from(response);
                }));
            rpc.bind("simAddVehicle", std::function<bool(const std::string&, const std::string&, const RpcLibAdaptorsBase::Pose&, const std::string&)>(
                [self](const std::string& vehicle_name, const std::string& vehicle_type, const RpcLibAdaptorsBase::Pose& pose, const std::string& pawn_path) -> bool {
                    return self->getWorldSimApi()->addVehicle(vehicle_name, vehicle_type, pose.to(), pawn_path);
                }));

            rpc.bind("simSetVehiclePose", std::function<void(const RpcLibAdaptorsBase::Pose&, bool, const std::string&)>(
                [self](const RpcLibAdaptorsBase::Pose& pose, bool ignore_collision, const std::string& vehicle_name) {
                    self->getVehicleSimApi(vehicle_name)->setPose(pose.to(), ignore_collision);
                }));
            rpc.bind("simGetVehiclePose", std::function<RpcLibAdaptorsBase::Pose(const std::string&)>(
                [self](const std::string& vehicle_name) -> RpcLibAdaptorsBase::Pose {
                    return RpcLibAdaptorsBase::Pose(self->getVehicleSimApi(vehicle_name)->getPose());
                }));
            rpc.bind("simSetTraceLine", std::function<void(const std::vector<float>&, float, const std::string&)>(
                [self](const std::vector<float>& color_rgba, float thickness, const std::string& vehicle_name) {
                    self->getVehicleSimApi(vehicle_name)->setTraceLine(color_rgba, thickness);
                }));

            rpc.bind("simSetSegmentationObjectID", std::function<bool(const std::string&, int, bool)>(
                [self](const std::string& mesh_name, int object_id, bool is_name_regex) -> bool {
                    return self->getWorldSimApi()->setSegmentationObjectID(mesh_name, object_id, is_name_regex);
                }));
            rpc.bind("simGetSegmentationObjectID", std::function<int(const std::string&)>(
                [self](const std::string& mesh_name) -> int { return self->getWorldSimApi()->getSegmentationObjectID(mesh_name); }));

            rpc.bind("simAddDetectionFilterMeshName", std::function<void(const std::string&, ImageCaptureBase::ImageType, const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, ImageCaptureBase::ImageType type, const std::string& mesh_name, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->addDetectionFilterMeshName(type, mesh_name, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simSetDetectionFilterRadius", std::function<void(const std::string&, ImageCaptureBase::ImageType, float, const std::string&, bool)>(
                [self](const std::string& camera_name, ImageCaptureBase::ImageType type, float radius_cm, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->setDetectionFilterRadius(type, radius_cm, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simClearDetectionMeshNames", std::function<void(const std::string&, ImageCaptureBase::ImageType, const std::string&, bool)>(
                [self](const std::string& camera_name, ImageCaptureBase::ImageType type, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->clearDetectionMeshNames(type, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simGetDetections", std::function<std::vector<RpcLibAdaptorsBase::DetectionInfo>(const std::string&, ImageCaptureBase::ImageType, const std::string&, bool)>(
                [self](const std::string& camera_name, ImageCaptureBase::ImageType type, const std::string& vehicle_name, bool external) -> std::vector<RpcLibAdaptorsBase::DetectionInfo> {
                    const auto& response = self->getWorldSimApi()->getDetections(type, CameraDetails(camera_name, vehicle_name, external));
                    return RpcLibAdaptorsBase::DetectionInfo::from(response);
                }));

            rpc.bind("reset", std::function<void()>([self]() {
                static bool resetInProgress;
                if (resetInProgress) return;
                resetInProgress = true;
                auto* sim_world_api = self->getWorldSimApi();
                if (sim_world_api) sim_world_api->reset();
                else self->getVehicleApi("")->reset();
                resetInProgress = false;
            }));

            rpc.bind("simPrintLogMessage", std::function<void(const std::string&, const std::string&, unsigned char)>(
                [self](const std::string& message, const std::string& message_param, unsigned char severity) {
                    self->getWorldSimApi()->printLogMessage(message, message_param, severity);
                }));
            rpc.bind("getHomeGeoPoint", std::function<RpcLibAdaptorsBase::GeoPoint(const std::string&)>(
                [self](const std::string& vehicle_name) -> RpcLibAdaptorsBase::GeoPoint {
                    return RpcLibAdaptorsBase::GeoPoint(self->getVehicleApi(vehicle_name)->getHomeGeoPoint());
                }));
            rpc.bind("getLidarData", std::function<RpcLibAdaptorsBase::LidarData(const std::string&, const std::string&)>(
                [self](const std::string& lidar_name, const std::string& vehicle_name) -> RpcLibAdaptorsBase::LidarData {
                    return RpcLibAdaptorsBase::LidarData(self->getVehicleApi(vehicle_name)->getLidarData(lidar_name));
                }));
            rpc.bind("getImuData", std::function<RpcLibAdaptorsBase::ImuData(const std::string&, const std::string&)>(
                [self](const std::string& imu_name, const std::string& vehicle_name) -> RpcLibAdaptorsBase::ImuData {
                    return RpcLibAdaptorsBase::ImuData(self->getVehicleApi(vehicle_name)->getImuData(imu_name));
                }));
            rpc.bind("getBarometerData", std::function<RpcLibAdaptorsBase::BarometerData(const std::string&, const std::string&)>(
                [self](const std::string& barometer_name, const std::string& vehicle_name) -> RpcLibAdaptorsBase::BarometerData {
                    return RpcLibAdaptorsBase::BarometerData(self->getVehicleApi(vehicle_name)->getBarometerData(barometer_name));
                }));
            rpc.bind("getMagnetometerData", std::function<RpcLibAdaptorsBase::MagnetometerData(const std::string&, const std::string&)>(
                [self](const std::string& magnetometer_name, const std::string& vehicle_name) -> RpcLibAdaptorsBase::MagnetometerData {
                    return RpcLibAdaptorsBase::MagnetometerData(self->getVehicleApi(vehicle_name)->getMagnetometerData(magnetometer_name));
                }));
            rpc.bind("getGpsData", std::function<RpcLibAdaptorsBase::GpsData(const std::string&, const std::string&)>(
                [self](const std::string& gps_name, const std::string& vehicle_name) -> RpcLibAdaptorsBase::GpsData {
                    return RpcLibAdaptorsBase::GpsData(self->getVehicleApi(vehicle_name)->getGpsData(gps_name));
                }));
            rpc.bind("getDistanceSensorData", std::function<RpcLibAdaptorsBase::DistanceSensorData(const std::string&, const std::string&)>(
                [self](const std::string& distance_sensor_name, const std::string& vehicle_name) -> RpcLibAdaptorsBase::DistanceSensorData {
                    return RpcLibAdaptorsBase::DistanceSensorData(self->getVehicleApi(vehicle_name)->getDistanceSensorData(distance_sensor_name));
                }));

            rpc.bind("simGetCameraInfo", std::function<RpcLibAdaptorsBase::CameraInfo(const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& vehicle_name, bool external) -> RpcLibAdaptorsBase::CameraInfo {
                    return RpcLibAdaptorsBase::CameraInfo(self->getWorldSimApi()->getCameraInfo(CameraDetails(camera_name, vehicle_name, external)));
                }));
            rpc.bind("simSetDistortionParam", std::function<void(const std::string&, const std::string&, float, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& param_name, float value, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->setDistortionParam(param_name, value, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simGetDistortionParams", std::function<std::vector<float>(const std::string&, const std::string&, bool)>(
                [self](const std::string& camera_name, const std::string& vehicle_name, bool external) -> std::vector<float> {
                    return self->getWorldSimApi()->getDistortionParams(CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simSetCameraPose", std::function<void(const std::string&, const RpcLibAdaptorsBase::Pose&, const std::string&, bool)>(
                [self](const std::string& camera_name, const RpcLibAdaptorsBase::Pose& pose, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->setCameraPose(pose.to(), CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simSetCameraFov", std::function<void(const std::string&, float, const std::string&, bool)>(
                [self](const std::string& camera_name, float fov_degrees, const std::string& vehicle_name, bool external) {
                    self->getWorldSimApi()->setCameraFoV(fov_degrees, CameraDetails(camera_name, vehicle_name, external));
                }));
            rpc.bind("simGetCollisionInfo", std::function<RpcLibAdaptorsBase::CollisionInfo(const std::string&)>(
                [self](const std::string& vehicle_name) -> RpcLibAdaptorsBase::CollisionInfo {
                    return RpcLibAdaptorsBase::CollisionInfo(self->getVehicleSimApi(vehicle_name)->getCollisionInfoAndReset());
                }));
            rpc.bind("simListSceneObjects", std::function<std::vector<std::string>(const std::string&)>(
                [self](const std::string& name_regex) -> std::vector<std::string> {
                    return self->getWorldSimApi()->listSceneObjects(name_regex);
                }));
            rpc.bind("simListSceneObjectsByTag", std::function<std::vector<std::string>(const std::string&)>(
                [self](const std::string& tag_regex) -> std::vector<std::string> {
                    return self->getWorldSimApi()->listSceneObjectsByTag(tag_regex);
                }));
            rpc.bind("simLoadLevel", std::function<bool(const std::string&)>(
                [self](const std::string& level_name) -> bool { return self->getWorldSimApi()->loadLevel(level_name); }));

            rpc.bind("simSpawnObject", std::function<std::string(const std::string&, const std::string&, const RpcLibAdaptorsBase::Pose&, const RpcLibAdaptorsBase::Vector3r&, bool, bool)>(
                [self](const std::string& object_name, const std::string& load_component, const RpcLibAdaptorsBase::Pose& pose, const RpcLibAdaptorsBase::Vector3r& scale, bool physics_enabled, bool is_blueprint) -> std::string {
                    std::string obj_name = object_name;
                    return self->getWorldSimApi()->spawnObject(obj_name, load_component, pose.to(), scale.to(), physics_enabled, is_blueprint);
                }));
            rpc.bind("simDestroyObject", std::function<bool(const std::string&)>(
                [self](const std::string& object_name) -> bool { return self->getWorldSimApi()->destroyObject(object_name); }));
            rpc.bind("simListAssets", std::function<std::vector<std::string>()>(
                [self]() -> std::vector<std::string> { return self->getWorldSimApi()->listAssets(); }));
            rpc.bind("simGetObjectPose", std::function<RpcLibAdaptorsBase::Pose(const std::string&)>(
                [self](const std::string& object_name) -> RpcLibAdaptorsBase::Pose {
                    return RpcLibAdaptorsBase::Pose(self->getWorldSimApi()->getObjectPose(object_name));
                }));
            rpc.bind("simGetObjectScale", std::function<RpcLibAdaptorsBase::Vector3r(const std::string&)>(
                [self](const std::string& object_name) -> RpcLibAdaptorsBase::Vector3r {
                    return RpcLibAdaptorsBase::Vector3r(self->getWorldSimApi()->getObjectScale(object_name));
                }));
            rpc.bind("simSetObjectPose", std::function<bool(const std::string&, const RpcLibAdaptorsBase::Pose&, bool)>(
                [self](const std::string& object_name, const RpcLibAdaptorsBase::Pose& pose, bool teleport) -> bool {
                    return self->getWorldSimApi()->setObjectPose(object_name, pose.to(), teleport);
                }));
            rpc.bind("simSetObjectScale", std::function<bool(const std::string&, const RpcLibAdaptorsBase::Vector3r&)>(
                [self](const std::string& object_name, const RpcLibAdaptorsBase::Vector3r& scale) -> bool {
                    return self->getWorldSimApi()->setObjectScale(object_name, scale.to());
                }));

            rpc.bind("simFlushPersistentMarkers", std::function<void()>([self]() { self->getWorldSimApi()->simFlushPersistentMarkers(); }));
            rpc.bind("simPlotPoints", std::function<void(const std::vector<RpcLibAdaptorsBase::Vector3r>&, const std::vector<float>&, float, float, bool)>(
                [self](const std::vector<RpcLibAdaptorsBase::Vector3r>& points, const std::vector<float>& color_rgba, float size, float duration, bool is_persistent) {
                    std::vector<Vector3r> conv_points; RpcLibAdaptorsBase::to(points, conv_points);
                    self->getWorldSimApi()->simPlotPoints(conv_points, color_rgba, size, duration, is_persistent);
                }));
            rpc.bind("simPlotLineStrip", std::function<void(const std::vector<RpcLibAdaptorsBase::Vector3r>&, const std::vector<float>&, float, float, bool)>(
                [self](const std::vector<RpcLibAdaptorsBase::Vector3r>& points, const std::vector<float>& color_rgba, float thickness, float duration, bool is_persistent) {
                    std::vector<Vector3r> conv_points; RpcLibAdaptorsBase::to(points, conv_points);
                    self->getWorldSimApi()->simPlotLineStrip(conv_points, color_rgba, thickness, duration, is_persistent);
                }));
            rpc.bind("simPlotLineList", std::function<void(const std::vector<RpcLibAdaptorsBase::Vector3r>&, const std::vector<float>&, float, float, bool)>(
                [self](const std::vector<RpcLibAdaptorsBase::Vector3r>& points, const std::vector<float>& color_rgba, float thickness, float duration, bool is_persistent) {
                    std::vector<Vector3r> conv_points; RpcLibAdaptorsBase::to(points, conv_points);
                    self->getWorldSimApi()->simPlotLineList(conv_points, color_rgba, thickness, duration, is_persistent);
                }));
            rpc.bind("simPlotArrows", std::function<void(const std::vector<RpcLibAdaptorsBase::Vector3r>&, const std::vector<RpcLibAdaptorsBase::Vector3r>&, const std::vector<float>&, float, float, float, bool)>(
                [self](const std::vector<RpcLibAdaptorsBase::Vector3r>& points_start, const std::vector<RpcLibAdaptorsBase::Vector3r>& points_end, const std::vector<float>& color_rgba, float thickness, float arrow_size, float duration, bool is_persistent) {
                    std::vector<Vector3r> conv_start; RpcLibAdaptorsBase::to(points_start, conv_start);
                    std::vector<Vector3r> conv_end; RpcLibAdaptorsBase::to(points_end, conv_end);
                    self->getWorldSimApi()->simPlotArrows(conv_start, conv_end, color_rgba, thickness, arrow_size, duration, is_persistent);
                }));
            rpc.bind("simPlotStrings", std::function<void(const std::vector<std::string>, const std::vector<RpcLibAdaptorsBase::Vector3r>&, float, const std::vector<float>&, float)>(
                [self](const std::vector<std::string> strings, const std::vector<RpcLibAdaptorsBase::Vector3r>& positions, float scale, const std::vector<float>& color_rgba, float duration) {
                    std::vector<Vector3r> conv_pos; RpcLibAdaptorsBase::to(positions, conv_pos);
                    self->getWorldSimApi()->simPlotStrings(strings, conv_pos, scale, color_rgba, duration);
                }));
            rpc.bind("simPlotTransforms", std::function<void(const std::vector<RpcLibAdaptorsBase::Pose>&, float, float, float, bool)>(
                [self](const std::vector<RpcLibAdaptorsBase::Pose>& poses, float scale, float thickness, float duration, bool is_persistent) {
                    std::vector<Pose> conv_poses; RpcLibAdaptorsBase::to(poses, conv_poses);
                    self->getWorldSimApi()->simPlotTransforms(conv_poses, scale, thickness, duration, is_persistent);
                }));
            rpc.bind("simPlotTransformsWithNames", std::function<void(const std::vector<RpcLibAdaptorsBase::Pose>&, const std::vector<std::string>, float, float, float, const std::vector<float>&, float)>(
                [self](const std::vector<RpcLibAdaptorsBase::Pose>& poses, const std::vector<std::string> names, float tf_scale, float tf_thickness, float text_scale, const std::vector<float>& text_color_rgba, float duration) {
                    std::vector<Pose> conv_poses; RpcLibAdaptorsBase::to(poses, conv_poses);
                    self->getWorldSimApi()->simPlotTransformsWithNames(conv_poses, names, tf_scale, tf_thickness, text_scale, text_color_rgba, duration);
                }));

            rpc.bind("simGetGroundTruthKinematics", std::function<RpcLibAdaptorsBase::KinematicsState(const std::string&)>(
                [self](const std::string& vehicle_name) -> RpcLibAdaptorsBase::KinematicsState {
                    const Kinematics::State& result = *self->getVehicleSimApi(vehicle_name)->getGroundTruthKinematics();
                    return RpcLibAdaptorsBase::KinematicsState(result);
                }));
            rpc.bind("simSetKinematics", std::function<void(const RpcLibAdaptorsBase::KinematicsState&, bool, const std::string&)>(
                [self](const RpcLibAdaptorsBase::KinematicsState& state, bool ignore_collision, const std::string& vehicle_name) {
                    self->getVehicleSimApi(vehicle_name)->setKinematics(state.to(), ignore_collision);
                }));
            rpc.bind("simGetGroundTruthEnvironment", std::function<RpcLibAdaptorsBase::EnvironmentState(const std::string&)>(
                [self](const std::string& vehicle_name) -> RpcLibAdaptorsBase::EnvironmentState {
                    const Environment::State& result = (*self->getVehicleSimApi(vehicle_name)->getGroundTruthEnvironment()).getState();
                    return RpcLibAdaptorsBase::EnvironmentState(result);
                }));
            rpc.bind("simCreateVoxelGrid", std::function<bool(const RpcLibAdaptorsBase::Vector3r&, int, int, int, float, const std::string&)>(
                [self](const RpcLibAdaptorsBase::Vector3r& position, int x, int y, int z, float res, const std::string& output_file) -> bool {
                    return self->getWorldSimApi()->createVoxelGrid(position.to(), x, y, z, res, output_file);
                }));

            rpc.bind("cancelLastTask", std::function<void(const std::string&)>(
                [self](const std::string& vehicle_name) { self->getVehicleApi(vehicle_name)->cancelLastTask(); }));

            rpc.bind("simSwapTextures", std::function<std::vector<std::string>(std::string, int, int, int)>(
                [self](std::string tag, int tex_id, int component_id, int material_id) -> std::vector<std::string> {
                    return *self->getWorldSimApi()->swapTextures(tag, tex_id, component_id, material_id);
                }));
            rpc.bind("simSetObjectMaterial", std::function<bool(const std::string&, const std::string&, int)>(
                [self](const std::string& object_name, const std::string& material_name, int component_id) -> bool {
                    return self->getWorldSimApi()->setObjectMaterial(object_name, material_name, component_id);
                }));
            rpc.bind("simSetObjectMaterialFromTexture", std::function<bool(const std::string&, const std::string&, int)>(
                [self](const std::string& object_name, const std::string& texture_path, int component_id) -> bool {
                    return self->getWorldSimApi()->setObjectMaterialFromTexture(object_name, texture_path, component_id);
                }));

            rpc.bind("startRecording", std::function<void()>([self]() { self->getWorldSimApi()->startRecording(); }));
            rpc.bind("stopRecording", std::function<void()>([self]() { self->getWorldSimApi()->stopRecording(); }));
            rpc.bind("isRecording", std::function<bool()>([self]() -> bool { return self->getWorldSimApi()->isRecording(); }));

            rpc.bind("simSetWind", std::function<void(const RpcLibAdaptorsBase::Vector3r&)>(
                [self](const RpcLibAdaptorsBase::Vector3r& wind) { self->getWorldSimApi()->setWind(wind.to()); }));
            rpc.bind("simSetExtForce", std::function<void(const RpcLibAdaptorsBase::Vector3r&)>(
                [self](const RpcLibAdaptorsBase::Vector3r& ext_force) { self->getWorldSimApi()->setExtForce(ext_force.to()); }));

            rpc.bind("listVehicles", std::function<std::vector<std::string>()>(
                [self]() -> std::vector<std::string> { return self->getWorldSimApi()->listVehicles(); }));
            rpc.bind("getSettingsString", std::function<std::string()>(
                [self]() -> std::string { return self->getWorldSimApi()->getSettingsString(); }));
            rpc.bind("simFindLookAtRotation", std::function<RpcLibAdaptorsBase::Vector3r(const std::string&, const std::string&)>(
                [self](const std::string& vehicle_name, const std::string& object_name) -> RpcLibAdaptorsBase::Vector3r {
                    return RpcLibAdaptorsBase::Vector3r(self->getWorldSimApi()->findLookAtRotation(vehicle_name, object_name));
                }));
        }

        ZenohRpc rpc;
        ApiProvider* api_provider_;
    };

    RpcLibServerBase::RpcLibServerBase(ApiProvider* api_provider, const std::string& server_address, uint16_t port)
        : api_provider_(api_provider)
    {
        pimpl_.reset(new impl(api_provider));
        pimpl_->bindAll(this);
    }

    RpcLibServerBase::~RpcLibServerBase() { stop(); }

    void RpcLibServerBase::start(bool block, std::size_t thread_count) {}
    void RpcLibServerBase::stop() {}

    void* RpcLibServerBase::getServer() const { return nullptr; }
    ZenohRpc& RpcLibServerBase::getZenohRpc() { return pimpl_->rpc; }
}
} //namespace
