#!/bin/sh

if [ $# -ge 1 ]; then
    ps.sh | grep $1 |grep -v make | grep -v grep | grep -v gdb | grep -v pkill | grep -v tail | grep -v vim | awk '{ print "USER=[" $1 "] PID=[" $2 "] [" $8 "]" }'
    ps.sh | grep $1 |grep -v make | grep -v grep | grep -v gdb | grep -v pkill | grep -v tail | grep -v vim | awk '{ print "kill -12 " $2 }' | sh
else
    echo "usage : pk.sh psname...."
fi
exit
