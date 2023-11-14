// SPDX-License-Identifier: GPL-2.0
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <xdp/parsing_helpers.h>

/* This XDP program is only needed for multi-buffer and XDP_SHARED_UMEM modes.
 * If you do not use these modes, libbpf can supply an XDP program for you.
 */

#if 1
struct {
	__uint(type, BPF_MAP_TYPE_XSKMAP);
	__uint(max_entries, 10);
	__uint(key_size, sizeof(int));
	__uint(value_size, sizeof(int));
} xsks_map SEC(".maps");

#else
#define memcpy(dest, src, n) __builtin_memcpy((dest), (src), (n))

#define cust_memcmp(src, dst, len, val) \
{ \
    int i; \
    val = 0; \
    for (i = 0; i < len; i ++) { \
        if (src[i] != dst[i]) { \
            val = 1; break;\
        }\
    } \
} 

static __always_inline int swap_mac_addresses(struct ethhdr *eth)
{
    static unsigned char mac_ub1_enp7[ETH_ALEN] = {0x52, 0x54, 0x00, 0xfe, 0xd8, 0x4b};
    static unsigned char mac_ub2_enp7[ETH_ALEN] = {0x52, 0x54, 0x00, 0x97, 0x15, 0x59};
    static unsigned char mac_ub2_enp8[ETH_ALEN] = {0x52, 0x54, 0x00, 0x75, 0x46, 0x88};
    static unsigned char mac_ub3_enp7[ETH_ALEN] = {0x52, 0x54, 0x00, 0x5d, 0x45, 0x86};
    static unsigned char mac_ub3_enp8[ETH_ALEN] = {0x52, 0x54, 0x00, 0x3e, 0x8e, 0xd2};
    static unsigned char mac_ub4_enp7[ETH_ALEN] = {0x52, 0x54, 0x00, 0x54, 0x9f, 0x06};
//    struct iphdr *iph = (void*)(eth + 1);
	unsigned char *src_addr = eth->h_source;
	unsigned char *dst_addr = eth->h_dest;
    int val = 0;

    cust_memcmp(src_addr, mac_ub1_enp7, ETH_ALEN, val);
    if (!val) {
        memcpy(src_addr, mac_ub2_enp8, ETH_ALEN);
        memcpy(dst_addr, mac_ub3_enp7, ETH_ALEN);
        return bpf_redirect(4, 0);
    }  
    cust_memcmp(src_addr, mac_ub2_enp8, ETH_ALEN, val);
    if (!val) {
        memcpy(src_addr, mac_ub3_enp8, ETH_ALEN);
        memcpy(dst_addr, mac_ub4_enp7, ETH_ALEN);
        return bpf_redirect(4, 0);
    }
    cust_memcmp(src_addr, mac_ub4_enp7, ETH_ALEN, val);
    if (!val) {
        memcpy(src_addr, mac_ub3_enp7, ETH_ALEN);
        memcpy(dst_addr, mac_ub2_enp8, ETH_ALEN);
        return bpf_redirect(3, 0);
    }  
    cust_memcmp(src_addr, mac_ub3_enp7, ETH_ALEN, val)
    if (!val) {
        memcpy(src_addr, mac_ub2_enp7, ETH_ALEN);
        memcpy(dst_addr, mac_ub1_enp7, ETH_ALEN);
        return bpf_redirect(3, 0);
    }
    return XDP_PASS;
}
#endif
// todo: redirect only if it is icmp or tcp or udp packet to end space dst and src subnet
SEC("xdp") int xdp_xsk(struct xdp_md *ctx)
{
    void *data = (void*)(long)ctx->data;
    void *data_end = (void*)(long)ctx->data_end;
    struct ethhdr *eth;

    struct hdr_cursor nh;
    int ether_type;

    nh.pos = data;
    ether_type = parse_ethhdr_vlan(&nh, data_end, &eth, NULL);
    if (ether_type != bpf_htons(ETH_P_IP)) {
        return XDP_PASS;
    }
//    bpf_printk("forward to %d:%d\n", ctx->ingress_ifindex, ctx->rx_queue_index);

   // return swap_mac_addresses(eth);
	return bpf_redirect_map(&xsks_map, ctx->rx_queue_index, XDP_DROP);
}

char _license[] SEC("license") = "GPL";
