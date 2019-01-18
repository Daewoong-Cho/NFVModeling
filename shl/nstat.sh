
if [ $# -ge 1 ]; then
	netstat -an | grep $1
else
    echo "usage : nstat.sh PORTNO...."
fi
exit
