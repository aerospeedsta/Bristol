from __future__ import print_function

from .utils import *
from .types import *

import msgpack
import numpy as np
import time
import math
import logging

class VehicleClient:
    def __init__(self, ip = "", port = 41451, timeout_value = 3600):
        import zenoh
        config = zenoh.Config()
        self._session = zenoh.open(config)
        self._rpc_prefix = "airsim/rpc/"

    def _call(self, name, *args):
        data = msgpack.packb(args)
        replies = self._session.get(self._rpc_prefix + name, payload=data)
        for reply in replies:
            if reply.ok:
                payload = reply.ok.payload.to_bytes()
                if payload and len(payload) > 0:
                    return msgpack.unpackb(payload)
                return None
        return None

    def _call_void(self, name, *args):
        data = msgpack.packb(args)
        self._session.get(self._rpc_prefix + name, payload=data)

    def __del__(self):
        if hasattr(self, '_session'):
            self._session.close()

    def reset(self):
        self._call_void('reset')

    def ping(self):
        return self._call('ping')

    def getClientVersion(self):
        return 1

    def getServerVersion(self):
        return self._call('getServerVersion')

    def getMinRequiredServerVersion(self):
        return 1

    def getMinRequiredClientVersion(self):
        return self._call('getMinRequiredClientVersion')

    def enableApiControl(self, is_enabled, vehicle_name = ''):
        self._call_void('enableApiControl', is_enabled, vehicle_name)

    def isApiControlEnabled(self, vehicle_name = ''):
        return self._call('isApiControlEnabled', vehicle_name)

    def armDisarm(self, arm, vehicle_name = ''):
        return self._call('armDisarm', arm, vehicle_name)

    def simPause(self, is_paused):
        self._call_void('simPause', is_paused)

    def simIsPaused(self):
        return self._call('simIsPaused')

    def simContinueForTime(self, seconds):
        self._call_void('simContinueForTime', seconds)

    def simContinueForFrames(self, frames):
        self._call_void('simContinueForFrames', frames)

    def simSetTimeOfDay(self, is_enabled, start_datetime = "", is_start_datetime_dst = False, celestial_clock_speed = 1, update_interval_secs = 60, move_sun = True):
        self._call_void('simSetTimeOfDay', is_enabled, start_datetime, is_start_datetime_dst, celestial_clock_speed, update_interval_secs, move_sun)

    def simEnableWeather(self, enable):
        self._call_void('simEnableWeather', enable)

    def simSetWeatherParameter(self, param, val):
        self._call_void('simSetWeatherParameter', param, val)

    def simGetObjectPose(self, object_name):
        return self._call('simGetObjectPose', object_name)

    def simGetObjectScale(self, object_name):
        return self._call('simGetObjectScale', object_name)

    def simSetObjectPose(self, object_name, pose, teleport = True):
        return self._call('simSetObjectPose', object_name, pose, teleport)

    def simSetObjectScale(self, object_name, scale):
        return self._call('simSetObjectScale', object_name, scale)

    def simListSceneObjects(self, name_regex = '.*'):
        return self._call('simListSceneObjects', name_regex)

    def simListSceneObjectsByTag(self, tag_regex = '.*'):
        return self._call('simListSceneObjectsByTag', tag_regex)

    def simGetVehiclePose(self, vehicle_name = ''):
        return self._call('simGetVehiclePose', vehicle_name)

    def simSetVehiclePose(self, pose, ignore_collision, vehicle_name = ''):
        self._call_void('simSetVehiclePose', pose, ignore_collision, vehicle_name)

    def simSetTraceLine(self, color_rgba, thickness = 3.0, vehicle_name = ''):
        self._call_void('simSetTraceLine', color_rgba, thickness, vehicle_name)

    def simGetCollisionInfo(self, vehicle_name = ''):
        return self._call('simGetCollisionInfo', vehicle_name)

    def simGetCameraInfo(self, camera_name, vehicle_name = '', external = False):
        return self._call('simGetCameraInfo', camera_name, vehicle_name, external)

    def simSetCameraPose(self, camera_name, pose, vehicle_name = '', external = False):
        self._call_void('simSetCameraPose', camera_name, pose, vehicle_name, external)

    def simSetCameraFov(self, camera_name, fov_degrees, vehicle_name = '', external = False):
        self._call_void('simSetCameraFov', camera_name, fov_degrees, vehicle_name, external)

    def simGetImages(self, requests, vehicle_name = '', external = False):
        responses = self._call('simGetImages', requests, vehicle_name, external)
        if responses is None:
            return []
        return responses

    def simGetImage(self, camera_name, image_type, vehicle_name = '', external = False):
        result = self._call('simGetImage', camera_name, image_type, vehicle_name, external)
        if result is None:
            return bytearray()
        return bytearray(result)

    def simSetSegmentationObjectID(self, mesh_name, object_id, is_name_regex = False):
        return self._call('simSetSegmentationObjectID', mesh_name, object_id, is_name_regex)

    def simGetSegmentationObjectID(self, mesh_name):
        return self._call('simGetSegmentationObjectID', mesh_name)

    def simPrintLogMessage(self, message, message_param = "", severity = 0):
        self._call_void('simPrintLogMessage', message, message_param, severity)

    def simGetObjectPose(self, object_name):
        return self._call('simGetObjectPose', object_name)

    def simSetObjectPose(self, object_name, pose, teleport = True):
        return self._call('simSetObjectPose', object_name, pose, teleport)

    def simLoadLevel(self, level_name):
        return self._call('simLoadLevel', level_name)

    def simSpawnObject(self, object_name, load_component, pose, scale, physics_enabled = False, is_blueprint = False):
        return self._call('simSpawnObject', object_name, load_component, pose, scale, physics_enabled, is_blueprint)

    def simDestroyObject(self, object_name):
        return self._call('simDestroyObject', object_name)

    def simGetObjectScale(self, object_name):
        return self._call('simGetObjectScale', object_name)

    def simSetObjectScale(self, object_name, scale):
        return self._call('simSetObjectScale', object_name, scale)

    def simListAssets(self):
        return self._call('simListAssets')

    def simRunConsoleCommand(self, command):
        return self._call('simRunConsoleCommand', command)

    def simCreateVoxelGrid(self, position, x, y, z, res, output_file):
        return self._call('simCreateVoxelGrid', position, x, y, z, res, output_file)

    def simTestLineOfSightBetweenPoints(self, point1, point2):
        return self._call('simTestLineOfSightBetweenPoints', point1, point2)

    def simTestLineOfSightToPoint(self, point, vehicle_name = ''):
        return self._call('simTestLineOfSightToPoint', point, vehicle_name)

    def simGetWorldExtents(self):
        return self._call('simGetWorldExtents')

    def simAddVehicle(self, vehicle_name, vehicle_type, pose, pawn_path = ""):
        return self._call('simAddVehicle', vehicle_name, vehicle_type, pose, pawn_path)

    def getHomeGeoPoint(self, vehicle_name = ''):
        return self._call('getHomeGeoPoint', vehicle_name)

    def confirmConnection(self):
        print("Connected!")

    def simSetWind(self, wind, vehicle_name = ''):
        self._call_void('simSetWind', wind, vehicle_name)

    def simSetExtForce(self, ext_force, vehicle_name = ''):
        self._call_void('simSetExtForce', ext_force, vehicle_name)

    def listVehicles(self):
        return self._call('listVehicles')

    def simGetGroundTruthKinematics(self, vehicle_name = ''):
        return self._call('simGetGroundTruthKinematics', vehicle_name)

    def simGetGroundTruthEnvironment(self, vehicle_name = ''):
        return self._call('simGetGroundTruthEnvironment', vehicle_name)

    def simGetMeshPositionVertexBuffers(self):
        return self._call('simGetMeshPositionVertexBuffers')

    def getSettingsString(self):
        return self._call('getSettingsString')

    def simSwapTextures(self, tags, tex_id = 0, component_id = 0, material_id = 0):
        return self._call('simSwapTextures', tags, tex_id, component_id, material_id)

    def simSetObjectMaterial(self, object_name, material_name, component_id = 0):
        return self._call('simSetObjectMaterial', object_name, material_name, component_id)

    def simSetObjectMaterialFromTexture(self, object_name, texture_path, component_id = 0):
        return self._call('simSetObjectMaterialFromTexture', object_name, texture_path, component_id)

    def simAddDetectionFilterMeshName(self, camera_name, image_type, mesh_name, vehicle_name = "", external = False):
        self._call_void('simAddDetectionFilterMeshName', camera_name, image_type, mesh_name, vehicle_name, external)

    def simSetDetectionFilterRadius(self, camera_name, image_type, radius_cm, vehicle_name = "", external = False):
        self._call_void('simSetDetectionFilterRadius', camera_name, image_type, radius_cm, vehicle_name, external)

    def simClearDetectionMeshNames(self, camera_name, image_type, vehicle_name = "", external = False):
        self._call_void('simClearDetectionMeshNames', camera_name, image_type, vehicle_name, external)

    def simGetDetections(self, camera_name, image_type, vehicle_name = "", external = False):
        return self._call('simGetDetections', camera_name, image_type, vehicle_name, external)

    def simFlushPersistentMarkers(self):
        self._call_void('simFlushPersistentMarkers')

    def simPlotPoints(self, points, color_rgba, size = 10.0, duration = 3.0, is_persistent = True):
        self._call_void('simPlotPoints', points, color_rgba, size, duration, is_persistent)

    def simPlotLineStrip(self, points, color_rgba, thickness = 3.0, duration = 3.0, is_persistent = True):
        self._call_void('simPlotLineStrip', points, color_rgba, thickness, duration, is_persistent)

    def simPlotLineList(self, points, color_rgba, thickness = 3.0, duration = 3.0, is_persistent = True):
        self._call_void('simPlotLineList', points, color_rgba, thickness, duration, is_persistent)

    def simPlotArrows(self, points_start, points_end, color_rgba, thickness = 3.0, arrow_size = 10.0, duration = 3.0, is_persistent = True):
        self._call_void('simPlotArrows', points_start, points_end, color_rgba, thickness, arrow_size, duration, is_persistent)

    def simPlotStrings(self, strings, positions, scale = 1.0, color_rgba = [1.0, 0.0, 0.0, 1.0], duration = 3.0):
        self._call_void('simPlotStrings', strings, positions, scale, color_rgba, duration)

    def simPlotTransforms(self, poses, scale = 1.0, thickness = 3.0, duration = 3.0, is_persistent = True):
        self._call_void('simPlotTransforms', poses, scale, thickness, duration, is_persistent)

    def simPlotTransformsWithNames(self, poses, names, tf_scale = 1.0, tf_thickness = 3.0, text_scale = 1.0, text_color_rgba = [1.0, 0.0, 0.0, 1.0], duration = 3.0):
        self._call_void('simPlotTransformsWithNames', poses, names, tf_scale, tf_thickness, text_scale, text_color_rgba, duration)

    def cancelLastTask(self, vehicle_name = ''):
        self._call_void('cancelLastTask', vehicle_name)

    def waitOnLastTask(self, task_result = None, timeout_sec = float('nan')):
        return self

    def startRecording(self):
        self._call_void('startRecording')

    def stopRecording(self):
        self._call_void('stopRecording')

    def isRecording(self):
        return self._call('isRecording')

    def simFindLookAtRotation(self, vehicle_name, object_name):
        return self._call('simFindLookAtRotation', vehicle_name, object_name)

    def simGetPresetLensSettings(self, camera_name, vehicle_name = "", external = False):
        return self._call('simGetPresetLensSettings', camera_name, vehicle_name, external)

    def simGetLensSettings(self, camera_name, vehicle_name = "", external = False):
        return self._call('simGetLensSettings', camera_name, vehicle_name, external)

    def simSetPresetLensSettings(self, preset, camera_name, vehicle_name = "", external = False):
        self._call_void('simSetPresetLensSettings', preset, camera_name, vehicle_name, external)

    def simGetPresetFilmbackSettings(self, camera_name, vehicle_name = "", external = False):
        return self._call('simGetPresetFilmbackSettings', camera_name, vehicle_name, external)

    def simSetPresetFilmbackSettings(self, preset, camera_name, vehicle_name = "", external = False):
        self._call_void('simSetPresetFilmbackSettings', preset, camera_name, vehicle_name, external)

    def simGetFilmbackSettings(self, camera_name, vehicle_name = "", external = False):
        return self._call('simGetFilmbackSettings', camera_name, vehicle_name, external)

    def simSetFilmbackSettings(self, width, height, camera_name, vehicle_name = "", external = False):
        return self._call('simSetFilmbackSettings', width, height, camera_name, vehicle_name, external)

    def simGetFocalLength(self, camera_name, vehicle_name = "", external = False):
        return self._call('simGetFocalLength', camera_name, vehicle_name, external)

    def simSetFocalLength(self, focal_length, camera_name, vehicle_name = "", external = False):
        self._call_void('simSetFocalLength', focal_length, camera_name, vehicle_name, external)

    def simEnableManualFocus(self, enable, camera_name, vehicle_name = "", external = False):
        self._call_void('simEnableManualFocus', enable, camera_name, vehicle_name, external)

    def simGetFocusDistance(self, camera_name, vehicle_name = "", external = False):
        return self._call('simGetFocusDistance', camera_name, vehicle_name, external)

    def simSetFocusDistance(self, focus_distance, camera_name, vehicle_name = "", external = False):
        self._call_void('simSetFocusDistance', focus_distance, camera_name, vehicle_name, external)

    def simGetFocusAperture(self, camera_name, vehicle_name = "", external = False):
        return self._call('simGetFocusAperture', camera_name, vehicle_name, external)

    def simSetFocusAperture(self, focus_aperture, camera_name, vehicle_name = "", external = False):
        self._call_void('simSetFocusAperture', focus_aperture, camera_name, vehicle_name, external)

    def simEnableFocusPlane(self, enable, camera_name, vehicle_name = "", external = False):
        self._call_void('simEnableFocusPlane', enable, camera_name, vehicle_name, external)

    def simGetCurrentFieldOfView(self, camera_name, vehicle_name = "", external = False):
        return self._call('simGetCurrentFieldOfView', camera_name, vehicle_name, external)


# ----------------------------------- Multirotor APIs ---------------------------------------------
class MultirotorClient(VehicleClient):
    def __init__(self, ip = "", port = 41451, timeout_value = 3600):
        super(MultirotorClient, self).__init__(ip, port, timeout_value)

    def takeoffAsync(self, timeout_sec = 20, vehicle_name = ''):
        self._call_void('takeoff', timeout_sec, vehicle_name)
        return self

    def landAsync(self, timeout_sec = 60, vehicle_name = ''):
        self._call_void('land', timeout_sec, vehicle_name)
        return self

    def goHomeAsync(self, timeout_sec = float('inf'), vehicle_name = ''):
        self._call_void('goHome', timeout_sec, vehicle_name)
        return self

    def moveByVelocityBodyFrameAsync(self, vx, vy, vz, duration, drivetrain = 3, yaw_mode = {}, vehicle_name = ''):
        self._call_void('moveByVelocityBodyFrame', vx, vy, vz, duration, drivetrain, yaw_mode, vehicle_name)
        return self

    def moveByVelocityZBodyFrameAsync(self, vx, vy, z, duration, drivetrain = 3, yaw_mode = {}, vehicle_name = ''):
        self._call_void('moveByVelocityZBodyFrame', vx, vy, z, duration, drivetrain, yaw_mode, vehicle_name)
        return self

    def moveByMotorPWMsAsync(self, front_right_pwm, rear_left_pwm, front_left_pwm, rear_right_pwm, duration, vehicle_name = ''):
        self._call_void('moveByMotorPWMs', front_right_pwm, rear_left_pwm, front_left_pwm, rear_right_pwm, duration, vehicle_name)
        return self

    def moveByRollPitchYawZAsync(self, roll, pitch, yaw, z, duration, vehicle_name = ''):
        self._call_void('moveByRollPitchYawZ', roll, pitch, yaw, z, duration, vehicle_name)
        return self

    def moveByRollPitchYawThrottleAsync(self, roll, pitch, yaw, throttle, duration, vehicle_name = ''):
        self._call_void('moveByRollPitchYawThrottle', roll, pitch, yaw, throttle, duration, vehicle_name)
        return self

    def moveByRollPitchYawrateThrottleAsync(self, roll, pitch, yaw_rate, throttle, duration, vehicle_name = ''):
        self._call_void('moveByRollPitchYawrateThrottle', roll, pitch, yaw_rate, throttle, duration, vehicle_name)
        return self

    def moveByRollPitchYawrateZAsync(self, roll, pitch, yaw_rate, z, duration, vehicle_name = ''):
        self._call_void('moveByRollPitchYawrateZ', roll, pitch, yaw_rate, z, duration, vehicle_name)
        return self

    def moveByAngleRatesZAsync(self, roll_rate, pitch_rate, yaw_rate, z, duration, vehicle_name = ''):
        self._call_void('moveByAngleRatesZ', roll_rate, pitch_rate, yaw_rate, z, duration, vehicle_name)
        return self

    def moveByAngleRatesThrottleAsync(self, roll_rate, pitch_rate, yaw_rate, throttle, duration, vehicle_name = ''):
        self._call_void('moveByAngleRatesThrottle', roll_rate, pitch_rate, yaw_rate, throttle, duration, vehicle_name)
        return self

    def moveByVelocityAsync(self, vx, vy, vz, duration, drivetrain = 3, yaw_mode = {}, vehicle_name = ''):
        self._call_void('moveByVelocity', vx, vy, vz, duration, drivetrain, yaw_mode, vehicle_name)
        return self

    def moveByVelocityZAsync(self, vx, vy, z, duration, drivetrain = 3, yaw_mode = {}, vehicle_name = ''):
        self._call_void('moveByVelocityZ', vx, vy, z, duration, drivetrain, yaw_mode, vehicle_name)
        return self

    def moveOnPathAsync(self, path, velocity, timeout_sec = float('inf'), drivetrain = 3, yaw_mode = {}, lookahead = -1, adaptive_lookahead = 1, vehicle_name = ''):
        self._call_void('moveOnPath', path, velocity, timeout_sec, drivetrain, yaw_mode, lookahead, adaptive_lookahead, vehicle_name)
        return self

    def moveToGPSAsync(self, latitude, longitude, altitude, velocity, timeout_sec = float('inf'), drivetrain = 3, yaw_mode = {}, lookahead = -1, adaptive_lookahead = 1, vehicle_name = ''):
        self._call_void('moveToGPS', latitude, longitude, altitude, velocity, timeout_sec, drivetrain, yaw_mode, lookahead, adaptive_lookahead, vehicle_name)
        return self

    def moveToPositionAsync(self, x, y, z, velocity, timeout_sec = float('inf'), drivetrain = 3, yaw_mode = {}, lookahead = -1, adaptive_lookahead = 1, vehicle_name = ''):
        self._call_void('moveToPosition', x, y, z, velocity, timeout_sec, drivetrain, yaw_mode, lookahead, adaptive_lookahead, vehicle_name)
        return self

    def moveToZAsync(self, z, velocity, timeout_sec = float('inf'), yaw_mode = {}, lookahead = -1, adaptive_lookahead = 1, vehicle_name = ''):
        self._call_void('moveToZ', z, velocity, timeout_sec, yaw_mode, lookahead, adaptive_lookahead, vehicle_name)
        return self

    def moveByManualAsync(self, vx_max, vy_max, z_min, duration, drivetrain = 3, yaw_mode = {}, vehicle_name = ''):
        self._call_void('moveByManual', vx_max, vy_max, z_min, duration, drivetrain, yaw_mode, vehicle_name)
        return self

    def rotateToYawAsync(self, yaw, timeout_sec = float('inf'), margin = 5, vehicle_name = ''):
        self._call_void('rotateToYaw', yaw, timeout_sec, margin, vehicle_name)
        return self

    def rotateByYawRateAsync(self, yaw_rate, duration, vehicle_name = ''):
        self._call_void('rotateByYawRate', yaw_rate, duration, vehicle_name)
        return self

    def hoverAsync(self, vehicle_name = ''):
        self._call_void('hover', vehicle_name)
        return self

    def setAngleLevelControllerGains(self, kp, ki, kd, vehicle_name = ''):
        self._call_void('setAngleLevelControllerGains', kp, ki, kd, vehicle_name)

    def setAngleRateControllerGains(self, kp, ki, kd, vehicle_name = ''):
        self._call_void('setAngleRateControllerGains', kp, ki, kd, vehicle_name)

    def setVelocityControllerGains(self, kp, ki, kd, vehicle_name = ''):
        self._call_void('setVelocityControllerGains', kp, ki, kd, vehicle_name)

    def setPositionControllerGains(self, kp, ki, kd, vehicle_name = ''):
        self._call_void('setPositionControllerGains', kp, ki, kd, vehicle_name)

    def moveByRC(self, rc_data, vehicle_name = ''):
        self._call_void('moveByRC', rc_data, vehicle_name)

    def getMultirotorState(self, vehicle_name = ''):
        return self._call('getMultirotorState', vehicle_name)

    def getRotorStates(self, vehicle_name = ''):
        return self._call('getRotorStates', vehicle_name)

    def setSafety(self, enable_reasons, obs_clearance, obs_startegy, obs_avoidance_vel, origin, xy_length, max_z, min_z, vehicle_name = ''):
        return self._call('setSafety', enable_reasons, obs_clearance, obs_startegy, obs_avoidance_vel, origin, xy_length, max_z, min_z, vehicle_name)


# ----------------------------------- Car APIs ---------------------------------------------
class CarClient(VehicleClient):
    def __init__(self, ip = "", port = 41451, timeout_value = 3600):
        super(CarClient, self).__init__(ip, port, timeout_value)

    def setCarControls(self, controls, vehicle_name = ''):
        self._call_void('setCarControls', controls, vehicle_name)

    def getCarState(self, vehicle_name = ''):
        return self._call('getCarState', vehicle_name)

    def getCarControls(self, vehicle_name = ''):
        return self._call('getCarControls', vehicle_name)
