cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

if(WIN32)
  set(CMAKE_C_COMPILER clang-cl)
  set(CMAKE_CXX_COMPILER clang-cl)
elseif(UNIX)
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)
endif()

project(cronetjs)

option(LINK_CRONET "Direct link the cronet library" ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -fdata-sections -ffunction-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -fdata-sections -ffunction-sections")

#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections -Wl,--exclude-libs,ALL")

add_definitions(-DNAPI_VERSION=8)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")

  add_definitions(-D_DEBUG)
  list(APPEND EXTRA_LIBS clang_rt.asan)
endif()

if(LINK_CRONET)
  add_definitions(-DLINK_CRONET)
endif()

set(LOCAL_PATH ${CMAKE_SOURCE_DIR}/../..)
set(SRC_PATH ${LOCAL_PATH}/src)

include_directories(${CMAKE_JS_INC} "${LOCAL_PATH}/third_party/cronet/include")

if(WIN32)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

file(GLOB SOURCE_FILES "${SRC_PATH}/*.cc")

find_library(
  # Sets the name of the path variable.
  hilog-lib
  # Specifies the name of the NDK library that
  # you want CMake to locate.
  hilog_ndk.z
)

find_library(
  # Sets the name of the path variable.
  libace-lib
  # Specifies the name of the NDK library that
  # you want CMake to locate.
  ace_ndk.z
)

find_library(
  # Sets the name of the path variable.
  libnapi-lib
  # Specifies the name of the NDK library that
  # you want CMake to locate.
  ace_napi.z
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

if(LINK_CRONET)
  list(APPEND EXTRA_LIBS "${LOCAL_PATH}/third_party/cronet/lib/ohos/libbilicr.so")
endif()

target_link_libraries(
  ${PROJECT_NAME}

  ${hilog-lib}
  ${libace-lib}
  ${libnapi-lib}

  ${EXTRA_LIBS}
)