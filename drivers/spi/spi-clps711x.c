/*
 *  CLPS711X SPI bus driver
 *
<<<<<<< HEAD
 *  Copyright (C) 2012 Alexander Shiyan <shc_work@mail.ru>
=======
 *  Copyright (C) 2012-2016 Alexander Shiyan <shc_work@mail.ru>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/io.h>
#include <linux/clk.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/platform_data/spi-clps711x.h>

#include <mach/hardware.h>

#define DRIVER_NAME	"spi-clps711x"

struct spi_clps711x_data {
	struct completion	done;

	struct clk		*spi_clk;
	u32			max_speed_hz;

	u8			*tx_buf;
	u8			*rx_buf;
	int			count;
	int			len;

	int			chipselect[0];
=======
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <linux/mfd/syscon/clps711x.h>
#include <linux/spi/spi.h>

#define DRIVER_NAME		"clps711x-spi"

#define SYNCIO_FRMLEN(x)	((x) << 8)
#define SYNCIO_TXFRMEN		(1 << 14)

struct spi_clps711x_data {
	void __iomem		*syncio;
	struct regmap		*syscon;
	struct clk		*spi_clk;

	u8			*tx_buf;
	u8			*rx_buf;
	unsigned int		bpw;
	int			len;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int spi_clps711x_setup(struct spi_device *spi)
{
<<<<<<< HEAD
	struct spi_clps711x_data *hw = spi_master_get_devdata(spi->master);

	if (spi->bits_per_word != 8) {
		dev_err(&spi->dev, "Unsupported master bus width %i\n",
			spi->bits_per_word);
		return -EINVAL;
	}

	/* We are expect that SPI-device is not selected */
	gpio_direction_output(hw->chipselect[spi->chip_select],
			      !(spi->mode & SPI_CS_HIGH));
=======
	if (!spi->controller_state) {
		int ret;

		ret = devm_gpio_request(&spi->master->dev, spi->cs_gpio,
					dev_name(&spi->master->dev));
		if (ret)
			return ret;

		spi->controller_state = spi;
	}

	/* We are expect that SPI-device is not selected */
	gpio_direction_output(spi->cs_gpio, !(spi->mode & SPI_CS_HIGH));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static void spi_clps711x_setup_mode(struct spi_device *spi)
{
	/* Setup edge for transfer */
	if (spi->mode & SPI_CPHA)
		clps_writew(clps_readw(SYSCON3) | SYSCON3_ADCCKNSEN, SYSCON3);
	else
		clps_writew(clps_readw(SYSCON3) & ~SYSCON3_ADCCKNSEN, SYSCON3);
}

static int spi_clps711x_setup_xfer(struct spi_device *spi,
				   struct spi_transfer *xfer)
{
	u32 speed = xfer->speed_hz ? : spi->max_speed_hz;
	u8 bpw = xfer->bits_per_word;
	struct spi_clps711x_data *hw = spi_master_get_devdata(spi->master);

	if (bpw != 8) {
		dev_err(&spi->dev, "Unsupported master bus width %i\n", bpw);
		return -EINVAL;
	}

	/* Setup SPI frequency divider */
	if (!speed || (speed >= hw->max_speed_hz))
		clps_writel((clps_readl(SYSCON1) & ~SYSCON1_ADCKSEL_MASK) |
			    SYSCON1_ADCKSEL(3), SYSCON1);
	else if (speed >= (hw->max_speed_hz / 2))
		clps_writel((clps_readl(SYSCON1) & ~SYSCON1_ADCKSEL_MASK) |
			    SYSCON1_ADCKSEL(2), SYSCON1);
	else if (speed >= (hw->max_speed_hz / 8))
		clps_writel((clps_readl(SYSCON1) & ~SYSCON1_ADCKSEL_MASK) |
			    SYSCON1_ADCKSEL(1), SYSCON1);
	else
		clps_writel((clps_readl(SYSCON1) & ~SYSCON1_ADCKSEL_MASK) |
			    SYSCON1_ADCKSEL(0), SYSCON1);

	return 0;
}

static int spi_clps711x_transfer_one_message(struct spi_master *master,
					     struct spi_message *msg)
{
	struct spi_clps711x_data *hw = spi_master_get_devdata(master);
	struct spi_transfer *xfer;
	int status = 0, cs = hw->chipselect[msg->spi->chip_select];
	u32 data;

	spi_clps711x_setup_mode(msg->spi);

	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		if (spi_clps711x_setup_xfer(msg->spi, xfer)) {
			status = -EINVAL;
			goto out_xfr;
		}

		gpio_set_value(cs, !!(msg->spi->mode & SPI_CS_HIGH));

		INIT_COMPLETION(hw->done);

		hw->count = 0;
		hw->len = xfer->len;
		hw->tx_buf = (u8 *)xfer->tx_buf;
		hw->rx_buf = (u8 *)xfer->rx_buf;

		/* Initiate transfer */
		data = hw->tx_buf ? hw->tx_buf[hw->count] : 0;
		clps_writel(data | SYNCIO_FRMLEN(8) | SYNCIO_TXFRMEN, SYNCIO);

		wait_for_completion(&hw->done);

		if (xfer->delay_usecs)
			udelay(xfer->delay_usecs);

		if (xfer->cs_change ||
		    list_is_last(&xfer->transfer_list, &msg->transfers))
			gpio_set_value(cs, !(msg->spi->mode & SPI_CS_HIGH));

		msg->actual_length += xfer->len;
	}

out_xfr:
	msg->status = status;
	spi_finalize_current_message(master);

	return 0;
=======
static int spi_clps711x_prepare_message(struct spi_master *master,
					struct spi_message *msg)
{
	struct spi_clps711x_data *hw = spi_master_get_devdata(master);
	struct spi_device *spi = msg->spi;

	/* Setup mode for transfer */
	return regmap_update_bits(hw->syscon, SYSCON_OFFSET, SYSCON3_ADCCKNSEN,
				  (spi->mode & SPI_CPHA) ?
				  SYSCON3_ADCCKNSEN : 0);
}

static int spi_clps711x_transfer_one(struct spi_master *master,
				     struct spi_device *spi,
				     struct spi_transfer *xfer)
{
	struct spi_clps711x_data *hw = spi_master_get_devdata(master);
	u8 data;

	clk_set_rate(hw->spi_clk, xfer->speed_hz ? : spi->max_speed_hz);

	hw->len = xfer->len;
	hw->bpw = xfer->bits_per_word;
	hw->tx_buf = (u8 *)xfer->tx_buf;
	hw->rx_buf = (u8 *)xfer->rx_buf;

	/* Initiate transfer */
	data = hw->tx_buf ? *hw->tx_buf++ : 0;
	writel(data | SYNCIO_FRMLEN(hw->bpw) | SYNCIO_TXFRMEN, hw->syncio);

	return 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static irqreturn_t spi_clps711x_isr(int irq, void *dev_id)
{
<<<<<<< HEAD
	struct spi_clps711x_data *hw = (struct spi_clps711x_data *)dev_id;
	u32 data;

	/* Handle RX */
	data = clps_readb(SYNCIO);
	if (hw->rx_buf)
		hw->rx_buf[hw->count] = (u8)data;

	hw->count++;

	/* Handle TX */
	if (hw->count < hw->len) {
		data = hw->tx_buf ? hw->tx_buf[hw->count] : 0;
		clps_writel(data | SYNCIO_FRMLEN(8) | SYNCIO_TXFRMEN, SYNCIO);
	} else
		complete(&hw->done);
=======
	struct spi_master *master = dev_id;
	struct spi_clps711x_data *hw = spi_master_get_devdata(master);
	u8 data;

	/* Handle RX */
	data = readb(hw->syncio);
	if (hw->rx_buf)
		*hw->rx_buf++ = data;

	/* Handle TX */
	if (--hw->len > 0) {
		data = hw->tx_buf ? *hw->tx_buf++ : 0;
		writel(data | SYNCIO_FRMLEN(hw->bpw) | SYNCIO_TXFRMEN,
		       hw->syncio);
	} else
		spi_finalize_current_transfer(master);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return IRQ_HANDLED;
}

static int spi_clps711x_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	int i, ret;
	struct spi_master *master;
	struct spi_clps711x_data *hw;
	struct spi_clps711x_pdata *pdata = dev_get_platdata(&pdev->dev);

	if (!pdata) {
		dev_err(&pdev->dev, "No platform data supplied\n");
		return -EINVAL;
	}

	if (pdata->num_chipselect < 1) {
		dev_err(&pdev->dev, "At least one CS must be defined\n");
		return -EINVAL;
	}

	master = spi_alloc_master(&pdev->dev,
				  sizeof(struct spi_clps711x_data) +
				  sizeof(int) * pdata->num_chipselect);
	if (!master) {
		dev_err(&pdev->dev, "SPI allocating memory error\n");
		return -ENOMEM;
	}

	master->bus_num = pdev->id;
	master->mode_bits = SPI_CPHA | SPI_CS_HIGH;
	master->num_chipselect = pdata->num_chipselect;
	master->setup = spi_clps711x_setup;
	master->transfer_one_message = spi_clps711x_transfer_one_message;

	hw = spi_master_get_devdata(master);

	for (i = 0; i < master->num_chipselect; i++) {
		hw->chipselect[i] = pdata->chipselect[i];
		if (!gpio_is_valid(hw->chipselect[i])) {
			dev_err(&pdev->dev, "Invalid CS GPIO %i\n", i);
			ret = -EINVAL;
			goto err_out;
		}
		if (gpio_request(hw->chipselect[i], DRIVER_NAME)) {
			dev_err(&pdev->dev, "Can't get CS GPIO %i\n", i);
			ret = -EINVAL;
			goto err_out;
		}
	}

	hw->spi_clk = devm_clk_get(&pdev->dev, "spi");
	if (IS_ERR(hw->spi_clk)) {
		dev_err(&pdev->dev, "Can't get clocks\n");
		ret = PTR_ERR(hw->spi_clk);
		goto err_out;
	}
	hw->max_speed_hz = clk_get_rate(hw->spi_clk);

	init_completion(&hw->done);
	platform_set_drvdata(pdev, master);

	/* Disable extended mode due hardware problems */
	clps_writew(clps_readw(SYSCON3) & ~SYSCON3_ADCCON, SYSCON3);

	/* Clear possible pending interrupt */
	clps_readl(SYNCIO);

	ret = devm_request_irq(&pdev->dev, IRQ_SSEOTI, spi_clps711x_isr, 0,
			       dev_name(&pdev->dev), hw);
	if (ret) {
		dev_err(&pdev->dev, "Can't request IRQ\n");
		clk_put(hw->spi_clk);
		goto clk_out;
	}

	ret = spi_register_master(master);
	if (!ret) {
		dev_info(&pdev->dev,
			 "SPI bus driver initialized. Master clock %u Hz\n",
			 hw->max_speed_hz);
		return 0;
	}

	dev_err(&pdev->dev, "Failed to register master\n");
	devm_free_irq(&pdev->dev, IRQ_SSEOTI, hw);

clk_out:
	devm_clk_put(&pdev->dev, hw->spi_clk);

err_out:
	while (--i >= 0)
		if (gpio_is_valid(hw->chipselect[i]))
			gpio_free(hw->chipselect[i]);

	platform_set_drvdata(pdev, NULL);
	spi_master_put(master);
	kfree(master);
=======
	struct spi_clps711x_data *hw;
	struct spi_master *master;
	struct resource *res;
	int irq, ret;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	master = spi_alloc_master(&pdev->dev, sizeof(*hw));
	if (!master)
		return -ENOMEM;

	master->bus_num = -1;
	master->mode_bits = SPI_CPHA | SPI_CS_HIGH;
	master->bits_per_word_mask =  SPI_BPW_RANGE_MASK(1, 8);
	master->dev.of_node = pdev->dev.of_node;
	master->setup = spi_clps711x_setup;
	master->prepare_message = spi_clps711x_prepare_message;
	master->transfer_one = spi_clps711x_transfer_one;

	hw = spi_master_get_devdata(master);

	hw->spi_clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(hw->spi_clk)) {
		ret = PTR_ERR(hw->spi_clk);
		goto err_out;
	}

	hw->syscon =
		syscon_regmap_lookup_by_compatible("cirrus,ep7209-syscon3");
	if (IS_ERR(hw->syscon)) {
		ret = PTR_ERR(hw->syscon);
		goto err_out;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	hw->syncio = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(hw->syncio)) {
		ret = PTR_ERR(hw->syncio);
		goto err_out;
	}

	/* Disable extended mode due hardware problems */
	regmap_update_bits(hw->syscon, SYSCON_OFFSET, SYSCON3_ADCCON, 0);

	/* Clear possible pending interrupt */
	readl(hw->syncio);

	ret = devm_request_irq(&pdev->dev, irq, spi_clps711x_isr, 0,
			       dev_name(&pdev->dev), master);
	if (ret)
		goto err_out;

	ret = devm_spi_register_master(&pdev->dev, master);
	if (!ret)
		return 0;

err_out:
	spi_master_put(master);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ret;
}

<<<<<<< HEAD
static int spi_clps711x_remove(struct platform_device *pdev)
{
	int i;
	struct spi_master *master = platform_get_drvdata(pdev);
	struct spi_clps711x_data *hw = spi_master_get_devdata(master);

	devm_free_irq(&pdev->dev, IRQ_SSEOTI, hw);

	for (i = 0; i < master->num_chipselect; i++)
		if (gpio_is_valid(hw->chipselect[i]))
			gpio_free(hw->chipselect[i]);

	devm_clk_put(&pdev->dev, hw->spi_clk);
	platform_set_drvdata(pdev, NULL);
	spi_unregister_master(master);
	kfree(master);

	return 0;
}
=======
static const struct of_device_id clps711x_spi_dt_ids[] = {
	{ .compatible = "cirrus,ep7209-spi", },
	{ }
};
MODULE_DEVICE_TABLE(of, clps711x_spi_dt_ids);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static struct platform_driver clps711x_spi_driver = {
	.driver	= {
		.name	= DRIVER_NAME,
<<<<<<< HEAD
		.owner	= THIS_MODULE,
	},
	.probe	= spi_clps711x_probe,
	.remove	= spi_clps711x_remove,
=======
		.of_match_table = clps711x_spi_dt_ids,
	},
	.probe	= spi_clps711x_probe,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
module_platform_driver(clps711x_spi_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexander Shiyan <shc_work@mail.ru>");
MODULE_DESCRIPTION("CLPS711X SPI bus driver");
<<<<<<< HEAD
=======
MODULE_ALIAS("platform:" DRIVER_NAME);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
