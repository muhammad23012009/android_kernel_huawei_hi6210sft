/*
 * Nomadik RNG support
 *  Copyright 2009 Alessandro Rubini
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/device.h>
#include <linux/amba/bus.h>
#include <linux/hw_random.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/err.h>

static struct clk *rng_clk;

static int nmk_rng_read(struct hwrng *rng, void *data, size_t max, bool wait)
{
	void __iomem *base = (void __iomem *)rng->priv;

	/*
	 * The register is 32 bits and gives 16 random bits (low half).
	 * A subsequent read will delay the core for 400ns, so we just read
	 * once and accept the very unlikely very small delay, even if wait==0.
	 */
	*(u16 *)data = __raw_readl(base + 8) & 0xffff;
	return 2;
}

/* we have at most one RNG per machine, granted */
static struct hwrng nmk_rng = {
	.name		= "nomadik",
	.read		= nmk_rng_read,
};

static int nmk_rng_probe(struct amba_device *dev, const struct amba_id *id)
{
	void __iomem *base;
	int ret;

<<<<<<< HEAD
	rng_clk = clk_get(&dev->dev, NULL);
=======
	rng_clk = devm_clk_get(&dev->dev, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (IS_ERR(rng_clk)) {
		dev_err(&dev->dev, "could not get rng clock\n");
		ret = PTR_ERR(rng_clk);
		return ret;
	}

<<<<<<< HEAD
	clk_enable(rng_clk);
=======
	clk_prepare_enable(rng_clk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = amba_request_regions(dev, dev->dev.init_name);
	if (ret)
		goto out_clk;
	ret = -ENOMEM;
<<<<<<< HEAD
	base = ioremap(dev->res.start, resource_size(&dev->res));
=======
	base = devm_ioremap(&dev->dev, dev->res.start,
			    resource_size(&dev->res));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!base)
		goto out_release;
	nmk_rng.priv = (unsigned long)base;
	ret = hwrng_register(&nmk_rng);
	if (ret)
<<<<<<< HEAD
		goto out_unmap;
	return 0;

out_unmap:
	iounmap(base);
=======
		goto out_release;
	return 0;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out_release:
	amba_release_regions(dev);
out_clk:
	clk_disable(rng_clk);
<<<<<<< HEAD
	clk_put(rng_clk);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static int nmk_rng_remove(struct amba_device *dev)
{
<<<<<<< HEAD
	void __iomem *base = (void __iomem *)nmk_rng.priv;
	hwrng_unregister(&nmk_rng);
	iounmap(base);
	amba_release_regions(dev);
	clk_disable(rng_clk);
	clk_put(rng_clk);
=======
	hwrng_unregister(&nmk_rng);
	amba_release_regions(dev);
	clk_disable(rng_clk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static struct amba_id nmk_rng_ids[] = {
	{
		.id	= 0x000805e1,
		.mask	= 0x000fffff, /* top bits are rev and cfg: accept all */
	},
	{0, 0},
};

MODULE_DEVICE_TABLE(amba, nmk_rng_ids);

static struct amba_driver nmk_rng_driver = {
	.drv = {
		.owner = THIS_MODULE,
		.name = "rng",
		},
	.probe = nmk_rng_probe,
	.remove = nmk_rng_remove,
	.id_table = nmk_rng_ids,
};

module_amba_driver(nmk_rng_driver);

MODULE_LICENSE("GPL");
