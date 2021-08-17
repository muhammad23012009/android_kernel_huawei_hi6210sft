#ifndef __LINUX_SERIAL_SCI_H
#define __LINUX_SERIAL_SCI_H

<<<<<<< HEAD
=======
#include <linux/bitops.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/serial_core.h>
#include <linux/sh_dma.h>

/*
<<<<<<< HEAD
 * Generic header for SuperH SCI(F) (used by sh/sh64/h8300 and related parts)
=======
 * Generic header for SuperH (H)SCI(F) (used by sh/sh64 and related parts)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#define SCIx_NOT_SUPPORTED	(-1)

<<<<<<< HEAD
enum {
	SCBRR_ALGO_1,		/* ((clk + 16 * bps) / (16 * bps) - 1) */
	SCBRR_ALGO_2,		/* ((clk + 16 * bps) / (32 * bps) - 1) */
	SCBRR_ALGO_3,		/* (((clk * 2) + 16 * bps) / (16 * bps) - 1) */
	SCBRR_ALGO_4,		/* (((clk * 2) + 16 * bps) / (32 * bps) - 1) */
	SCBRR_ALGO_5,		/* (((clk * 1000 / 32) / bps) - 1) */
};

#define SCSCR_TIE	(1 << 7)
#define SCSCR_RIE	(1 << 6)
#define SCSCR_TE	(1 << 5)
#define SCSCR_RE	(1 << 4)
#define SCSCR_REIE	(1 << 3)	/* not supported by all parts */
#define SCSCR_TOIE	(1 << 2)	/* not supported by all parts */
#define SCSCR_CKE1	(1 << 1)
#define SCSCR_CKE0	(1 << 0)

/* SCxSR SCI */
#define SCI_TDRE  0x80
#define SCI_RDRF  0x40
#define SCI_ORER  0x20
#define SCI_FER   0x10
#define SCI_PER   0x08
#define SCI_TEND  0x04

#define SCI_DEFAULT_ERROR_MASK (SCI_PER | SCI_FER)

/* SCxSR SCIF */
#define SCIF_ER    0x0080
#define SCIF_TEND  0x0040
#define SCIF_TDFE  0x0020
#define SCIF_BRK   0x0010
#define SCIF_FER   0x0008
#define SCIF_PER   0x0004
#define SCIF_RDF   0x0002
#define SCIF_DR    0x0001

#define SCIF_DEFAULT_ERROR_MASK (SCIF_PER | SCIF_FER | SCIF_ER | SCIF_BRK)

/* SCSPTR, optional */
#define SCSPTR_RTSIO	(1 << 7)
#define SCSPTR_CTSIO	(1 << 5)
#define SCSPTR_SPB2IO	(1 << 1)
#define SCSPTR_SPB2DT	(1 << 0)

/* Offsets into the sci_port->irqs array */
enum {
	SCIx_ERI_IRQ,
	SCIx_RXI_IRQ,
	SCIx_TXI_IRQ,
	SCIx_BRI_IRQ,
	SCIx_NR_IRQS,

	SCIx_MUX_IRQ = SCIx_NR_IRQS,	/* special case */
};

/* Offsets into the sci_port->gpios array */
enum {
	SCIx_SCK,
	SCIx_RXD,
	SCIx_TXD,
	SCIx_CTS,
	SCIx_RTS,

	SCIx_NR_FNS,
};
=======
/* Serial Control Register (@ = not supported by all parts) */
#define SCSCR_TIE	BIT(7)	/* Transmit Interrupt Enable */
#define SCSCR_RIE	BIT(6)	/* Receive Interrupt Enable */
#define SCSCR_TE	BIT(5)	/* Transmit Enable */
#define SCSCR_RE	BIT(4)	/* Receive Enable */
#define SCSCR_REIE	BIT(3)	/* Receive Error Interrupt Enable @ */
#define SCSCR_TOIE	BIT(2)	/* Timeout Interrupt Enable @ */
#define SCSCR_CKE1	BIT(1)	/* Clock Enable 1 */
#define SCSCR_CKE0	BIT(0)	/* Clock Enable 0 */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

enum {
	SCIx_PROBE_REGTYPE,

	SCIx_SCI_REGTYPE,
	SCIx_IRDA_REGTYPE,
	SCIx_SCIFA_REGTYPE,
	SCIx_SCIFB_REGTYPE,
	SCIx_SH2_SCIF_FIFODATA_REGTYPE,
	SCIx_SH3_SCIF_REGTYPE,
	SCIx_SH4_SCIF_REGTYPE,
<<<<<<< HEAD
	SCIx_SH4_SCIF_NO_SCSPTR_REGTYPE,
	SCIx_SH4_SCIF_FIFODATA_REGTYPE,
	SCIx_SH7705_SCIF_REGTYPE,
=======
	SCIx_SH4_SCIF_BRG_REGTYPE,
	SCIx_SH4_SCIF_NO_SCSPTR_REGTYPE,
	SCIx_SH4_SCIF_FIFODATA_REGTYPE,
	SCIx_SH7705_SCIF_REGTYPE,
	SCIx_HSCIF_REGTYPE,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	SCIx_NR_REGTYPES,
};

<<<<<<< HEAD
#define SCIx_IRQ_MUXED(irq)		\
{					\
	[SCIx_ERI_IRQ]	= (irq),	\
	[SCIx_RXI_IRQ]	= (irq),	\
	[SCIx_TXI_IRQ]	= (irq),	\
	[SCIx_BRI_IRQ]	= (irq),	\
}

#define SCIx_IRQ_IS_MUXED(port)			\
	((port)->cfg->irqs[SCIx_ERI_IRQ] ==	\
	 (port)->cfg->irqs[SCIx_RXI_IRQ]) ||	\
	((port)->cfg->irqs[SCIx_ERI_IRQ] &&	\
	 !(port)->cfg->irqs[SCIx_RXI_IRQ])
/*
 * SCI register subset common for all port types.
 * Not all registers will exist on all parts.
 */
enum {
	SCSMR, SCBRR, SCSCR, SCxSR,
	SCFCR, SCFDR, SCxTDR, SCxRDR,
	SCLSR, SCTFDR, SCRFDR, SCSPTR,

	SCIx_NR_REGS,
};

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct device;

struct plat_sci_port_ops {
	void (*init_pins)(struct uart_port *, unsigned int cflag);
};

/*
 * Port-specific capabilities
 */
<<<<<<< HEAD
#define SCIx_HAVE_RTSCTS	(1 << 0)
=======
#define SCIx_HAVE_RTSCTS	BIT(0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Platform device specific platform_data struct
 */
struct plat_sci_port {
<<<<<<< HEAD
	unsigned long	mapbase;		/* resource base */
	unsigned int	irqs[SCIx_NR_IRQS];	/* ERI, RXI, TXI, BRI */
	unsigned int	gpios[SCIx_NR_FNS];	/* SCK, RXD, TXD, CTS, RTS */
	unsigned int	type;			/* SCI / SCIF / IRDA */
	upf_t		flags;			/* UPF_* flags */
	unsigned long	capabilities;		/* Port features/capabilities */

	unsigned int	scbrr_algo_id;		/* SCBRR calculation algo */
=======
	unsigned int	type;			/* SCI / SCIF / IRDA / HSCIF */
	upf_t		flags;			/* UPF_* flags */
	unsigned long	capabilities;		/* Port features/capabilities */

	unsigned int	sampling_rate;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int	scscr;			/* SCSCR initialization */

	/*
	 * Platform overrides if necessary, defaults otherwise.
	 */
<<<<<<< HEAD
	int		overrun_bit;
	unsigned int	error_mask;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int		port_reg;
	unsigned char	regshift;
	unsigned char	regtype;

	struct plat_sci_port_ops	*ops;

	unsigned int	dma_slave_tx;
	unsigned int	dma_slave_rx;
};

#endif /* __LINUX_SERIAL_SCI_H */
