/*
<<<<<<< HEAD
 * arch/arm/plat-orion/include/plat/ehci-orion.h
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

<<<<<<< HEAD
#ifndef __PLAT_EHCI_ORION_H
#define __PLAT_EHCI_ORION_H
=======
#ifndef __USB_EHCI_ORION_H
#define __USB_EHCI_ORION_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/mbus.h>

enum orion_ehci_phy_ver {
	EHCI_PHY_ORION,
	EHCI_PHY_DD,
	EHCI_PHY_KW,
	EHCI_PHY_NA,
};

struct orion_ehci_data {
	enum orion_ehci_phy_ver phy_version;
};


#endif
