#! /bin/bash
set -e

PLATFORM="$(uname -s)"
ARCH="$(uname -m)"

if [ "$PLATFORM" == "Darwin" ]; then
    OS="mac"
    if [ "$ARCH" == "arm64" ]; then
        BUILD_PLATFORM="arm64"
    else
        BUILD_PLATFORM="x64"
    fi
else
    OS="linux"
    BUILD_PLATFORM="x64"
fi

RPC_VERSION_FOLDER="rpclib-2.3.0"
build_dir=build
folder_name="Release"

mkdir -p $build_dir
pushd $build_dir >/dev/null

if [ "$OS" == "mac" ]; then
    cmake ../cmake -DCMAKE_BUILD_TYPE=Release \
                   -DAPPLE_METAL_4=ON
else
    CC=${CC:-clang-18} CXX=${CXX:-clang++-18} cmake ../cmake \
        -DCMAKE_CXX_FLAGS='-stdlib=libc++ -I/usr/lib/llvm-18/include/c++/v1'
fi

make -j$(sysctl -n hw.ncpu 2>/dev/null || nproc)

popd >/dev/null

mkdir -p AirLib/lib/$BUILD_PLATFORM/$folder_name
mkdir -p AirLib/deps/MavLinkCom/lib

cp $build_dir/output/lib/libAirLib.a AirLib/lib/
cp $build_dir/output/lib/libMavLinkCom.a AirLib/deps/MavLinkCom/lib/

rsync -a --delete $build_dir/output/lib/ AirLib/lib/$BUILD_PLATFORM/$folder_name
rsync -a --delete MavLinkCom/include AirLib/deps/MavLinkCom/
rsync -a --delete AirLib Unreal/Plugins/AirSim/Source/
rm -rf Unreal/Plugins/AirSim/Source/AirLib/src

echo "=================================================================="
echo " Bristol (Colosseum) plugin is built for $OS ($BUILD_PLATFORM)!"
echo "=================================================================="
