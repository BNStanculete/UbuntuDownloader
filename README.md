# UbuntuDownloader

The application connects to https://cloud-images.ubuntu.com/releases/streams/v1/com.ubuntu.cloud:released:download.json
and provides method to easily retrieve the hash of any disk image and check the latest supported versions.

## Prerequisites

This application makes use of Boost Http library.

As an initial step make sure to download and install the library for your OS.

## Build instructions

For a quick (production) build run
```
cmake . -B build -DCMAKE_BUILD_TYPE="Release" -DJSONCPP_WITH_TESTS=OFF
cmake --build build --config Release
cmake --install application
```

The executable will be then located in build/bin.

## Usage

For a comprehensive list of the arguments supported run
```
UbuntuDownloader -h
```

## Git repository

https://github.com/BNStanculete/UbuntuDownloader