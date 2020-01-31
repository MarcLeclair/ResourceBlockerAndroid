# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.4.1)

# Specifies a library name, specifies whether the library is STATIC or
# SHARED, and provides relative paths to the source code. You can
# define multiple libraries by adding multiple add_library() commands,
# and CMake builds them for you. When you build your app, Gradle
# automatically packages shared libraries with your APK.

#
#project(App)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
#
#include(AndroidNdkGdb)
#include(AndroidNdkModules)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/src/main/libs/armeabi-v7a)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

include_directories(/Users/mleclair/Library/Android/sdk/ndk)
include_directories(/Users/mleclair/Library/Android/sdk/ndk/21.0.6113669/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/android)

find_library(log-lib log
        PATHS /Users/mleclair/Library/Android/sdk/ndk/21.0.6113669/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/lib
        NO_DEFAULT_PATH)

find_library(android log
        PATHS /Users/mleclair/Library/Android/sdk/ndk/21.0.6113669/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/
        NO_DEFAULT_PATH)


#add_library(log SHARED /Users/mleclair/Library/Android/sdk/ndk/21.0.6113669/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/android/)

add_library( # Specifies the name of the library.
             native-lib
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             src/main/cpp/ResourceBlocker.cpp
             src/main/cpp/JNIResourceBlocker.cpp
            )

target_link_libraries(native-lib
        android
        log)

#target_link_libraries(native-lib
#        ${android})
#
#target_link_libraries(native-lib
#        android)

