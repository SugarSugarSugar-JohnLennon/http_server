#!/bin/bash
if [ ! -f http_server.pid ]; then
  echo "[stop.sh] No PID file found. Server may not be running."
  exit 1
fi
PID=$(cat http_server.pid)
if kill $PID 2>/dev/null; then
  echo "[stop.sh] Server (PID $PID) stopped."
  rm -f http_server.pid
else
  echo "[stop.sh] Failed to stop server or already stopped."
fi 