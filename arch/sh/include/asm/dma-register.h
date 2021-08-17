/*
 * Common header for the legacy SH DMA driver and the new dmaengine driver
 *
 * extracted from arch/sh/include/asm/dma-sh.h:
 *
 * Copyright (C) 2000  Takashi YOSHII
 * Copyright (C) 2003  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef DMA_REGISTER_H
#define DMA_REGISTER_H

<<<<<<< HEAD
/* DMA register */
#define SAR	0x00
#define DAR	0x04
#define TCR	0x08
#define CHCR	0x0C
#define DMAOR	0x40

/* DMAOR definitions */
#define DMAOR_AE	0x00000004
#define DMAOR_NMIF	0x00000002
#define DMAOR_DME	0x00000001
=======
/* DMA registers */
#define SAR	0x00	/* Source Address Register */
#define DAR	0x04	/* Destination Address Register */
#define TCR	0x08	/* Transfer Count Register */
#define CHCR	0x0C	/* Channel Control Register */
#define DMAOR	0x40	/* DMA Operation Register */

/* DMAOR definitions */
#define DMAOR_AE	0x00000004	/* Address Error Flag */
#define DMAOR_NMIF	0x00000002
#define DMAOR_DME	0x00000001	/* DMA Master Enable */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Definitions for the SuperH DMAC */
#define REQ_L	0x00000000
#define REQ_E	0x00080000
#define RACK_H	0x00000000
#define RACK_L	0x00040000
#define ACK_R	0x00000000
#define ACK_W	0x00020000
#define ACK_H	0x00000000
#define ACK_L	0x00010000
<<<<<<< HEAD
#define DM_INC	0x00004000
#define DM_DEC	0x00008000
#define DM_FIX	0x0000c000
#define SM_INC	0x00001000
#define SM_DEC	0x00002000
#define SM_FIX	0x00003000
#define RS_IN	0x00000200
#define RS_OUT	0x00000300
#define TS_BLK	0x00000040
#define TM_BUR	0x00000020
#define CHCR_DE	0x00000001
#define CHCR_TE	0x00000002
#define CHCR_IE	0x00000004
=======
#define DM_INC	0x00004000	/* Destination addresses are incremented */
#define DM_DEC	0x00008000	/* Destination addresses are decremented */
#define DM_FIX	0x0000c000	/* Destination address is fixed */
#define SM_INC	0x00001000	/* Source addresses are incremented */
#define SM_DEC	0x00002000	/* Source addresses are decremented */
#define SM_FIX	0x00003000	/* Source address is fixed */
#define RS_IN	0x00000200
#define RS_OUT	0x00000300
#define RS_AUTO	0x00000400	/* Auto Request */
#define RS_ERS	0x00000800	/* DMA extended resource selector */
#define TS_BLK	0x00000040
#define TM_BUR	0x00000020
#define CHCR_DE	0x00000001	/* DMA Enable */
#define CHCR_TE	0x00000002	/* Transfer End Flag */
#define CHCR_IE	0x00000004	/* Interrupt Enable */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
