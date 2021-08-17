/*
 * NVIDIA Tegra SoC device tree board support
 *
 * Copyright (C) 2011, 2013, NVIDIA Corporation
 * Copyright (C) 2010 Secret Lab Technologies, Ltd.
 * Copyright (C) 2010 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

<<<<<<< HEAD
#include <linux/clocksource.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/serial_8250.h>
#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/irqdomain.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>
#include <linux/pda_power.h>
#include <linux/platform_data/tegra_usb.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/sys_soc.h>
#include <linux/usb/tegra_usb_phy.h>
#include <linux/clk/tegra.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/setup.h>

#include "board.h"
#include "common.h"
#include "fuse.h"
#include "iomap.h"

static struct tegra_ehci_platform_data tegra_ehci1_pdata = {
	.operating_mode = TEGRA_USB_OTG,
	.power_down_on_bus_suspend = 1,
	.vbus_gpio = -1,
};

static struct tegra_ulpi_config tegra_ehci2_ulpi_phy_config = {
	.reset_gpio = -1,
	.clk = "cdev2",
};

static struct tegra_ehci_platform_data tegra_ehci2_pdata = {
	.phy_config = &tegra_ehci2_ulpi_phy_config,
	.operating_mode = TEGRA_USB_HOST,
	.power_down_on_bus_suspend = 1,
	.vbus_gpio = -1,
};

static struct tegra_ehci_platform_data tegra_ehci3_pdata = {
	.operating_mode = TEGRA_USB_HOST,
	.power_down_on_bus_suspend = 1,
	.vbus_gpio = -1,
};

static struct of_dev_auxdata tegra20_auxdata_lookup[] __initdata = {
	OF_DEV_AUXDATA("nvidia,tegra20-ehci", 0xC5000000, "tegra-ehci.0",
		       &tegra_ehci1_pdata),
	OF_DEV_AUXDATA("nvidia,tegra20-ehci", 0xC5004000, "tegra-ehci.1",
		       &tegra_ehci2_pdata),
	OF_DEV_AUXDATA("nvidia,tegra20-ehci", 0xC5008000, "tegra-ehci.2",
		       &tegra_ehci3_pdata),
	{}
};
=======
#include <linux/clk.h>
#include <linux/clk/tegra.h>
#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/irqchip.h>
#include <linux/irqdomain.h>
#include <linux/kernel.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/pda_power.h>
#include <linux/platform_device.h>
#include <linux/serial_8250.h>
#include <linux/slab.h>
#include <linux/sys_soc.h>
#include <linux/usb/tegra_usb_phy.h>

#include <soc/tegra/fuse.h>
#include <soc/tegra/pmc.h>

#include <asm/hardware/cache-l2x0.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach-types.h>
#include <asm/setup.h>
#include <asm/trusted_foundations.h>

#include "board.h"
#include "common.h"
#include "cpuidle.h"
#include "flowctrl.h"
#include "iomap.h"
#include "irq.h"
#include "pm.h"
#include "reset.h"
#include "sleep.h"

/*
 * Storage for debug-macro.S's state.
 *
 * This must be in .data not .bss so that it gets initialized each time the
 * kernel is loaded. The data is declared here rather than debug-macro.S so
 * that multiple inclusions of debug-macro.S point at the same data.
 */
u32 tegra_uart_config[3] = {
	/* Debug UART initialization required */
	1,
	/* Debug UART physical address */
	0,
	/* Debug UART virtual address */
	0,
};

static void __init tegra_init_early(void)
{
	of_register_trusted_foundations();
	tegra_cpu_reset_handler_init();
	tegra_flowctrl_init();
}

static void __init tegra_dt_init_irq(void)
{
	tegra_init_irq();
	irqchip_init();
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void __init tegra_dt_init(void)
{
	struct soc_device_attribute *soc_dev_attr;
	struct soc_device *soc_dev;
	struct device *parent = NULL;

<<<<<<< HEAD
	tegra_clocks_apply_init_table();

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	soc_dev_attr = kzalloc(sizeof(*soc_dev_attr), GFP_KERNEL);
	if (!soc_dev_attr)
		goto out;

	soc_dev_attr->family = kasprintf(GFP_KERNEL, "Tegra");
<<<<<<< HEAD
	soc_dev_attr->revision = kasprintf(GFP_KERNEL, "%d", tegra_revision);
	soc_dev_attr->soc_id = kasprintf(GFP_KERNEL, "%d", tegra_chip_id);
=======
	soc_dev_attr->revision = kasprintf(GFP_KERNEL, "%d",
					   tegra_sku_info.revision);
	soc_dev_attr->soc_id = kasprintf(GFP_KERNEL, "%u", tegra_get_chip_id());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	soc_dev = soc_device_register(soc_dev_attr);
	if (IS_ERR(soc_dev)) {
		kfree(soc_dev_attr->family);
		kfree(soc_dev_attr->revision);
		kfree(soc_dev_attr->soc_id);
		kfree(soc_dev_attr);
		goto out;
	}

	parent = soc_device_to_device(soc_dev);

	/*
	 * Finished with the static registrations now; fill in the missing
	 * devices
	 */
out:
<<<<<<< HEAD
	of_platform_populate(NULL, of_default_bus_match_table,
				tegra20_auxdata_lookup, parent);
}

static void __init trimslice_init(void)
{
#ifdef CONFIG_TEGRA_PCI
	int ret;

	ret = tegra_pcie_init(true, true);
	if (ret)
		pr_err("tegra_pci_init() failed: %d\n", ret);
#endif
}

static void __init harmony_init(void)
{
#ifdef CONFIG_TEGRA_PCI
	int ret;

	ret = harmony_pcie_init();
	if (ret)
		pr_err("harmony_pcie_init() failed: %d\n", ret);
#endif
}

static void __init paz00_init(void)
{
	if (IS_ENABLED(CONFIG_ARCH_TEGRA_2x_SOC))
		tegra_paz00_wifikill_init();
}

static struct {
	char *machine;
	void (*init)(void);
} board_init_funcs[] = {
	{ "compulab,trimslice", trimslice_init },
	{ "nvidia,harmony", harmony_init },
	{ "compal,paz00", paz00_init },
};

static void __init tegra_dt_init_late(void)
{
	int i;

	tegra_init_late();

	for (i = 0; i < ARRAY_SIZE(board_init_funcs); i++) {
		if (of_machine_is_compatible(board_init_funcs[i].machine)) {
			board_init_funcs[i].init();
			break;
		}
	}
}

static const char * const tegra_dt_board_compat[] = {
=======
	of_platform_default_populate(NULL, NULL, parent);
}

static void __init tegra_dt_init_late(void)
{
	tegra_init_suspend();
	tegra_cpuidle_init();

	if (IS_ENABLED(CONFIG_ARCH_TEGRA_2x_SOC) &&
	    of_machine_is_compatible("compal,paz00"))
		tegra_paz00_wifikill_init();
}

static const char * const tegra_dt_board_compat[] = {
	"nvidia,tegra124",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	"nvidia,tegra114",
	"nvidia,tegra30",
	"nvidia,tegra20",
	NULL
};

DT_MACHINE_START(TEGRA_DT, "NVIDIA Tegra SoC (Flattened Device Tree)")
<<<<<<< HEAD
	.map_io		= tegra_map_common_io,
	.smp		= smp_ops(tegra_smp_ops),
	.init_early	= tegra_init_early,
	.init_irq	= tegra_dt_init_irq,
	.init_time	= clocksource_of_init,
	.init_machine	= tegra_dt_init,
	.init_late	= tegra_dt_init_late,
	.restart	= tegra_assert_system_reset,
=======
	.l2c_aux_val	= 0x3c400000,
	.l2c_aux_mask	= 0xc20fc3ff,
	.smp		= smp_ops(tegra_smp_ops),
	.map_io		= tegra_map_common_io,
	.init_early	= tegra_init_early,
	.init_irq	= tegra_dt_init_irq,
	.init_machine	= tegra_dt_init,
	.init_late	= tegra_dt_init_late,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.dt_compat	= tegra_dt_board_compat,
MACHINE_END
