#!/bin/bash

HOST="localhost"
PORT=1024
NICK="tutu"
USER="netcat 0 * :real name"
PASS="toto"

# nc -C $HOST $PORT << EOF
# PASS $PASS
# NICK $NICK
# USER $USER 0 * :Real User
# EOF

{ echo "PASS $PASS"; echo "NICK $NICK"; echo "USER $USER"; cat; } | nc -C $HOST $PORT
