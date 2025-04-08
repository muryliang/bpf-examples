#!/bin/bash

rm -rf /sys/fs/bpf/xdp/dispatch* 
rm -rf /sys/fs/bpf/{eth1,eth4}
killall xskfwd 2>/dev/null
ip addr flush dev eth1
ip addr flush dev eth4


if [ "x$1" == "xclean" ]; then
    exit 0
fi
sleep .2

echo "begin create" 
# add intf
ip addr add 12.0.0.21/24 dev eth1
ip addr add 13.0.0.23/24 dev eth4
ip link set eth1 up
ip link set eth4 up

(cd cafwd; ./xskfwd -i eth1 -q 0 -i eth4 -q 0 -c 2 ) &
echo "done"
