// SPDX-License-Identifier: GPL-2.0
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <xdp/parsing_helpers.h>

/* This XDP program is only needed for multi-buffer and XDP_SHARED_UMEM modes.
 * If you do not use these modes, libbpf can supply an XDP program for you.
 */

struct {
	__uint(type, BPF_MAP_TYPE_XSKMAP);
	__uint(max_entries, 10);
	__uint(key_size, sizeof(int));
	__uint(value_size, sizeof(int));
} xsks_map SEC(".maps");


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

	return bpf_redirect_map(&xsks_map, ctx->rx_queue_index, XDP_DROP);
}

char _license[] SEC("license") = "GPL";
