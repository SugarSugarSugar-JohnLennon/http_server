#!/bin/bash
set -e
if [ ! -f build/http_server ]; then
  echo "[start.sh] Executable not found. Please run ./build.sh first."
  exit 1
fi
nohup ./build/http_server > http_server.log 2>&1 &
echo $! > http_server.pid
echo "[start.sh] Server started. PID: $(cat http_server.pid)" 