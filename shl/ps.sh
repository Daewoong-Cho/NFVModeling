#!/bin/sh

if [ "$#" -ge 1 ]; then
    ps -f -u $USER | grep -v "ps.sh" | grep -v "grep" | grep $1
else
    ps -f -u $USER | grep -v "ps.sh" | grep -v "grep"
fi
