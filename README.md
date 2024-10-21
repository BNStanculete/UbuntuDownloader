# UbuntuDownloader

The application connects to https://cloud-images.ubuntu.com/releases/streams/v1/com.ubuntu.cloud:released:download.json
and provides method to easily retrieve the hash of any disk image and check the latest supported versions.

## Prerequisites

This application makes use of Boost Http library.

As an initial step make sure to download and install the library for your OS.

## Build instructions

For a quick (production) build run
```
cmake . -B build -DCMAKE_BUILD_TYPE="Release" -DJSONCPP_WITH_TESTS=OFF -DBUILD_DOCS=OFF -DBUILD_TESTS=OFF
cmake --build build --config Release
cmake --install application
```

The executable will be then located in build/bin. If you want to build the documentation, first
make sure to install the following additional prerequisites:
- Python3 & pip
- Doxygen

After that, make sure to run ```pip install -r docs/requirements.txt``` to have all the necessary
packages for Sphinx to run. Then you follow the build steps above, but remove the ```-DBUILD_DOCS=OFF``` option.

## Usage

For a comprehensive list of the arguments supported run
```
UbuntuDownloader -h
```

## Git repository

https://github.com/BNStanculete/UbuntuDownloader