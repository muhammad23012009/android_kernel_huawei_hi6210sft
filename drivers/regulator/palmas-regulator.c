/*
 * Driver for Regulator part of Palmas PMIC Chips
 *
 * Copyright 2011-2013 Texas Instruments Inc.
 *
 * Author: Graeme Gregory <gg@slimlogic.co.uk>
 * Author: Ian Lartey <ian@slimlogic.co.uk>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under  the terms of the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/slab.h>
#include <linux/regmap.h>
#include <linux/mfd/palmas.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/regulator/of_regulator.h>

<<<<<<< HEAD
struct regs_info {
	char	*name;
	char	*sname;
	u8	vsel_addr;
	u8	ctrl_addr;
	u8	tstep_addr;
};

static const struct regs_info palmas_regs_info[] = {
=======
static const struct regulator_linear_range smps_low_ranges[] = {
	REGULATOR_LINEAR_RANGE(0, 0x0, 0x0, 0),
	REGULATOR_LINEAR_RANGE(500000, 0x1, 0x6, 0),
	REGULATOR_LINEAR_RANGE(510000, 0x7, 0x79, 10000),
	REGULATOR_LINEAR_RANGE(1650000, 0x7A, 0x7f, 0),
};

static const struct regulator_linear_range smps_high_ranges[] = {
	REGULATOR_LINEAR_RANGE(0, 0x0, 0x0, 0),
	REGULATOR_LINEAR_RANGE(1000000, 0x1, 0x6, 0),
	REGULATOR_LINEAR_RANGE(1020000, 0x7, 0x79, 20000),
	REGULATOR_LINEAR_RANGE(3300000, 0x7A, 0x7f, 0),
};

static struct palmas_regs_info palmas_generic_regs_info[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{
		.name		= "SMPS12",
		.sname		= "smps1-in",
		.vsel_addr	= PALMAS_SMPS12_VOLTAGE,
		.ctrl_addr	= PALMAS_SMPS12_CTRL,
		.tstep_addr	= PALMAS_SMPS12_TSTEP,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS12,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "SMPS123",
		.sname		= "smps1-in",
		.vsel_addr	= PALMAS_SMPS12_VOLTAGE,
		.ctrl_addr	= PALMAS_SMPS12_CTRL,
		.tstep_addr	= PALMAS_SMPS12_TSTEP,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS12,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "SMPS3",
		.sname		= "smps3-in",
		.vsel_addr	= PALMAS_SMPS3_VOLTAGE,
		.ctrl_addr	= PALMAS_SMPS3_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS3,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "SMPS45",
		.sname		= "smps4-in",
		.vsel_addr	= PALMAS_SMPS45_VOLTAGE,
		.ctrl_addr	= PALMAS_SMPS45_CTRL,
		.tstep_addr	= PALMAS_SMPS45_TSTEP,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS45,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "SMPS457",
		.sname		= "smps4-in",
		.vsel_addr	= PALMAS_SMPS45_VOLTAGE,
		.ctrl_addr	= PALMAS_SMPS45_CTRL,
		.tstep_addr	= PALMAS_SMPS45_TSTEP,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS45,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "SMPS6",
		.sname		= "smps6-in",
		.vsel_addr	= PALMAS_SMPS6_VOLTAGE,
		.ctrl_addr	= PALMAS_SMPS6_CTRL,
		.tstep_addr	= PALMAS_SMPS6_TSTEP,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS6,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "SMPS7",
		.sname		= "smps7-in",
		.vsel_addr	= PALMAS_SMPS7_VOLTAGE,
		.ctrl_addr	= PALMAS_SMPS7_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS7,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "SMPS8",
		.sname		= "smps8-in",
		.vsel_addr	= PALMAS_SMPS8_VOLTAGE,
		.ctrl_addr	= PALMAS_SMPS8_CTRL,
		.tstep_addr	= PALMAS_SMPS8_TSTEP,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS8,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "SMPS9",
		.sname		= "smps9-in",
		.vsel_addr	= PALMAS_SMPS9_VOLTAGE,
		.ctrl_addr	= PALMAS_SMPS9_CTRL,
<<<<<<< HEAD
	},
	{
		.name		= "SMPS10",
		.sname		= "smps10-in",
		.ctrl_addr	= PALMAS_SMPS10_CTRL,
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS9,
	},
	{
		.name		= "SMPS10_OUT2",
		.sname		= "smps10-in",
		.ctrl_addr	= PALMAS_SMPS10_CTRL,
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS10,
	},
	{
		.name		= "SMPS10_OUT1",
		.sname		= "smps10-out2",
		.ctrl_addr	= PALMAS_SMPS10_CTRL,
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SMPS10,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDO1",
		.sname		= "ldo1-in",
		.vsel_addr	= PALMAS_LDO1_VOLTAGE,
		.ctrl_addr	= PALMAS_LDO1_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDO1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDO2",
		.sname		= "ldo2-in",
		.vsel_addr	= PALMAS_LDO2_VOLTAGE,
		.ctrl_addr	= PALMAS_LDO2_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDO2,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDO3",
		.sname		= "ldo3-in",
		.vsel_addr	= PALMAS_LDO3_VOLTAGE,
		.ctrl_addr	= PALMAS_LDO3_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDO3,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDO4",
		.sname		= "ldo4-in",
		.vsel_addr	= PALMAS_LDO4_VOLTAGE,
		.ctrl_addr	= PALMAS_LDO4_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDO4,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDO5",
		.sname		= "ldo5-in",
		.vsel_addr	= PALMAS_LDO5_VOLTAGE,
		.ctrl_addr	= PALMAS_LDO5_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDO5,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDO6",
		.sname		= "ldo6-in",
		.vsel_addr	= PALMAS_LDO6_VOLTAGE,
		.ctrl_addr	= PALMAS_LDO6_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDO6,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDO7",
		.sname		= "ldo7-in",
		.vsel_addr	= PALMAS_LDO7_VOLTAGE,
		.ctrl_addr	= PALMAS_LDO7_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDO7,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDO8",
		.sname		= "ldo8-in",
		.vsel_addr	= PALMAS_LDO8_VOLTAGE,
		.ctrl_addr	= PALMAS_LDO8_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDO8,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDO9",
		.sname		= "ldo9-in",
		.vsel_addr	= PALMAS_LDO9_VOLTAGE,
		.ctrl_addr	= PALMAS_LDO9_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDO9,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDOLN",
		.sname		= "ldoln-in",
		.vsel_addr	= PALMAS_LDOLN_VOLTAGE,
		.ctrl_addr	= PALMAS_LDOLN_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDOLN,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "LDOUSB",
		.sname		= "ldousb-in",
		.vsel_addr	= PALMAS_LDOUSB_VOLTAGE,
		.ctrl_addr	= PALMAS_LDOUSB_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_LDOUSB,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "REGEN1",
		.ctrl_addr	= PALMAS_REGEN1_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_REGEN1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "REGEN2",
		.ctrl_addr	= PALMAS_REGEN2_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_REGEN2,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "REGEN3",
		.ctrl_addr	= PALMAS_REGEN3_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_REGEN3,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "SYSEN1",
		.ctrl_addr	= PALMAS_SYSEN1_CTRL,
<<<<<<< HEAD
=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SYSEN1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name		= "SYSEN2",
		.ctrl_addr	= PALMAS_SYSEN2_CTRL,
<<<<<<< HEAD
	},
};

=======
		.sleep_id	= PALMAS_EXTERNAL_REQSTR_ID_SYSEN2,
	},
};

static struct palmas_regs_info tps65917_regs_info[] = {
	{
		.name		= "SMPS1",
		.sname		= "smps1-in",
		.vsel_addr	= TPS65917_SMPS1_VOLTAGE,
		.ctrl_addr	= TPS65917_SMPS1_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_SMPS1,
	},
	{
		.name		= "SMPS2",
		.sname		= "smps2-in",
		.vsel_addr	= TPS65917_SMPS2_VOLTAGE,
		.ctrl_addr	= TPS65917_SMPS2_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_SMPS2,
	},
	{
		.name		= "SMPS3",
		.sname		= "smps3-in",
		.vsel_addr	= TPS65917_SMPS3_VOLTAGE,
		.ctrl_addr	= TPS65917_SMPS3_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_SMPS3,
	},
	{
		.name		= "SMPS4",
		.sname		= "smps4-in",
		.vsel_addr	= TPS65917_SMPS4_VOLTAGE,
		.ctrl_addr	= TPS65917_SMPS4_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_SMPS4,
	},
	{
		.name		= "SMPS5",
		.sname		= "smps5-in",
		.vsel_addr	= TPS65917_SMPS5_VOLTAGE,
		.ctrl_addr	= TPS65917_SMPS5_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_SMPS5,
	},
	{
		.name		= "LDO1",
		.sname		= "ldo1-in",
		.vsel_addr	= TPS65917_LDO1_VOLTAGE,
		.ctrl_addr	= TPS65917_LDO1_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_LDO1,
	},
	{
		.name		= "LDO2",
		.sname		= "ldo2-in",
		.vsel_addr	= TPS65917_LDO2_VOLTAGE,
		.ctrl_addr	= TPS65917_LDO2_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_LDO2,
	},
	{
		.name		= "LDO3",
		.sname		= "ldo3-in",
		.vsel_addr	= TPS65917_LDO3_VOLTAGE,
		.ctrl_addr	= TPS65917_LDO3_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_LDO3,
	},
	{
		.name		= "LDO4",
		.sname		= "ldo4-in",
		.vsel_addr	= TPS65917_LDO4_VOLTAGE,
		.ctrl_addr	= TPS65917_LDO4_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_LDO4,
	},
	{
		.name		= "LDO5",
		.sname		= "ldo5-in",
		.vsel_addr	= TPS65917_LDO5_VOLTAGE,
		.ctrl_addr	= TPS65917_LDO5_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_LDO5,
	},
	{
		.name		= "REGEN1",
		.ctrl_addr	= TPS65917_REGEN1_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_REGEN1,
	},
	{
		.name		= "REGEN2",
		.ctrl_addr	= TPS65917_REGEN2_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_REGEN2,
	},
	{
		.name		= "REGEN3",
		.ctrl_addr	= TPS65917_REGEN3_CTRL,
		.sleep_id	= TPS65917_EXTERNAL_REQSTR_ID_REGEN3,
	},
};

#define EXTERNAL_REQUESTOR(_id, _offset, _pos)		\
	[PALMAS_EXTERNAL_REQSTR_ID_##_id] = {		\
		.id = PALMAS_EXTERNAL_REQSTR_ID_##_id,	\
		.reg_offset = _offset,			\
		.bit_pos = _pos,			\
	}

static struct palmas_sleep_requestor_info palma_sleep_req_info[] = {
	EXTERNAL_REQUESTOR(REGEN1, 0, 0),
	EXTERNAL_REQUESTOR(REGEN2, 0, 1),
	EXTERNAL_REQUESTOR(SYSEN1, 0, 2),
	EXTERNAL_REQUESTOR(SYSEN2, 0, 3),
	EXTERNAL_REQUESTOR(CLK32KG, 0, 4),
	EXTERNAL_REQUESTOR(CLK32KGAUDIO, 0, 5),
	EXTERNAL_REQUESTOR(REGEN3, 0, 6),
	EXTERNAL_REQUESTOR(SMPS12, 1, 0),
	EXTERNAL_REQUESTOR(SMPS3, 1, 1),
	EXTERNAL_REQUESTOR(SMPS45, 1, 2),
	EXTERNAL_REQUESTOR(SMPS6, 1, 3),
	EXTERNAL_REQUESTOR(SMPS7, 1, 4),
	EXTERNAL_REQUESTOR(SMPS8, 1, 5),
	EXTERNAL_REQUESTOR(SMPS9, 1, 6),
	EXTERNAL_REQUESTOR(SMPS10, 1, 7),
	EXTERNAL_REQUESTOR(LDO1, 2, 0),
	EXTERNAL_REQUESTOR(LDO2, 2, 1),
	EXTERNAL_REQUESTOR(LDO3, 2, 2),
	EXTERNAL_REQUESTOR(LDO4, 2, 3),
	EXTERNAL_REQUESTOR(LDO5, 2, 4),
	EXTERNAL_REQUESTOR(LDO6, 2, 5),
	EXTERNAL_REQUESTOR(LDO7, 2, 6),
	EXTERNAL_REQUESTOR(LDO8, 2, 7),
	EXTERNAL_REQUESTOR(LDO9, 3, 0),
	EXTERNAL_REQUESTOR(LDOLN, 3, 1),
	EXTERNAL_REQUESTOR(LDOUSB, 3, 2),
};

#define EXTERNAL_REQUESTOR_TPS65917(_id, _offset, _pos)		\
	[TPS65917_EXTERNAL_REQSTR_ID_##_id] = {		\
		.id = TPS65917_EXTERNAL_REQSTR_ID_##_id,	\
		.reg_offset = _offset,			\
		.bit_pos = _pos,			\
	}

static struct palmas_sleep_requestor_info tps65917_sleep_req_info[] = {
	EXTERNAL_REQUESTOR_TPS65917(REGEN1, 0, 0),
	EXTERNAL_REQUESTOR_TPS65917(REGEN2, 0, 1),
	EXTERNAL_REQUESTOR_TPS65917(REGEN3, 0, 6),
	EXTERNAL_REQUESTOR_TPS65917(SMPS1, 1, 0),
	EXTERNAL_REQUESTOR_TPS65917(SMPS2, 1, 1),
	EXTERNAL_REQUESTOR_TPS65917(SMPS3, 1, 2),
	EXTERNAL_REQUESTOR_TPS65917(SMPS4, 1, 3),
	EXTERNAL_REQUESTOR_TPS65917(SMPS5, 1, 4),
	EXTERNAL_REQUESTOR_TPS65917(LDO1, 2, 0),
	EXTERNAL_REQUESTOR_TPS65917(LDO2, 2, 1),
	EXTERNAL_REQUESTOR_TPS65917(LDO3, 2, 2),
	EXTERNAL_REQUESTOR_TPS65917(LDO4, 2, 3),
	EXTERNAL_REQUESTOR_TPS65917(LDO5, 2, 4),
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static unsigned int palmas_smps_ramp_delay[4] = {0, 10000, 5000, 2500};

#define SMPS_CTRL_MODE_OFF		0x00
#define SMPS_CTRL_MODE_ON		0x01
#define SMPS_CTRL_MODE_ECO		0x02
#define SMPS_CTRL_MODE_PWM		0x03

<<<<<<< HEAD
/* These values are derived from the data sheet. And are the number of steps
 * where there is a voltage change, the ranges at beginning and end of register
 * max/min values where there are no change are ommitted.
 *
 * So they are basically (maxV-minV)/stepV
 */
#define PALMAS_SMPS_NUM_VOLTAGES	117
=======
#define PALMAS_SMPS_NUM_VOLTAGES	122
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define PALMAS_SMPS10_NUM_VOLTAGES	2
#define PALMAS_LDO_NUM_VOLTAGES		50

#define SMPS10_VSEL			(1<<3)
#define SMPS10_BOOST_EN			(1<<2)
#define SMPS10_BYPASS_EN		(1<<1)
#define SMPS10_SWITCH_EN		(1<<0)

#define REGULATOR_SLAVE			0

static int palmas_smps_read(struct palmas *palmas, unsigned int reg,
		unsigned int *dest)
{
	unsigned int addr;

	addr = PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE, reg);

	return regmap_read(palmas->regmap[REGULATOR_SLAVE], addr, dest);
}

static int palmas_smps_write(struct palmas *palmas, unsigned int reg,
		unsigned int value)
{
	unsigned int addr;

	addr = PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE, reg);

	return regmap_write(palmas->regmap[REGULATOR_SLAVE], addr, value);
}

static int palmas_ldo_read(struct palmas *palmas, unsigned int reg,
		unsigned int *dest)
{
	unsigned int addr;

	addr = PALMAS_BASE_TO_REG(PALMAS_LDO_BASE, reg);

	return regmap_read(palmas->regmap[REGULATOR_SLAVE], addr, dest);
}

static int palmas_ldo_write(struct palmas *palmas, unsigned int reg,
		unsigned int value)
{
	unsigned int addr;

	addr = PALMAS_BASE_TO_REG(PALMAS_LDO_BASE, reg);

	return regmap_write(palmas->regmap[REGULATOR_SLAVE], addr, value);
}

<<<<<<< HEAD
static int palmas_is_enabled_smps(struct regulator_dev *dev)
{
	struct palmas_pmic *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev);
	unsigned int reg;

	palmas_smps_read(pmic->palmas, palmas_regs_info[id].ctrl_addr, &reg);

	reg &= PALMAS_SMPS12_CTRL_STATUS_MASK;
	reg >>= PALMAS_SMPS12_CTRL_STATUS_SHIFT;

	return !!(reg);
}

static int palmas_enable_smps(struct regulator_dev *dev)
{
	struct palmas_pmic *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev);
	unsigned int reg;

	palmas_smps_read(pmic->palmas, palmas_regs_info[id].ctrl_addr, &reg);

	reg &= ~PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK;
	if (pmic->current_reg_mode[id])
		reg |= pmic->current_reg_mode[id];
	else
		reg |= SMPS_CTRL_MODE_ON;

	palmas_smps_write(pmic->palmas, palmas_regs_info[id].ctrl_addr, reg);

	return 0;
}

static int palmas_disable_smps(struct regulator_dev *dev)
{
	struct palmas_pmic *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev);
	unsigned int reg;

	palmas_smps_read(pmic->palmas, palmas_regs_info[id].ctrl_addr, &reg);

	reg &= ~PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK;

	palmas_smps_write(pmic->palmas, palmas_regs_info[id].ctrl_addr, reg);

	return 0;
}

static int palmas_set_mode_smps(struct regulator_dev *dev, unsigned int mode)
{
	struct palmas_pmic *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev);
	unsigned int reg;
	bool rail_enable = true;

	palmas_smps_read(pmic->palmas, palmas_regs_info[id].ctrl_addr, &reg);
=======
static int palmas_set_mode_smps(struct regulator_dev *dev, unsigned int mode)
{
	int id = rdev_get_id(dev);
	int ret;
	struct palmas_pmic *pmic = rdev_get_drvdata(dev);
	struct palmas_pmic_driver_data *ddata = pmic->palmas->pmic_ddata;
	struct palmas_regs_info *rinfo = &ddata->palmas_regs_info[id];
	unsigned int reg;
	bool rail_enable = true;

	ret = palmas_smps_read(pmic->palmas, rinfo->ctrl_addr, &reg);
	if (ret)
		return ret;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	reg &= ~PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK;

	if (reg == SMPS_CTRL_MODE_OFF)
		rail_enable = false;

	switch (mode) {
	case REGULATOR_MODE_NORMAL:
		reg |= SMPS_CTRL_MODE_ON;
		break;
	case REGULATOR_MODE_IDLE:
		reg |= SMPS_CTRL_MODE_ECO;
		break;
	case REGULATOR_MODE_FAST:
		reg |= SMPS_CTRL_MODE_PWM;
		break;
	default:
		return -EINVAL;
	}

	pmic->current_reg_mode[id] = reg & PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK;
	if (rail_enable)
<<<<<<< HEAD
		palmas_smps_write(pmic->palmas,
			palmas_regs_info[id].ctrl_addr, reg);
=======
		palmas_smps_write(pmic->palmas, rinfo->ctrl_addr, reg);

	/* Switch the enable value to ensure this is used for enable */
	pmic->desc[id].enable_val = pmic->current_reg_mode[id];

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static unsigned int palmas_get_mode_smps(struct regulator_dev *dev)
{
	struct palmas_pmic *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev);
	unsigned int reg;

	reg = pmic->current_reg_mode[id] & PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK;

	switch (reg) {
	case SMPS_CTRL_MODE_ON:
		return REGULATOR_MODE_NORMAL;
	case SMPS_CTRL_MODE_ECO:
		return REGULATOR_MODE_IDLE;
	case SMPS_CTRL_MODE_PWM:
		return REGULATOR_MODE_FAST;
	}

	return 0;
}

<<<<<<< HEAD
static int palmas_list_voltage_smps(struct regulator_dev *dev,
					unsigned selector)
{
	struct palmas_pmic *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev);
	int mult = 1;

	/* Read the multiplier set in VSEL register to return
	 * the correct voltage.
	 */
	if (pmic->range[id])
		mult = 2;

	if (selector == 0)
		return 0;
	else if (selector < 6)
		return 500000 * mult;
	else
		/* Voltage is linear mapping starting from selector 6,
		 * volt = (0.49V + ((selector - 5) * 0.01V)) * RANGE
		 * RANGE is either x1 or x2
		 */
		return (490000 + ((selector - 5) * 10000)) * mult;
}

static int palmas_map_voltage_smps(struct regulator_dev *rdev,
		int min_uV, int max_uV)
{
	struct palmas_pmic *pmic = rdev_get_drvdata(rdev);
	int id = rdev_get_id(rdev);
	int ret, voltage;

	if (min_uV == 0)
		return 0;

	if (pmic->range[id]) { /* RANGE is x2 */
		if (min_uV < 1000000)
			min_uV = 1000000;
		ret = DIV_ROUND_UP(min_uV - 1000000, 20000) + 6;
	} else {		/* RANGE is x1 */
		if (min_uV < 500000)
			min_uV = 500000;
		ret = DIV_ROUND_UP(min_uV - 500000, 10000) + 6;
	}

	/* Map back into a voltage to verify we're still in bounds */
	voltage = palmas_list_voltage_smps(rdev, ret);
	if (voltage < min_uV || voltage > max_uV)
		return -EINVAL;

	return ret;
}

static int palma_smps_set_voltage_smps_time_sel(struct regulator_dev *rdev,
	unsigned int old_selector, unsigned int new_selector)
{
	struct palmas_pmic *pmic = rdev_get_drvdata(rdev);
	int id = rdev_get_id(rdev);
	int old_uv, new_uv;
	unsigned int ramp_delay = pmic->ramp_delay[id];

	if (!ramp_delay)
		return 0;

	old_uv = palmas_list_voltage_smps(rdev, old_selector);
	if (old_uv < 0)
		return old_uv;

	new_uv = palmas_list_voltage_smps(rdev, new_selector);
	if (new_uv < 0)
		return new_uv;

	return DIV_ROUND_UP(abs(old_uv - new_uv), ramp_delay);
}

static int palmas_smps_set_ramp_delay(struct regulator_dev *rdev,
		 int ramp_delay)
{
	struct palmas_pmic *pmic = rdev_get_drvdata(rdev);
	int id = rdev_get_id(rdev);
	unsigned int reg = 0;
	unsigned int addr = palmas_regs_info[id].tstep_addr;
=======
static int palmas_smps_set_ramp_delay(struct regulator_dev *rdev,
		 int ramp_delay)
{
	int id = rdev_get_id(rdev);
	struct palmas_pmic *pmic = rdev_get_drvdata(rdev);
	struct palmas_pmic_driver_data *ddata = pmic->palmas->pmic_ddata;
	struct palmas_regs_info *rinfo = &ddata->palmas_regs_info[id];
	unsigned int reg = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	/* SMPS3 and SMPS7 do not have tstep_addr setting */
	switch (id) {
	case PALMAS_REG_SMPS3:
	case PALMAS_REG_SMPS7:
		return 0;
	}

	if (ramp_delay <= 0)
		reg = 0;
	else if (ramp_delay <= 2500)
		reg = 3;
	else if (ramp_delay <= 5000)
		reg = 2;
	else
		reg = 1;

<<<<<<< HEAD
	ret = palmas_smps_write(pmic->palmas, addr, reg);
=======
	ret = palmas_smps_write(pmic->palmas, rinfo->tstep_addr, reg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret < 0) {
		dev_err(pmic->palmas->dev, "TSTEP write failed: %d\n", ret);
		return ret;
	}

	pmic->ramp_delay[id] = palmas_smps_ramp_delay[reg];
	return ret;
}

static struct regulator_ops palmas_ops_smps = {
<<<<<<< HEAD
	.is_enabled		= palmas_is_enabled_smps,
	.enable			= palmas_enable_smps,
	.disable		= palmas_disable_smps,
=======
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.set_mode		= palmas_set_mode_smps,
	.get_mode		= palmas_get_mode_smps,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear_range,
	.map_voltage		= regulator_map_voltage_linear_range,
	.set_voltage_time_sel	= regulator_set_voltage_time_sel,
	.set_ramp_delay		= palmas_smps_set_ramp_delay,
};

static struct regulator_ops palmas_ops_ext_control_smps = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.set_mode		= palmas_set_mode_smps,
	.get_mode		= palmas_get_mode_smps,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
<<<<<<< HEAD
	.list_voltage		= palmas_list_voltage_smps,
	.map_voltage		= palmas_map_voltage_smps,
	.set_voltage_time_sel	= palma_smps_set_voltage_smps_time_sel,
=======
	.list_voltage		= regulator_list_voltage_linear_range,
	.map_voltage		= regulator_map_voltage_linear_range,
	.set_voltage_time_sel	= regulator_set_voltage_time_sel,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.set_ramp_delay		= palmas_smps_set_ramp_delay,
};

static struct regulator_ops palmas_ops_smps10 = {
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
	.map_voltage		= regulator_map_voltage_linear,
<<<<<<< HEAD
=======
	.set_bypass		= regulator_set_bypass_regmap,
	.get_bypass		= regulator_get_bypass_regmap,
};

static struct regulator_ops tps65917_ops_smps = {
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.set_mode		= palmas_set_mode_smps,
	.get_mode		= palmas_get_mode_smps,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear_range,
	.map_voltage		= regulator_map_voltage_linear_range,
	.set_voltage_time_sel	= regulator_set_voltage_time_sel,
};

static struct regulator_ops tps65917_ops_ext_control_smps = {
	.set_mode		= palmas_set_mode_smps,
	.get_mode		= palmas_get_mode_smps,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear_range,
	.map_voltage		= regulator_map_voltage_linear_range,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int palmas_is_enabled_ldo(struct regulator_dev *dev)
{
<<<<<<< HEAD
	struct palmas_pmic *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev);
	unsigned int reg;

	palmas_ldo_read(pmic->palmas, palmas_regs_info[id].ctrl_addr, &reg);
=======
	int id = rdev_get_id(dev);
	struct palmas_pmic *pmic = rdev_get_drvdata(dev);
	struct palmas_pmic_driver_data *ddata = pmic->palmas->pmic_ddata;
	struct palmas_regs_info *rinfo = &ddata->palmas_regs_info[id];
	unsigned int reg;

	palmas_ldo_read(pmic->palmas, rinfo->ctrl_addr, &reg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	reg &= PALMAS_LDO1_CTRL_STATUS;

	return !!(reg);
}

static struct regulator_ops palmas_ops_ldo = {
	.is_enabled		= palmas_is_enabled_ldo,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
	.map_voltage		= regulator_map_voltage_linear,
};

<<<<<<< HEAD
=======
static struct regulator_ops palmas_ops_ldo9 = {
	.is_enabled		= palmas_is_enabled_ldo,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
	.map_voltage		= regulator_map_voltage_linear,
	.set_bypass		= regulator_set_bypass_regmap,
	.get_bypass		= regulator_get_bypass_regmap,
};

static struct regulator_ops palmas_ops_ext_control_ldo = {
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
	.map_voltage		= regulator_map_voltage_linear,
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct regulator_ops palmas_ops_extreg = {
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
};

<<<<<<< HEAD
=======
static struct regulator_ops palmas_ops_ext_control_extreg = {
};

static struct regulator_ops tps65917_ops_ldo = {
	.is_enabled		= palmas_is_enabled_ldo,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
	.map_voltage		= regulator_map_voltage_linear,
	.set_voltage_time_sel	= regulator_set_voltage_time_sel,
};

static struct regulator_ops tps65917_ops_ldo_1_2 = {
	.is_enabled		= palmas_is_enabled_ldo,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
	.map_voltage		= regulator_map_voltage_linear,
	.set_voltage_time_sel	= regulator_set_voltage_time_sel,
	.set_bypass		= regulator_set_bypass_regmap,
	.get_bypass		= regulator_get_bypass_regmap,
};

static int palmas_regulator_config_external(struct palmas *palmas, int id,
		struct palmas_reg_init *reg_init)
{
	struct palmas_pmic_driver_data *ddata = palmas->pmic_ddata;
	struct palmas_regs_info *rinfo = &ddata->palmas_regs_info[id];
	int ret;

	ret = palmas_ext_control_req_config(palmas, rinfo->sleep_id,
					    reg_init->roof_floor, true);
	if (ret < 0)
		dev_err(palmas->dev,
			"Ext control config for regulator %d failed %d\n",
			id, ret);
	return ret;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * setup the hardware based sleep configuration of the SMPS/LDO regulators
 * from the platform data. This is different to the software based control
 * supported by the regulator framework as it is controlled by toggling
 * pins on the PMIC such as PREQ, SYSEN, ...
 */
static int palmas_smps_init(struct palmas *palmas, int id,
		struct palmas_reg_init *reg_init)
{
	unsigned int reg;
<<<<<<< HEAD
	unsigned int addr;
	int ret;

	addr = palmas_regs_info[id].ctrl_addr;
=======
	int ret;
	struct palmas_pmic_driver_data *ddata = palmas->pmic_ddata;
	struct palmas_regs_info *rinfo = &ddata->palmas_regs_info[id];
	unsigned int addr = rinfo->ctrl_addr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = palmas_smps_read(palmas, addr, &reg);
	if (ret)
		return ret;

	switch (id) {
<<<<<<< HEAD
	case PALMAS_REG_SMPS10:
=======
	case PALMAS_REG_SMPS10_OUT1:
	case PALMAS_REG_SMPS10_OUT2:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		reg &= ~PALMAS_SMPS10_CTRL_MODE_SLEEP_MASK;
		if (reg_init->mode_sleep)
			reg |= reg_init->mode_sleep <<
					PALMAS_SMPS10_CTRL_MODE_SLEEP_SHIFT;
		break;
	default:
		if (reg_init->warm_reset)
			reg |= PALMAS_SMPS12_CTRL_WR_S;
		else
			reg &= ~PALMAS_SMPS12_CTRL_WR_S;

		if (reg_init->roof_floor)
			reg |= PALMAS_SMPS12_CTRL_ROOF_FLOOR_EN;
		else
			reg &= ~PALMAS_SMPS12_CTRL_ROOF_FLOOR_EN;

		reg &= ~PALMAS_SMPS12_CTRL_MODE_SLEEP_MASK;
		if (reg_init->mode_sleep)
			reg |= reg_init->mode_sleep <<
					PALMAS_SMPS12_CTRL_MODE_SLEEP_SHIFT;
	}

	ret = palmas_smps_write(palmas, addr, reg);
	if (ret)
		return ret;

<<<<<<< HEAD
	if (palmas_regs_info[id].vsel_addr && reg_init->vsel) {
		addr = palmas_regs_info[id].vsel_addr;

		reg = reg_init->vsel;

		ret = palmas_smps_write(palmas, addr, reg);
=======
	if (rinfo->vsel_addr && reg_init->vsel) {

		reg = reg_init->vsel;

		ret = palmas_smps_write(palmas, rinfo->vsel_addr, reg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (ret)
			return ret;
	}

<<<<<<< HEAD

=======
	if (reg_init->roof_floor && (id != PALMAS_REG_SMPS10_OUT1) &&
			(id != PALMAS_REG_SMPS10_OUT2)) {
		/* Enable externally controlled regulator */
		ret = palmas_smps_read(palmas, addr, &reg);
		if (ret < 0)
			return ret;

		if (!(reg & PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK)) {
			reg |= SMPS_CTRL_MODE_ON;
			ret = palmas_smps_write(palmas, addr, reg);
			if (ret < 0)
				return ret;
		}
		return palmas_regulator_config_external(palmas, id, reg_init);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static int palmas_ldo_init(struct palmas *palmas, int id,
		struct palmas_reg_init *reg_init)
{
	unsigned int reg;
	unsigned int addr;
	int ret;
<<<<<<< HEAD

	addr = palmas_regs_info[id].ctrl_addr;
=======
	struct palmas_pmic_driver_data *ddata = palmas->pmic_ddata;
	struct palmas_regs_info *rinfo = &ddata->palmas_regs_info[id];

	addr = rinfo->ctrl_addr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = palmas_ldo_read(palmas, addr, &reg);
	if (ret)
		return ret;

	if (reg_init->warm_reset)
		reg |= PALMAS_LDO1_CTRL_WR_S;
	else
		reg &= ~PALMAS_LDO1_CTRL_WR_S;

	if (reg_init->mode_sleep)
		reg |= PALMAS_LDO1_CTRL_MODE_SLEEP;
	else
		reg &= ~PALMAS_LDO1_CTRL_MODE_SLEEP;

	ret = palmas_ldo_write(palmas, addr, reg);
	if (ret)
		return ret;

<<<<<<< HEAD
=======
	if (reg_init->roof_floor) {
		/* Enable externally controlled regulator */
		ret = palmas_update_bits(palmas, PALMAS_LDO_BASE,
				addr, PALMAS_LDO1_CTRL_MODE_ACTIVE,
				PALMAS_LDO1_CTRL_MODE_ACTIVE);
		if (ret < 0) {
			dev_err(palmas->dev,
				"LDO Register 0x%02x update failed %d\n",
				addr, ret);
			return ret;
		}
		return palmas_regulator_config_external(palmas, id, reg_init);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static int palmas_extreg_init(struct palmas *palmas, int id,
		struct palmas_reg_init *reg_init)
{
	unsigned int addr;
	int ret;
	unsigned int val = 0;
<<<<<<< HEAD

	addr = palmas_regs_info[id].ctrl_addr;
=======
	struct palmas_pmic_driver_data *ddata = palmas->pmic_ddata;
	struct palmas_regs_info *rinfo = &ddata->palmas_regs_info[id];

	addr = rinfo->ctrl_addr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (reg_init->mode_sleep)
		val = PALMAS_REGEN1_CTRL_MODE_SLEEP;

	ret = palmas_update_bits(palmas, PALMAS_RESOURCE_BASE,
			addr, PALMAS_REGEN1_CTRL_MODE_SLEEP, val);
	if (ret < 0) {
		dev_err(palmas->dev, "Resource reg 0x%02x update failed %d\n",
			addr, ret);
		return ret;
	}
<<<<<<< HEAD
=======

	if (reg_init->roof_floor) {
		/* Enable externally controlled regulator */
		ret = palmas_update_bits(palmas, PALMAS_RESOURCE_BASE,
				addr, PALMAS_REGEN1_CTRL_MODE_ACTIVE,
				PALMAS_REGEN1_CTRL_MODE_ACTIVE);
		if (ret < 0) {
			dev_err(palmas->dev,
				"Resource Register 0x%02x update failed %d\n",
				addr, ret);
			return ret;
		}
		return palmas_regulator_config_external(palmas, id, reg_init);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static void palmas_enable_ldo8_track(struct palmas *palmas)
{
	unsigned int reg;
	unsigned int addr;
	int ret;
<<<<<<< HEAD

	addr = palmas_regs_info[PALMAS_REG_LDO8].ctrl_addr;
=======
	struct palmas_pmic_driver_data *ddata = palmas->pmic_ddata;
	struct palmas_regs_info *rinfo;

	rinfo = &ddata->palmas_regs_info[PALMAS_REG_LDO8];
	addr = rinfo->ctrl_addr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = palmas_ldo_read(palmas, addr, &reg);
	if (ret) {
		dev_err(palmas->dev, "Error in reading ldo8 control reg\n");
		return;
	}

	reg |= PALMAS_LDO8_CTRL_LDO_TRACKING_EN;
	ret = palmas_ldo_write(palmas, addr, reg);
	if (ret < 0) {
		dev_err(palmas->dev, "Error in enabling tracking mode\n");
		return;
	}
	/*
	 * When SMPS45 is set to off and LDO8 tracking is enabled, the LDO8
	 * output is defined by the LDO8_VOLTAGE.VSEL register divided by two,
	 * and can be set from 0.45 to 1.65 V.
	 */
<<<<<<< HEAD
	addr = palmas_regs_info[PALMAS_REG_LDO8].vsel_addr;
=======
	addr = rinfo->vsel_addr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ret = palmas_ldo_read(palmas, addr, &reg);
	if (ret) {
		dev_err(palmas->dev, "Error in reading ldo8 voltage reg\n");
		return;
	}

	reg = (reg << 1) & PALMAS_LDO8_VOLTAGE_VSEL_MASK;
	ret = palmas_ldo_write(palmas, addr, reg);
	if (ret < 0)
		dev_err(palmas->dev, "Error in setting ldo8 voltage reg\n");

	return;
}

<<<<<<< HEAD
static struct of_regulator_match palmas_matches[] = {
	{ .name = "smps12", },
	{ .name = "smps123", },
	{ .name = "smps3", },
	{ .name = "smps45", },
	{ .name = "smps457", },
	{ .name = "smps6", },
	{ .name = "smps7", },
	{ .name = "smps8", },
	{ .name = "smps9", },
	{ .name = "smps10", },
	{ .name = "ldo1", },
	{ .name = "ldo2", },
	{ .name = "ldo3", },
	{ .name = "ldo4", },
	{ .name = "ldo5", },
	{ .name = "ldo6", },
	{ .name = "ldo7", },
	{ .name = "ldo8", },
	{ .name = "ldo9", },
	{ .name = "ldoln", },
	{ .name = "ldousb", },
	{ .name = "regen1", },
	{ .name = "regen2", },
	{ .name = "regen3", },
	{ .name = "sysen1", },
	{ .name = "sysen2", },
};

static void palmas_dt_to_pdata(struct device *dev,
		struct device_node *node,
		struct palmas_pmic_platform_data *pdata)
{
	struct device_node *regulators;
	u32 prop;
	int idx, ret;

	node = of_node_get(node);
	regulators = of_find_node_by_name(node, "regulators");
	if (!regulators) {
		dev_info(dev, "regulator node not found\n");
		return;
	}

	ret = of_regulator_match(dev, regulators, palmas_matches,
			PALMAS_NUM_REGS);
	of_node_put(regulators);
	if (ret < 0) {
		dev_err(dev, "Error parsing regulator init data: %d\n", ret);
		return;
	}

	for (idx = 0; idx < PALMAS_NUM_REGS; idx++) {
		if (!palmas_matches[idx].init_data ||
				!palmas_matches[idx].of_node)
			continue;

		pdata->reg_data[idx] = palmas_matches[idx].init_data;

		pdata->reg_init[idx] = devm_kzalloc(dev,
				sizeof(struct palmas_reg_init), GFP_KERNEL);

		pdata->reg_init[idx]->warm_reset =
			of_property_read_bool(palmas_matches[idx].of_node,
					     "ti,warm-reset");

		pdata->reg_init[idx]->roof_floor =
			of_property_read_bool(palmas_matches[idx].of_node,
					      "ti,roof-floor");

		ret = of_property_read_u32(palmas_matches[idx].of_node,
				"ti,mode-sleep", &prop);
		if (!ret)
			pdata->reg_init[idx]->mode_sleep = prop;

		ret = of_property_read_bool(palmas_matches[idx].of_node,
					    "ti,smps-range");
		if (ret)
			pdata->reg_init[idx]->vsel =
				PALMAS_SMPS12_VOLTAGE_RANGE;

		if (idx == PALMAS_REG_LDO8)
			pdata->enable_ldo8_tracking = of_property_read_bool(
						palmas_matches[idx].of_node,
						"ti,enable-ldo8-tracking");
	}

	pdata->ldo6_vibrator = of_property_read_bool(node, "ti,ldo6-vibrator");
}


static int palmas_regulators_probe(struct platform_device *pdev)
{
	struct palmas *palmas = dev_get_drvdata(pdev->dev.parent);
	struct palmas_pmic_platform_data *pdata = pdev->dev.platform_data;
	struct device_node *node = pdev->dev.of_node;
	struct regulator_dev *rdev;
	struct regulator_config config = { };
	struct palmas_pmic *pmic;
	struct palmas_reg_init *reg_init;
	int id = 0, ret;
	unsigned int addr, reg;

	if (node && !pdata) {
		pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);

		if (!pdata)
			return -ENOMEM;

		palmas_dt_to_pdata(&pdev->dev, node, pdata);
	}

	pmic = devm_kzalloc(&pdev->dev, sizeof(*pmic), GFP_KERNEL);
	if (!pmic)
		return -ENOMEM;

	pmic->dev = &pdev->dev;
	pmic->palmas = palmas;
	palmas->pmic = pmic;
	platform_set_drvdata(pdev, pmic);

	ret = palmas_smps_read(palmas, PALMAS_SMPS_CTRL, &reg);
	if (ret)
		return ret;

	if (reg & PALMAS_SMPS_CTRL_SMPS12_SMPS123_EN)
		pmic->smps123 = 1;

	if (reg & PALMAS_SMPS_CTRL_SMPS45_SMPS457_EN)
		pmic->smps457 = 1;

	config.regmap = palmas->regmap[REGULATOR_SLAVE];
	config.dev = &pdev->dev;
	config.driver_data = pmic;

	for (id = 0; id < PALMAS_REG_LDO1; id++) {
=======
static int palmas_ldo_registration(struct palmas_pmic *pmic,
				   struct palmas_pmic_driver_data *ddata,
				   struct palmas_pmic_platform_data *pdata,
				   const char *pdev_name,
				   struct regulator_config config)
{
	int id, ret;
	struct regulator_dev *rdev;
	struct palmas_reg_init *reg_init;
	struct palmas_regs_info *rinfo;
	struct regulator_desc *desc;

	for (id = ddata->ldo_begin; id < ddata->max_reg; id++) {
		if (pdata && pdata->reg_init[id])
			reg_init = pdata->reg_init[id];
		else
			reg_init = NULL;

		rinfo = &ddata->palmas_regs_info[id];
		/* Miss out regulators which are not available due
		 * to alternate functions.
		 */

		/* Register the regulators */
		desc = &pmic->desc[id];
		desc->name = rinfo->name;
		desc->id = id;
		desc->type = REGULATOR_VOLTAGE;
		desc->owner = THIS_MODULE;

		if (id < PALMAS_REG_REGEN1) {
			desc->n_voltages = PALMAS_LDO_NUM_VOLTAGES;
			if (reg_init && reg_init->roof_floor)
				desc->ops = &palmas_ops_ext_control_ldo;
			else
				desc->ops = &palmas_ops_ldo;
			desc->min_uV = 900000;
			desc->uV_step = 50000;
			desc->linear_min_sel = 1;
			desc->enable_time = 500;
			desc->vsel_reg = PALMAS_BASE_TO_REG(PALMAS_LDO_BASE,
							    rinfo->vsel_addr);
			desc->vsel_mask = PALMAS_LDO1_VOLTAGE_VSEL_MASK;
			desc->enable_reg = PALMAS_BASE_TO_REG(PALMAS_LDO_BASE,
							      rinfo->ctrl_addr);
			desc->enable_mask = PALMAS_LDO1_CTRL_MODE_ACTIVE;

			/* Check if LDO8 is in tracking mode or not */
			if (pdata && (id == PALMAS_REG_LDO8) &&
			    pdata->enable_ldo8_tracking) {
				palmas_enable_ldo8_track(pmic->palmas);
				desc->min_uV = 450000;
				desc->uV_step = 25000;
			}

			/* LOD6 in vibrator mode will have enable time 2000us */
			if (pdata && pdata->ldo6_vibrator &&
			    (id == PALMAS_REG_LDO6))
				desc->enable_time = 2000;

			if (id == PALMAS_REG_LDO9) {
				desc->ops = &palmas_ops_ldo9;
				desc->bypass_reg = desc->enable_reg;
				desc->bypass_val_on =
						PALMAS_LDO9_CTRL_LDO_BYPASS_EN;
				desc->bypass_mask =
						PALMAS_LDO9_CTRL_LDO_BYPASS_EN;
			}
		} else {
			if (!ddata->has_regen3 && id == PALMAS_REG_REGEN3)
				continue;

			desc->n_voltages = 1;
			if (reg_init && reg_init->roof_floor)
				desc->ops = &palmas_ops_ext_control_extreg;
			else
				desc->ops = &palmas_ops_extreg;
			desc->enable_reg =
					PALMAS_BASE_TO_REG(PALMAS_RESOURCE_BASE,
							   rinfo->ctrl_addr);
			desc->enable_mask = PALMAS_REGEN1_CTRL_MODE_ACTIVE;
		}

		if (pdata)
			config.init_data = pdata->reg_data[id];
		else
			config.init_data = NULL;

		desc->supply_name = rinfo->sname;
		config.of_node = ddata->palmas_matches[id].of_node;

		rdev = devm_regulator_register(pmic->dev, desc, &config);
		if (IS_ERR(rdev)) {
			dev_err(pmic->dev,
				"failed to register %s regulator\n",
				pdev_name);
			return PTR_ERR(rdev);
		}

		/* Save regulator for cleanup */
		pmic->rdev[id] = rdev;

		/* Initialise sleep/init values from platform data */
		if (pdata) {
			reg_init = pdata->reg_init[id];
			if (reg_init) {
				if (id <= ddata->ldo_end)
					ret = palmas_ldo_init(pmic->palmas, id,
							      reg_init);
				else
					ret = palmas_extreg_init(pmic->palmas,
								 id, reg_init);
				if (ret)
					return ret;
			}
		}
	}

	return 0;
}

static int tps65917_ldo_registration(struct palmas_pmic *pmic,
				     struct palmas_pmic_driver_data *ddata,
				     struct palmas_pmic_platform_data *pdata,
				     const char *pdev_name,
				     struct regulator_config config)
{
	int id, ret;
	struct regulator_dev *rdev;
	struct palmas_reg_init *reg_init;
	struct palmas_regs_info *rinfo;
	struct regulator_desc *desc;

	for (id = ddata->ldo_begin; id < ddata->max_reg; id++) {
		if (pdata && pdata->reg_init[id])
			reg_init = pdata->reg_init[id];
		else
			reg_init = NULL;

		/* Miss out regulators which are not available due
		 * to alternate functions.
		 */
		rinfo = &ddata->palmas_regs_info[id];

		/* Register the regulators */
		desc = &pmic->desc[id];
		desc->name = rinfo->name;
		desc->id = id;
		desc->type = REGULATOR_VOLTAGE;
		desc->owner = THIS_MODULE;

		if (id < TPS65917_REG_REGEN1) {
			desc->n_voltages = PALMAS_LDO_NUM_VOLTAGES;
			if (reg_init && reg_init->roof_floor)
				desc->ops = &palmas_ops_ext_control_ldo;
			else
				desc->ops = &tps65917_ops_ldo;
			desc->min_uV = 900000;
			desc->uV_step = 50000;
			desc->linear_min_sel = 1;
			desc->enable_time = 500;
			desc->vsel_reg = PALMAS_BASE_TO_REG(PALMAS_LDO_BASE,
							    rinfo->vsel_addr);
			desc->vsel_mask = PALMAS_LDO1_VOLTAGE_VSEL_MASK;
			desc->enable_reg = PALMAS_BASE_TO_REG(PALMAS_LDO_BASE,
							      rinfo->ctrl_addr);
			desc->enable_mask = PALMAS_LDO1_CTRL_MODE_ACTIVE;
			/*
			 * To be confirmed. Discussion on going with PMIC Team.
			 * It is of the order of ~60mV/uS.
			 */
			desc->ramp_delay = 2500;
			if (id == TPS65917_REG_LDO1 ||
			    id == TPS65917_REG_LDO2) {
				desc->ops = &tps65917_ops_ldo_1_2;
				desc->bypass_reg = desc->enable_reg;
				desc->bypass_val_on =
						TPS65917_LDO1_CTRL_BYPASS_EN;
				desc->bypass_mask =
						TPS65917_LDO1_CTRL_BYPASS_EN;
			}
		} else {
			desc->n_voltages = 1;
			if (reg_init && reg_init->roof_floor)
				desc->ops = &palmas_ops_ext_control_extreg;
			else
				desc->ops = &palmas_ops_extreg;
			desc->enable_reg =
					PALMAS_BASE_TO_REG(PALMAS_RESOURCE_BASE,
							   rinfo->ctrl_addr);
			desc->enable_mask = PALMAS_REGEN1_CTRL_MODE_ACTIVE;
		}

		if (pdata)
			config.init_data = pdata->reg_data[id];
		else
			config.init_data = NULL;

		desc->supply_name = rinfo->sname;
		config.of_node = ddata->palmas_matches[id].of_node;

		rdev = devm_regulator_register(pmic->dev, desc, &config);
		if (IS_ERR(rdev)) {
			dev_err(pmic->dev,
				"failed to register %s regulator\n",
				pdev_name);
			return PTR_ERR(rdev);
		}

		/* Save regulator for cleanup */
		pmic->rdev[id] = rdev;

		/* Initialise sleep/init values from platform data */
		if (pdata) {
			reg_init = pdata->reg_init[id];
			if (reg_init) {
				if (id < TPS65917_REG_REGEN1)
					ret = palmas_ldo_init(pmic->palmas,
							      id, reg_init);
				else
					ret = palmas_extreg_init(pmic->palmas,
								 id, reg_init);
				if (ret)
					return ret;
			}
		}
	}

	return 0;
}

static int palmas_smps_registration(struct palmas_pmic *pmic,
				    struct palmas_pmic_driver_data *ddata,
				    struct palmas_pmic_platform_data *pdata,
				    const char *pdev_name,
				    struct regulator_config config)
{
	int id, ret;
	unsigned int addr, reg;
	struct regulator_dev *rdev;
	struct palmas_reg_init *reg_init;
	struct palmas_regs_info *rinfo;
	struct regulator_desc *desc;

	for (id = ddata->smps_start; id <= ddata->smps_end; id++) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		bool ramp_delay_support = false;

		/*
		 * Miss out regulators which are not available due
		 * to slaving configurations.
		 */
		switch (id) {
		case PALMAS_REG_SMPS12:
		case PALMAS_REG_SMPS3:
			if (pmic->smps123)
				continue;
			if (id == PALMAS_REG_SMPS12)
				ramp_delay_support = true;
			break;
		case PALMAS_REG_SMPS123:
			if (!pmic->smps123)
				continue;
			ramp_delay_support = true;
			break;
		case PALMAS_REG_SMPS45:
		case PALMAS_REG_SMPS7:
			if (pmic->smps457)
				continue;
			if (id == PALMAS_REG_SMPS45)
				ramp_delay_support = true;
			break;
		case PALMAS_REG_SMPS457:
			if (!pmic->smps457)
				continue;
			ramp_delay_support = true;
			break;
<<<<<<< HEAD
		}
=======
		case PALMAS_REG_SMPS10_OUT1:
		case PALMAS_REG_SMPS10_OUT2:
			if (!PALMAS_PMIC_HAS(pmic->palmas, SMPS10_BOOST))
				continue;
		}
		rinfo = &ddata->palmas_regs_info[id];
		desc = &pmic->desc[id];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if ((id == PALMAS_REG_SMPS6) || (id == PALMAS_REG_SMPS8))
			ramp_delay_support = true;

		if (ramp_delay_support) {
<<<<<<< HEAD
			addr = palmas_regs_info[id].tstep_addr;
			ret = palmas_smps_read(pmic->palmas, addr, &reg);
			if (ret < 0) {
				dev_err(&pdev->dev,
					"reading TSTEP reg failed: %d\n", ret);
				goto err_unregister_regulator;
			}
			pmic->desc[id].ramp_delay =
					palmas_smps_ramp_delay[reg & 0x3];
			pmic->ramp_delay[id] = pmic->desc[id].ramp_delay;
=======
			addr = rinfo->tstep_addr;
			ret = palmas_smps_read(pmic->palmas, addr, &reg);
			if (ret < 0) {
				dev_err(pmic->dev,
					"reading TSTEP reg failed: %d\n", ret);
				return ret;
			}
			desc->ramp_delay = palmas_smps_ramp_delay[reg & 0x3];
			pmic->ramp_delay[id] = desc->ramp_delay;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/* Initialise sleep/init values from platform data */
		if (pdata && pdata->reg_init[id]) {
			reg_init = pdata->reg_init[id];
<<<<<<< HEAD
			ret = palmas_smps_init(palmas, id, reg_init);
			if (ret)
				goto err_unregister_regulator;
		}

		/* Register the regulators */
		pmic->desc[id].name = palmas_regs_info[id].name;
		pmic->desc[id].id = id;

		switch (id) {
		case PALMAS_REG_SMPS10:
			pmic->desc[id].n_voltages = PALMAS_SMPS10_NUM_VOLTAGES;
			pmic->desc[id].ops = &palmas_ops_smps10;
			pmic->desc[id].vsel_reg =
					PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE,
							PALMAS_SMPS10_CTRL);
			pmic->desc[id].vsel_mask = SMPS10_VSEL;
			pmic->desc[id].enable_reg =
					PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE,
							PALMAS_SMPS10_CTRL);
			pmic->desc[id].enable_mask = SMPS10_BOOST_EN;
			pmic->desc[id].min_uV = 3750000;
			pmic->desc[id].uV_step = 1250000;
=======
			ret = palmas_smps_init(pmic->palmas, id, reg_init);
			if (ret)
				return ret;
		} else {
			reg_init = NULL;
		}

		/* Register the regulators */
		desc->name = rinfo->name;
		desc->id = id;

		switch (id) {
		case PALMAS_REG_SMPS10_OUT1:
		case PALMAS_REG_SMPS10_OUT2:
			desc->n_voltages = PALMAS_SMPS10_NUM_VOLTAGES;
			desc->ops = &palmas_ops_smps10;
			desc->vsel_reg = PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE,
							    PALMAS_SMPS10_CTRL);
			desc->vsel_mask = SMPS10_VSEL;
			desc->enable_reg = PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE,
							    PALMAS_SMPS10_CTRL);
			if (id == PALMAS_REG_SMPS10_OUT1)
				desc->enable_mask = SMPS10_SWITCH_EN;
			else
				desc->enable_mask = SMPS10_BOOST_EN;
			desc->bypass_reg = PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE,
							    PALMAS_SMPS10_CTRL);
			desc->bypass_val_on = SMPS10_BYPASS_EN;
			desc->bypass_mask = SMPS10_BYPASS_EN;
			desc->min_uV = 3750000;
			desc->uV_step = 1250000;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		default:
			/*
			 * Read and store the RANGE bit for later use
			 * This must be done before regulator is probed,
			 * otherwise we error in probe with unsupportable
			 * ranges. Read the current smps mode for later use.
			 */
<<<<<<< HEAD
			addr = palmas_regs_info[id].vsel_addr;

			ret = palmas_smps_read(pmic->palmas, addr, &reg);
			if (ret)
				goto err_unregister_regulator;
			if (reg & PALMAS_SMPS12_VOLTAGE_RANGE)
				pmic->range[id] = 1;

			pmic->desc[id].ops = &palmas_ops_smps;
			pmic->desc[id].n_voltages = PALMAS_SMPS_NUM_VOLTAGES;
			pmic->desc[id].vsel_reg =
					PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE,
						palmas_regs_info[id].vsel_addr);
			pmic->desc[id].vsel_mask =
					PALMAS_SMPS12_VOLTAGE_VSEL_MASK;

			/* Read the smps mode for later use. */
			addr = palmas_regs_info[id].ctrl_addr;
			ret = palmas_smps_read(pmic->palmas, addr, &reg);
			if (ret)
				goto err_unregister_regulator;
			pmic->current_reg_mode[id] = reg &
					PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK;
		}

		pmic->desc[id].type = REGULATOR_VOLTAGE;
		pmic->desc[id].owner = THIS_MODULE;
=======
			addr = rinfo->vsel_addr;
			desc->n_linear_ranges = 3;

			ret = palmas_smps_read(pmic->palmas, addr, &reg);
			if (ret)
				return ret;
			if (reg & PALMAS_SMPS12_VOLTAGE_RANGE)
				pmic->range[id] = 1;
			if (pmic->range[id])
				desc->linear_ranges = smps_high_ranges;
			else
				desc->linear_ranges = smps_low_ranges;

			if (reg_init && reg_init->roof_floor)
				desc->ops = &palmas_ops_ext_control_smps;
			else
				desc->ops = &palmas_ops_smps;
			desc->n_voltages = PALMAS_SMPS_NUM_VOLTAGES;
			desc->vsel_reg = PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE,
							    rinfo->vsel_addr);
			desc->vsel_mask = PALMAS_SMPS12_VOLTAGE_VSEL_MASK;

			/* Read the smps mode for later use. */
			addr = rinfo->ctrl_addr;
			ret = palmas_smps_read(pmic->palmas, addr, &reg);
			if (ret)
				return ret;
			pmic->current_reg_mode[id] = reg &
					PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK;

			desc->enable_reg = PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE,
							      rinfo->ctrl_addr);
			desc->enable_mask = PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK;
			/* set_mode overrides this value */
			desc->enable_val = SMPS_CTRL_MODE_ON;
		}

		desc->type = REGULATOR_VOLTAGE;
		desc->owner = THIS_MODULE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (pdata)
			config.init_data = pdata->reg_data[id];
		else
			config.init_data = NULL;

<<<<<<< HEAD
		pmic->desc[id].supply_name = palmas_regs_info[id].sname;
		config.of_node = palmas_matches[id].of_node;

		rdev = regulator_register(&pmic->desc[id], &config);
		if (IS_ERR(rdev)) {
			dev_err(&pdev->dev,
				"failed to register %s regulator\n",
				pdev->name);
			ret = PTR_ERR(rdev);
			goto err_unregister_regulator;
=======
		desc->supply_name = rinfo->sname;
		config.of_node = ddata->palmas_matches[id].of_node;

		rdev = devm_regulator_register(pmic->dev, desc, &config);
		if (IS_ERR(rdev)) {
			dev_err(pmic->dev,
				"failed to register %s regulator\n",
				pdev_name);
			return PTR_ERR(rdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/* Save regulator for cleanup */
		pmic->rdev[id] = rdev;
	}

<<<<<<< HEAD
	/* Start this loop from the id left from previous loop */
	for (; id < PALMAS_NUM_REGS; id++) {

		/* Miss out regulators which are not available due
		 * to alternate functions.
		 */

		/* Register the regulators */
		pmic->desc[id].name = palmas_regs_info[id].name;
		pmic->desc[id].id = id;
		pmic->desc[id].type = REGULATOR_VOLTAGE;
		pmic->desc[id].owner = THIS_MODULE;

		if (id < PALMAS_REG_REGEN1) {
			pmic->desc[id].n_voltages = PALMAS_LDO_NUM_VOLTAGES;
			pmic->desc[id].ops = &palmas_ops_ldo;
			pmic->desc[id].min_uV = 900000;
			pmic->desc[id].uV_step = 50000;
			pmic->desc[id].linear_min_sel = 1;
			pmic->desc[id].vsel_reg =
					PALMAS_BASE_TO_REG(PALMAS_LDO_BASE,
						palmas_regs_info[id].vsel_addr);
			pmic->desc[id].vsel_mask =
					PALMAS_LDO1_VOLTAGE_VSEL_MASK;
			pmic->desc[id].enable_reg =
					PALMAS_BASE_TO_REG(PALMAS_LDO_BASE,
						palmas_regs_info[id].ctrl_addr);
			pmic->desc[id].enable_mask =
					PALMAS_LDO1_CTRL_MODE_ACTIVE;

			/* Check if LDO8 is in tracking mode or not */
			if (pdata && (id == PALMAS_REG_LDO8) &&
					pdata->enable_ldo8_tracking) {
				palmas_enable_ldo8_track(palmas);
				pmic->desc[id].min_uV = 450000;
				pmic->desc[id].uV_step = 25000;
			}
		} else {
			pmic->desc[id].n_voltages = 1;
			pmic->desc[id].ops = &palmas_ops_extreg;
			pmic->desc[id].enable_reg =
					PALMAS_BASE_TO_REG(PALMAS_RESOURCE_BASE,
						palmas_regs_info[id].ctrl_addr);
			pmic->desc[id].enable_mask =
					PALMAS_REGEN1_CTRL_MODE_ACTIVE;
		}
=======
	return 0;
}

static int tps65917_smps_registration(struct palmas_pmic *pmic,
				      struct palmas_pmic_driver_data *ddata,
				      struct palmas_pmic_platform_data *pdata,
				      const char *pdev_name,
				      struct regulator_config config)
{
	int id, ret;
	unsigned int addr, reg;
	struct regulator_dev *rdev;
	struct palmas_reg_init *reg_init;
	struct palmas_regs_info *rinfo;
	struct regulator_desc *desc;

	for (id = ddata->smps_start; id <= ddata->smps_end; id++) {
		/*
		 * Miss out regulators which are not available due
		 * to slaving configurations.
		 */
		desc = &pmic->desc[id];
		desc->n_linear_ranges = 3;
		if ((id == TPS65917_REG_SMPS2) && pmic->smps12)
			continue;

		/* Initialise sleep/init values from platform data */
		if (pdata && pdata->reg_init[id]) {
			reg_init = pdata->reg_init[id];
			ret = palmas_smps_init(pmic->palmas, id, reg_init);
			if (ret)
				return ret;
		} else {
			reg_init = NULL;
		}
		rinfo = &ddata->palmas_regs_info[id];

		/* Register the regulators */
		desc->name = rinfo->name;
		desc->id = id;

		/*
		 * Read and store the RANGE bit for later use
		 * This must be done before regulator is probed,
		 * otherwise we error in probe with unsupportable
		 * ranges. Read the current smps mode for later use.
		 */
		addr = rinfo->vsel_addr;

		ret = palmas_smps_read(pmic->palmas, addr, &reg);
		if (ret)
			return ret;
		if (reg & TPS65917_SMPS1_VOLTAGE_RANGE)
			pmic->range[id] = 1;

		if (pmic->range[id])
			desc->linear_ranges = smps_high_ranges;
		else
			desc->linear_ranges = smps_low_ranges;

		if (reg_init && reg_init->roof_floor)
			desc->ops = &tps65917_ops_ext_control_smps;
		else
			desc->ops = &tps65917_ops_smps;
		desc->n_voltages = PALMAS_SMPS_NUM_VOLTAGES;
		desc->vsel_reg = PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE,
						    rinfo->vsel_addr);
		desc->vsel_mask = PALMAS_SMPS12_VOLTAGE_VSEL_MASK;
		desc->ramp_delay = 2500;

		/* Read the smps mode for later use. */
		addr = rinfo->ctrl_addr;
		ret = palmas_smps_read(pmic->palmas, addr, &reg);
		if (ret)
			return ret;
		pmic->current_reg_mode[id] = reg &
				PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK;
		desc->enable_reg = PALMAS_BASE_TO_REG(PALMAS_SMPS_BASE,
						      rinfo->ctrl_addr);
		desc->enable_mask = PALMAS_SMPS12_CTRL_MODE_ACTIVE_MASK;
		/* set_mode overrides this value */
		desc->enable_val = SMPS_CTRL_MODE_ON;

		desc->type = REGULATOR_VOLTAGE;
		desc->owner = THIS_MODULE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (pdata)
			config.init_data = pdata->reg_data[id];
		else
			config.init_data = NULL;

<<<<<<< HEAD
		pmic->desc[id].supply_name = palmas_regs_info[id].sname;
		config.of_node = palmas_matches[id].of_node;

		rdev = regulator_register(&pmic->desc[id], &config);
		if (IS_ERR(rdev)) {
			dev_err(&pdev->dev,
				"failed to register %s regulator\n",
				pdev->name);
			ret = PTR_ERR(rdev);
			goto err_unregister_regulator;
=======
		desc->supply_name = rinfo->sname;
		config.of_node = ddata->palmas_matches[id].of_node;

		rdev = devm_regulator_register(pmic->dev, desc, &config);
		if (IS_ERR(rdev)) {
			dev_err(pmic->dev,
				"failed to register %s regulator\n",
				pdev_name);
			return PTR_ERR(rdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/* Save regulator for cleanup */
		pmic->rdev[id] = rdev;
<<<<<<< HEAD

		/* Initialise sleep/init values from platform data */
		if (pdata) {
			reg_init = pdata->reg_init[id];
			if (reg_init) {
				if (id < PALMAS_REG_REGEN1)
					ret = palmas_ldo_init(palmas,
							id, reg_init);
				else
					ret = palmas_extreg_init(palmas,
							id, reg_init);
				if (ret) {
					regulator_unregister(pmic->rdev[id]);
					goto err_unregister_regulator;
				}
			}
		}
	}


	return 0;

err_unregister_regulator:
	while (--id >= 0)
		regulator_unregister(pmic->rdev[id]);
	return ret;
}

static int palmas_regulators_remove(struct platform_device *pdev)
{
	struct palmas_pmic *pmic = platform_get_drvdata(pdev);
	int id;

	for (id = 0; id < PALMAS_NUM_REGS; id++)
		regulator_unregister(pmic->rdev[id]);
	return 0;
}

static struct of_device_id of_palmas_match_tbl[] = {
	{ .compatible = "ti,palmas-pmic", },
	{ .compatible = "ti,twl6035-pmic", },
	{ .compatible = "ti,twl6036-pmic", },
	{ .compatible = "ti,twl6037-pmic", },
	{ .compatible = "ti,tps65913-pmic", },
	{ .compatible = "ti,tps65914-pmic", },
	{ .compatible = "ti,tps80036-pmic", },
	{ /* end */ }
};

=======
	}

	return 0;
}

static struct of_regulator_match palmas_matches[] = {
	{ .name = "smps12", },
	{ .name = "smps123", },
	{ .name = "smps3", },
	{ .name = "smps45", },
	{ .name = "smps457", },
	{ .name = "smps6", },
	{ .name = "smps7", },
	{ .name = "smps8", },
	{ .name = "smps9", },
	{ .name = "smps10_out2", },
	{ .name = "smps10_out1", },
	{ .name = "ldo1", },
	{ .name = "ldo2", },
	{ .name = "ldo3", },
	{ .name = "ldo4", },
	{ .name = "ldo5", },
	{ .name = "ldo6", },
	{ .name = "ldo7", },
	{ .name = "ldo8", },
	{ .name = "ldo9", },
	{ .name = "ldoln", },
	{ .name = "ldousb", },
	{ .name = "regen1", },
	{ .name = "regen2", },
	{ .name = "regen3", },
	{ .name = "sysen1", },
	{ .name = "sysen2", },
};

static struct of_regulator_match tps65917_matches[] = {
	{ .name = "smps1", },
	{ .name = "smps2", },
	{ .name = "smps3", },
	{ .name = "smps4", },
	{ .name = "smps5", },
	{ .name = "ldo1", },
	{ .name = "ldo2", },
	{ .name = "ldo3", },
	{ .name = "ldo4", },
	{ .name = "ldo5", },
	{ .name = "regen1", },
	{ .name = "regen2", },
	{ .name = "regen3", },
	{ .name = "sysen1", },
	{ .name = "sysen2", },
};

static struct palmas_pmic_driver_data palmas_ddata = {
	.smps_start = PALMAS_REG_SMPS12,
	.smps_end = PALMAS_REG_SMPS10_OUT1,
	.ldo_begin = PALMAS_REG_LDO1,
	.ldo_end = PALMAS_REG_LDOUSB,
	.max_reg = PALMAS_NUM_REGS,
	.has_regen3 = true,
	.palmas_regs_info = palmas_generic_regs_info,
	.palmas_matches = palmas_matches,
	.sleep_req_info = palma_sleep_req_info,
	.smps_register = palmas_smps_registration,
	.ldo_register = palmas_ldo_registration,
};

static struct palmas_pmic_driver_data tps65917_ddata = {
	.smps_start = TPS65917_REG_SMPS1,
	.smps_end = TPS65917_REG_SMPS5,
	.ldo_begin = TPS65917_REG_LDO1,
	.ldo_end = TPS65917_REG_LDO5,
	.max_reg = TPS65917_NUM_REGS,
	.has_regen3 = true,
	.palmas_regs_info = tps65917_regs_info,
	.palmas_matches = tps65917_matches,
	.sleep_req_info = tps65917_sleep_req_info,
	.smps_register = tps65917_smps_registration,
	.ldo_register = tps65917_ldo_registration,
};

static int palmas_dt_to_pdata(struct device *dev,
			      struct device_node *node,
			      struct palmas_pmic_platform_data *pdata,
			      struct palmas_pmic_driver_data *ddata)
{
	struct device_node *regulators;
	u32 prop;
	int idx, ret;

	regulators = of_get_child_by_name(node, "regulators");
	if (!regulators) {
		dev_info(dev, "regulator node not found\n");
		return 0;
	}

	ret = of_regulator_match(dev, regulators, ddata->palmas_matches,
				 ddata->max_reg);
	of_node_put(regulators);
	if (ret < 0) {
		dev_err(dev, "Error parsing regulator init data: %d\n", ret);
		return 0;
	}

	for (idx = 0; idx < ddata->max_reg; idx++) {
		static struct of_regulator_match *match;
		struct palmas_reg_init *rinit;
		struct device_node *np;

		match = &ddata->palmas_matches[idx];
		np = match->of_node;

		if (!match->init_data || !np)
			continue;

		rinit = devm_kzalloc(dev, sizeof(*rinit), GFP_KERNEL);
		if (!rinit)
			return -ENOMEM;

		pdata->reg_data[idx] = match->init_data;
		pdata->reg_init[idx] = rinit;

		rinit->warm_reset = of_property_read_bool(np, "ti,warm-reset");
		ret = of_property_read_u32(np, "ti,roof-floor", &prop);
		/* EINVAL: Property not found */
		if (ret != -EINVAL) {
			int econtrol;

			/* use default value, when no value is specified */
			econtrol = PALMAS_EXT_CONTROL_NSLEEP;
			if (!ret) {
				switch (prop) {
				case 1:
					econtrol = PALMAS_EXT_CONTROL_ENABLE1;
					break;
				case 2:
					econtrol = PALMAS_EXT_CONTROL_ENABLE2;
					break;
				case 3:
					econtrol = PALMAS_EXT_CONTROL_NSLEEP;
					break;
				default:
					WARN_ON(1);
					dev_warn(dev,
						 "%s: Invalid roof-floor option: %u\n",
						 match->name, prop);
					break;
				}
			}
			rinit->roof_floor = econtrol;
		}

		ret = of_property_read_u32(np, "ti,mode-sleep", &prop);
		if (!ret)
			rinit->mode_sleep = prop;

		ret = of_property_read_bool(np, "ti,smps-range");
		if (ret)
			rinit->vsel = PALMAS_SMPS12_VOLTAGE_RANGE;

		if (idx == PALMAS_REG_LDO8)
			pdata->enable_ldo8_tracking = of_property_read_bool(
						np, "ti,enable-ldo8-tracking");
	}

	pdata->ldo6_vibrator = of_property_read_bool(node, "ti,ldo6-vibrator");

	return 0;
}

static const struct of_device_id of_palmas_match_tbl[] = {
	{
		.compatible = "ti,palmas-pmic",
		.data = &palmas_ddata,
	},
	{
		.compatible = "ti,twl6035-pmic",
		.data = &palmas_ddata,
	},
	{
		.compatible = "ti,twl6036-pmic",
		.data = &palmas_ddata,
	},
	{
		.compatible = "ti,twl6037-pmic",
		.data = &palmas_ddata,
	},
	{
		.compatible = "ti,tps65913-pmic",
		.data = &palmas_ddata,
	},
	{
		.compatible = "ti,tps65914-pmic",
		.data = &palmas_ddata,
	},
	{
		.compatible = "ti,tps80036-pmic",
		.data = &palmas_ddata,
	},
	{
		.compatible = "ti,tps659038-pmic",
		.data = &palmas_ddata,
	},
	 {
		.compatible = "ti,tps65917-pmic",
		.data = &tps65917_ddata,
	},
	{ /* end */ }
};

static int palmas_regulators_probe(struct platform_device *pdev)
{
	struct palmas *palmas = dev_get_drvdata(pdev->dev.parent);
	struct palmas_pmic_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct device_node *node = pdev->dev.of_node;
	struct palmas_pmic_driver_data *driver_data;
	struct regulator_config config = { };
	struct palmas_pmic *pmic;
	const char *pdev_name;
	const struct of_device_id *match;
	int ret = 0;
	unsigned int reg;

	match = of_match_device(of_match_ptr(of_palmas_match_tbl), &pdev->dev);

	if (!match)
		return -ENODATA;

	driver_data = (struct palmas_pmic_driver_data *)match->data;
	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	pmic = devm_kzalloc(&pdev->dev, sizeof(*pmic), GFP_KERNEL);
	if (!pmic)
		return -ENOMEM;

	if (of_device_is_compatible(node, "ti,tps659038-pmic")) {
		palmas_generic_regs_info[PALMAS_REG_REGEN2].ctrl_addr =
							TPS659038_REGEN2_CTRL;
		palmas_ddata.has_regen3 = false;
	}

	pmic->dev = &pdev->dev;
	pmic->palmas = palmas;
	palmas->pmic = pmic;
	platform_set_drvdata(pdev, pmic);
	pmic->palmas->pmic_ddata = driver_data;

	ret = palmas_dt_to_pdata(&pdev->dev, node, pdata, driver_data);
	if (ret)
		return ret;

	ret = palmas_smps_read(palmas, PALMAS_SMPS_CTRL, &reg);
	if (ret)
		return ret;

	if (reg & PALMAS_SMPS_CTRL_SMPS12_SMPS123_EN)
		pmic->smps123 = 1;

	if (reg & PALMAS_SMPS_CTRL_SMPS45_SMPS457_EN)
		pmic->smps457 = 1;

	config.regmap = palmas->regmap[REGULATOR_SLAVE];
	config.dev = &pdev->dev;
	config.driver_data = pmic;
	pdev_name = pdev->name;

	ret = driver_data->smps_register(pmic, driver_data, pdata, pdev_name,
					 config);
	if (ret)
		return ret;

	ret = driver_data->ldo_register(pmic, driver_data, pdata, pdev_name,
					config);

	return ret;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct platform_driver palmas_driver = {
	.driver = {
		.name = "palmas-pmic",
		.of_match_table = of_palmas_match_tbl,
<<<<<<< HEAD
		.owner = THIS_MODULE,
	},
	.probe = palmas_regulators_probe,
	.remove = palmas_regulators_remove,
=======
	},
	.probe = palmas_regulators_probe,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int __init palmas_init(void)
{
	return platform_driver_register(&palmas_driver);
}
subsys_initcall(palmas_init);

static void __exit palmas_exit(void)
{
	platform_driver_unregister(&palmas_driver);
}
module_exit(palmas_exit);

MODULE_AUTHOR("Graeme Gregory <gg@slimlogic.co.uk>");
MODULE_DESCRIPTION("Palmas voltage regulator driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:palmas-pmic");
MODULE_DEVICE_TABLE(of, of_palmas_match_tbl);
