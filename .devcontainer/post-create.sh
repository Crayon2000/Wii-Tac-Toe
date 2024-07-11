#!/bin/sh
set -ex

# Remove installed version of cmake
apt-get purge cmake -y

# Install required version
curl --location https://github.com/Kitware/CMake/releases/download/v3.30.0/cmake-3.30.0-linux-x86_64.sh --output /tmp/cmake.sh
sh /tmp/cmake.sh --skip-license --prefix=/usr
rm -rf /tmp/cmake.sh
