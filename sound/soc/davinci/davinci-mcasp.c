/*
 * ALSA SoC McASP Audio Layer for TI DAVINCI processor
 *
 * Multi-channel Audio Serial Port Driver
 *
 * Author: Nirmal Pandey <n-pandey@ti.com>,
 *         Suresh Rajashekara <suresh.r@ti.com>
 *         Steve Chen <schen@.mvista.com>
 *
 * Copyright:   (C) 2009 MontaVista Software, Inc., <source@mvista.com>
 * Copyright:   (C) 2009  Texas Instruments, India
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/io.h>
<<<<<<< HEAD
=======
#include <linux/clk.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/pm_runtime.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>
<<<<<<< HEAD

=======
#include <linux/platform_data/davinci_asp.h>
#include <linux/math64.h>

#include <sound/asoundef.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>
<<<<<<< HEAD

#include "davinci-pcm.h"
#include "davinci-mcasp.h"

/*
 * McASP register definitions
 */
#define DAVINCI_MCASP_PID_REG		0x00
#define DAVINCI_MCASP_PWREMUMGT_REG	0x04

#define DAVINCI_MCASP_PFUNC_REG		0x10
#define DAVINCI_MCASP_PDIR_REG		0x14
#define DAVINCI_MCASP_PDOUT_REG		0x18
#define DAVINCI_MCASP_PDSET_REG		0x1c

#define DAVINCI_MCASP_PDCLR_REG		0x20

#define DAVINCI_MCASP_TLGC_REG		0x30
#define DAVINCI_MCASP_TLMR_REG		0x34

#define DAVINCI_MCASP_GBLCTL_REG	0x44
#define DAVINCI_MCASP_AMUTE_REG		0x48
#define DAVINCI_MCASP_LBCTL_REG		0x4c

#define DAVINCI_MCASP_TXDITCTL_REG	0x50

#define DAVINCI_MCASP_GBLCTLR_REG	0x60
#define DAVINCI_MCASP_RXMASK_REG	0x64
#define DAVINCI_MCASP_RXFMT_REG		0x68
#define DAVINCI_MCASP_RXFMCTL_REG	0x6c

#define DAVINCI_MCASP_ACLKRCTL_REG	0x70
#define DAVINCI_MCASP_AHCLKRCTL_REG	0x74
#define DAVINCI_MCASP_RXTDM_REG		0x78
#define DAVINCI_MCASP_EVTCTLR_REG	0x7c

#define DAVINCI_MCASP_RXSTAT_REG	0x80
#define DAVINCI_MCASP_RXTDMSLOT_REG	0x84
#define DAVINCI_MCASP_RXCLKCHK_REG	0x88
#define DAVINCI_MCASP_REVTCTL_REG	0x8c

#define DAVINCI_MCASP_GBLCTLX_REG	0xa0
#define DAVINCI_MCASP_TXMASK_REG	0xa4
#define DAVINCI_MCASP_TXFMT_REG		0xa8
#define DAVINCI_MCASP_TXFMCTL_REG	0xac

#define DAVINCI_MCASP_ACLKXCTL_REG	0xb0
#define DAVINCI_MCASP_AHCLKXCTL_REG	0xb4
#define DAVINCI_MCASP_TXTDM_REG		0xb8
#define DAVINCI_MCASP_EVTCTLX_REG	0xbc

#define DAVINCI_MCASP_TXSTAT_REG	0xc0
#define DAVINCI_MCASP_TXTDMSLOT_REG	0xc4
#define DAVINCI_MCASP_TXCLKCHK_REG	0xc8
#define DAVINCI_MCASP_XEVTCTL_REG	0xcc

/* Left(even TDM Slot) Channel Status Register File */
#define DAVINCI_MCASP_DITCSRA_REG	0x100
/* Right(odd TDM slot) Channel Status Register File */
#define DAVINCI_MCASP_DITCSRB_REG	0x118
/* Left(even TDM slot) User Data Register File */
#define DAVINCI_MCASP_DITUDRA_REG	0x130
/* Right(odd TDM Slot) User Data Register File */
#define DAVINCI_MCASP_DITUDRB_REG	0x148

/* Serializer n Control Register */
#define DAVINCI_MCASP_XRSRCTL_BASE_REG	0x180
#define DAVINCI_MCASP_XRSRCTL_REG(n)	(DAVINCI_MCASP_XRSRCTL_BASE_REG + \
						(n << 2))

/* Transmit Buffer for Serializer n */
#define DAVINCI_MCASP_TXBUF_REG		0x200
/* Receive Buffer for Serializer n */
#define DAVINCI_MCASP_RXBUF_REG		0x280

/* McASP FIFO Registers */
#define DAVINCI_MCASP_WFIFOCTL		(0x1010)
#define DAVINCI_MCASP_WFIFOSTS		(0x1014)
#define DAVINCI_MCASP_RFIFOCTL		(0x1018)
#define DAVINCI_MCASP_RFIFOSTS		(0x101C)
#define MCASP_VER3_WFIFOCTL		(0x1000)
#define MCASP_VER3_WFIFOSTS		(0x1004)
#define MCASP_VER3_RFIFOCTL		(0x1008)
#define MCASP_VER3_RFIFOSTS		(0x100C)

/*
 * DAVINCI_MCASP_PWREMUMGT_REG - Power Down and Emulation Management
 *     Register Bits
 */
#define MCASP_FREE	BIT(0)
#define MCASP_SOFT	BIT(1)

/*
 * DAVINCI_MCASP_PFUNC_REG - Pin Function / GPIO Enable Register Bits
 */
#define AXR(n)		(1<<n)
#define PFUNC_AMUTE	BIT(25)
#define ACLKX		BIT(26)
#define AHCLKX		BIT(27)
#define AFSX		BIT(28)
#define ACLKR		BIT(29)
#define AHCLKR		BIT(30)
#define AFSR		BIT(31)

/*
 * DAVINCI_MCASP_PDIR_REG - Pin Direction Register Bits
 */
#define AXR(n)		(1<<n)
#define PDIR_AMUTE	BIT(25)
#define ACLKX		BIT(26)
#define AHCLKX		BIT(27)
#define AFSX		BIT(28)
#define ACLKR		BIT(29)
#define AHCLKR		BIT(30)
#define AFSR		BIT(31)

/*
 * DAVINCI_MCASP_TXDITCTL_REG - Transmit DIT Control Register Bits
 */
#define DITEN	BIT(0)	/* Transmit DIT mode enable/disable */
#define VA	BIT(2)
#define VB	BIT(3)

/*
 * DAVINCI_MCASP_TXFMT_REG - Transmit Bitstream Format Register Bits
 */
#define TXROT(val)	(val)
#define TXSEL		BIT(3)
#define TXSSZ(val)	(val<<4)
#define TXPBIT(val)	(val<<8)
#define TXPAD(val)	(val<<13)
#define TXORD		BIT(15)
#define FSXDLY(val)	(val<<16)

/*
 * DAVINCI_MCASP_RXFMT_REG - Receive Bitstream Format Register Bits
 */
#define RXROT(val)	(val)
#define RXSEL		BIT(3)
#define RXSSZ(val)	(val<<4)
#define RXPBIT(val)	(val<<8)
#define RXPAD(val)	(val<<13)
#define RXORD		BIT(15)
#define FSRDLY(val)	(val<<16)

/*
 * DAVINCI_MCASP_TXFMCTL_REG -  Transmit Frame Control Register Bits
 */
#define FSXPOL		BIT(0)
#define AFSXE		BIT(1)
#define FSXDUR		BIT(4)
#define FSXMOD(val)	(val<<7)

/*
 * DAVINCI_MCASP_RXFMCTL_REG - Receive Frame Control Register Bits
 */
#define FSRPOL		BIT(0)
#define AFSRE		BIT(1)
#define FSRDUR		BIT(4)
#define FSRMOD(val)	(val<<7)

/*
 * DAVINCI_MCASP_ACLKXCTL_REG - Transmit Clock Control Register Bits
 */
#define ACLKXDIV(val)	(val)
#define ACLKXE		BIT(5)
#define TX_ASYNC	BIT(6)
#define ACLKXPOL	BIT(7)
#define ACLKXDIV_MASK	0x1f

/*
 * DAVINCI_MCASP_ACLKRCTL_REG Receive Clock Control Register Bits
 */
#define ACLKRDIV(val)	(val)
#define ACLKRE		BIT(5)
#define RX_ASYNC	BIT(6)
#define ACLKRPOL	BIT(7)
#define ACLKRDIV_MASK	0x1f

/*
 * DAVINCI_MCASP_AHCLKXCTL_REG - High Frequency Transmit Clock Control
 *     Register Bits
 */
#define AHCLKXDIV(val)	(val)
#define AHCLKXPOL	BIT(14)
#define AHCLKXE		BIT(15)
#define AHCLKXDIV_MASK	0xfff

/*
 * DAVINCI_MCASP_AHCLKRCTL_REG - High Frequency Receive Clock Control
 *     Register Bits
 */
#define AHCLKRDIV(val)	(val)
#define AHCLKRPOL	BIT(14)
#define AHCLKRE		BIT(15)
#define AHCLKRDIV_MASK	0xfff

/*
 * DAVINCI_MCASP_XRSRCTL_BASE_REG -  Serializer Control Register Bits
 */
#define MODE(val)	(val)
#define DISMOD		(val)(val<<2)
#define TXSTATE		BIT(4)
#define RXSTATE		BIT(5)
#define SRMOD_MASK	3
#define SRMOD_INACTIVE	0

/*
 * DAVINCI_MCASP_LBCTL_REG - Loop Back Control Register Bits
 */
#define LBEN		BIT(0)
#define LBORD		BIT(1)
#define LBGENMODE(val)	(val<<2)

/*
 * DAVINCI_MCASP_TXTDMSLOT_REG - Transmit TDM Slot Register configuration
 */
#define TXTDMS(n)	(1<<n)

/*
 * DAVINCI_MCASP_RXTDMSLOT_REG - Receive TDM Slot Register configuration
 */
#define RXTDMS(n)	(1<<n)

/*
 * DAVINCI_MCASP_GBLCTL_REG -  Global Control Register Bits
 */
#define RXCLKRST	BIT(0)	/* Receiver Clock Divider Reset */
#define RXHCLKRST	BIT(1)	/* Receiver High Frequency Clock Divider */
#define RXSERCLR	BIT(2)	/* Receiver Serializer Clear */
#define RXSMRST		BIT(3)	/* Receiver State Machine Reset */
#define RXFSRST		BIT(4)	/* Frame Sync Generator Reset */
#define TXCLKRST	BIT(8)	/* Transmitter Clock Divider Reset */
#define TXHCLKRST	BIT(9)	/* Transmitter High Frequency Clock Divider*/
#define TXSERCLR	BIT(10)	/* Transmit Serializer Clear */
#define TXSMRST		BIT(11)	/* Transmitter State Machine Reset */
#define TXFSRST		BIT(12)	/* Frame Sync Generator Reset */

/*
 * DAVINCI_MCASP_AMUTE_REG -  Mute Control Register Bits
 */
#define MUTENA(val)	(val)
#define MUTEINPOL	BIT(2)
#define MUTEINENA	BIT(3)
#define MUTEIN		BIT(4)
#define MUTER		BIT(5)
#define MUTEX		BIT(6)
#define MUTEFSR		BIT(7)
#define MUTEFSX		BIT(8)
#define MUTEBADCLKR	BIT(9)
#define MUTEBADCLKX	BIT(10)
#define MUTERXDMAERR	BIT(11)
#define MUTETXDMAERR	BIT(12)

/*
 * DAVINCI_MCASP_REVTCTL_REG - Receiver DMA Event Control Register bits
 */
#define RXDATADMADIS	BIT(0)

/*
 * DAVINCI_MCASP_XEVTCTL_REG - Transmitter DMA Event Control Register bits
 */
#define TXDATADMADIS	BIT(0)

/*
 * DAVINCI_MCASP_W[R]FIFOCTL - Write/Read FIFO Control Register bits
 */
#define FIFO_ENABLE	BIT(16)
#define NUMEVT_MASK	(0xFF << 8)
#define NUMDMA_MASK	(0xFF)

#define DAVINCI_MCASP_NUM_SERIALIZER	16

static inline void mcasp_set_bits(void __iomem *reg, u32 val)
{
	__raw_writel(__raw_readl(reg) | val, reg);
}

static inline void mcasp_clr_bits(void __iomem *reg, u32 val)
{
	__raw_writel((__raw_readl(reg) & ~(val)), reg);
}

static inline void mcasp_mod_bits(void __iomem *reg, u32 val, u32 mask)
{
	__raw_writel((__raw_readl(reg) & ~mask) | val, reg);
}

static inline void mcasp_set_reg(void __iomem *reg, u32 val)
{
	__raw_writel(val, reg);
}

static inline u32 mcasp_get_reg(void __iomem *reg)
{
	return (unsigned int)__raw_readl(reg);
}

static inline void mcasp_set_ctl_reg(void __iomem *regs, u32 val)
{
	int i = 0;

	mcasp_set_bits(regs, val);
=======
#include <sound/dmaengine_pcm.h>
#include <sound/omap-pcm.h>

#include "edma-pcm.h"
#include "davinci-mcasp.h"

#define MCASP_MAX_AFIFO_DEPTH	64

#ifdef CONFIG_PM
static u32 context_regs[] = {
	DAVINCI_MCASP_TXFMCTL_REG,
	DAVINCI_MCASP_RXFMCTL_REG,
	DAVINCI_MCASP_TXFMT_REG,
	DAVINCI_MCASP_RXFMT_REG,
	DAVINCI_MCASP_ACLKXCTL_REG,
	DAVINCI_MCASP_ACLKRCTL_REG,
	DAVINCI_MCASP_AHCLKXCTL_REG,
	DAVINCI_MCASP_AHCLKRCTL_REG,
	DAVINCI_MCASP_PDIR_REG,
	DAVINCI_MCASP_RXMASK_REG,
	DAVINCI_MCASP_TXMASK_REG,
	DAVINCI_MCASP_RXTDM_REG,
	DAVINCI_MCASP_TXTDM_REG,
};

struct davinci_mcasp_context {
	u32	config_regs[ARRAY_SIZE(context_regs)];
	u32	afifo_regs[2]; /* for read/write fifo control registers */
	u32	*xrsr_regs; /* for serializer configuration */
	bool	pm_state;
};
#endif

struct davinci_mcasp_ruledata {
	struct davinci_mcasp *mcasp;
	int serializers;
};

struct davinci_mcasp {
	struct snd_dmaengine_dai_dma_data dma_data[2];
	void __iomem *base;
	u32 fifo_base;
	struct device *dev;
	struct snd_pcm_substream *substreams[2];
	unsigned int dai_fmt;

	/* McASP specific data */
	int	tdm_slots;
	u32	tdm_mask[2];
	int	slot_width;
	u8	op_mode;
	u8	num_serializer;
	u8	*serial_dir;
	u8	version;
	u8	bclk_div;
	int	streams;
	u32	irq_request[2];
	int	dma_request[2];

	int	sysclk_freq;
	bool	bclk_master;

	/* McASP FIFO related */
	u8	txnumevt;
	u8	rxnumevt;

	bool	dat_port;

	/* Used for comstraint setting on the second stream */
	u32	channels;

#ifdef CONFIG_PM_SLEEP
	struct davinci_mcasp_context context;
#endif

	struct davinci_mcasp_ruledata ruledata[2];
	struct snd_pcm_hw_constraint_list chconstr[2];
};

static inline void mcasp_set_bits(struct davinci_mcasp *mcasp, u32 offset,
				  u32 val)
{
	void __iomem *reg = mcasp->base + offset;
	__raw_writel(__raw_readl(reg) | val, reg);
}

static inline void mcasp_clr_bits(struct davinci_mcasp *mcasp, u32 offset,
				  u32 val)
{
	void __iomem *reg = mcasp->base + offset;
	__raw_writel((__raw_readl(reg) & ~(val)), reg);
}

static inline void mcasp_mod_bits(struct davinci_mcasp *mcasp, u32 offset,
				  u32 val, u32 mask)
{
	void __iomem *reg = mcasp->base + offset;
	__raw_writel((__raw_readl(reg) & ~mask) | val, reg);
}

static inline void mcasp_set_reg(struct davinci_mcasp *mcasp, u32 offset,
				 u32 val)
{
	__raw_writel(val, mcasp->base + offset);
}

static inline u32 mcasp_get_reg(struct davinci_mcasp *mcasp, u32 offset)
{
	return (u32)__raw_readl(mcasp->base + offset);
}

static void mcasp_set_ctl_reg(struct davinci_mcasp *mcasp, u32 ctl_reg, u32 val)
{
	int i = 0;

	mcasp_set_bits(mcasp, ctl_reg, val);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* programming GBLCTL needs to read back from GBLCTL and verfiy */
	/* loop count is to avoid the lock-up */
	for (i = 0; i < 1000; i++) {
<<<<<<< HEAD
		if ((mcasp_get_reg(regs) & val) == val)
			break;
	}

	if (i == 1000 && ((mcasp_get_reg(regs) & val) != val))
		printk(KERN_ERR "GBLCTL write error\n");
}

static void mcasp_start_rx(struct davinci_audio_dev *dev)
{
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXHCLKRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXCLKRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXSERCLR);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_RXBUF_REG, 0);

	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXSMRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXFSRST);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_RXBUF_REG, 0);

	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXSMRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXFSRST);
}

static void mcasp_start_tx(struct davinci_audio_dev *dev)
{
	u8 offset = 0, i;
	u32 cnt;

	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXHCLKRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXCLKRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXSERCLR);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXBUF_REG, 0);

	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXSMRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXFSRST);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXBUF_REG, 0);
	for (i = 0; i < dev->num_serializer; i++) {
		if (dev->serial_dir[i] == TX_MODE) {
			offset = i;
			break;
		}
	}

	/* wait for TX ready */
	cnt = 0;
	while (!(mcasp_get_reg(dev->base + DAVINCI_MCASP_XRSRCTL_REG(offset)) &
		 TXSTATE) && (cnt < 100000))
		cnt++;

	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXBUF_REG, 0);
}

static void davinci_mcasp_start(struct davinci_audio_dev *dev, int stream)
{
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		if (dev->txnumevt) {	/* enable FIFO */
			switch (dev->version) {
			case MCASP_VERSION_3:
				mcasp_clr_bits(dev->base + MCASP_VER3_WFIFOCTL,
								FIFO_ENABLE);
				mcasp_set_bits(dev->base + MCASP_VER3_WFIFOCTL,
								FIFO_ENABLE);
				break;
			default:
				mcasp_clr_bits(dev->base +
					DAVINCI_MCASP_WFIFOCTL,	FIFO_ENABLE);
				mcasp_set_bits(dev->base +
					DAVINCI_MCASP_WFIFOCTL,	FIFO_ENABLE);
			}
		}
		mcasp_start_tx(dev);
	} else {
		if (dev->rxnumevt) {	/* enable FIFO */
			switch (dev->version) {
			case MCASP_VERSION_3:
				mcasp_clr_bits(dev->base + MCASP_VER3_RFIFOCTL,
								FIFO_ENABLE);
				mcasp_set_bits(dev->base + MCASP_VER3_RFIFOCTL,
								FIFO_ENABLE);
				break;
			default:
				mcasp_clr_bits(dev->base +
					DAVINCI_MCASP_RFIFOCTL,	FIFO_ENABLE);
				mcasp_set_bits(dev->base +
					DAVINCI_MCASP_RFIFOCTL,	FIFO_ENABLE);
			}
		}
		mcasp_start_rx(dev);
	}
}

static void mcasp_stop_rx(struct davinci_audio_dev *dev)
{
	mcasp_set_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, 0);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_RXSTAT_REG, 0xFFFFFFFF);
}

static void mcasp_stop_tx(struct davinci_audio_dev *dev)
{
	mcasp_set_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, 0);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXSTAT_REG, 0xFFFFFFFF);
}

static void davinci_mcasp_stop(struct davinci_audio_dev *dev, int stream)
{
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		if (dev->txnumevt) {	/* disable FIFO */
			switch (dev->version) {
			case MCASP_VERSION_3:
				mcasp_clr_bits(dev->base + MCASP_VER3_WFIFOCTL,
								FIFO_ENABLE);
				break;
			default:
				mcasp_clr_bits(dev->base +
					DAVINCI_MCASP_WFIFOCTL,	FIFO_ENABLE);
			}
		}
		mcasp_stop_tx(dev);
	} else {
		if (dev->rxnumevt) {	/* disable FIFO */
			switch (dev->version) {
			case MCASP_VERSION_3:
				mcasp_clr_bits(dev->base + MCASP_VER3_RFIFOCTL,
								FIFO_ENABLE);
			break;

			default:
				mcasp_clr_bits(dev->base +
					DAVINCI_MCASP_RFIFOCTL,	FIFO_ENABLE);
			}
		}
		mcasp_stop_rx(dev);
	}
=======
		if ((mcasp_get_reg(mcasp, ctl_reg) & val) == val)
			break;
	}

	if (i == 1000 && ((mcasp_get_reg(mcasp, ctl_reg) & val) != val))
		printk(KERN_ERR "GBLCTL write error\n");
}

static bool mcasp_is_synchronous(struct davinci_mcasp *mcasp)
{
	u32 rxfmctl = mcasp_get_reg(mcasp, DAVINCI_MCASP_RXFMCTL_REG);
	u32 aclkxctl = mcasp_get_reg(mcasp, DAVINCI_MCASP_ACLKXCTL_REG);

	return !(aclkxctl & TX_ASYNC) && rxfmctl & AFSRE;
}

static void mcasp_start_rx(struct davinci_mcasp *mcasp)
{
	if (mcasp->rxnumevt) {	/* enable FIFO */
		u32 reg = mcasp->fifo_base + MCASP_RFIFOCTL_OFFSET;

		mcasp_clr_bits(mcasp, reg, FIFO_ENABLE);
		mcasp_set_bits(mcasp, reg, FIFO_ENABLE);
	}

	/* Start clocks */
	mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLR_REG, RXHCLKRST);
	mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLR_REG, RXCLKRST);
	/*
	 * When ASYNC == 0 the transmit and receive sections operate
	 * synchronously from the transmit clock and frame sync. We need to make
	 * sure that the TX signlas are enabled when starting reception.
	 */
	if (mcasp_is_synchronous(mcasp)) {
		mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLX_REG, TXHCLKRST);
		mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLX_REG, TXCLKRST);
	}

	/* Activate serializer(s) */
	mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLR_REG, RXSERCLR);
	/* Release RX state machine */
	mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLR_REG, RXSMRST);
	/* Release Frame Sync generator */
	mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLR_REG, RXFSRST);
	if (mcasp_is_synchronous(mcasp))
		mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLX_REG, TXFSRST);

	/* enable receive IRQs */
	mcasp_set_bits(mcasp, DAVINCI_MCASP_EVTCTLR_REG,
		       mcasp->irq_request[SNDRV_PCM_STREAM_CAPTURE]);
}

static void mcasp_start_tx(struct davinci_mcasp *mcasp)
{
	u32 cnt;

	if (mcasp->txnumevt) {	/* enable FIFO */
		u32 reg = mcasp->fifo_base + MCASP_WFIFOCTL_OFFSET;

		mcasp_clr_bits(mcasp, reg, FIFO_ENABLE);
		mcasp_set_bits(mcasp, reg, FIFO_ENABLE);
	}

	/* Start clocks */
	mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLX_REG, TXHCLKRST);
	mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLX_REG, TXCLKRST);
	/* Activate serializer(s) */
	mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLX_REG, TXSERCLR);

	/* wait for XDATA to be cleared */
	cnt = 0;
	while ((mcasp_get_reg(mcasp, DAVINCI_MCASP_TXSTAT_REG) & XRDATA) &&
	       (cnt < 100000))
		cnt++;

	/* Release TX state machine */
	mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLX_REG, TXSMRST);
	/* Release Frame Sync generator */
	mcasp_set_ctl_reg(mcasp, DAVINCI_MCASP_GBLCTLX_REG, TXFSRST);

	/* enable transmit IRQs */
	mcasp_set_bits(mcasp, DAVINCI_MCASP_EVTCTLX_REG,
		       mcasp->irq_request[SNDRV_PCM_STREAM_PLAYBACK]);
}

static void davinci_mcasp_start(struct davinci_mcasp *mcasp, int stream)
{
	mcasp->streams++;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		mcasp_start_tx(mcasp);
	else
		mcasp_start_rx(mcasp);
}

static void mcasp_stop_rx(struct davinci_mcasp *mcasp)
{
	/* disable IRQ sources */
	mcasp_clr_bits(mcasp, DAVINCI_MCASP_EVTCTLR_REG,
		       mcasp->irq_request[SNDRV_PCM_STREAM_CAPTURE]);

	/*
	 * In synchronous mode stop the TX clocks if no other stream is
	 * running
	 */
	if (mcasp_is_synchronous(mcasp) && !mcasp->streams)
		mcasp_set_reg(mcasp, DAVINCI_MCASP_GBLCTLX_REG, 0);

	mcasp_set_reg(mcasp, DAVINCI_MCASP_GBLCTLR_REG, 0);
	mcasp_set_reg(mcasp, DAVINCI_MCASP_RXSTAT_REG, 0xFFFFFFFF);

	if (mcasp->rxnumevt) {	/* disable FIFO */
		u32 reg = mcasp->fifo_base + MCASP_RFIFOCTL_OFFSET;

		mcasp_clr_bits(mcasp, reg, FIFO_ENABLE);
	}
}

static void mcasp_stop_tx(struct davinci_mcasp *mcasp)
{
	u32 val = 0;

	/* disable IRQ sources */
	mcasp_clr_bits(mcasp, DAVINCI_MCASP_EVTCTLX_REG,
		       mcasp->irq_request[SNDRV_PCM_STREAM_PLAYBACK]);

	/*
	 * In synchronous mode keep TX clocks running if the capture stream is
	 * still running.
	 */
	if (mcasp_is_synchronous(mcasp) && mcasp->streams)
		val =  TXHCLKRST | TXCLKRST | TXFSRST;

	mcasp_set_reg(mcasp, DAVINCI_MCASP_GBLCTLX_REG, val);
	mcasp_set_reg(mcasp, DAVINCI_MCASP_TXSTAT_REG, 0xFFFFFFFF);

	if (mcasp->txnumevt) {	/* disable FIFO */
		u32 reg = mcasp->fifo_base + MCASP_WFIFOCTL_OFFSET;

		mcasp_clr_bits(mcasp, reg, FIFO_ENABLE);
	}
}

static void davinci_mcasp_stop(struct davinci_mcasp *mcasp, int stream)
{
	mcasp->streams--;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		mcasp_stop_tx(mcasp);
	else
		mcasp_stop_rx(mcasp);
}

static irqreturn_t davinci_mcasp_tx_irq_handler(int irq, void *data)
{
	struct davinci_mcasp *mcasp = (struct davinci_mcasp *)data;
	struct snd_pcm_substream *substream;
	u32 irq_mask = mcasp->irq_request[SNDRV_PCM_STREAM_PLAYBACK];
	u32 handled_mask = 0;
	u32 stat;

	stat = mcasp_get_reg(mcasp, DAVINCI_MCASP_TXSTAT_REG);
	if (stat & XUNDRN & irq_mask) {
		dev_warn(mcasp->dev, "Transmit buffer underflow\n");
		handled_mask |= XUNDRN;

		substream = mcasp->substreams[SNDRV_PCM_STREAM_PLAYBACK];
		if (substream) {
			snd_pcm_stream_lock_irq(substream);
			if (snd_pcm_running(substream))
				snd_pcm_stop(substream, SNDRV_PCM_STATE_XRUN);
			snd_pcm_stream_unlock_irq(substream);
		}
	}

	if (!handled_mask)
		dev_warn(mcasp->dev, "unhandled tx event. txstat: 0x%08x\n",
			 stat);

	if (stat & XRERR)
		handled_mask |= XRERR;

	/* Ack the handled event only */
	mcasp_set_reg(mcasp, DAVINCI_MCASP_TXSTAT_REG, handled_mask);

	return IRQ_RETVAL(handled_mask);
}

static irqreturn_t davinci_mcasp_rx_irq_handler(int irq, void *data)
{
	struct davinci_mcasp *mcasp = (struct davinci_mcasp *)data;
	struct snd_pcm_substream *substream;
	u32 irq_mask = mcasp->irq_request[SNDRV_PCM_STREAM_CAPTURE];
	u32 handled_mask = 0;
	u32 stat;

	stat = mcasp_get_reg(mcasp, DAVINCI_MCASP_RXSTAT_REG);
	if (stat & ROVRN & irq_mask) {
		dev_warn(mcasp->dev, "Receive buffer overflow\n");
		handled_mask |= ROVRN;

		substream = mcasp->substreams[SNDRV_PCM_STREAM_CAPTURE];
		if (substream) {
			snd_pcm_stream_lock_irq(substream);
			if (snd_pcm_running(substream))
				snd_pcm_stop(substream, SNDRV_PCM_STATE_XRUN);
			snd_pcm_stream_unlock_irq(substream);
		}
	}

	if (!handled_mask)
		dev_warn(mcasp->dev, "unhandled rx event. rxstat: 0x%08x\n",
			 stat);

	if (stat & XRERR)
		handled_mask |= XRERR;

	/* Ack the handled event only */
	mcasp_set_reg(mcasp, DAVINCI_MCASP_RXSTAT_REG, handled_mask);

	return IRQ_RETVAL(handled_mask);
}

static irqreturn_t davinci_mcasp_common_irq_handler(int irq, void *data)
{
	struct davinci_mcasp *mcasp = (struct davinci_mcasp *)data;
	irqreturn_t ret = IRQ_NONE;

	if (mcasp->substreams[SNDRV_PCM_STREAM_PLAYBACK])
		ret = davinci_mcasp_tx_irq_handler(irq, data);

	if (mcasp->substreams[SNDRV_PCM_STREAM_CAPTURE])
		ret |= davinci_mcasp_rx_irq_handler(irq, data);

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int davinci_mcasp_set_dai_fmt(struct snd_soc_dai *cpu_dai,
					 unsigned int fmt)
{
<<<<<<< HEAD
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(cpu_dai);
	void __iomem *base = dev->base;

	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_DSP_B:
	case SND_SOC_DAIFMT_AC97:
		mcasp_clr_bits(dev->base + DAVINCI_MCASP_TXFMCTL_REG, FSXDUR);
		mcasp_clr_bits(dev->base + DAVINCI_MCASP_RXFMCTL_REG, FSRDUR);
		break;
	default:
		/* configure a full-word SYNC pulse (LRCLK) */
		mcasp_set_bits(dev->base + DAVINCI_MCASP_TXFMCTL_REG, FSXDUR);
		mcasp_set_bits(dev->base + DAVINCI_MCASP_RXFMCTL_REG, FSRDUR);

		/* make 1st data bit occur one ACLK cycle after the frame sync */
		mcasp_set_bits(dev->base + DAVINCI_MCASP_TXFMT_REG, FSXDLY(1));
		mcasp_set_bits(dev->base + DAVINCI_MCASP_RXFMT_REG, FSRDLY(1));
		break;
	}

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBS_CFS:
		/* codec is clock and frame slave */
		mcasp_set_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE);
		mcasp_set_bits(base + DAVINCI_MCASP_TXFMCTL_REG, AFSXE);

		mcasp_set_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRE);
		mcasp_set_bits(base + DAVINCI_MCASP_RXFMCTL_REG, AFSRE);

		mcasp_set_bits(base + DAVINCI_MCASP_PDIR_REG,
				ACLKX | ACLKR);
		mcasp_set_bits(base + DAVINCI_MCASP_PDIR_REG,
				AFSX | AFSR);
		break;
	case SND_SOC_DAIFMT_CBM_CFS:
		/* codec is clock master and frame slave */
		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE);
		mcasp_set_bits(base + DAVINCI_MCASP_TXFMCTL_REG, AFSXE);

		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRE);
		mcasp_set_bits(base + DAVINCI_MCASP_RXFMCTL_REG, AFSRE);

		mcasp_clr_bits(base + DAVINCI_MCASP_PDIR_REG,
				ACLKX | ACLKR);
		mcasp_set_bits(base + DAVINCI_MCASP_PDIR_REG,
				AFSX | AFSR);
		break;
	case SND_SOC_DAIFMT_CBM_CFM:
		/* codec is clock and frame master */
		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE);
		mcasp_clr_bits(base + DAVINCI_MCASP_TXFMCTL_REG, AFSXE);

		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRE);
		mcasp_clr_bits(base + DAVINCI_MCASP_RXFMCTL_REG, AFSRE);

		mcasp_clr_bits(base + DAVINCI_MCASP_PDIR_REG,
				ACLKX | AHCLKX | AFSX | ACLKR | AHCLKR | AFSR);
		break;

	default:
		return -EINVAL;
=======
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(cpu_dai);
	int ret = 0;
	u32 data_delay;
	bool fs_pol_rising;
	bool inv_fs = false;

	if (!fmt)
		return 0;

	pm_runtime_get_sync(mcasp->dev);
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_DSP_A:
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG, FSXDUR);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG, FSRDUR);
		/* 1st data bit occur one ACLK cycle after the frame sync */
		data_delay = 1;
		break;
	case SND_SOC_DAIFMT_DSP_B:
	case SND_SOC_DAIFMT_AC97:
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG, FSXDUR);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG, FSRDUR);
		/* No delay after FS */
		data_delay = 0;
		break;
	case SND_SOC_DAIFMT_I2S:
		/* configure a full-word SYNC pulse (LRCLK) */
		mcasp_set_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG, FSXDUR);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG, FSRDUR);
		/* 1st data bit occur one ACLK cycle after the frame sync */
		data_delay = 1;
		/* FS need to be inverted */
		inv_fs = true;
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		/* configure a full-word SYNC pulse (LRCLK) */
		mcasp_set_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG, FSXDUR);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG, FSRDUR);
		/* No delay after FS */
		data_delay = 0;
		break;
	default:
		ret = -EINVAL;
		goto out;
	}

	mcasp_mod_bits(mcasp, DAVINCI_MCASP_TXFMT_REG, FSXDLY(data_delay),
		       FSXDLY(3));
	mcasp_mod_bits(mcasp, DAVINCI_MCASP_RXFMT_REG, FSRDLY(data_delay),
		       FSRDLY(3));

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBS_CFS:
		/* codec is clock and frame slave */
		mcasp_set_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG, AFSXE);

		mcasp_set_bits(mcasp, DAVINCI_MCASP_ACLKRCTL_REG, ACLKRE);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG, AFSRE);

		mcasp_set_bits(mcasp, DAVINCI_MCASP_PDIR_REG, ACLKX | ACLKR);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_PDIR_REG, AFSX | AFSR);
		mcasp->bclk_master = 1;
		break;
	case SND_SOC_DAIFMT_CBS_CFM:
		/* codec is clock slave and frame master */
		mcasp_set_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG, AFSXE);

		mcasp_set_bits(mcasp, DAVINCI_MCASP_ACLKRCTL_REG, ACLKRE);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG, AFSRE);

		mcasp_set_bits(mcasp, DAVINCI_MCASP_PDIR_REG, ACLKX | ACLKR);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_PDIR_REG, AFSX | AFSR);
		mcasp->bclk_master = 1;
		break;
	case SND_SOC_DAIFMT_CBM_CFS:
		/* codec is clock master and frame slave */
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG, AFSXE);

		mcasp_clr_bits(mcasp, DAVINCI_MCASP_ACLKRCTL_REG, ACLKRE);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG, AFSRE);

		mcasp_clr_bits(mcasp, DAVINCI_MCASP_PDIR_REG, ACLKX | ACLKR);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_PDIR_REG, AFSX | AFSR);
		mcasp->bclk_master = 0;
		break;
	case SND_SOC_DAIFMT_CBM_CFM:
		/* codec is clock and frame master */
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG, AFSXE);

		mcasp_clr_bits(mcasp, DAVINCI_MCASP_ACLKRCTL_REG, ACLKRE);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG, AFSRE);

		mcasp_clr_bits(mcasp, DAVINCI_MCASP_PDIR_REG,
			       ACLKX | AFSX | ACLKR | AHCLKR | AFSR);
		mcasp->bclk_master = 0;
		break;
	default:
		ret = -EINVAL;
		goto out;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_IB_NF:
<<<<<<< HEAD
		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL);
		mcasp_clr_bits(base + DAVINCI_MCASP_TXFMCTL_REG, FSXPOL);

		mcasp_set_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL);
		mcasp_clr_bits(base + DAVINCI_MCASP_RXFMCTL_REG, FSRPOL);
		break;

	case SND_SOC_DAIFMT_NB_IF:
		mcasp_set_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL);
		mcasp_set_bits(base + DAVINCI_MCASP_TXFMCTL_REG, FSXPOL);

		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL);
		mcasp_set_bits(base + DAVINCI_MCASP_RXFMCTL_REG, FSRPOL);
		break;

	case SND_SOC_DAIFMT_IB_IF:
		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL);
		mcasp_set_bits(base + DAVINCI_MCASP_TXFMCTL_REG, FSXPOL);

		mcasp_set_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL);
		mcasp_set_bits(base + DAVINCI_MCASP_RXFMCTL_REG, FSRPOL);
		break;

	case SND_SOC_DAIFMT_NB_NF:
		mcasp_set_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL);
		mcasp_clr_bits(base + DAVINCI_MCASP_TXFMCTL_REG, FSXPOL);

		mcasp_set_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL);
		mcasp_clr_bits(base + DAVINCI_MCASP_RXFMCTL_REG, FSRPOL);
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static int davinci_mcasp_set_clkdiv(struct snd_soc_dai *dai, int div_id, int div)
{
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(dai);

	switch (div_id) {
	case 0:		/* MCLK divider */
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_AHCLKXCTL_REG,
			       AHCLKXDIV(div - 1), AHCLKXDIV_MASK);
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_AHCLKRCTL_REG,
			       AHCLKRDIV(div - 1), AHCLKRDIV_MASK);
		break;

	case 1:		/* BCLK divider */
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_ACLKXCTL_REG,
			       ACLKXDIV(div - 1), ACLKXDIV_MASK);
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_ACLKRCTL_REG,
			       ACLKRDIV(div - 1), ACLKRDIV_MASK);
		break;

	case 2:		/* BCLK/LRCLK ratio */
		dev->bclk_lrclk_ratio = div;
=======
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL);
		fs_pol_rising = true;
		break;
	case SND_SOC_DAIFMT_NB_IF:
		mcasp_set_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL);
		fs_pol_rising = false;
		break;
	case SND_SOC_DAIFMT_IB_IF:
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL);
		fs_pol_rising = false;
		break;
	case SND_SOC_DAIFMT_NB_NF:
		mcasp_set_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL);
		fs_pol_rising = true;
		break;
	default:
		ret = -EINVAL;
		goto out;
	}

	if (inv_fs)
		fs_pol_rising = !fs_pol_rising;

	if (fs_pol_rising) {
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG, FSXPOL);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG, FSRPOL);
	} else {
		mcasp_set_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG, FSXPOL);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG, FSRPOL);
	}

	mcasp->dai_fmt = fmt;
out:
	pm_runtime_put(mcasp->dev);
	return ret;
}

static int __davinci_mcasp_set_clkdiv(struct davinci_mcasp *mcasp, int div_id,
				      int div, bool explicit)
{
	pm_runtime_get_sync(mcasp->dev);
	switch (div_id) {
	case MCASP_CLKDIV_AUXCLK:			/* MCLK divider */
		mcasp_mod_bits(mcasp, DAVINCI_MCASP_AHCLKXCTL_REG,
			       AHCLKXDIV(div - 1), AHCLKXDIV_MASK);
		mcasp_mod_bits(mcasp, DAVINCI_MCASP_AHCLKRCTL_REG,
			       AHCLKRDIV(div - 1), AHCLKRDIV_MASK);
		break;

	case MCASP_CLKDIV_BCLK:			/* BCLK divider */
		mcasp_mod_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG,
			       ACLKXDIV(div - 1), ACLKXDIV_MASK);
		mcasp_mod_bits(mcasp, DAVINCI_MCASP_ACLKRCTL_REG,
			       ACLKRDIV(div - 1), ACLKRDIV_MASK);
		if (explicit)
			mcasp->bclk_div = div;
		break;

	case MCASP_CLKDIV_BCLK_FS_RATIO:
		/*
		 * BCLK/LRCLK ratio descries how many bit-clock cycles
		 * fit into one frame. The clock ratio is given for a
		 * full period of data (for I2S format both left and
		 * right channels), so it has to be divided by number
		 * of tdm-slots (for I2S - divided by 2).
		 * Instead of storing this ratio, we calculate a new
		 * tdm_slot width by dividing the the ratio by the
		 * number of configured tdm slots.
		 */
		mcasp->slot_width = div / mcasp->tdm_slots;
		if (div % mcasp->tdm_slots)
			dev_warn(mcasp->dev,
				 "%s(): BCLK/LRCLK %d is not divisible by %d tdm slots",
				 __func__, div, mcasp->tdm_slots);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;

	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	return 0;
}

static int davinci_mcasp_set_sysclk(struct snd_soc_dai *dai, int clk_id,
				    unsigned int freq, int dir)
{
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(dai);

	if (dir == SND_SOC_CLOCK_OUT) {
		mcasp_set_bits(dev->base + DAVINCI_MCASP_AHCLKXCTL_REG, AHCLKXE);
		mcasp_set_bits(dev->base + DAVINCI_MCASP_AHCLKRCTL_REG, AHCLKRE);
		mcasp_set_bits(dev->base + DAVINCI_MCASP_PDIR_REG, AHCLKX);
	} else {
		mcasp_clr_bits(dev->base + DAVINCI_MCASP_AHCLKXCTL_REG, AHCLKXE);
		mcasp_clr_bits(dev->base + DAVINCI_MCASP_AHCLKRCTL_REG, AHCLKRE);
		mcasp_clr_bits(dev->base + DAVINCI_MCASP_PDIR_REG, AHCLKX);
	}

	return 0;
}

static int davinci_config_channel_size(struct davinci_audio_dev *dev,
				       int word_length)
{
	u32 fmt;
	u32 tx_rotate = (word_length / 4) & 0x7;
	u32 mask = (1ULL << word_length) - 1;
=======
	pm_runtime_put(mcasp->dev);
	return 0;
}

static int davinci_mcasp_set_clkdiv(struct snd_soc_dai *dai, int div_id,
				    int div)
{
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(dai);

	return __davinci_mcasp_set_clkdiv(mcasp, div_id, div, 1);
}

static int davinci_mcasp_set_sysclk(struct snd_soc_dai *dai, int clk_id,
				    unsigned int freq, int dir)
{
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(dai);

	pm_runtime_get_sync(mcasp->dev);
	if (dir == SND_SOC_CLOCK_OUT) {
		mcasp_set_bits(mcasp, DAVINCI_MCASP_AHCLKXCTL_REG, AHCLKXE);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_AHCLKRCTL_REG, AHCLKRE);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_PDIR_REG, AHCLKX);
	} else {
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_AHCLKXCTL_REG, AHCLKXE);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_AHCLKRCTL_REG, AHCLKRE);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_PDIR_REG, AHCLKX);
	}

	mcasp->sysclk_freq = freq;

	pm_runtime_put(mcasp->dev);
	return 0;
}

/* All serializers must have equal number of channels */
static int davinci_mcasp_ch_constraint(struct davinci_mcasp *mcasp, int stream,
				       int serializers)
{
	struct snd_pcm_hw_constraint_list *cl = &mcasp->chconstr[stream];
	unsigned int *list = (unsigned int *) cl->list;
	int slots = mcasp->tdm_slots;
	int i, count = 0;

	if (mcasp->tdm_mask[stream])
		slots = hweight32(mcasp->tdm_mask[stream]);

	for (i = 2; i <= slots; i++)
		list[count++] = i;

	for (i = 2; i <= serializers; i++)
		list[count++] = i*slots;

	cl->count = count;

	return 0;
}

static int davinci_mcasp_set_ch_constraints(struct davinci_mcasp *mcasp)
{
	int rx_serializers = 0, tx_serializers = 0, ret, i;

	for (i = 0; i < mcasp->num_serializer; i++)
		if (mcasp->serial_dir[i] == TX_MODE)
			tx_serializers++;
		else if (mcasp->serial_dir[i] == RX_MODE)
			rx_serializers++;

	ret = davinci_mcasp_ch_constraint(mcasp, SNDRV_PCM_STREAM_PLAYBACK,
					  tx_serializers);
	if (ret)
		return ret;

	ret = davinci_mcasp_ch_constraint(mcasp, SNDRV_PCM_STREAM_CAPTURE,
					  rx_serializers);

	return ret;
}


static int davinci_mcasp_set_tdm_slot(struct snd_soc_dai *dai,
				      unsigned int tx_mask,
				      unsigned int rx_mask,
				      int slots, int slot_width)
{
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(dai);

	dev_dbg(mcasp->dev,
		 "%s() tx_mask 0x%08x rx_mask 0x%08x slots %d width %d\n",
		 __func__, tx_mask, rx_mask, slots, slot_width);

	if (tx_mask >= (1<<slots) || rx_mask >= (1<<slots)) {
		dev_err(mcasp->dev,
			"Bad tdm mask tx: 0x%08x rx: 0x%08x slots %d\n",
			tx_mask, rx_mask, slots);
		return -EINVAL;
	}

	if (slot_width &&
	    (slot_width < 8 || slot_width > 32 || slot_width % 4 != 0)) {
		dev_err(mcasp->dev, "%s: Unsupported slot_width %d\n",
			__func__, slot_width);
		return -EINVAL;
	}

	mcasp->tdm_slots = slots;
	mcasp->tdm_mask[SNDRV_PCM_STREAM_PLAYBACK] = tx_mask;
	mcasp->tdm_mask[SNDRV_PCM_STREAM_CAPTURE] = rx_mask;
	mcasp->slot_width = slot_width;

	return davinci_mcasp_set_ch_constraints(mcasp);
}

static int davinci_config_channel_size(struct davinci_mcasp *mcasp,
				       int sample_width)
{
	u32 fmt;
	u32 tx_rotate = (sample_width / 4) & 0x7;
	u32 mask = (1ULL << sample_width) - 1;
	u32 slot_width = sample_width;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * For captured data we should not rotate, inversion and masking is
	 * enoguh to get the data to the right position:
	 * Format	  data from bus		after reverse (XRBUF)
	 * S16_LE:	|LSB|MSB|xxx|xxx|	|xxx|xxx|MSB|LSB|
	 * S24_3LE:	|LSB|DAT|MSB|xxx|	|xxx|MSB|DAT|LSB|
	 * S24_LE:	|LSB|DAT|MSB|xxx|	|xxx|MSB|DAT|LSB|
	 * S32_LE:	|LSB|DAT|DAT|MSB|	|MSB|DAT|DAT|LSB|
	 */
	u32 rx_rotate = 0;

	/*
<<<<<<< HEAD
	 * if s BCLK-to-LRCLK ratio has been configured via the set_clkdiv()
	 * callback, take it into account here. That allows us to for example
	 * send 32 bits per channel to the codec, while only 16 of them carry
	 * audio payload.
	 * The clock ratio is given for a full period of data (for I2S format
	 * both left and right channels), so it has to be divided by number of
	 * tdm-slots (for I2S - divided by 2).
	 */
	if (dev->bclk_lrclk_ratio)
		word_length = dev->bclk_lrclk_ratio / dev->tdm_slots;

	/* mapping of the XSSZ bit-field as described in the datasheet */
	fmt = (word_length >> 1) - 1;

	if (dev->op_mode != DAVINCI_MCASP_DIT_MODE) {
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_RXFMT_REG,
				RXSSZ(fmt), RXSSZ(0x0F));
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_TXFMT_REG,
				TXSSZ(fmt), TXSSZ(0x0F));
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_TXFMT_REG,
				TXROT(tx_rotate), TXROT(7));
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_RXFMT_REG,
				RXROT(rx_rotate), RXROT(7));
		mcasp_set_reg(dev->base + DAVINCI_MCASP_RXMASK_REG,
				mask);
	}

	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXMASK_REG, mask);
=======
	 * Setting the tdm slot width either with set_clkdiv() or
	 * set_tdm_slot() allows us to for example send 32 bits per
	 * channel to the codec, while only 16 of them carry audio
	 * payload.
	 */
	if (mcasp->slot_width) {
		/*
		 * When we have more bclk then it is needed for the
		 * data, we need to use the rotation to move the
		 * received samples to have correct alignment.
		 */
		slot_width = mcasp->slot_width;
		rx_rotate = (slot_width - sample_width) / 4;
	}

	/* mapping of the XSSZ bit-field as described in the datasheet */
	fmt = (slot_width >> 1) - 1;

	if (mcasp->op_mode != DAVINCI_MCASP_DIT_MODE) {
		mcasp_mod_bits(mcasp, DAVINCI_MCASP_RXFMT_REG, RXSSZ(fmt),
			       RXSSZ(0x0F));
		mcasp_mod_bits(mcasp, DAVINCI_MCASP_TXFMT_REG, TXSSZ(fmt),
			       TXSSZ(0x0F));
		mcasp_mod_bits(mcasp, DAVINCI_MCASP_TXFMT_REG, TXROT(tx_rotate),
			       TXROT(7));
		mcasp_mod_bits(mcasp, DAVINCI_MCASP_RXFMT_REG, RXROT(rx_rotate),
			       RXROT(7));
		mcasp_set_reg(mcasp, DAVINCI_MCASP_RXMASK_REG, mask);
	}

	mcasp_set_reg(mcasp, DAVINCI_MCASP_TXMASK_REG, mask);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static int davinci_hw_common_param(struct davinci_audio_dev *dev, int stream,
				    int channels)
{
	int i;
	u8 tx_ser = 0;
	u8 rx_ser = 0;
	u8 ser;
	u8 slots = dev->tdm_slots;
	u8 max_active_serializers = (channels + slots - 1) / slots;
	/* Default configuration */
	mcasp_set_bits(dev->base + DAVINCI_MCASP_PWREMUMGT_REG, MCASP_SOFT);

	/* All PINS as McASP */
	mcasp_set_reg(dev->base + DAVINCI_MCASP_PFUNC_REG, 0x00000000);

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		mcasp_set_reg(dev->base + DAVINCI_MCASP_TXSTAT_REG, 0xFFFFFFFF);
		mcasp_clr_bits(dev->base + DAVINCI_MCASP_XEVTCTL_REG,
				TXDATADMADIS);
	} else {
		mcasp_set_reg(dev->base + DAVINCI_MCASP_RXSTAT_REG, 0xFFFFFFFF);
		mcasp_clr_bits(dev->base + DAVINCI_MCASP_REVTCTL_REG,
				RXDATADMADIS);
	}

	for (i = 0; i < dev->num_serializer; i++) {
		mcasp_set_bits(dev->base + DAVINCI_MCASP_XRSRCTL_REG(i),
					dev->serial_dir[i]);
		if (dev->serial_dir[i] == TX_MODE &&
					tx_ser < max_active_serializers) {
			mcasp_set_bits(dev->base + DAVINCI_MCASP_PDIR_REG,
					AXR(i));
			tx_ser++;
		} else if (dev->serial_dir[i] == RX_MODE &&
					rx_ser < max_active_serializers) {
			mcasp_clr_bits(dev->base + DAVINCI_MCASP_PDIR_REG,
					AXR(i));
			rx_ser++;
		} else {
			mcasp_mod_bits(dev->base + DAVINCI_MCASP_XRSRCTL_REG(i),
					SRMOD_INACTIVE, SRMOD_MASK);
		}
	}

	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		ser = tx_ser;
	else
		ser = rx_ser;

	if (ser < max_active_serializers) {
		dev_warn(dev->dev, "stream has more channels (%d) than are "
			"enabled in mcasp (%d)\n", channels, ser * slots);
		return -EINVAL;
	}

	if (dev->txnumevt && stream == SNDRV_PCM_STREAM_PLAYBACK) {
		if (dev->txnumevt * tx_ser > 64)
			dev->txnumevt = 1;

		switch (dev->version) {
		case MCASP_VERSION_3:
			mcasp_mod_bits(dev->base + MCASP_VER3_WFIFOCTL, tx_ser,
								NUMDMA_MASK);
			mcasp_mod_bits(dev->base + MCASP_VER3_WFIFOCTL,
				((dev->txnumevt * tx_ser) << 8), NUMEVT_MASK);
			break;
		default:
			mcasp_mod_bits(dev->base + DAVINCI_MCASP_WFIFOCTL,
							tx_ser,	NUMDMA_MASK);
			mcasp_mod_bits(dev->base + DAVINCI_MCASP_WFIFOCTL,
				((dev->txnumevt * tx_ser) << 8), NUMEVT_MASK);
		}
	}

	if (dev->rxnumevt && stream == SNDRV_PCM_STREAM_CAPTURE) {
		if (dev->rxnumevt * rx_ser > 64)
			dev->rxnumevt = 1;
		switch (dev->version) {
		case MCASP_VERSION_3:
			mcasp_mod_bits(dev->base + MCASP_VER3_RFIFOCTL, rx_ser,
								NUMDMA_MASK);
			mcasp_mod_bits(dev->base + MCASP_VER3_RFIFOCTL,
				((dev->rxnumevt * rx_ser) << 8), NUMEVT_MASK);
			break;
		default:
			mcasp_mod_bits(dev->base + DAVINCI_MCASP_RFIFOCTL,
							rx_ser,	NUMDMA_MASK);
			mcasp_mod_bits(dev->base + DAVINCI_MCASP_RFIFOCTL,
				((dev->rxnumevt * rx_ser) << 8), NUMEVT_MASK);
		}
	}

	return 0;
}

static void davinci_hw_param(struct davinci_audio_dev *dev, int stream)
{
	int i, active_slots;
	u32 mask = 0;

	active_slots = (dev->tdm_slots > 31) ? 32 : dev->tdm_slots;
	for (i = 0; i < active_slots; i++)
		mask |= (1 << i);

	mcasp_clr_bits(dev->base + DAVINCI_MCASP_ACLKXCTL_REG, TX_ASYNC);

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		/* bit stream is MSB first  with no delay */
		/* DSP_B mode */
		mcasp_set_reg(dev->base + DAVINCI_MCASP_TXTDM_REG, mask);
		mcasp_set_bits(dev->base + DAVINCI_MCASP_TXFMT_REG, TXORD);

		if ((dev->tdm_slots >= 2) && (dev->tdm_slots <= 32))
			mcasp_mod_bits(dev->base + DAVINCI_MCASP_TXFMCTL_REG,
					FSXMOD(dev->tdm_slots), FSXMOD(0x1FF));
		else
			printk(KERN_ERR "playback tdm slot %d not supported\n",
				dev->tdm_slots);
	} else {
		/* bit stream is MSB first with no delay */
		/* DSP_B mode */
		mcasp_set_bits(dev->base + DAVINCI_MCASP_RXFMT_REG, RXORD);
		mcasp_set_reg(dev->base + DAVINCI_MCASP_RXTDM_REG, mask);

		if ((dev->tdm_slots >= 2) && (dev->tdm_slots <= 32))
			mcasp_mod_bits(dev->base + DAVINCI_MCASP_RXFMCTL_REG,
					FSRMOD(dev->tdm_slots), FSRMOD(0x1FF));
		else
			printk(KERN_ERR "capture tdm slot %d not supported\n",
				dev->tdm_slots);
	}
}

/* S/PDIF */
static void davinci_hw_dit_param(struct davinci_audio_dev *dev)
{
	/* Set the TX format : 24 bit right rotation, 32 bit slot, Pad 0
	   and LSB first */
	mcasp_set_bits(dev->base + DAVINCI_MCASP_TXFMT_REG,
						TXROT(6) | TXSSZ(15));

	/* Set TX frame synch : DIT Mode, 1 bit width, internal, rising edge */
	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXFMCTL_REG,
						AFSXE | FSXMOD(0x180));

	/* Set the TX tdm : for all the slots */
	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXTDM_REG, 0xFFFFFFFF);

	/* Set the TX clock controls : div = 1 and internal */
	mcasp_set_bits(dev->base + DAVINCI_MCASP_ACLKXCTL_REG,
						ACLKXE | TX_ASYNC);

	mcasp_clr_bits(dev->base + DAVINCI_MCASP_XEVTCTL_REG, TXDATADMADIS);

	/* Only 44100 and 48000 are valid, both have the same setting */
	mcasp_set_bits(dev->base + DAVINCI_MCASP_AHCLKXCTL_REG, AHCLKXDIV(3));

	/* Enable the DIT */
	mcasp_set_bits(dev->base + DAVINCI_MCASP_TXDITCTL_REG, DITEN);
=======
static int mcasp_common_hw_param(struct davinci_mcasp *mcasp, int stream,
				 int period_words, int channels)
{
	struct snd_dmaengine_dai_dma_data *dma_data = &mcasp->dma_data[stream];
	int i;
	u8 tx_ser = 0;
	u8 rx_ser = 0;
	u8 slots = mcasp->tdm_slots;
	u8 max_active_serializers = (channels + slots - 1) / slots;
	int active_serializers, numevt;
	u32 reg;
	/* Default configuration */
	if (mcasp->version < MCASP_VERSION_3)
		mcasp_set_bits(mcasp, DAVINCI_MCASP_PWREMUMGT_REG, MCASP_SOFT);

	/* All PINS as McASP */
	mcasp_set_reg(mcasp, DAVINCI_MCASP_PFUNC_REG, 0x00000000);

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		mcasp_set_reg(mcasp, DAVINCI_MCASP_TXSTAT_REG, 0xFFFFFFFF);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_XEVTCTL_REG, TXDATADMADIS);
	} else {
		mcasp_set_reg(mcasp, DAVINCI_MCASP_RXSTAT_REG, 0xFFFFFFFF);
		mcasp_clr_bits(mcasp, DAVINCI_MCASP_REVTCTL_REG, RXDATADMADIS);
	}

	for (i = 0; i < mcasp->num_serializer; i++) {
		mcasp_set_bits(mcasp, DAVINCI_MCASP_XRSRCTL_REG(i),
			       mcasp->serial_dir[i]);
		if (mcasp->serial_dir[i] == TX_MODE &&
					tx_ser < max_active_serializers) {
			mcasp_set_bits(mcasp, DAVINCI_MCASP_PDIR_REG, AXR(i));
			mcasp_mod_bits(mcasp, DAVINCI_MCASP_XRSRCTL_REG(i),
				       DISMOD_LOW, DISMOD_MASK);
			tx_ser++;
		} else if (mcasp->serial_dir[i] == RX_MODE &&
					rx_ser < max_active_serializers) {
			mcasp_clr_bits(mcasp, DAVINCI_MCASP_PDIR_REG, AXR(i));
			rx_ser++;
		} else {
			mcasp_mod_bits(mcasp, DAVINCI_MCASP_XRSRCTL_REG(i),
				       SRMOD_INACTIVE, SRMOD_MASK);
		}
	}

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		active_serializers = tx_ser;
		numevt = mcasp->txnumevt;
		reg = mcasp->fifo_base + MCASP_WFIFOCTL_OFFSET;
	} else {
		active_serializers = rx_ser;
		numevt = mcasp->rxnumevt;
		reg = mcasp->fifo_base + MCASP_RFIFOCTL_OFFSET;
	}

	if (active_serializers < max_active_serializers) {
		dev_warn(mcasp->dev, "stream has more channels (%d) than are "
			 "enabled in mcasp (%d)\n", channels,
			 active_serializers * slots);
		return -EINVAL;
	}

	/* AFIFO is not in use */
	if (!numevt) {
		/* Configure the burst size for platform drivers */
		if (active_serializers > 1) {
			/*
			 * If more than one serializers are in use we have one
			 * DMA request to provide data for all serializers.
			 * For example if three serializers are enabled the DMA
			 * need to transfer three words per DMA request.
			 */
			dma_data->maxburst = active_serializers;
		} else {
			dma_data->maxburst = 0;
		}
		return 0;
	}

	if (period_words % active_serializers) {
		dev_err(mcasp->dev, "Invalid combination of period words and "
			"active serializers: %d, %d\n", period_words,
			active_serializers);
		return -EINVAL;
	}

	/*
	 * Calculate the optimal AFIFO depth for platform side:
	 * The number of words for numevt need to be in steps of active
	 * serializers.
	 */
	numevt = (numevt / active_serializers) * active_serializers;

	while (period_words % numevt && numevt > 0)
		numevt -= active_serializers;
	if (numevt <= 0)
		numevt = active_serializers;

	mcasp_mod_bits(mcasp, reg, active_serializers, NUMDMA_MASK);
	mcasp_mod_bits(mcasp, reg, NUMEVT(numevt), NUMEVT_MASK);

	/* Configure the burst size for platform drivers */
	if (numevt == 1)
		numevt = 0;
	dma_data->maxburst = numevt;

	return 0;
}

static int mcasp_i2s_hw_param(struct davinci_mcasp *mcasp, int stream,
			      int channels)
{
	int i, active_slots;
	int total_slots;
	int active_serializers;
	u32 mask = 0;
	u32 busel = 0;

	total_slots = mcasp->tdm_slots;

	/*
	 * If more than one serializer is needed, then use them with
	 * all the specified tdm_slots. Otherwise, one serializer can
	 * cope with the transaction using just as many slots as there
	 * are channels in the stream.
	 */
	if (mcasp->tdm_mask[stream]) {
		active_slots = hweight32(mcasp->tdm_mask[stream]);
		active_serializers = (channels + active_slots - 1) /
			active_slots;
		if (active_serializers == 1)
			active_slots = channels;
		for (i = 0; i < total_slots; i++) {
			if ((1 << i) & mcasp->tdm_mask[stream]) {
				mask |= (1 << i);
				if (--active_slots <= 0)
					break;
			}
		}
	} else {
		active_serializers = (channels + total_slots - 1) / total_slots;
		if (active_serializers == 1)
			active_slots = channels;
		else
			active_slots = total_slots;

		for (i = 0; i < active_slots; i++)
			mask |= (1 << i);
	}
	mcasp_clr_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG, TX_ASYNC);

	if (!mcasp->dat_port)
		busel = TXSEL;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		mcasp_set_reg(mcasp, DAVINCI_MCASP_TXTDM_REG, mask);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_TXFMT_REG, busel | TXORD);
		mcasp_mod_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG,
			       FSXMOD(total_slots), FSXMOD(0x1FF));
	} else if (stream == SNDRV_PCM_STREAM_CAPTURE) {
		mcasp_set_reg(mcasp, DAVINCI_MCASP_RXTDM_REG, mask);
		mcasp_set_bits(mcasp, DAVINCI_MCASP_RXFMT_REG, busel | RXORD);
		mcasp_mod_bits(mcasp, DAVINCI_MCASP_RXFMCTL_REG,
			       FSRMOD(total_slots), FSRMOD(0x1FF));
		/*
		 * If McASP is set to be TX/RX synchronous and the playback is
		 * not running already we need to configure the TX slots in
		 * order to have correct FSX on the bus
		 */
		if (mcasp_is_synchronous(mcasp) && !mcasp->channels)
			mcasp_mod_bits(mcasp, DAVINCI_MCASP_TXFMCTL_REG,
				       FSXMOD(total_slots), FSXMOD(0x1FF));
	}

	return 0;
}

/* S/PDIF */
static int mcasp_dit_hw_param(struct davinci_mcasp *mcasp,
			      unsigned int rate)
{
	u32 cs_value = 0;
	u8 *cs_bytes = (u8*) &cs_value;

	/* Set the TX format : 24 bit right rotation, 32 bit slot, Pad 0
	   and LSB first */
	mcasp_set_bits(mcasp, DAVINCI_MCASP_TXFMT_REG, TXROT(6) | TXSSZ(15));

	/* Set TX frame synch : DIT Mode, 1 bit width, internal, rising edge */
	mcasp_set_reg(mcasp, DAVINCI_MCASP_TXFMCTL_REG, AFSXE | FSXMOD(0x180));

	/* Set the TX tdm : for all the slots */
	mcasp_set_reg(mcasp, DAVINCI_MCASP_TXTDM_REG, 0xFFFFFFFF);

	/* Set the TX clock controls : div = 1 and internal */
	mcasp_set_bits(mcasp, DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE | TX_ASYNC);

	mcasp_clr_bits(mcasp, DAVINCI_MCASP_XEVTCTL_REG, TXDATADMADIS);

	/* Only 44100 and 48000 are valid, both have the same setting */
	mcasp_set_bits(mcasp, DAVINCI_MCASP_AHCLKXCTL_REG, AHCLKXDIV(3));

	/* Enable the DIT */
	mcasp_set_bits(mcasp, DAVINCI_MCASP_TXDITCTL_REG, DITEN);

	/* Set S/PDIF channel status bits */
	cs_bytes[0] = IEC958_AES0_CON_NOT_COPYRIGHT;
	cs_bytes[1] = IEC958_AES1_CON_PCM_CODER;

	switch (rate) {
	case 22050:
		cs_bytes[3] |= IEC958_AES3_CON_FS_22050;
		break;
	case 24000:
		cs_bytes[3] |= IEC958_AES3_CON_FS_24000;
		break;
	case 32000:
		cs_bytes[3] |= IEC958_AES3_CON_FS_32000;
		break;
	case 44100:
		cs_bytes[3] |= IEC958_AES3_CON_FS_44100;
		break;
	case 48000:
		cs_bytes[3] |= IEC958_AES3_CON_FS_48000;
		break;
	case 88200:
		cs_bytes[3] |= IEC958_AES3_CON_FS_88200;
		break;
	case 96000:
		cs_bytes[3] |= IEC958_AES3_CON_FS_96000;
		break;
	case 176400:
		cs_bytes[3] |= IEC958_AES3_CON_FS_176400;
		break;
	case 192000:
		cs_bytes[3] |= IEC958_AES3_CON_FS_192000;
		break;
	default:
		printk(KERN_WARNING "unsupported sampling rate: %d\n", rate);
		return -EINVAL;
	}

	mcasp_set_reg(mcasp, DAVINCI_MCASP_DITCSRA_REG, cs_value);
	mcasp_set_reg(mcasp, DAVINCI_MCASP_DITCSRB_REG, cs_value);

	return 0;
}

static int davinci_mcasp_calc_clk_div(struct davinci_mcasp *mcasp,
				      unsigned int bclk_freq, bool set)
{
	int error_ppm;
	unsigned int sysclk_freq = mcasp->sysclk_freq;
	u32 reg = mcasp_get_reg(mcasp, DAVINCI_MCASP_AHCLKXCTL_REG);
	int div = sysclk_freq / bclk_freq;
	int rem = sysclk_freq % bclk_freq;
	int aux_div = 1;

	if (div > (ACLKXDIV_MASK + 1)) {
		if (reg & AHCLKXE) {
			aux_div = div / (ACLKXDIV_MASK + 1);
			if (div % (ACLKXDIV_MASK + 1))
				aux_div++;

			sysclk_freq /= aux_div;
			div = sysclk_freq / bclk_freq;
			rem = sysclk_freq % bclk_freq;
		} else if (set) {
			dev_warn(mcasp->dev, "Too fast reference clock (%u)\n",
				 sysclk_freq);
		}
	}

	if (rem != 0) {
		if (div == 0 ||
		    ((sysclk_freq / div) - bclk_freq) >
		    (bclk_freq - (sysclk_freq / (div+1)))) {
			div++;
			rem = rem - bclk_freq;
		}
	}
	error_ppm = (div*1000000 + (int)div64_long(1000000LL*rem,
		     (int)bclk_freq)) / div - 1000000;

	if (set) {
		if (error_ppm)
			dev_info(mcasp->dev, "Sample-rate is off by %d PPM\n",
				 error_ppm);

		__davinci_mcasp_set_clkdiv(mcasp, MCASP_CLKDIV_BCLK, div, 0);
		if (reg & AHCLKXE)
			__davinci_mcasp_set_clkdiv(mcasp, MCASP_CLKDIV_AUXCLK,
						   aux_div, 0);
	}

	return error_ppm;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int davinci_mcasp_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params,
					struct snd_soc_dai *cpu_dai)
{
<<<<<<< HEAD
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(cpu_dai);
	struct davinci_pcm_dma_params *dma_params =
					&dev->dma_params[substream->stream];
	int word_length;
	u8 fifo_level;
	u8 slots = dev->tdm_slots;
	u8 active_serializers;
	int channels;
	struct snd_interval *pcm_channels = hw_param_interval(params,
					SNDRV_PCM_HW_PARAM_CHANNELS);
	channels = pcm_channels->min;

	active_serializers = (channels + slots - 1) / slots;

	if (davinci_hw_common_param(dev, substream->stream, channels) == -EINVAL)
		return -EINVAL;
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		fifo_level = dev->txnumevt * active_serializers;
	else
		fifo_level = dev->rxnumevt * active_serializers;

	if (dev->op_mode == DAVINCI_MCASP_DIT_MODE)
		davinci_hw_dit_param(dev);
	else
		davinci_hw_param(dev, substream->stream);
=======
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(cpu_dai);
	int word_length;
	int channels = params_channels(params);
	int period_size = params_period_size(params);
	int ret;

	ret = davinci_mcasp_set_dai_fmt(cpu_dai, mcasp->dai_fmt);
	if (ret)
		return ret;

	/*
	 * If mcasp is BCLK master, and a BCLK divider was not provided by
	 * the machine driver, we need to calculate the ratio.
	 */
	if (mcasp->bclk_master && mcasp->bclk_div == 0 && mcasp->sysclk_freq) {
		int slots = mcasp->tdm_slots;
		int rate = params_rate(params);
		int sbits = params_width(params);

		if (mcasp->slot_width)
			sbits = mcasp->slot_width;

		davinci_mcasp_calc_clk_div(mcasp, rate * sbits * slots, true);
	}

	ret = mcasp_common_hw_param(mcasp, substream->stream,
				    period_size * channels, channels);
	if (ret)
		return ret;

	if (mcasp->op_mode == DAVINCI_MCASP_DIT_MODE)
		ret = mcasp_dit_hw_param(mcasp, params_rate(params));
	else
		ret = mcasp_i2s_hw_param(mcasp, substream->stream,
					 channels);

	if (ret)
		return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_U8:
	case SNDRV_PCM_FORMAT_S8:
<<<<<<< HEAD
		dma_params->data_type = 1;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		word_length = 8;
		break;

	case SNDRV_PCM_FORMAT_U16_LE:
	case SNDRV_PCM_FORMAT_S16_LE:
<<<<<<< HEAD
		dma_params->data_type = 2;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		word_length = 16;
		break;

	case SNDRV_PCM_FORMAT_U24_3LE:
	case SNDRV_PCM_FORMAT_S24_3LE:
<<<<<<< HEAD
		dma_params->data_type = 3;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		word_length = 24;
		break;

	case SNDRV_PCM_FORMAT_U24_LE:
	case SNDRV_PCM_FORMAT_S24_LE:
<<<<<<< HEAD
	case SNDRV_PCM_FORMAT_U32_LE:
	case SNDRV_PCM_FORMAT_S32_LE:
		dma_params->data_type = 4;
=======
		word_length = 24;
		break;

	case SNDRV_PCM_FORMAT_U32_LE:
	case SNDRV_PCM_FORMAT_S32_LE:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		word_length = 32;
		break;

	default:
		printk(KERN_WARNING "davinci-mcasp: unsupported PCM format");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (dev->version == MCASP_VERSION_2 && !fifo_level)
		dma_params->acnt = 4;
	else
		dma_params->acnt = dma_params->data_type;

	dma_params->fifo_level = fifo_level;
	davinci_config_channel_size(dev, word_length);
=======
	davinci_config_channel_size(mcasp, word_length);

	if (mcasp->op_mode == DAVINCI_MCASP_IIS_MODE)
		mcasp->channels = channels;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static int davinci_mcasp_trigger(struct snd_pcm_substream *substream,
				     int cmd, struct snd_soc_dai *cpu_dai)
{
<<<<<<< HEAD
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(cpu_dai);
=======
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(cpu_dai);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret = 0;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
<<<<<<< HEAD
		ret = pm_runtime_get_sync(dev->dev);
		if (IS_ERR_VALUE(ret))
			dev_err(dev->dev, "pm_runtime_get_sync() failed\n");
		davinci_mcasp_start(dev, substream->stream);
		break;

	case SNDRV_PCM_TRIGGER_SUSPEND:
		davinci_mcasp_stop(dev, substream->stream);
		ret = pm_runtime_put_sync(dev->dev);
		if (IS_ERR_VALUE(ret))
			dev_err(dev->dev, "pm_runtime_put_sync() failed\n");
		break;

	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		davinci_mcasp_stop(dev, substream->stream);
=======
		davinci_mcasp_start(mcasp, substream->stream);
		break;
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		davinci_mcasp_stop(mcasp, substream->stream);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;

	default:
		ret = -EINVAL;
	}

	return ret;
}

<<<<<<< HEAD
static int davinci_mcasp_startup(struct snd_pcm_substream *substream,
				 struct snd_soc_dai *dai)
{
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(dai);

	snd_soc_dai_set_dma_data(dai, substream, dev->dma_params);
	return 0;
}

static const struct snd_soc_dai_ops davinci_mcasp_dai_ops = {
	.startup	= davinci_mcasp_startup,
=======
static int davinci_mcasp_hw_rule_slot_width(struct snd_pcm_hw_params *params,
					    struct snd_pcm_hw_rule *rule)
{
	struct davinci_mcasp_ruledata *rd = rule->private;
	struct snd_mask *fmt = hw_param_mask(params, SNDRV_PCM_HW_PARAM_FORMAT);
	struct snd_mask nfmt;
	int i, slot_width;

	snd_mask_none(&nfmt);
	slot_width = rd->mcasp->slot_width;

	for (i = 0; i <= SNDRV_PCM_FORMAT_LAST; i++) {
		if (snd_mask_test(fmt, i)) {
			if (snd_pcm_format_width(i) <= slot_width) {
				snd_mask_set(&nfmt, i);
			}
		}
	}

	return snd_mask_refine(fmt, &nfmt);
}

static const unsigned int davinci_mcasp_dai_rates[] = {
	8000, 11025, 16000, 22050, 32000, 44100, 48000, 64000,
	88200, 96000, 176400, 192000,
};

#define DAVINCI_MAX_RATE_ERROR_PPM 1000

static int davinci_mcasp_hw_rule_rate(struct snd_pcm_hw_params *params,
				      struct snd_pcm_hw_rule *rule)
{
	struct davinci_mcasp_ruledata *rd = rule->private;
	struct snd_interval *ri =
		hw_param_interval(params, SNDRV_PCM_HW_PARAM_RATE);
	int sbits = params_width(params);
	int slots = rd->mcasp->tdm_slots;
	struct snd_interval range;
	int i;

	if (rd->mcasp->slot_width)
		sbits = rd->mcasp->slot_width;

	snd_interval_any(&range);
	range.empty = 1;

	for (i = 0; i < ARRAY_SIZE(davinci_mcasp_dai_rates); i++) {
		if (snd_interval_test(ri, davinci_mcasp_dai_rates[i])) {
			uint bclk_freq = sbits*slots*
				davinci_mcasp_dai_rates[i];
			int ppm;

			ppm = davinci_mcasp_calc_clk_div(rd->mcasp, bclk_freq,
							 false);
			if (abs(ppm) < DAVINCI_MAX_RATE_ERROR_PPM) {
				if (range.empty) {
					range.min = davinci_mcasp_dai_rates[i];
					range.empty = 0;
				}
				range.max = davinci_mcasp_dai_rates[i];
			}
		}
	}

	dev_dbg(rd->mcasp->dev,
		"Frequencies %d-%d -> %d-%d for %d sbits and %d tdm slots\n",
		ri->min, ri->max, range.min, range.max, sbits, slots);

	return snd_interval_refine(hw_param_interval(params, rule->var),
				   &range);
}

static int davinci_mcasp_hw_rule_format(struct snd_pcm_hw_params *params,
					struct snd_pcm_hw_rule *rule)
{
	struct davinci_mcasp_ruledata *rd = rule->private;
	struct snd_mask *fmt = hw_param_mask(params, SNDRV_PCM_HW_PARAM_FORMAT);
	struct snd_mask nfmt;
	int rate = params_rate(params);
	int slots = rd->mcasp->tdm_slots;
	int i, count = 0;

	snd_mask_none(&nfmt);

	for (i = 0; i <= SNDRV_PCM_FORMAT_LAST; i++) {
		if (snd_mask_test(fmt, i)) {
			uint sbits = snd_pcm_format_width(i);
			int ppm;

			if (rd->mcasp->slot_width)
				sbits = rd->mcasp->slot_width;

			ppm = davinci_mcasp_calc_clk_div(rd->mcasp,
							 sbits * slots * rate,
							 false);
			if (abs(ppm) < DAVINCI_MAX_RATE_ERROR_PPM) {
				snd_mask_set(&nfmt, i);
				count++;
			}
		}
	}
	dev_dbg(rd->mcasp->dev,
		"%d possible sample format for %d Hz and %d tdm slots\n",
		count, rate, slots);

	return snd_mask_refine(fmt, &nfmt);
}

static int davinci_mcasp_startup(struct snd_pcm_substream *substream,
				 struct snd_soc_dai *cpu_dai)
{
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(cpu_dai);
	struct davinci_mcasp_ruledata *ruledata =
					&mcasp->ruledata[substream->stream];
	u32 max_channels = 0;
	int i, dir, ret;
	int tdm_slots = mcasp->tdm_slots;

	/* Do not allow more then one stream per direction */
	if (mcasp->substreams[substream->stream])
		return -EBUSY;

	mcasp->substreams[substream->stream] = substream;

	if (mcasp->tdm_mask[substream->stream])
		tdm_slots = hweight32(mcasp->tdm_mask[substream->stream]);

	if (mcasp->op_mode == DAVINCI_MCASP_DIT_MODE)
		return 0;

	/*
	 * Limit the maximum allowed channels for the first stream:
	 * number of serializers for the direction * tdm slots per serializer
	 */
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dir = TX_MODE;
	else
		dir = RX_MODE;

	for (i = 0; i < mcasp->num_serializer; i++) {
		if (mcasp->serial_dir[i] == dir)
			max_channels++;
	}
	ruledata->serializers = max_channels;
	ruledata->mcasp = mcasp;
	max_channels *= tdm_slots;
	/*
	 * If the already active stream has less channels than the calculated
	 * limnit based on the seirializers * tdm_slots, we need to use that as
	 * a constraint for the second stream.
	 * Otherwise (first stream or less allowed channels) we use the
	 * calculated constraint.
	 */
	if (mcasp->channels && mcasp->channels < max_channels)
		max_channels = mcasp->channels;
	/*
	 * But we can always allow channels upto the amount of
	 * the available tdm_slots.
	 */
	if (max_channels < tdm_slots)
		max_channels = tdm_slots;

	snd_pcm_hw_constraint_minmax(substream->runtime,
				     SNDRV_PCM_HW_PARAM_CHANNELS,
				     2, max_channels);

	snd_pcm_hw_constraint_list(substream->runtime,
				   0, SNDRV_PCM_HW_PARAM_CHANNELS,
				   &mcasp->chconstr[substream->stream]);

	if (mcasp->slot_width) {
		/* Only allow formats require <= slot_width bits on the bus */
		ret = snd_pcm_hw_rule_add(substream->runtime, 0,
					  SNDRV_PCM_HW_PARAM_FORMAT,
					  davinci_mcasp_hw_rule_slot_width,
					  ruledata,
					  SNDRV_PCM_HW_PARAM_FORMAT, -1);
		if (ret)
			return ret;
	}

	/*
	 * If we rely on implicit BCLK divider setting we should
	 * set constraints based on what we can provide.
	 */
	if (mcasp->bclk_master && mcasp->bclk_div == 0 && mcasp->sysclk_freq) {
		ret = snd_pcm_hw_rule_add(substream->runtime, 0,
					  SNDRV_PCM_HW_PARAM_RATE,
					  davinci_mcasp_hw_rule_rate,
					  ruledata,
					  SNDRV_PCM_HW_PARAM_FORMAT, -1);
		if (ret)
			return ret;
		ret = snd_pcm_hw_rule_add(substream->runtime, 0,
					  SNDRV_PCM_HW_PARAM_FORMAT,
					  davinci_mcasp_hw_rule_format,
					  ruledata,
					  SNDRV_PCM_HW_PARAM_RATE, -1);
		if (ret)
			return ret;
	}

	return 0;
}

static void davinci_mcasp_shutdown(struct snd_pcm_substream *substream,
				   struct snd_soc_dai *cpu_dai)
{
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(cpu_dai);

	mcasp->substreams[substream->stream] = NULL;

	if (mcasp->op_mode == DAVINCI_MCASP_DIT_MODE)
		return;

	if (!cpu_dai->active)
		mcasp->channels = 0;
}

static const struct snd_soc_dai_ops davinci_mcasp_dai_ops = {
	.startup	= davinci_mcasp_startup,
	.shutdown	= davinci_mcasp_shutdown,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.trigger	= davinci_mcasp_trigger,
	.hw_params	= davinci_mcasp_hw_params,
	.set_fmt	= davinci_mcasp_set_dai_fmt,
	.set_clkdiv	= davinci_mcasp_set_clkdiv,
	.set_sysclk	= davinci_mcasp_set_sysclk,
<<<<<<< HEAD
};

=======
	.set_tdm_slot	= davinci_mcasp_set_tdm_slot,
};

static int davinci_mcasp_dai_probe(struct snd_soc_dai *dai)
{
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(dai);

	dai->playback_dma_data = &mcasp->dma_data[SNDRV_PCM_STREAM_PLAYBACK];
	dai->capture_dma_data = &mcasp->dma_data[SNDRV_PCM_STREAM_CAPTURE];

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int davinci_mcasp_suspend(struct snd_soc_dai *dai)
{
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(dai);
	struct davinci_mcasp_context *context = &mcasp->context;
	u32 reg;
	int i;

	context->pm_state = pm_runtime_active(mcasp->dev);
	if (!context->pm_state)
		pm_runtime_get_sync(mcasp->dev);

	for (i = 0; i < ARRAY_SIZE(context_regs); i++)
		context->config_regs[i] = mcasp_get_reg(mcasp, context_regs[i]);

	if (mcasp->txnumevt) {
		reg = mcasp->fifo_base + MCASP_WFIFOCTL_OFFSET;
		context->afifo_regs[0] = mcasp_get_reg(mcasp, reg);
	}
	if (mcasp->rxnumevt) {
		reg = mcasp->fifo_base + MCASP_RFIFOCTL_OFFSET;
		context->afifo_regs[1] = mcasp_get_reg(mcasp, reg);
	}

	for (i = 0; i < mcasp->num_serializer; i++)
		context->xrsr_regs[i] = mcasp_get_reg(mcasp,
						DAVINCI_MCASP_XRSRCTL_REG(i));

	pm_runtime_put_sync(mcasp->dev);

	return 0;
}

static int davinci_mcasp_resume(struct snd_soc_dai *dai)
{
	struct davinci_mcasp *mcasp = snd_soc_dai_get_drvdata(dai);
	struct davinci_mcasp_context *context = &mcasp->context;
	u32 reg;
	int i;

	pm_runtime_get_sync(mcasp->dev);

	for (i = 0; i < ARRAY_SIZE(context_regs); i++)
		mcasp_set_reg(mcasp, context_regs[i], context->config_regs[i]);

	if (mcasp->txnumevt) {
		reg = mcasp->fifo_base + MCASP_WFIFOCTL_OFFSET;
		mcasp_set_reg(mcasp, reg, context->afifo_regs[0]);
	}
	if (mcasp->rxnumevt) {
		reg = mcasp->fifo_base + MCASP_RFIFOCTL_OFFSET;
		mcasp_set_reg(mcasp, reg, context->afifo_regs[1]);
	}

	for (i = 0; i < mcasp->num_serializer; i++)
		mcasp_set_reg(mcasp, DAVINCI_MCASP_XRSRCTL_REG(i),
			      context->xrsr_regs[i]);

	if (!context->pm_state)
		pm_runtime_put_sync(mcasp->dev);

	return 0;
}
#else
#define davinci_mcasp_suspend NULL
#define davinci_mcasp_resume NULL
#endif

#define DAVINCI_MCASP_RATES	SNDRV_PCM_RATE_8000_192000

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define DAVINCI_MCASP_PCM_FMTS (SNDRV_PCM_FMTBIT_S8 | \
				SNDRV_PCM_FMTBIT_U8 | \
				SNDRV_PCM_FMTBIT_S16_LE | \
				SNDRV_PCM_FMTBIT_U16_LE | \
				SNDRV_PCM_FMTBIT_S24_LE | \
				SNDRV_PCM_FMTBIT_U24_LE | \
				SNDRV_PCM_FMTBIT_S24_3LE | \
				SNDRV_PCM_FMTBIT_U24_3LE | \
				SNDRV_PCM_FMTBIT_S32_LE | \
				SNDRV_PCM_FMTBIT_U32_LE)

static struct snd_soc_dai_driver davinci_mcasp_dai[] = {
	{
		.name		= "davinci-mcasp.0",
<<<<<<< HEAD
=======
		.probe		= davinci_mcasp_dai_probe,
		.suspend	= davinci_mcasp_suspend,
		.resume		= davinci_mcasp_resume,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.playback	= {
			.channels_min	= 2,
			.channels_max	= 32 * 16,
			.rates 		= DAVINCI_MCASP_RATES,
			.formats	= DAVINCI_MCASP_PCM_FMTS,
		},
		.capture 	= {
			.channels_min 	= 2,
			.channels_max	= 32 * 16,
			.rates 		= DAVINCI_MCASP_RATES,
			.formats	= DAVINCI_MCASP_PCM_FMTS,
		},
		.ops 		= &davinci_mcasp_dai_ops,

<<<<<<< HEAD
	},
	{
		"davinci-mcasp.1",
=======
		.symmetric_samplebits	= 1,
		.symmetric_rates	= 1,
	},
	{
		.name		= "davinci-mcasp.1",
		.probe		= davinci_mcasp_dai_probe,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.playback 	= {
			.channels_min	= 1,
			.channels_max	= 384,
			.rates		= DAVINCI_MCASP_RATES,
			.formats	= DAVINCI_MCASP_PCM_FMTS,
		},
		.ops 		= &davinci_mcasp_dai_ops,
	},

};

static const struct snd_soc_component_driver davinci_mcasp_component = {
	.name		= "davinci-mcasp",
};

<<<<<<< HEAD
static const struct of_device_id mcasp_dt_ids[] = {
	{
		.compatible = "ti,dm646x-mcasp-audio",
		.data = (void *)MCASP_VERSION_1,
	},
	{
		.compatible = "ti,da830-mcasp-audio",
		.data = (void *)MCASP_VERSION_2,
	},
	{
		.compatible = "ti,omap2-mcasp-audio",
		.data = (void *)MCASP_VERSION_3,
=======
/* Some HW specific values and defaults. The rest is filled in from DT. */
static struct davinci_mcasp_pdata dm646x_mcasp_pdata = {
	.tx_dma_offset = 0x400,
	.rx_dma_offset = 0x400,
	.version = MCASP_VERSION_1,
};

static struct davinci_mcasp_pdata da830_mcasp_pdata = {
	.tx_dma_offset = 0x2000,
	.rx_dma_offset = 0x2000,
	.version = MCASP_VERSION_2,
};

static struct davinci_mcasp_pdata am33xx_mcasp_pdata = {
	.tx_dma_offset = 0,
	.rx_dma_offset = 0,
	.version = MCASP_VERSION_3,
};

static struct davinci_mcasp_pdata dra7_mcasp_pdata = {
	/* The CFG port offset will be calculated if it is needed */
	.tx_dma_offset = 0,
	.rx_dma_offset = 0,
	.version = MCASP_VERSION_4,
};

static const struct of_device_id mcasp_dt_ids[] = {
	{
		.compatible = "ti,dm646x-mcasp-audio",
		.data = &dm646x_mcasp_pdata,
	},
	{
		.compatible = "ti,da830-mcasp-audio",
		.data = &da830_mcasp_pdata,
	},
	{
		.compatible = "ti,am33xx-mcasp-audio",
		.data = &am33xx_mcasp_pdata,
	},
	{
		.compatible = "ti,dra7-mcasp-audio",
		.data = &dra7_mcasp_pdata,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mcasp_dt_ids);

<<<<<<< HEAD
static struct snd_platform_data *davinci_mcasp_set_pdata_from_of(
						struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct snd_platform_data *pdata = NULL;
	const struct of_device_id *match =
			of_match_device(of_match_ptr(mcasp_dt_ids), &pdev->dev);

	const u32 *of_serial_dir32;
	u8 *of_serial_dir;
=======
static int mcasp_reparent_fck(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct clk *gfclk, *parent_clk;
	const char *parent_name;
	int ret;

	if (!node)
		return 0;

	parent_name = of_get_property(node, "fck_parent", NULL);
	if (!parent_name)
		return 0;

	dev_warn(&pdev->dev, "Update the bindings to use assigned-clocks!\n");

	gfclk = clk_get(&pdev->dev, "fck");
	if (IS_ERR(gfclk)) {
		dev_err(&pdev->dev, "failed to get fck\n");
		return PTR_ERR(gfclk);
	}

	parent_clk = clk_get(NULL, parent_name);
	if (IS_ERR(parent_clk)) {
		dev_err(&pdev->dev, "failed to get parent clock\n");
		ret = PTR_ERR(parent_clk);
		goto err1;
	}

	ret = clk_set_parent(gfclk, parent_clk);
	if (ret) {
		dev_err(&pdev->dev, "failed to reparent fck\n");
		goto err2;
	}

err2:
	clk_put(parent_clk);
err1:
	clk_put(gfclk);
	return ret;
}

static struct davinci_mcasp_pdata *davinci_mcasp_set_pdata_from_of(
						struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct davinci_mcasp_pdata *pdata = NULL;
	const struct of_device_id *match =
			of_match_device(mcasp_dt_ids, &pdev->dev);
	struct of_phandle_args dma_spec;

	const u32 *of_serial_dir32;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32 val;
	int i, ret = 0;

	if (pdev->dev.platform_data) {
		pdata = pdev->dev.platform_data;
		return pdata;
	} else if (match) {
<<<<<<< HEAD
		pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata) {
			ret = -ENOMEM;
			goto nodata;
=======
		pdata = devm_kmemdup(&pdev->dev, match->data, sizeof(*pdata),
				     GFP_KERNEL);
		if (!pdata) {
			dev_err(&pdev->dev,
				"Failed to allocate memory for pdata\n");
			ret = -ENOMEM;
			return pdata;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	} else {
		/* control shouldn't reach here. something is wrong */
		ret = -EINVAL;
		goto nodata;
	}

<<<<<<< HEAD
	if (match->data)
		pdata->version = (u8)((int)match->data);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ret = of_property_read_u32(np, "op-mode", &val);
	if (ret >= 0)
		pdata->op_mode = val;

	ret = of_property_read_u32(np, "tdm-slots", &val);
	if (ret >= 0) {
		if (val < 2 || val > 32) {
			dev_err(&pdev->dev,
				"tdm-slots must be in rage [2-32]\n");
			ret = -EINVAL;
			goto nodata;
		}

		pdata->tdm_slots = val;
	}

<<<<<<< HEAD
	ret = of_property_read_u32(np, "num-serializer", &val);
	if (ret >= 0)
		pdata->num_serializer = val;

	of_serial_dir32 = of_get_property(np, "serial-dir", &val);
	val /= sizeof(u32);
	if (val != pdata->num_serializer) {
		dev_err(&pdev->dev,
				"num-serializer(%d) != serial-dir size(%d)\n",
				pdata->num_serializer, val);
		ret = -EINVAL;
		goto nodata;
	}

	if (of_serial_dir32) {
		of_serial_dir = devm_kzalloc(&pdev->dev,
						(sizeof(*of_serial_dir) * val),
						GFP_KERNEL);
=======
	of_serial_dir32 = of_get_property(np, "serial-dir", &val);
	val /= sizeof(u32);
	if (of_serial_dir32) {
		u8 *of_serial_dir = devm_kzalloc(&pdev->dev,
						 (sizeof(*of_serial_dir) * val),
						 GFP_KERNEL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!of_serial_dir) {
			ret = -ENOMEM;
			goto nodata;
		}

<<<<<<< HEAD
		for (i = 0; i < pdata->num_serializer; i++)
			of_serial_dir[i] = be32_to_cpup(&of_serial_dir32[i]);

		pdata->serial_dir = of_serial_dir;
	}

=======
		for (i = 0; i < val; i++)
			of_serial_dir[i] = be32_to_cpup(&of_serial_dir32[i]);

		pdata->num_serializer = val;
		pdata->serial_dir = of_serial_dir;
	}

	ret = of_property_match_string(np, "dma-names", "tx");
	if (ret < 0)
		goto nodata;

	ret = of_parse_phandle_with_args(np, "dmas", "#dma-cells", ret,
					 &dma_spec);
	if (ret < 0)
		goto nodata;

	pdata->tx_dma_channel = dma_spec.args[0];

	/* RX is not valid in DIT mode */
	if (pdata->op_mode != DAVINCI_MCASP_DIT_MODE) {
		ret = of_property_match_string(np, "dma-names", "rx");
		if (ret < 0)
			goto nodata;

		ret = of_parse_phandle_with_args(np, "dmas", "#dma-cells", ret,
						 &dma_spec);
		if (ret < 0)
			goto nodata;

		pdata->rx_dma_channel = dma_spec.args[0];
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ret = of_property_read_u32(np, "tx-num-evt", &val);
	if (ret >= 0)
		pdata->txnumevt = val;

	ret = of_property_read_u32(np, "rx-num-evt", &val);
	if (ret >= 0)
		pdata->rxnumevt = val;

	ret = of_property_read_u32(np, "sram-size-playback", &val);
	if (ret >= 0)
		pdata->sram_size_playback = val;

	ret = of_property_read_u32(np, "sram-size-capture", &val);
	if (ret >= 0)
		pdata->sram_size_capture = val;

	return  pdata;

nodata:
	if (ret < 0) {
		dev_err(&pdev->dev, "Error populating platform data, err %d\n",
			ret);
		pdata = NULL;
	}
	return  pdata;
}

<<<<<<< HEAD
static int davinci_mcasp_probe(struct platform_device *pdev)
{
	struct davinci_pcm_dma_params *dma_data;
	struct resource *mem, *ioarea, *res;
	struct snd_platform_data *pdata;
	struct davinci_audio_dev *dev;
=======
enum {
	PCM_EDMA,
	PCM_SDMA,
};
static const char *sdma_prefix = "ti,omap";

static int davinci_mcasp_get_dma_type(struct davinci_mcasp *mcasp)
{
	struct dma_chan *chan;
	const char *tmp;
	int ret = PCM_EDMA;

	if (!mcasp->dev->of_node)
		return PCM_EDMA;

	tmp = mcasp->dma_data[SNDRV_PCM_STREAM_PLAYBACK].filter_data;
	chan = dma_request_slave_channel_reason(mcasp->dev, tmp);
	if (IS_ERR(chan)) {
		if (PTR_ERR(chan) != -EPROBE_DEFER)
			dev_err(mcasp->dev,
				"Can't verify DMA configuration (%ld)\n",
				PTR_ERR(chan));
		return PTR_ERR(chan);
	}
	BUG_ON(!chan->device || !chan->device->dev);

	if (chan->device->dev->of_node)
		ret = of_property_read_string(chan->device->dev->of_node,
					      "compatible", &tmp);
	else
		dev_dbg(mcasp->dev, "DMA controller has no of-node\n");

	dma_release_channel(chan);
	if (ret)
		return ret;

	dev_dbg(mcasp->dev, "DMA controller compatible = \"%s\"\n", tmp);
	if (!strncmp(tmp, sdma_prefix, strlen(sdma_prefix)))
		return PCM_SDMA;

	return PCM_EDMA;
}

static u32 davinci_mcasp_txdma_offset(struct davinci_mcasp_pdata *pdata)
{
	int i;
	u32 offset = 0;

	if (pdata->version != MCASP_VERSION_4)
		return pdata->tx_dma_offset;

	for (i = 0; i < pdata->num_serializer; i++) {
		if (pdata->serial_dir[i] == TX_MODE) {
			if (!offset) {
				offset = DAVINCI_MCASP_TXBUF_REG(i);
			} else {
				pr_err("%s: Only one serializer allowed!\n",
				       __func__);
				break;
			}
		}
	}

	return offset;
}

static u32 davinci_mcasp_rxdma_offset(struct davinci_mcasp_pdata *pdata)
{
	int i;
	u32 offset = 0;

	if (pdata->version != MCASP_VERSION_4)
		return pdata->rx_dma_offset;

	for (i = 0; i < pdata->num_serializer; i++) {
		if (pdata->serial_dir[i] == RX_MODE) {
			if (!offset) {
				offset = DAVINCI_MCASP_RXBUF_REG(i);
			} else {
				pr_err("%s: Only one serializer allowed!\n",
				       __func__);
				break;
			}
		}
	}

	return offset;
}

static int davinci_mcasp_probe(struct platform_device *pdev)
{
	struct snd_dmaengine_dai_dma_data *dma_data;
	struct resource *mem, *res, *dat;
	struct davinci_mcasp_pdata *pdata;
	struct davinci_mcasp *mcasp;
	char *irq_name;
	int *dma;
	int irq;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	if (!pdev->dev.platform_data && !pdev->dev.of_node) {
		dev_err(&pdev->dev, "No platform data supplied\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	dev = devm_kzalloc(&pdev->dev, sizeof(struct davinci_audio_dev),
			   GFP_KERNEL);
	if (!dev)
=======
	mcasp = devm_kzalloc(&pdev->dev, sizeof(struct davinci_mcasp),
			   GFP_KERNEL);
	if (!mcasp)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return	-ENOMEM;

	pdata = davinci_mcasp_set_pdata_from_of(pdev);
	if (!pdata) {
		dev_err(&pdev->dev, "no platform data\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem) {
		dev_err(&pdev->dev, "no mem resource?\n");
		return -ENODEV;
	}

	ioarea = devm_request_mem_region(&pdev->dev, mem->start,
			resource_size(mem), pdev->name);
	if (!ioarea) {
		dev_err(&pdev->dev, "Audio region already claimed\n");
		return -EBUSY;
	}

	pm_runtime_enable(&pdev->dev);

	ret = pm_runtime_get_sync(&pdev->dev);
	if (IS_ERR_VALUE(ret)) {
		dev_err(&pdev->dev, "pm_runtime_get_sync() failed\n");
		return ret;
	}

	dev->base = devm_ioremap(&pdev->dev, mem->start, resource_size(mem));
	if (!dev->base) {
		dev_err(&pdev->dev, "ioremap failed\n");
		ret = -ENOMEM;
		goto err_release_clk;
	}

	dev->op_mode = pdata->op_mode;
	dev->tdm_slots = pdata->tdm_slots;
	dev->num_serializer = pdata->num_serializer;
	dev->serial_dir = pdata->serial_dir;
	dev->version = pdata->version;
	dev->txnumevt = pdata->txnumevt;
	dev->rxnumevt = pdata->rxnumevt;
	dev->dev = &pdev->dev;

	dma_data = &dev->dma_params[SNDRV_PCM_STREAM_PLAYBACK];
	dma_data->asp_chan_q = pdata->asp_chan_q;
	dma_data->ram_chan_q = pdata->ram_chan_q;
	dma_data->sram_pool = pdata->sram_pool;
	dma_data->sram_size = pdata->sram_size_playback;
	dma_data->dma_addr = (dma_addr_t) (pdata->tx_dma_offset +
							mem->start);

	/* first TX, then RX */
	res = platform_get_resource(pdev, IORESOURCE_DMA, 0);
	if (!res) {
		dev_err(&pdev->dev, "no DMA resource\n");
		ret = -ENODEV;
		goto err_release_clk;
	}

	dma_data->channel = res->start;

	dma_data = &dev->dma_params[SNDRV_PCM_STREAM_CAPTURE];
	dma_data->asp_chan_q = pdata->asp_chan_q;
	dma_data->ram_chan_q = pdata->ram_chan_q;
	dma_data->sram_pool = pdata->sram_pool;
	dma_data->sram_size = pdata->sram_size_capture;
	dma_data->dma_addr = (dma_addr_t)(pdata->rx_dma_offset +
							mem->start);

	res = platform_get_resource(pdev, IORESOURCE_DMA, 1);
	if (!res) {
		dev_err(&pdev->dev, "no DMA resource\n");
		ret = -ENODEV;
		goto err_release_clk;
	}

	dma_data->channel = res->start;
	dev_set_drvdata(&pdev->dev, dev);
	ret = snd_soc_register_component(&pdev->dev, &davinci_mcasp_component,
					 &davinci_mcasp_dai[pdata->op_mode], 1);

	if (ret != 0)
		goto err_release_clk;

	ret = davinci_soc_platform_register(&pdev->dev);
	if (ret) {
		dev_err(&pdev->dev, "register PCM failed: %d\n", ret);
		goto err_unregister_component;
=======
	mem = platform_get_resource_byname(pdev, IORESOURCE_MEM, "mpu");
	if (!mem) {
		dev_warn(mcasp->dev,
			 "\"mpu\" mem resource not found, using index 0\n");
		mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
		if (!mem) {
			dev_err(&pdev->dev, "no mem resource?\n");
			return -ENODEV;
		}
	}

	mcasp->base = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(mcasp->base))
		return PTR_ERR(mcasp->base);

	pm_runtime_enable(&pdev->dev);

	mcasp->op_mode = pdata->op_mode;
	/* sanity check for tdm slots parameter */
	if (mcasp->op_mode == DAVINCI_MCASP_IIS_MODE) {
		if (pdata->tdm_slots < 2) {
			dev_err(&pdev->dev, "invalid tdm slots: %d\n",
				pdata->tdm_slots);
			mcasp->tdm_slots = 2;
		} else if (pdata->tdm_slots > 32) {
			dev_err(&pdev->dev, "invalid tdm slots: %d\n",
				pdata->tdm_slots);
			mcasp->tdm_slots = 32;
		} else {
			mcasp->tdm_slots = pdata->tdm_slots;
		}
	}

	mcasp->num_serializer = pdata->num_serializer;
#ifdef CONFIG_PM_SLEEP
	mcasp->context.xrsr_regs = devm_kzalloc(&pdev->dev,
					sizeof(u32) * mcasp->num_serializer,
					GFP_KERNEL);
#endif
	mcasp->serial_dir = pdata->serial_dir;
	mcasp->version = pdata->version;
	mcasp->txnumevt = pdata->txnumevt;
	mcasp->rxnumevt = pdata->rxnumevt;

	mcasp->dev = &pdev->dev;

	irq = platform_get_irq_byname(pdev, "common");
	if (irq >= 0) {
		irq_name = devm_kasprintf(&pdev->dev, GFP_KERNEL, "%s_common",
					  dev_name(&pdev->dev));
		ret = devm_request_threaded_irq(&pdev->dev, irq, NULL,
						davinci_mcasp_common_irq_handler,
						IRQF_ONESHOT | IRQF_SHARED,
						irq_name, mcasp);
		if (ret) {
			dev_err(&pdev->dev, "common IRQ request failed\n");
			goto err;
		}

		mcasp->irq_request[SNDRV_PCM_STREAM_PLAYBACK] = XUNDRN;
		mcasp->irq_request[SNDRV_PCM_STREAM_CAPTURE] = ROVRN;
	}

	irq = platform_get_irq_byname(pdev, "rx");
	if (irq >= 0) {
		irq_name = devm_kasprintf(&pdev->dev, GFP_KERNEL, "%s_rx",
					  dev_name(&pdev->dev));
		ret = devm_request_threaded_irq(&pdev->dev, irq, NULL,
						davinci_mcasp_rx_irq_handler,
						IRQF_ONESHOT, irq_name, mcasp);
		if (ret) {
			dev_err(&pdev->dev, "RX IRQ request failed\n");
			goto err;
		}

		mcasp->irq_request[SNDRV_PCM_STREAM_CAPTURE] = ROVRN;
	}

	irq = platform_get_irq_byname(pdev, "tx");
	if (irq >= 0) {
		irq_name = devm_kasprintf(&pdev->dev, GFP_KERNEL, "%s_tx",
					  dev_name(&pdev->dev));
		ret = devm_request_threaded_irq(&pdev->dev, irq, NULL,
						davinci_mcasp_tx_irq_handler,
						IRQF_ONESHOT, irq_name, mcasp);
		if (ret) {
			dev_err(&pdev->dev, "TX IRQ request failed\n");
			goto err;
		}

		mcasp->irq_request[SNDRV_PCM_STREAM_PLAYBACK] = XUNDRN;
	}

	dat = platform_get_resource_byname(pdev, IORESOURCE_MEM, "dat");
	if (dat)
		mcasp->dat_port = true;

	dma_data = &mcasp->dma_data[SNDRV_PCM_STREAM_PLAYBACK];
	if (dat)
		dma_data->addr = dat->start;
	else
		dma_data->addr = mem->start + davinci_mcasp_txdma_offset(pdata);

	dma = &mcasp->dma_request[SNDRV_PCM_STREAM_PLAYBACK];
	res = platform_get_resource(pdev, IORESOURCE_DMA, 0);
	if (res)
		*dma = res->start;
	else
		*dma = pdata->tx_dma_channel;

	/* dmaengine filter data for DT and non-DT boot */
	if (pdev->dev.of_node)
		dma_data->filter_data = "tx";
	else
		dma_data->filter_data = dma;

	/* RX is not valid in DIT mode */
	if (mcasp->op_mode != DAVINCI_MCASP_DIT_MODE) {
		dma_data = &mcasp->dma_data[SNDRV_PCM_STREAM_CAPTURE];
		if (dat)
			dma_data->addr = dat->start;
		else
			dma_data->addr =
				mem->start + davinci_mcasp_rxdma_offset(pdata);

		dma = &mcasp->dma_request[SNDRV_PCM_STREAM_CAPTURE];
		res = platform_get_resource(pdev, IORESOURCE_DMA, 1);
		if (res)
			*dma = res->start;
		else
			*dma = pdata->rx_dma_channel;

		/* dmaengine filter data for DT and non-DT boot */
		if (pdev->dev.of_node)
			dma_data->filter_data = "rx";
		else
			dma_data->filter_data = dma;
	}

	if (mcasp->version < MCASP_VERSION_3) {
		mcasp->fifo_base = DAVINCI_MCASP_V2_AFIFO_BASE;
		/* dma_params->dma_addr is pointing to the data port address */
		mcasp->dat_port = true;
	} else {
		mcasp->fifo_base = DAVINCI_MCASP_V3_AFIFO_BASE;
	}

	/* Allocate memory for long enough list for all possible
	 * scenarios. Maximum number tdm slots is 32 and there cannot
	 * be more serializers than given in the configuration.  The
	 * serializer directions could be taken into account, but it
	 * would make code much more complex and save only couple of
	 * bytes.
	 */
	mcasp->chconstr[SNDRV_PCM_STREAM_PLAYBACK].list =
		devm_kzalloc(mcasp->dev, sizeof(unsigned int) *
			     (32 + mcasp->num_serializer - 2),
			     GFP_KERNEL);

	mcasp->chconstr[SNDRV_PCM_STREAM_CAPTURE].list =
		devm_kzalloc(mcasp->dev, sizeof(unsigned int) *
			     (32 + mcasp->num_serializer - 2),
			     GFP_KERNEL);

	if (!mcasp->chconstr[SNDRV_PCM_STREAM_PLAYBACK].list ||
	    !mcasp->chconstr[SNDRV_PCM_STREAM_CAPTURE].list)
		return -ENOMEM;

	ret = davinci_mcasp_set_ch_constraints(mcasp);
	if (ret)
		goto err;

	dev_set_drvdata(&pdev->dev, mcasp);

	mcasp_reparent_fck(pdev);

	ret = devm_snd_soc_register_component(&pdev->dev,
					&davinci_mcasp_component,
					&davinci_mcasp_dai[pdata->op_mode], 1);

	if (ret != 0)
		goto err;

	ret = davinci_mcasp_get_dma_type(mcasp);
	switch (ret) {
	case PCM_EDMA:
#if IS_BUILTIN(CONFIG_SND_EDMA_SOC) || \
	(IS_MODULE(CONFIG_SND_DAVINCI_SOC_MCASP) && \
	 IS_MODULE(CONFIG_SND_EDMA_SOC))
		ret = edma_pcm_platform_register(&pdev->dev);
#else
		dev_err(&pdev->dev, "Missing SND_EDMA_SOC\n");
		ret = -EINVAL;
		goto err;
#endif
		break;
	case PCM_SDMA:
#if IS_BUILTIN(CONFIG_SND_OMAP_SOC) || \
	(IS_MODULE(CONFIG_SND_DAVINCI_SOC_MCASP) && \
	 IS_MODULE(CONFIG_SND_OMAP_SOC))
		ret = omap_pcm_platform_register(&pdev->dev);
#else
		dev_err(&pdev->dev, "Missing SND_SDMA_SOC\n");
		ret = -EINVAL;
		goto err;
#endif
		break;
	default:
		dev_err(&pdev->dev, "No DMA controller found (%d)\n", ret);
	case -EPROBE_DEFER:
		goto err;
		break;
	}

	if (ret) {
		dev_err(&pdev->dev, "register PCM failed: %d\n", ret);
		goto err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return 0;

<<<<<<< HEAD
err_unregister_component:
	snd_soc_unregister_component(&pdev->dev);
err_release_clk:
	pm_runtime_put_sync(&pdev->dev);
=======
err:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pm_runtime_disable(&pdev->dev);
	return ret;
}

static int davinci_mcasp_remove(struct platform_device *pdev)
{
<<<<<<< HEAD

	snd_soc_unregister_component(&pdev->dev);
	davinci_soc_platform_unregister(&pdev->dev);

	pm_runtime_put_sync(&pdev->dev);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pm_runtime_disable(&pdev->dev);

	return 0;
}

static struct platform_driver davinci_mcasp_driver = {
	.probe		= davinci_mcasp_probe,
	.remove		= davinci_mcasp_remove,
	.driver		= {
		.name	= "davinci-mcasp",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(mcasp_dt_ids),
=======
		.of_match_table = mcasp_dt_ids,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
};

module_platform_driver(davinci_mcasp_driver);

MODULE_AUTHOR("Steve Chen");
MODULE_DESCRIPTION("TI DAVINCI McASP SoC Interface");
MODULE_LICENSE("GPL");
<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
