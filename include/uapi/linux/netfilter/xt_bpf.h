#ifndef _XT_BPF_H
#define _XT_BPF_H

#include <linux/filter.h>
#include <linux/types.h>

#define XT_BPF_MAX_NUM_INSTR	64

<<<<<<< HEAD
struct sk_filter;
=======
struct bpf_prog;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct xt_bpf_info {
	__u16 bpf_program_num_elem;
	struct sock_filter bpf_program[XT_BPF_MAX_NUM_INSTR];

	/* only used in the kernel */
<<<<<<< HEAD
	struct sk_filter *filter __attribute__((aligned(8)));
=======
	struct bpf_prog *filter __attribute__((aligned(8)));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif /*_XT_BPF_H */
