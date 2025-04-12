#!/bin/bash

SERVER="localhost"
PORT=1024
NICK="Tester"
USER="tester"
CHANNEL="#test"
PASS="toto"

(
sleep 1
echo "PASS $PASS"    
sleep 1
echo "NICK $NICK"
sleep 1
echo "USER $USER 0 * :Test User"
sleep 1
echo "JOIN $CHANNEL"
sleep 1

# Become channel operator (assume you allow self-op on new channel)
echo "MODE $CHANNEL +o $NICK"
sleep 1

# Set invite-only and key
echo "MODE $CHANNEL +ik secret"
sleep 1

# Set limit and another operator (pretend we had UserB)
echo "MODE $CHANNEL +ol 5 Tester"
sleep 1

# Remove key, limit, and invite-only
echo "MODE $CHANNEL -kli"
sleep 1

# Invalid: missing parameter for +k
echo "MODE $CHANNEL +k"
sleep 1

# Invalid: non-op tries to set mode (we’ll simulate by removing op)
echo "MODE $CHANNEL -o $NICK"
sleep 1
echo "MODE $CHANNEL +i"  # should fail: not op anymore
sleep 1

# Query current mode
echo "MODE $CHANNEL"
sleep 1

# Exit
echo "PART $CHANNEL"
sleep 1
echo "QUIT :Done testing"
) | nc -C $SERVER $PORT
