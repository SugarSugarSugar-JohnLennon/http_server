#!/bin/bash
if [ ! -f http_server.log ]; then
  echo "[log.sh] http_server.log not found."
  exit 1
fi
tail -f http_server.log 