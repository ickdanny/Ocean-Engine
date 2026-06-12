#!/bin/bash

# wrapper script to set cwd properly
EXEC="$(cd "$(dirname "$0")/../../.." && pwd)/executable_mac"
echo "$EXEC" > /tmp/exec.txt
cd "$(dirname "$0")/../../.."
pwd > /tmp/pwd.txt
exec "$EXEC"