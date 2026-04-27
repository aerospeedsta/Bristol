# Bristol

Autonomous vehicle simulator for macOS (Apple Silicon). Fork of AirSim/Colosseum with Zenoh-based RPC, UE 5.7 support, and modernized build tooling.

## Status

Experimental. The core simulation stack (physics, sensors, flight controllers, MAVLink) runs on macOS arm64. The UE 5.7 plugin builds and links. Python API works via Zenoh. Godot integration is planned.

**What works:**
- Native libs: AirLib, MavLinkCom (cmake + Apple Clang)
- UE 5.7 plugin: BlocksV2Editor builds and runs on arm64
- Vehicle models: SimpleFlight, PX4/ArduPilot (via MAVLink), Chaos Car
- RPC layer: Zenoh get/queryable (replaces rpclib)
- Python client: API via `eclipse-zenoh` + msgpack

**What's partial:**
- Godot engine port — not started
- PX4 SITL on macOS — needs Docker or manual setup
- Headless mode for ML training — not implemented
- Recording / deterministic replay — not implemented

## Quick start

```bash
# Install toolchain
mise install

# Download C++ deps (Eigen, car assets)
./setup.sh

# Build native libs (AirLib, MavLinkCom)
./build.sh

# Install Python client
cd PythonClient && uv sync

# Generate UE Xcode project
Unreal/Environments/BlocksV2/GenerateProjectFiles.sh "/Users/Shared/Epic Games/UE_5.7"

# Open Xcode workspace and build
open "Unreal/Environments/BlocksV2/BlocksV2 (Mac).xcworkspace"
```

## Dependencies

| Dep | Source | Managed by |
|---|---|---|
| cmake, python, uv | — | `mise` |
| zenoh-c, zenoh-cpp | GitHub releases | `mise github:` |
| nlohmann/json | GitHub releases | `mise github:` |
| Eigen | ConanCenter | `setup.sh` → `conan install` |
| msgpack-c | Vendored (AirLib/deps/msgpack/) | — |
| UE 5.7 | Epic Games Launcher | Manual |

## Architecture

```
AirLib/              → Standalone C++ simulation SDK (physics, sensors, flight controllers)
  ├── MavLinkCom/    → MAVLink transport for PX4 / ArduPilot
  ├── ZenohRpc       → RPC layer (replaces rpclib)
  └── deps/          → Eigen, msgpack-c

Unreal/Plugins/AirSim → UE 5.7 plugin (render layer)
PythonClient/         → Python API via zenoh + msgpack
```

Clients communicate with the simulator via Zenoh get/queryable RPC over msgpack-serialized payloads.

## Vehicle types

- SimpleFlight (built-in flight controller, no external firmware needed)
- PX4 Multirotor (via MAVLink)
- ArduPilot Copter / Rover (via MAVLink)
- Chaos Car

## License

Bristol is a fork of [AirSim](https://github.com/microsoft/AirSim) (MIT) and [Colosseum](https://github.com/CodexLabsLLC/Colosseum) (MIT).

Copyright (c) 2026 Bristol contributors

MIT License — see [LICENSE](LICENSE) for details.
