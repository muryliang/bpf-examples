# see https://doc.dpdk.org/guides-21.08/nics/af_xdp.html
#echo 200000 >  /sys/class/net/enp7s0/gro_flush_timeout 
#echo 2 >  /sys/class/net/enp7s0/napi_defer_hard_irqs 
#echo 200000 >  /sys/class/net/enp8s0/gro_flush_timeout 
#echo 2 >  /sys/class/net/enp8s0/napi_defer_hard_irqs 
#echo 0 >  /sys/class/net/enp7s0/gro_flush_timeout 
#echo 0 >  /sys/class/net/enp7s0/napi_defer_hard_irqs 
#echo 0 >  /sys/class/net/enp8s0/gro_flush_timeout 
#echo 0 >  /sys/class/net/enp8s0/napi_defer_hard_irqs 

echo '1' > /proc/irq/49/smp_affinity_list
echo '2' > /proc/irq/50/smp_affinity_list
echo '1' > /proc/irq/52/smp_affinity_list
echo '3' > /proc/irq/53/smp_affinity_list
