rm -rf ../lib/install
rm -rf ../install
(cd ../lib/xdp-tools/lib/libxdp; make clean)
make clean
export BPF_DIR_MNT=/etc/bpf 
make
