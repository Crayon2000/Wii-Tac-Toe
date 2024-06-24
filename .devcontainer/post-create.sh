#!/bin/sh
set -ex

curl --location https://github.com/Kitware/CMake/releases/download/v3.29.6/cmake-3.29.6-linux-x86_64.sh --output /tmp/cmake.sh 
sh /tmp/cmake.sh --skip-license --prefix=/usr
rm -rf /tmp/cmake.sh 
