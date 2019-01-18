#!/bin/sh

if [ "$#" -ge 1 ]; then
    pid=`ps.sh |grep $1|egrep -v gdb|egrep -v "grep"|egrep -v vim|egrep -v tail|egrep -v truss|egrep -v run_process|awk '{ print $2 }'`
    process=`ps.sh |grep $1|egrep -v gdb|egrep -v "grep"|egrep -v vim|egrep -v tail|egrep -v truss|egrep -v run_process|awk '{ print $8 }'`
    echo "gdb -p [$pid] [$process]"
    gdb -p $pid $process
else
    echo "usage : gdb.sh psname...."
fi
exit
