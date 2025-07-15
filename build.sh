#!/bin/bash
set -e
mkdir -p build
cd build
cmake ..
make -j$(nproc)
cd ..
echo "[build.sh] Build finished. Executable: build/http_server" 