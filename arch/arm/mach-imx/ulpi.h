#ifndef __MACH_ULPI_H
#define __MACH_ULPI_H

<<<<<<< HEAD
#ifdef CONFIG_USB_ULPI
struct usb_phy *imx_otg_ulpi_create(unsigned int flags);
=======
#include <linux/usb/ulpi.h>

#ifdef CONFIG_USB_ULPI_VIEWPORT
static inline struct usb_phy *imx_otg_ulpi_create(unsigned int flags)
{
	return otg_ulpi_create(&ulpi_viewport_access_ops, flags);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
static inline struct usb_phy *imx_otg_ulpi_create(unsigned int flags)
{
	return NULL;
}
#endif

<<<<<<< HEAD
extern struct usb_phy_io_ops mxc_ulpi_access_ops;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __MACH_ULPI_H */

