#!/bin/bash

rm -rf /sys/fs/bpf/xdp/dispatch* 
rm -rf /sys/fs/bpf/{eth7,eth9}
killall xskfwd 2>/dev/null
ip addr flush dev eth7
ip addr flush dev eth9
./xdp-loader unload --all eth7
./xdp-loader unload --all eth9
rmmod bridge
rmmod stp
rmmod llc
#sysctl -w net.ipv4.conf.eth7.arp_filter=0
#sysctl -w net.ipv4.conf.eth7.arp_ignore=0
#sysctl -w net.ipv4.conf.eth7.arp_announce=0
#sysctl -w net.ipv4.conf.eth9.arp_filter=0
#sysctl -w net.ipv4.conf.eth9.arp_ignore=0
#sysctl -w net.ipv4.conf.eth9.arp_announce=0
ip link del br0


if [ "x$1" == "xclean" ]; then
    exit 0
fi
sleep .2

echo "begin create" 
#sysctl -w net.ipv4.conf.eth7.arp_filter=1
#sysctl -w net.ipv4.conf.eth7.arp_ignore=2
#sysctl -w net.ipv4.conf.eth7.arp_announce=2
#sysctl -w net.ipv4.conf.eth9.arp_filter=1
#sysctl -w net.ipv4.conf.eth9.arp_ignore=2
#sysctl -w net.ipv4.conf.eth9.arp_announce=2
# add intf
#ip addr add 12.0.0.21/24 dev eth7
#ip addr add 12.0.0.23/24 dev eth9
sysctl -w net.ipv4.ip_forward=1
ethtool -L eth7 combined 1
ethtool -L eth9 combined 1
ethtool -G eth7 rx 4096
ethtool -G eth7 tx 4096
ethtool -G eth9 rx 4096
ethtool -G eth9 tx 4096
ip link set eth7 up
ip link set eth9 up
# must be needed for afxdp to work on arp, bridge auto set this
ip link set eth7 promisc on
ip link set eth9 promisc on
ip link set eth7 allmulticast on
ip link set eth9 allmulticast on
#ip link add name br0 type bridge
#ip link set br0 up
#ip link set dev eth7 master br0
#ip link set dev eth9 master br0

(cd cafwd; ./xskfwd -i eth7 -q 0 -m 68:91:d0:6f:6d:41 -i eth9 -q 0 -c 4 -m 68:91:d0:6f:6d:3f ) &
#cd cafwd; gdb ./xskfwd 
# set args -i eth7 -q 0 -m 68:91:d0:6f:6d:41 -i eth9 -q 0 -c 2 -m 68:91:d0:6f:6d:3f
echo "done"
