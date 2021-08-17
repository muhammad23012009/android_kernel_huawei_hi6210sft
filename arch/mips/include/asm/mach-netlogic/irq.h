/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2011 Netlogic Microsystems.
 */
#ifndef __ASM_NETLOGIC_IRQ_H
#define __ASM_NETLOGIC_IRQ_H

#include <asm/mach-netlogic/multi-node.h>
<<<<<<< HEAD
#define NR_IRQS			(64 * NLM_NR_NODES)
=======
#define NLM_IRQS_PER_NODE	1024
#define NR_IRQS			(NLM_IRQS_PER_NODE * NLM_NR_NODES)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define MIPS_CPU_IRQ_BASE	0

#endif /* __ASM_NETLOGIC_IRQ_H */
