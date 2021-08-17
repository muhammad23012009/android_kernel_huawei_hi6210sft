/*
 * Register map access API - MMIO support
 *
 * Copyright (c) 2012, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/clk.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/io.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/slab.h>

<<<<<<< HEAD
=======
#include "internal.h"

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct regmap_mmio_context {
	void __iomem *regs;
	unsigned val_bytes;
	struct clk *clk;
<<<<<<< HEAD
};

static int regmap_mmio_gather_write(void *context,
				    const void *reg, size_t reg_size,
				    const void *val, size_t val_size)
{
	struct regmap_mmio_context *ctx = context;
	u32 offset;
	int ret;

	BUG_ON(reg_size != 4);

	if (ctx->clk) {
		ret = clk_enable(ctx->clk);
		if (ret < 0)
			return ret;
	}

	offset = *(u32 *)reg;

	while (val_size) {
		switch (ctx->val_bytes) {
		case 1:
			writeb(*(u8 *)val, ctx->regs + offset);
			break;
		case 2:
			writew(*(u16 *)val, ctx->regs + offset);
			break;
		case 4:
			writel(*(u32 *)val, ctx->regs + offset);
			break;
#ifdef CONFIG_64BIT
		case 8:
			writeq(*(u64 *)val, ctx->regs + offset);
			break;
#endif
		default:
			/* Should be caught by regmap_mmio_check_config */
			BUG();
		}
		val_size -= ctx->val_bytes;
		val += ctx->val_bytes;
		offset += ctx->val_bytes;
	}

	if (ctx->clk)
		clk_disable(ctx->clk);

	return 0;
}

static int regmap_mmio_write(void *context, const void *data, size_t count)
{
	BUG_ON(count < 4);

	return regmap_mmio_gather_write(context, data, 4, data + 4, count - 4);
}

static int regmap_mmio_read(void *context,
			    const void *reg, size_t reg_size,
			    void *val, size_t val_size)
{
	struct regmap_mmio_context *ctx = context;
	u32 offset;
	int ret;

	BUG_ON(reg_size != 4);

	if (ctx->clk) {
=======

	void (*reg_write)(struct regmap_mmio_context *ctx,
			  unsigned int reg, unsigned int val);
	unsigned int (*reg_read)(struct regmap_mmio_context *ctx,
			         unsigned int reg);
};

static int regmap_mmio_regbits_check(size_t reg_bits)
{
	switch (reg_bits) {
	case 8:
	case 16:
	case 32:
#ifdef CONFIG_64BIT
	case 64:
#endif
		return 0;
	default:
		return -EINVAL;
	}
}

static int regmap_mmio_get_min_stride(size_t val_bits)
{
	int min_stride;

	switch (val_bits) {
	case 8:
		/* The core treats 0 as 1 */
		min_stride = 0;
		return 0;
	case 16:
		min_stride = 2;
		break;
	case 32:
		min_stride = 4;
		break;
#ifdef CONFIG_64BIT
	case 64:
		min_stride = 8;
		break;
#endif
	default:
		return -EINVAL;
	}

	return min_stride;
}

static void regmap_mmio_write8(struct regmap_mmio_context *ctx,
				unsigned int reg,
				unsigned int val)
{
	writeb(val, ctx->regs + reg);
}

static void regmap_mmio_write16le(struct regmap_mmio_context *ctx,
				  unsigned int reg,
				  unsigned int val)
{
	writew(val, ctx->regs + reg);
}

static void regmap_mmio_write16be(struct regmap_mmio_context *ctx,
				  unsigned int reg,
				  unsigned int val)
{
	iowrite16be(val, ctx->regs + reg);
}

static void regmap_mmio_write32le(struct regmap_mmio_context *ctx,
				  unsigned int reg,
				  unsigned int val)
{
	writel(val, ctx->regs + reg);
}

static void regmap_mmio_write32be(struct regmap_mmio_context *ctx,
				  unsigned int reg,
				  unsigned int val)
{
	iowrite32be(val, ctx->regs + reg);
}

#ifdef CONFIG_64BIT
static void regmap_mmio_write64le(struct regmap_mmio_context *ctx,
				  unsigned int reg,
				  unsigned int val)
{
	writeq(val, ctx->regs + reg);
}
#endif

static int regmap_mmio_write(void *context, unsigned int reg, unsigned int val)
{
	struct regmap_mmio_context *ctx = context;
	int ret;

	if (!IS_ERR(ctx->clk)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ret = clk_enable(ctx->clk);
		if (ret < 0)
			return ret;
	}

<<<<<<< HEAD
	offset = *(u32 *)reg;

	while (val_size) {
		switch (ctx->val_bytes) {
		case 1:
			*(u8 *)val = readb(ctx->regs + offset);
			break;
		case 2:
			*(u16 *)val = readw(ctx->regs + offset);
			break;
		case 4:
			*(u32 *)val = readl(ctx->regs + offset);
			break;
#ifdef CONFIG_64BIT
		case 8:
			*(u64 *)val = readq(ctx->regs + offset);
			break;
#endif
		default:
			/* Should be caught by regmap_mmio_check_config */
			BUG();
		}
		val_size -= ctx->val_bytes;
		val += ctx->val_bytes;
		offset += ctx->val_bytes;
	}

	if (ctx->clk)
=======
	ctx->reg_write(ctx, reg, val);

	if (!IS_ERR(ctx->clk))
		clk_disable(ctx->clk);

	return 0;
}

static unsigned int regmap_mmio_read8(struct regmap_mmio_context *ctx,
				      unsigned int reg)
{
	return readb(ctx->regs + reg);
}

static unsigned int regmap_mmio_read16le(struct regmap_mmio_context *ctx,
				         unsigned int reg)
{
	return readw(ctx->regs + reg);
}

static unsigned int regmap_mmio_read16be(struct regmap_mmio_context *ctx,
				         unsigned int reg)
{
	return ioread16be(ctx->regs + reg);
}

static unsigned int regmap_mmio_read32le(struct regmap_mmio_context *ctx,
				         unsigned int reg)
{
	return readl(ctx->regs + reg);
}

static unsigned int regmap_mmio_read32be(struct regmap_mmio_context *ctx,
				         unsigned int reg)
{
	return ioread32be(ctx->regs + reg);
}

#ifdef CONFIG_64BIT
static unsigned int regmap_mmio_read64le(struct regmap_mmio_context *ctx,
				         unsigned int reg)
{
	return readq(ctx->regs + reg);
}
#endif

static int regmap_mmio_read(void *context, unsigned int reg, unsigned int *val)
{
	struct regmap_mmio_context *ctx = context;
	int ret;

	if (!IS_ERR(ctx->clk)) {
		ret = clk_enable(ctx->clk);
		if (ret < 0)
			return ret;
	}

	*val = ctx->reg_read(ctx, reg);

	if (!IS_ERR(ctx->clk))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		clk_disable(ctx->clk);

	return 0;
}

static void regmap_mmio_free_context(void *context)
{
	struct regmap_mmio_context *ctx = context;

<<<<<<< HEAD
	if (ctx->clk) {
=======
	if (!IS_ERR(ctx->clk)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		clk_unprepare(ctx->clk);
		clk_put(ctx->clk);
	}
	kfree(context);
}

<<<<<<< HEAD
static struct regmap_bus regmap_mmio = {
	.fast_io = true,
	.write = regmap_mmio_write,
	.gather_write = regmap_mmio_gather_write,
	.read = regmap_mmio_read,
	.free_context = regmap_mmio_free_context,
	.reg_format_endian_default = REGMAP_ENDIAN_NATIVE,
	.val_format_endian_default = REGMAP_ENDIAN_NATIVE,
=======
static const struct regmap_bus regmap_mmio = {
	.fast_io = true,
	.reg_write = regmap_mmio_write,
	.reg_read = regmap_mmio_read,
	.free_context = regmap_mmio_free_context,
	.val_format_endian_default = REGMAP_ENDIAN_LITTLE,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static struct regmap_mmio_context *regmap_mmio_gen_context(struct device *dev,
					const char *clk_id,
					void __iomem *regs,
					const struct regmap_config *config)
{
	struct regmap_mmio_context *ctx;
	int min_stride;
	int ret;

<<<<<<< HEAD
	if (config->reg_bits != 32)
		return ERR_PTR(-EINVAL);
=======
	ret = regmap_mmio_regbits_check(config->reg_bits);
	if (ret)
		return ERR_PTR(ret);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (config->pad_bits)
		return ERR_PTR(-EINVAL);

<<<<<<< HEAD
	switch (config->val_bits) {
	case 8:
		/* The core treats 0 as 1 */
		min_stride = 0;
		break;
	case 16:
		min_stride = 2;
		break;
	case 32:
		min_stride = 4;
		break;
#ifdef CONFIG_64BIT
	case 64:
		min_stride = 8;
		break;
#endif
		break;
	default:
		return ERR_PTR(-EINVAL);
	}
=======
	min_stride = regmap_mmio_get_min_stride(config->val_bits);
	if (min_stride < 0)
		return ERR_PTR(min_stride);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (config->reg_stride < min_stride)
		return ERR_PTR(-EINVAL);

<<<<<<< HEAD
	switch (config->reg_format_endian) {
	case REGMAP_ENDIAN_DEFAULT:
	case REGMAP_ENDIAN_NATIVE:
		break;
	default:
		return ERR_PTR(-EINVAL);
	}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return ERR_PTR(-ENOMEM);

	ctx->regs = regs;
	ctx->val_bytes = config->val_bits / 8;
<<<<<<< HEAD
=======
	ctx->clk = ERR_PTR(-ENODEV);

	switch (regmap_get_val_endian(dev, &regmap_mmio, config)) {
	case REGMAP_ENDIAN_DEFAULT:
	case REGMAP_ENDIAN_LITTLE:
#ifdef __LITTLE_ENDIAN
	case REGMAP_ENDIAN_NATIVE:
#endif
		switch (config->val_bits) {
		case 8:
			ctx->reg_read = regmap_mmio_read8;
			ctx->reg_write = regmap_mmio_write8;
			break;
		case 16:
			ctx->reg_read = regmap_mmio_read16le;
			ctx->reg_write = regmap_mmio_write16le;
			break;
		case 32:
			ctx->reg_read = regmap_mmio_read32le;
			ctx->reg_write = regmap_mmio_write32le;
			break;
#ifdef CONFIG_64BIT
		case 64:
			ctx->reg_read = regmap_mmio_read64le;
			ctx->reg_write = regmap_mmio_write64le;
			break;
#endif
		default:
			ret = -EINVAL;
			goto err_free;
		}
		break;
	case REGMAP_ENDIAN_BIG:
#ifdef __BIG_ENDIAN
	case REGMAP_ENDIAN_NATIVE:
#endif
		switch (config->val_bits) {
		case 8:
			ctx->reg_read = regmap_mmio_read8;
			ctx->reg_write = regmap_mmio_write8;
			break;
		case 16:
			ctx->reg_read = regmap_mmio_read16be;
			ctx->reg_write = regmap_mmio_write16be;
			break;
		case 32:
			ctx->reg_read = regmap_mmio_read32be;
			ctx->reg_write = regmap_mmio_write32be;
			break;
		default:
			ret = -EINVAL;
			goto err_free;
		}
		break;
	default:
		ret = -EINVAL;
		goto err_free;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (clk_id == NULL)
		return ctx;

	ctx->clk = clk_get(dev, clk_id);
	if (IS_ERR(ctx->clk)) {
		ret = PTR_ERR(ctx->clk);
		goto err_free;
	}

	ret = clk_prepare(ctx->clk);
	if (ret < 0) {
		clk_put(ctx->clk);
		goto err_free;
	}

	return ctx;

err_free:
	kfree(ctx);

	return ERR_PTR(ret);
}

<<<<<<< HEAD
/**
 * regmap_init_mmio_clk(): Initialise register map with register clock
 *
 * @dev: Device that will be interacted with
 * @clk_id: register clock consumer ID
 * @regs: Pointer to memory-mapped IO region
 * @config: Configuration for register map
 *
 * The return value will be an ERR_PTR() on error or a valid pointer to
 * a struct regmap.
 */
struct regmap *regmap_init_mmio_clk(struct device *dev, const char *clk_id,
				    void __iomem *regs,
				    const struct regmap_config *config)
=======
struct regmap *__regmap_init_mmio_clk(struct device *dev, const char *clk_id,
				      void __iomem *regs,
				      const struct regmap_config *config,
				      struct lock_class_key *lock_key,
				      const char *lock_name)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct regmap_mmio_context *ctx;

	ctx = regmap_mmio_gen_context(dev, clk_id, regs, config);
	if (IS_ERR(ctx))
		return ERR_CAST(ctx);

<<<<<<< HEAD
	return regmap_init(dev, &regmap_mmio, ctx, config);
}
EXPORT_SYMBOL_GPL(regmap_init_mmio_clk);

/**
 * devm_regmap_init_mmio_clk(): Initialise managed register map with clock
 *
 * @dev: Device that will be interacted with
 * @clk_id: register clock consumer ID
 * @regs: Pointer to memory-mapped IO region
 * @config: Configuration for register map
 *
 * The return value will be an ERR_PTR() on error or a valid pointer
 * to a struct regmap.  The regmap will be automatically freed by the
 * device management code.
 */
struct regmap *devm_regmap_init_mmio_clk(struct device *dev, const char *clk_id,
					 void __iomem *regs,
					 const struct regmap_config *config)
=======
	return __regmap_init(dev, &regmap_mmio, ctx, config,
			     lock_key, lock_name);
}
EXPORT_SYMBOL_GPL(__regmap_init_mmio_clk);

struct regmap *__devm_regmap_init_mmio_clk(struct device *dev,
					   const char *clk_id,
					   void __iomem *regs,
					   const struct regmap_config *config,
					   struct lock_class_key *lock_key,
					   const char *lock_name)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct regmap_mmio_context *ctx;

	ctx = regmap_mmio_gen_context(dev, clk_id, regs, config);
	if (IS_ERR(ctx))
		return ERR_CAST(ctx);

<<<<<<< HEAD
	return devm_regmap_init(dev, &regmap_mmio, ctx, config);
}
EXPORT_SYMBOL_GPL(devm_regmap_init_mmio_clk);
=======
	return __devm_regmap_init(dev, &regmap_mmio, ctx, config,
				  lock_key, lock_name);
}
EXPORT_SYMBOL_GPL(__devm_regmap_init_mmio_clk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

MODULE_LICENSE("GPL v2");
