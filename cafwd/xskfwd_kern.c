// SPDX-License-Identifier: GPL-2.0
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

struct {
	__uint(type, BPF_MAP_TYPE_XSKMAP);
	__uint(key_size, sizeof(int));
	__uint(value_size, sizeof(int));
	__uint(max_entries, 4);
	// __uint(pinning, LIBBPF_PIN_BY_NAME);
} xsks_map SEC(".maps");

SEC("xdp_fwd_af")
int xdp_fwd_af_prog(struct xdp_md *ctx)
{
    int key = ctx->ingress_ifindex;
    int *value;
    value = bpf_map_lookup_elem(&xsks_map, &key);
    if (value) {
//        bpf_trace_printk("map of key is %d\n", sizeof("map of key is %d\n"), *value);
    } else {
        bpf_trace_printk("failed get key\n", sizeof("failed get key\n"));
        return XDP_PASS;
    }
//    bpf_trace_printk("fwd got pkt %d\n", sizeof("fwd got pkt %d\n"), ctx->ingress_ifindex);
    return bpf_redirect_map(&xsks_map, ctx->ingress_ifindex, 0);
}
char _license[] SEC("license") = "GPL";
