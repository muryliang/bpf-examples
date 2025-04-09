export BPF_CFLAGS="-g -fPIE"
export CFLAGS="-g -fPIE"
rm -rf ../lib/install
rm -rf ../install
(cd ../lib/xdp-tools/lib/libxdp; make clean)
make clean
find .. -name '*.o' | xargs -n 100 rm -f
#export BPF_DIR_MNT=/etc/bpf 
make
