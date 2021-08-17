/*
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __MACB_PDATA_H__
#define __MACB_PDATA_H__

<<<<<<< HEAD
struct macb_platform_data {
	u32		phy_mask;
	int		phy_irq_pin;	/* PHY IRQ */
	u8		is_rmii;	/* using RMII interface? */
	u8		rev_eth_addr;	/* reverse Ethernet address byte order */
=======
/**
 * struct macb_platform_data - platform data for MACB Ethernet
 * @phy_mask:		phy mask passed when register the MDIO bus
 *			within the driver
 * @phy_irq_pin:	PHY IRQ
 * @is_rmii:		using RMII interface?
 * @rev_eth_addr:	reverse Ethernet address byte order
 */
struct macb_platform_data {
	u32		phy_mask;
	int		phy_irq_pin;
	u8		is_rmii;
	u8		rev_eth_addr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif /* __MACB_PDATA_H__ */
