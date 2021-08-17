/*
 *  linux/drivers/mmc/core/bus.c
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *  Copyright (C) 2007 Pierre Ossman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  MMC card bus driver model
 */

#include <linux/export.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/stat.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/pm_runtime.h>

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>

#include "core.h"
#include "sdio_cis.h"
#include "bus.h"

#define to_mmc_driver(d)	container_of(d, struct mmc_driver, drv)

<<<<<<< HEAD
static ssize_t mmc_type_show(struct device *dev,
=======
static ssize_t type_show(struct device *dev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct device_attribute *attr, char *buf)
{
	struct mmc_card *card = mmc_dev_to_card(dev);

	switch (card->type) {
	case MMC_TYPE_MMC:
		return sprintf(buf, "MMC\n");
	case MMC_TYPE_SD:
		return sprintf(buf, "SD\n");
	case MMC_TYPE_SDIO:
		return sprintf(buf, "SDIO\n");
	case MMC_TYPE_SD_COMBO:
		return sprintf(buf, "SDcombo\n");
	default:
		return -EFAULT;
	}
}
<<<<<<< HEAD

static struct device_attribute mmc_dev_attrs[] = {
	__ATTR(type, S_IRUGO, mmc_type_show, NULL),
	__ATTR_NULL,
};
=======
static DEVICE_ATTR_RO(type);

static struct attribute *mmc_dev_attrs[] = {
	&dev_attr_type.attr,
	NULL,
};
ATTRIBUTE_GROUPS(mmc_dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * This currently matches any MMC driver to any MMC card - drivers
 * themselves make the decision whether to drive this card in their
 * probe method.
 */
static int mmc_bus_match(struct device *dev, struct device_driver *drv)
{
<<<<<<< HEAD
/*Begin DTS2013041101544 added by c00217097 for add SD Lock Function 2013-4-10*/
#ifdef CONFIG_MMC_PASSWORDS
	struct mmc_card *card = mmc_dev_to_card(dev);
	
    /*Begin DTS2013041101544 modify by x00186870 for add SD Lock Function 2013-5-15*/
	if ((card->type == MMC_TYPE_SD) && mmc_card_locked(card)) {
		dev_dbg(&card->dev, "sd card is locked; binding is deferred\n");
		return 0;
	}
    /*End DTS2013041101544 modify by x00186870 for add SD Lock Function 2013-5-15*/
#endif
/*End DTS2013041101544 added by c00217097 for add SD Lock Function 2013-4-10*/
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 1;
}

static int
mmc_bus_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
	const char *type;
	int retval = 0;

	switch (card->type) {
	case MMC_TYPE_MMC:
		type = "MMC";
		break;
	case MMC_TYPE_SD:
		type = "SD";
		break;
	case MMC_TYPE_SDIO:
		type = "SDIO";
		break;
	case MMC_TYPE_SD_COMBO:
		type = "SDcombo";
		break;
	default:
		type = NULL;
	}
<<<<<<< HEAD
#ifdef CONFIG_MMC_PASSWORDS
	if (mmc_card_locked(card)) {
		printk("[SDLOCK] %s MMC_LOCK=LOCKED", __func__);
		retval = add_uevent_var(env, "MMC_LOCK=LOCKED");
		if (retval)
			return retval;
	}
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (type) {
		retval = add_uevent_var(env, "MMC_TYPE=%s", type);
		if (retval)
			return retval;
	}

	retval = add_uevent_var(env, "MMC_NAME=%s", mmc_card_name(card));
	if (retval)
		return retval;

	/*
	 * Request the mmc_block device.  Note: that this is a direct request
	 * for the module it carries no information as to what is inserted.
	 */
	retval = add_uevent_var(env, "MODALIAS=mmc:block");

	return retval;
}

static int mmc_bus_probe(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);

	return drv->probe(card);
}

static int mmc_bus_remove(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);

	drv->remove(card);

	return 0;
}

static void mmc_bus_shutdown(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
	int ret;

<<<<<<< HEAD
	printk("%s:%d ++\n", __func__, __LINE__);
	if (dev->driver && drv->shutdown) {
		ret = drv->shutdown(card);
		if (ret){
			pr_err("%s: error during driver shutdown,ret = %d\n",
					mmc_hostname(host),ret);
		}
	}

	if (host->bus_ops->shutdown) {
		ret = host->bus_ops->shutdown(host);
		if (ret){
			pr_err("%s: error during bus shutdown,ret = %d\n",
					mmc_hostname(host),ret);
		}
	}
	printk("%s:%d --\n", __func__, __LINE__);

	return;
=======
	if (dev->driver && drv->shutdown)
		drv->shutdown(card);

	if (host->bus_ops->shutdown) {
		ret = host->bus_ops->shutdown(host);
		if (ret)
			pr_warn("%s: error %d during shutdown\n",
				mmc_hostname(host), ret);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifdef CONFIG_PM_SLEEP
static int mmc_bus_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
	int ret = 0;

	if (dev->driver && drv->suspend)
		ret = drv->suspend(card);
=======
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
	int ret;

	ret = pm_generic_suspend(dev);
	if (ret)
		return ret;

	ret = host->bus_ops->suspend(host);
	if (ret)
		pm_generic_resume(dev);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static int mmc_bus_resume(struct device *dev)
{
<<<<<<< HEAD
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
	int ret = 0;

	if (dev->driver && drv->resume)
		ret = drv->resume(card);
=======
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
	int ret;

	ret = host->bus_ops->resume(host);
	if (ret)
		pr_warn("%s: error %d during resume (card was removed?)\n",
			mmc_hostname(host), ret);

	ret = pm_generic_resume(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_PM_RUNTIME

=======
#ifdef CONFIG_PM
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int mmc_runtime_suspend(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
<<<<<<< HEAD
	int ret = 0;

	if (host->bus_ops->runtime_suspend)
		ret = host->bus_ops->runtime_suspend(host);

	return ret;
=======

	return host->bus_ops->runtime_suspend(host);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int mmc_runtime_resume(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
<<<<<<< HEAD
	int ret = 0;

	if (host->bus_ops->runtime_resume)
		ret = host->bus_ops->runtime_resume(host);

	return ret;
}

static int mmc_runtime_idle(struct device *dev)
{
	return pm_runtime_suspend(dev);
}

#endif /* !CONFIG_PM_RUNTIME */

static const struct dev_pm_ops mmc_bus_pm_ops = {
	SET_RUNTIME_PM_OPS(mmc_runtime_suspend, mmc_runtime_resume,
			mmc_runtime_idle)
=======

	return host->bus_ops->runtime_resume(host);
}
#endif /* !CONFIG_PM */

static const struct dev_pm_ops mmc_bus_pm_ops = {
	SET_RUNTIME_PM_OPS(mmc_runtime_suspend, mmc_runtime_resume, NULL)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	SET_SYSTEM_SLEEP_PM_OPS(mmc_bus_suspend, mmc_bus_resume)
};

static struct bus_type mmc_bus_type = {
	.name		= "mmc",
<<<<<<< HEAD
	.dev_attrs	= mmc_dev_attrs,
=======
	.dev_groups	= mmc_dev_groups,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.match		= mmc_bus_match,
	.uevent		= mmc_bus_uevent,
	.probe		= mmc_bus_probe,
	.remove		= mmc_bus_remove,
	.shutdown	= mmc_bus_shutdown,
	.pm		= &mmc_bus_pm_ops,
};

int mmc_register_bus(void)
{
	return bus_register(&mmc_bus_type);
}

void mmc_unregister_bus(void)
{
	bus_unregister(&mmc_bus_type);
}

/**
 *	mmc_register_driver - register a media driver
 *	@drv: MMC media driver
 */
int mmc_register_driver(struct mmc_driver *drv)
{
	drv->drv.bus = &mmc_bus_type;
	return driver_register(&drv->drv);
}

EXPORT_SYMBOL(mmc_register_driver);

/**
 *	mmc_unregister_driver - unregister a media driver
 *	@drv: MMC media driver
 */
void mmc_unregister_driver(struct mmc_driver *drv)
{
	drv->drv.bus = &mmc_bus_type;
	driver_unregister(&drv->drv);
}

EXPORT_SYMBOL(mmc_unregister_driver);

static void mmc_release_card(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);

	sdio_free_common_cis(card);

<<<<<<< HEAD
	if (card->info)
		kfree(card->info);
=======
	kfree(card->info);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	kfree(card);
}

/*
 * Allocate and initialise a new MMC card structure.
 */
struct mmc_card *mmc_alloc_card(struct mmc_host *host, struct device_type *type)
{
	struct mmc_card *card;

	card = kzalloc(sizeof(struct mmc_card), GFP_KERNEL);
	if (!card)
		return ERR_PTR(-ENOMEM);

	card->host = host;

	device_initialize(&card->dev);

	card->dev.parent = mmc_classdev(host);
	card->dev.bus = &mmc_bus_type;
	card->dev.release = mmc_release_card;
	card->dev.type = type;

	return card;
}

/*
 * Register a new MMC card with the driver model.
 */
int mmc_add_card(struct mmc_card *card)
{
	int ret;
	const char *type;
	const char *uhs_bus_speed_mode = "";
	static const char *const uhs_speeds[] = {
		[UHS_SDR12_BUS_SPEED] = "SDR12 ",
		[UHS_SDR25_BUS_SPEED] = "SDR25 ",
		[UHS_SDR50_BUS_SPEED] = "SDR50 ",
		[UHS_SDR104_BUS_SPEED] = "SDR104 ",
		[UHS_DDR50_BUS_SPEED] = "DDR50 ",
	};


	dev_set_name(&card->dev, "%s:%04x", mmc_hostname(card->host), card->rca);

	switch (card->type) {
	case MMC_TYPE_MMC:
		type = "MMC";
		break;
	case MMC_TYPE_SD:
		type = "SD";
		if (mmc_card_blockaddr(card)) {
			if (mmc_card_ext_capacity(card))
				type = "SDXC";
			else
				type = "SDHC";
		}
		break;
	case MMC_TYPE_SDIO:
		type = "SDIO";
		break;
	case MMC_TYPE_SD_COMBO:
		type = "SD-combo";
		if (mmc_card_blockaddr(card))
			type = "SDHC-combo";
		break;
	default:
		type = "?";
		break;
	}

<<<<<<< HEAD
	if (mmc_sd_card_uhs(card) &&
=======
	if (mmc_card_uhs(card) &&
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		(card->sd_bus_speed < ARRAY_SIZE(uhs_speeds)))
		uhs_bus_speed_mode = uhs_speeds[card->sd_bus_speed];

	if (mmc_host_is_spi(card->host)) {
		pr_info("%s: new %s%s%s card on SPI\n",
			mmc_hostname(card->host),
<<<<<<< HEAD
			mmc_card_highspeed(card) ? "high speed " : "",
			mmc_card_ddr_mode(card) ? "DDR " : "",
			type);
	} else {
    /* BEGIN PN:DTS2014112203072  , Modified by d00168349, 2014/11/22 */
		pr_info("%s: new %s%s%s%s%s card at address %04x,manfid:0x%02x,date:%d/%d\n",
			mmc_hostname(card->host),
			mmc_card_uhs(card) ? "ultra high speed " :
			(mmc_card_highspeed(card) ? "high speed " : ""),
			(mmc_card_hs200(card) ? "HS200 " : ""),
			mmc_card_ddr_mode(card) ? "DDR " : "",
			uhs_bus_speed_mode, type, card->rca,card->cid.manfid,card->cid.year,card->cid.month);
    /* END PN:DTS2014112203072  , Modified by d00168349, 2014/11/22 */
=======
			mmc_card_hs(card) ? "high speed " : "",
			mmc_card_ddr52(card) ? "DDR " : "",
			type);
	} else {
		pr_info("%s: new %s%s%s%s%s%s card at address %04x\n",
			mmc_hostname(card->host),
			mmc_card_uhs(card) ? "ultra high speed " :
			(mmc_card_hs(card) ? "high speed " : ""),
			mmc_card_hs400(card) ? "HS400 " :
			(mmc_card_hs200(card) ? "HS200 " : ""),
			mmc_card_hs400es(card) ? "Enhanced strobe " : "",
			mmc_card_ddr52(card) ? "DDR " : "",
			uhs_bus_speed_mode, type, card->rca);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

#ifdef CONFIG_DEBUG_FS
	mmc_add_card_debugfs(card);
#endif
	mmc_init_context_info(card->host);

<<<<<<< HEAD
	ret = device_add(&card->dev);
	if (ret)
		return ret;
#ifdef CONFIG_MMC_PASSWORDS 
	if (card->host->bus_ops->sysfs_add) { 
		ret = card->host->bus_ops->sysfs_add(card->host, card);
		if (ret) { 
			device_del(&card->dev);
			return ret;
		 }
	}
#endif
=======
	card->dev.of_node = mmc_of_find_child_device(card->host, 0);

	device_enable_async_suspend(&card->dev);

	ret = device_add(&card->dev);
	if (ret)
		return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	mmc_card_set_present(card);

	return 0;
}

/*
 * Unregister a new MMC card with the driver model, and
 * (eventually) free it.
 */
void mmc_remove_card(struct mmc_card *card)
{
#ifdef CONFIG_DEBUG_FS
	mmc_remove_card_debugfs(card);
#endif

	if (mmc_card_present(card)) {
		if (mmc_host_is_spi(card->host)) {
			pr_info("%s: SPI card removed\n",
				mmc_hostname(card->host));
		} else {
			pr_info("%s: card %04x removed\n",
				mmc_hostname(card->host), card->rca);
		}
<<<<<<< HEAD
#ifdef CONFIG_MMC_PASSWORDS		
		if (card->host->bus_ops->sysfs_remove)
			card->host->bus_ops->sysfs_remove(card->host, card);
#endif
		device_del(&card->dev);
=======
		device_del(&card->dev);
		of_node_put(card->dev.of_node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	put_device(&card->dev);
}

