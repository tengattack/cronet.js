#!/bin/bash
set -e

NDK_PATH=~/Library/OpenHarmony/sdk/5.0.5/native
CMAKE=$NDK_PATH/build-tools/cmake/bin/cmake
STRIP=$NDK_PATH/llvm/bin/llvm-strip

arch=arm64-v8a

rm -rf build

$CMAKE -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK_PATH/build/cmake/ohos.toolchain.cmake \
  -DOHOS_PLATFORM=OHOS \
  -DOHOS_ARCH=$arch \
  -DOHOS_STL=c++_static \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_CXX_STANDARD_REQUIRED=ON \
  -DCMAKE_CXX_EXTENSIONS=OFF

cd build
ninja

mkdir -p ../libs/$arch
mkdir -p ../obj/$arch
$STRIP -s libcronetjs.so -o ../libs/$arch/libcronetjs.so
mv libcronetjs.so ../obj/$arch

cd ..
