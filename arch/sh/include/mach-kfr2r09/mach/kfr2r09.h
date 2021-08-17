#ifndef __ASM_SH_KFR2R09_H
#define __ASM_SH_KFR2R09_H

#include <video/sh_mobile_lcdc.h>

#if defined(CONFIG_FB_SH_MOBILE_LCDC) || defined(CONFIG_FB_SH_MOBILE_LCDC_MODULE)
<<<<<<< HEAD
int kfr2r09_lcd_set_brightness(int brightness);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int kfr2r09_lcd_setup(void *sys_ops_handle,
		      struct sh_mobile_lcdc_sys_bus_ops *sys_ops);
void kfr2r09_lcd_start(void *sys_ops_handle,
		       struct sh_mobile_lcdc_sys_bus_ops *sys_ops);
#else
<<<<<<< HEAD
static int kfr2r09_lcd_set_brightness(int brightness) {}
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int kfr2r09_lcd_setup(void *sys_ops_handle,
				struct sh_mobile_lcdc_sys_bus_ops *sys_ops)
{
	return -ENODEV;
}
static void kfr2r09_lcd_start(void *sys_ops_handle,
				struct sh_mobile_lcdc_sys_bus_ops *sys_ops)
{
}
#endif

#endif /* __ASM_SH_KFR2R09_H */
