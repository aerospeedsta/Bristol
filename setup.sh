#!/usr/bin/env bash
set -x
set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$SCRIPT_DIR" >/dev/null

downloadHighPolySuv=true

MIN_CMAKE_VERSION=3.25.0

function brew_install() { brew list "$1" &>/dev/null || brew install "$1"; }

while [[ $# -gt 0 ]]
do
key="$1"
case $key in
    --no-full-poly-car)
        downloadHighPolySuv=false
        shift
        ;;
esac
done

if [ "$(uname)" == "Darwin" ]; then
    brew update
    brew_install wget
    brew_install coreutils
else
    sudo apt-get update
    sudo apt-get -y install --no-install-recommends \
        lsb-release \
        rsync \
        software-properties-common \
        wget \
        libvulkan1 \
        vulkan-utils \
        build-essential
fi

if ! which cmake; then
    echo "cmake not found. Install cmake >= $MIN_CMAKE_VERSION"
    if [ "$(uname)" == "Darwin" ]; then
        brew install cmake
    else
        sudo apt-get install -y cmake
    fi
fi

cmake_ver=$(cmake --version 2>&1 | head -n1 | cut -d ' ' -f3 | awk '{print $NF}')
echo "cmake version: $cmake_ver"

# Zenoh is vendored under AirLib/deps/zenoh/ — no download needed.

# Download high-polycount SUV model
if $downloadHighPolySuv; then
    if [ ! -d "Unreal/Plugins/AirSim/Content/VehicleAdv" ]; then
        mkdir -p "Unreal/Plugins/AirSim/Content/VehicleAdv"
    fi
    if [ ! -d "Unreal/Plugins/AirSim/Content/VehicleAdv/SUV/v1.2.0" ]; then
        echo "*********************************************************************************************"
        echo "Downloading high-poly car assets.... (~37MB)"
        echo "*********************************************************************************************"
        if [ -d "suv_download_tmp" ]; then rm -rf "suv_download_tmp"; fi
        mkdir -p "suv_download_tmp"
        cd suv_download_tmp
        wget https://github.com/CodexLabsLLC/Colosseum/releases/download/v2.0.0-beta.0/car_assets.zip
        if [ -d "../Unreal/Plugins/AirSim/Content/VehicleAdv/SUV" ]; then
            rm -rf "../Unreal/Plugins/AirSim/Content/VehicleAdv/SUV"
        fi
        unzip -q car_assets.zip -d ../Unreal/Plugins/AirSim/Content/VehicleAdv
        cd ..
        rm -rf "suv_download_tmp"
    fi
else
    echo "### Not downloading high-poly car asset (--no-full-poly-car)."
fi

echo "Eigen is managed by mise (conan:eigen). Run 'mise install' to install."

popd >/dev/null

set +x
echo ""
echo "************************************"
echo "Setup completed successfully!"
echo "************************************"
