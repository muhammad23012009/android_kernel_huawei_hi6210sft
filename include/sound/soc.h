/*
 * linux/sound/soc.h -- ALSA SoC Layer
 *
 * Author:		Liam Girdwood
 * Created:		Aug 11th 2005
 * Copyright:	Wolfson Microelectronics. PLC.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __LINUX_SND_SOC_H
#define __LINUX_SND_SOC_H

<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/notifier.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/regmap.h>
#include <linux/log2.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/compress_driver.h>
#include <sound/control.h>
#include <sound/ac97_codec.h>

/*
 * Convenience kcontrol builders
 */
<<<<<<< HEAD
#define SOC_DOUBLE_VALUE(xreg, shift_left, shift_right, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .rreg = xreg, .shift = shift_left, \
	.rshift = shift_right, .max = xmax, .platform_max = xmax, \
	.invert = xinvert})
#define SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert) \
	SOC_DOUBLE_VALUE(xreg, xshift, xshift, xmax, xinvert)
=======
#define SOC_DOUBLE_VALUE(xreg, shift_left, shift_right, xmax, xinvert, xautodisable) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .rreg = xreg, .shift = shift_left, \
	.rshift = shift_right, .max = xmax, .platform_max = xmax, \
	.invert = xinvert, .autodisable = xautodisable})
#define SOC_DOUBLE_S_VALUE(xreg, shift_left, shift_right, xmin, xmax, xsign_bit, xinvert, xautodisable) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .rreg = xreg, .shift = shift_left, \
	.rshift = shift_right, .min = xmin, .max = xmax, .platform_max = xmax, \
	.sign_bit = xsign_bit, .invert = xinvert, .autodisable = xautodisable})
#define SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert, xautodisable) \
	SOC_DOUBLE_VALUE(xreg, xshift, xshift, xmax, xinvert, xautodisable)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_SINGLE_VALUE_EXT(xreg, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .max = xmax, .platform_max = xmax, .invert = xinvert})
#define SOC_DOUBLE_R_VALUE(xlreg, xrreg, xshift, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xlreg, .rreg = xrreg, .shift = xshift, .rshift = xshift, \
	.max = xmax, .platform_max = xmax, .invert = xinvert})
<<<<<<< HEAD
=======
#define SOC_DOUBLE_R_S_VALUE(xlreg, xrreg, xshift, xmin, xmax, xsign_bit, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xlreg, .rreg = xrreg, .shift = xshift, .rshift = xshift, \
	.max = xmax, .min = xmin, .platform_max = xmax, .sign_bit = xsign_bit, \
	.invert = xinvert})
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_DOUBLE_R_RANGE_VALUE(xlreg, xrreg, xshift, xmin, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xlreg, .rreg = xrreg, .shift = xshift, .rshift = xshift, \
	.min = xmin, .max = xmax, .platform_max = xmax, .invert = xinvert})
#define SOC_SINGLE(xname, reg, shift, max, invert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,\
	.put = snd_soc_put_volsw, \
<<<<<<< HEAD
	.private_value =  SOC_SINGLE_VALUE(reg, shift, max, invert) }
=======
	.private_value = SOC_SINGLE_VALUE(reg, shift, max, invert, 0) }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_SINGLE_RANGE(xname, xreg, xshift, xmin, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw_range, .get = snd_soc_get_volsw_range, \
	.put = snd_soc_put_volsw_range, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xreg, .shift = xshift, \
		 .rshift = xshift,  .min = xmin, .max = xmax, \
		 .platform_max = xmax, .invert = xinvert} }
#define SOC_SINGLE_TLV(xname, reg, shift, max, invert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,\
	.put = snd_soc_put_volsw, \
<<<<<<< HEAD
	.private_value =  SOC_SINGLE_VALUE(reg, shift, max, invert) }
=======
	.private_value = SOC_SINGLE_VALUE(reg, shift, max, invert, 0) }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_SINGLE_SX_TLV(xname, xreg, xshift, xmin, xmax, tlv_array) \
{       .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
	SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array),\
<<<<<<< HEAD
	.info = snd_soc_info_volsw, \
=======
	.info = snd_soc_info_volsw_sx, \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.get = snd_soc_get_volsw_sx,\
	.put = snd_soc_put_volsw_sx, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xreg, \
		.shift = xshift, .rshift = xshift, \
		.max = xmax, .min = xmin} }
#define SOC_SINGLE_RANGE_TLV(xname, xreg, xshift, xmin, xmax, xinvert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw_range, \
	.get = snd_soc_get_volsw_range, .put = snd_soc_put_volsw_range, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xreg, .shift = xshift, \
		 .rshift = xshift, .min = xmin, .max = xmax, \
		 .platform_max = xmax, .invert = xinvert} }
#define SOC_DOUBLE(xname, reg, shift_left, shift_right, max, invert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw, \
	.put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_VALUE(reg, shift_left, shift_right, \
<<<<<<< HEAD
					  max, invert) }
=======
					  max, invert, 0) }
#define SOC_DOUBLE_STS(xname, reg, shift_left, shift_right, max, invert) \
{									\
	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),		\
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,		\
	.access = SNDRV_CTL_ELEM_ACCESS_READ |				\
		SNDRV_CTL_ELEM_ACCESS_VOLATILE,				\
	.private_value = SOC_DOUBLE_VALUE(reg, shift_left, shift_right,	\
					  max, invert, 0) }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_DOUBLE_R(xname, reg_left, reg_right, xshift, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.info = snd_soc_info_volsw, \
	.get = snd_soc_get_volsw, .put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
#define SOC_DOUBLE_R_RANGE(xname, reg_left, reg_right, xshift, xmin, \
			   xmax, xinvert)		\
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw_range, \
	.get = snd_soc_get_volsw_range, .put = snd_soc_put_volsw_range, \
	.private_value = SOC_DOUBLE_R_RANGE_VALUE(reg_left, reg_right, \
					    xshift, xmin, xmax, xinvert) }
#define SOC_DOUBLE_TLV(xname, reg, shift_left, shift_right, max, invert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw, \
	.put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_VALUE(reg, shift_left, shift_right, \
<<<<<<< HEAD
					  max, invert) }
=======
					  max, invert, 0) }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_DOUBLE_R_TLV(xname, reg_left, reg_right, xshift, xmax, xinvert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = snd_soc_get_volsw, .put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
#define SOC_DOUBLE_R_RANGE_TLV(xname, reg_left, reg_right, xshift, xmin, \
			       xmax, xinvert, tlv_array)		\
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw_range, \
	.get = snd_soc_get_volsw_range, .put = snd_soc_put_volsw_range, \
	.private_value = SOC_DOUBLE_R_RANGE_VALUE(reg_left, reg_right, \
					    xshift, xmin, xmax, xinvert) }
#define SOC_DOUBLE_R_SX_TLV(xname, xreg, xrreg, xshift, xmin, xmax, tlv_array) \
{       .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
	SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
<<<<<<< HEAD
	.info = snd_soc_info_volsw, \
=======
	.info = snd_soc_info_volsw_sx, \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.get = snd_soc_get_volsw_sx, \
	.put = snd_soc_put_volsw_sx, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xrreg, \
		.shift = xshift, .rshift = xshift, \
		.max = xmax, .min = xmin} }
<<<<<<< HEAD
#define SOC_DOUBLE_S8_TLV(xname, xreg, xmin, xmax, tlv_array) \
=======
#define SOC_DOUBLE_R_S_TLV(xname, reg_left, reg_right, xshift, xmin, xmax, xsign_bit, xinvert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = snd_soc_get_volsw, .put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_R_S_VALUE(reg_left, reg_right, xshift, \
					    xmin, xmax, xsign_bit, xinvert) }
#define SOC_SINGLE_S8_TLV(xname, xreg, xmin, xmax, tlv_array) \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{	.iface  = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		  SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
<<<<<<< HEAD
	.info   = snd_soc_info_volsw_s8, .get = snd_soc_get_volsw_s8, \
	.put    = snd_soc_put_volsw_s8, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .min = xmin, .max = xmax, \
		 .platform_max = xmax} }
#define SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmax, xtexts) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.max = xmax, .texts = xtexts, \
	.mask = xmax ? roundup_pow_of_two(xmax) - 1 : 0}
#define SOC_ENUM_SINGLE(xreg, xshift, xmax, xtexts) \
	SOC_ENUM_DOUBLE(xreg, xshift, xshift, xmax, xtexts)
#define SOC_ENUM_SINGLE_EXT(xmax, xtexts) \
{	.max = xmax, .texts = xtexts }
#define SOC_VALUE_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmask, xmax, xtexts, xvalues) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.mask = xmask, .max = xmax, .texts = xtexts, .values = xvalues}
#define SOC_VALUE_ENUM_SINGLE(xreg, xshift, xmask, xmax, xtexts, xvalues) \
	SOC_VALUE_ENUM_DOUBLE(xreg, xshift, xshift, xmask, xmax, xtexts, xvalues)
=======
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,\
	.put = snd_soc_put_volsw, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .rreg = xreg,  \
	 .min = xmin, .max = xmax, .platform_max = xmax, \
	.sign_bit = 7,} }
#define SOC_DOUBLE_S8_TLV(xname, xreg, xmin, xmax, tlv_array) \
{	.iface  = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		  SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,\
	.put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_S_VALUE(xreg, 0, 8, xmin, xmax, 7, 0, 0) }
#define SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xitems, xtexts) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.items = xitems, .texts = xtexts, \
	.mask = xitems ? roundup_pow_of_two(xitems) - 1 : 0}
#define SOC_ENUM_SINGLE(xreg, xshift, xitems, xtexts) \
	SOC_ENUM_DOUBLE(xreg, xshift, xshift, xitems, xtexts)
#define SOC_ENUM_SINGLE_EXT(xitems, xtexts) \
{	.items = xitems, .texts = xtexts }
#define SOC_VALUE_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmask, xitems, xtexts, xvalues) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.mask = xmask, .items = xitems, .texts = xtexts, .values = xvalues}
#define SOC_VALUE_ENUM_SINGLE(xreg, xshift, xmask, xitems, xtexts, xvalues) \
	SOC_VALUE_ENUM_DOUBLE(xreg, xshift, xshift, xmask, xitems, xtexts, xvalues)
#define SOC_VALUE_ENUM_SINGLE_AUTODISABLE(xreg, xshift, xmask, xitems, xtexts, xvalues) \
{	.reg = xreg, .shift_l = xshift, .shift_r = xshift, \
	.mask = xmask, .items = xitems, .texts = xtexts, \
	.values = xvalues, .autodisable = 1}
#define SOC_ENUM_SINGLE_VIRT(xitems, xtexts) \
	SOC_ENUM_SINGLE(SND_SOC_NOPM, 0, xitems, xtexts)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_ENUM(xname, xenum) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,\
	.info = snd_soc_info_enum_double, \
	.get = snd_soc_get_enum_double, .put = snd_soc_put_enum_double, \
	.private_value = (unsigned long)&xenum }
<<<<<<< HEAD
#define SOC_VALUE_ENUM(xname, xenum) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,\
	.info = snd_soc_info_enum_double, \
	.get = snd_soc_get_value_enum_double, \
	.put = snd_soc_put_value_enum_double, \
	.private_value = (unsigned long)&xenum }
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_SINGLE_EXT(xname, xreg, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
<<<<<<< HEAD
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert) }
=======
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert, 0) }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_DOUBLE_EXT(xname, reg, shift_left, shift_right, max, invert,\
	 xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = \
<<<<<<< HEAD
		SOC_DOUBLE_VALUE(reg, shift_left, shift_right, max, invert) }
=======
		SOC_DOUBLE_VALUE(reg, shift_left, shift_right, max, invert, 0) }
#define SOC_DOUBLE_R_EXT(xname, reg_left, reg_right, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_SINGLE_EXT_TLV(xname, xreg, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
<<<<<<< HEAD
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert) }
=======
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert, 0) }
#define SOC_SINGLE_RANGE_EXT_TLV(xname, xreg, xshift, xmin, xmax, xinvert, \
				 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw_range, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xreg, .shift = xshift, \
		 .rshift = xshift, .min = xmin, .max = xmax, \
		 .platform_max = xmax, .invert = xinvert} }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_DOUBLE_EXT_TLV(xname, xreg, shift_left, shift_right, xmax, xinvert,\
	 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		 SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = SOC_DOUBLE_VALUE(xreg, shift_left, shift_right, \
<<<<<<< HEAD
					  xmax, xinvert) }
=======
					  xmax, xinvert, 0) }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_DOUBLE_R_EXT_TLV(xname, reg_left, reg_right, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		 SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
#define SOC_SINGLE_BOOL_EXT(xname, xdata, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_bool_ext, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = xdata }
#define SOC_ENUM_EXT(xname, xenum, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
<<<<<<< HEAD
	.info = snd_soc_info_enum_ext, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&xenum }
=======
	.info = snd_soc_info_enum_double, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&xenum }
#define SOC_VALUE_ENUM_EXT(xname, xenum, xhandler_get, xhandler_put) \
	SOC_ENUM_EXT(xname, xenum, xhandler_get, xhandler_put)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define SND_SOC_BYTES(xname, xbase, xregs)		      \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,   \
	.info = snd_soc_bytes_info, .get = snd_soc_bytes_get, \
	.put = snd_soc_bytes_put, .private_value =	      \
		((unsigned long)&(struct soc_bytes)           \
		{.base = xbase, .num_regs = xregs }) }

#define SND_SOC_BYTES_MASK(xname, xbase, xregs, xmask)	      \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,   \
	.info = snd_soc_bytes_info, .get = snd_soc_bytes_get, \
	.put = snd_soc_bytes_put, .private_value =	      \
		((unsigned long)&(struct soc_bytes)           \
		{.base = xbase, .num_regs = xregs,	      \
		 .mask = xmask }) }

<<<<<<< HEAD
=======
/*
 * SND_SOC_BYTES_EXT is deprecated, please USE SND_SOC_BYTES_TLV instead
 */
#define SND_SOC_BYTES_EXT(xname, xcount, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_bytes_info_ext, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&(struct soc_bytes_ext) \
		{.max = xcount} }
#define SND_SOC_BYTES_TLV(xname, xcount, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READWRITE | \
		  SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK, \
	.tlv.c = (snd_soc_bytes_tlv_callback), \
	.info = snd_soc_bytes_info_ext, \
	.private_value = (unsigned long)&(struct soc_bytes_ext) \
		{.max = xcount, .get = xhandler_get, .put = xhandler_put, } }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SOC_SINGLE_XR_SX(xname, xregbase, xregcount, xnbits, \
		xmin, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.info = snd_soc_info_xr_sx, .get = snd_soc_get_xr_sx, \
	.put = snd_soc_put_xr_sx, \
	.private_value = (unsigned long)&(struct soc_mreg_control) \
		{.regbase = xregbase, .regcount = xregcount, .nbits = xnbits, \
		.invert = xinvert, .min = xmin, .max = xmax} }

#define SOC_SINGLE_STROBE(xname, xreg, xshift, xinvert) \
	SOC_SINGLE_EXT(xname, xreg, xshift, 1, xinvert, \
		snd_soc_get_strobe, snd_soc_put_strobe)

/*
 * Simplified versions of above macros, declaring a struct and calculating
 * ARRAY_SIZE internally
 */
#define SOC_ENUM_DOUBLE_DECL(name, xreg, xshift_l, xshift_r, xtexts) \
<<<<<<< HEAD
	struct soc_enum name = SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, \
=======
	const struct soc_enum name = SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						ARRAY_SIZE(xtexts), xtexts)
#define SOC_ENUM_SINGLE_DECL(name, xreg, xshift, xtexts) \
	SOC_ENUM_DOUBLE_DECL(name, xreg, xshift, xshift, xtexts)
#define SOC_ENUM_SINGLE_EXT_DECL(name, xtexts) \
<<<<<<< HEAD
	struct soc_enum name = SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(xtexts), xtexts)
#define SOC_VALUE_ENUM_DOUBLE_DECL(name, xreg, xshift_l, xshift_r, xmask, xtexts, xvalues) \
	struct soc_enum name = SOC_VALUE_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmask, \
=======
	const struct soc_enum name = SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(xtexts), xtexts)
#define SOC_VALUE_ENUM_DOUBLE_DECL(name, xreg, xshift_l, xshift_r, xmask, xtexts, xvalues) \
	const struct soc_enum name = SOC_VALUE_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmask, \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
							ARRAY_SIZE(xtexts), xtexts, xvalues)
#define SOC_VALUE_ENUM_SINGLE_DECL(name, xreg, xshift, xmask, xtexts, xvalues) \
	SOC_VALUE_ENUM_DOUBLE_DECL(name, xreg, xshift, xshift, xmask, xtexts, xvalues)

<<<<<<< HEAD
=======
#define SOC_VALUE_ENUM_SINGLE_AUTODISABLE_DECL(name, xreg, xshift, xmask, xtexts, xvalues) \
	const struct soc_enum name = SOC_VALUE_ENUM_SINGLE_AUTODISABLE(xreg, \
		xshift, xmask, ARRAY_SIZE(xtexts), xtexts, xvalues)

#define SOC_ENUM_SINGLE_VIRT_DECL(name, xtexts) \
	const struct soc_enum name = SOC_ENUM_SINGLE_VIRT(ARRAY_SIZE(xtexts), xtexts)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Component probe and remove ordering levels for components with runtime
 * dependencies.
 */
#define SND_SOC_COMP_ORDER_FIRST		-2
#define SND_SOC_COMP_ORDER_EARLY		-1
#define SND_SOC_COMP_ORDER_NORMAL		0
#define SND_SOC_COMP_ORDER_LATE		1
#define SND_SOC_COMP_ORDER_LAST		2

/*
 * Bias levels
 *
 * @ON:      Bias is fully on for audio playback and capture operations.
 * @PREPARE: Prepare for audio operations. Called before DAPM switching for
 *           stream start and stop operations.
 * @STANDBY: Low power standby state when no playback/capture operations are
 *           in progress. NOTE: The transition time between STANDBY and ON
 *           should be as fast as possible and no longer than 10ms.
 * @OFF:     Power Off. No restrictions on transition times.
 */
enum snd_soc_bias_level {
	SND_SOC_BIAS_OFF = 0,
	SND_SOC_BIAS_STANDBY = 1,
	SND_SOC_BIAS_PREPARE = 2,
	SND_SOC_BIAS_ON = 3,
};

struct device_node;
struct snd_jack;
struct snd_soc_card;
struct snd_soc_pcm_stream;
struct snd_soc_ops;
struct snd_soc_pcm_runtime;
struct snd_soc_dai;
struct snd_soc_dai_driver;
struct snd_soc_platform;
struct snd_soc_dai_link;
struct snd_soc_platform_driver;
struct snd_soc_codec;
struct snd_soc_codec_driver;
struct snd_soc_component;
struct snd_soc_component_driver;
struct soc_enum;
struct snd_soc_jack;
struct snd_soc_jack_zone;
struct snd_soc_jack_pin;
<<<<<<< HEAD
struct snd_soc_cache_ops;
#include <sound/soc-dapm.h>
#include <sound/soc-dpcm.h>

#ifdef CONFIG_GPIOLIB
struct snd_soc_jack_gpio;
#endif

typedef int (*hw_write_t)(void *,const char* ,int);

extern struct snd_ac97_bus_ops soc_ac97_ops;

enum snd_soc_control_type {
	SND_SOC_I2C = 1,
	SND_SOC_SPI,
	SND_SOC_REGMAP,
};

enum snd_soc_compress_type {
	SND_SOC_FLAT_COMPRESSION = 1,
};

=======
#include <sound/soc-dapm.h>
#include <sound/soc-dpcm.h>
#include <sound/soc-topology.h>

struct snd_soc_jack_gpio;

typedef int (*hw_write_t)(void *,const char* ,int);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
enum snd_soc_pcm_subclass {
	SND_SOC_PCM_CLASS_PCM	= 0,
	SND_SOC_PCM_CLASS_BE	= 1,
};

enum snd_soc_card_subclass {
	SND_SOC_CARD_CLASS_INIT		= 0,
	SND_SOC_CARD_CLASS_RUNTIME	= 1,
};

int snd_soc_codec_set_sysclk(struct snd_soc_codec *codec, int clk_id,
			     int source, unsigned int freq, int dir);
int snd_soc_codec_set_pll(struct snd_soc_codec *codec, int pll_id, int source,
			  unsigned int freq_in, unsigned int freq_out);

int snd_soc_register_card(struct snd_soc_card *card);
int snd_soc_unregister_card(struct snd_soc_card *card);
<<<<<<< HEAD
int snd_soc_suspend(struct device *dev);
int snd_soc_resume(struct device *dev);
int snd_soc_poweroff(struct device *dev);
int snd_soc_register_platform(struct device *dev,
		const struct snd_soc_platform_driver *platform_drv);
=======
int devm_snd_soc_register_card(struct device *dev, struct snd_soc_card *card);
#ifdef CONFIG_PM_SLEEP
int snd_soc_suspend(struct device *dev);
int snd_soc_resume(struct device *dev);
#else
static inline int snd_soc_suspend(struct device *dev)
{
	return 0;
}

static inline int snd_soc_resume(struct device *dev)
{
	return 0;
}
#endif
int snd_soc_poweroff(struct device *dev);
int snd_soc_register_platform(struct device *dev,
		const struct snd_soc_platform_driver *platform_drv);
int devm_snd_soc_register_platform(struct device *dev,
		const struct snd_soc_platform_driver *platform_drv);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void snd_soc_unregister_platform(struct device *dev);
int snd_soc_add_platform(struct device *dev, struct snd_soc_platform *platform,
		const struct snd_soc_platform_driver *platform_drv);
void snd_soc_remove_platform(struct snd_soc_platform *platform);
struct snd_soc_platform *snd_soc_lookup_platform(struct device *dev);
int snd_soc_register_codec(struct device *dev,
		const struct snd_soc_codec_driver *codec_drv,
		struct snd_soc_dai_driver *dai_drv, int num_dai);
void snd_soc_unregister_codec(struct device *dev);
int snd_soc_register_component(struct device *dev,
			 const struct snd_soc_component_driver *cmpnt_drv,
			 struct snd_soc_dai_driver *dai_drv, int num_dai);
<<<<<<< HEAD
void snd_soc_unregister_component(struct device *dev);
int snd_soc_codec_volatile_register(struct snd_soc_codec *codec,
				    unsigned int reg);
int snd_soc_codec_readable_register(struct snd_soc_codec *codec,
				    unsigned int reg);
int snd_soc_codec_writable_register(struct snd_soc_codec *codec,
				    unsigned int reg);
int snd_soc_codec_set_cache_io(struct snd_soc_codec *codec,
			       int addr_bits, int data_bits,
			       enum snd_soc_control_type control);
int snd_soc_cache_sync(struct snd_soc_codec *codec);
int snd_soc_cache_init(struct snd_soc_codec *codec);
int snd_soc_cache_exit(struct snd_soc_codec *codec);
int snd_soc_cache_write(struct snd_soc_codec *codec,
			unsigned int reg, unsigned int value);
int snd_soc_cache_read(struct snd_soc_codec *codec,
		       unsigned int reg, unsigned int *value);
int snd_soc_default_volatile_register(struct snd_soc_codec *codec,
				      unsigned int reg);
int snd_soc_default_readable_register(struct snd_soc_codec *codec,
				      unsigned int reg);
int snd_soc_default_writable_register(struct snd_soc_codec *codec,
				      unsigned int reg);
=======
int devm_snd_soc_register_component(struct device *dev,
			 const struct snd_soc_component_driver *cmpnt_drv,
			 struct snd_soc_dai_driver *dai_drv, int num_dai);
void snd_soc_unregister_component(struct device *dev);
int snd_soc_cache_init(struct snd_soc_codec *codec);
int snd_soc_cache_exit(struct snd_soc_codec *codec);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int snd_soc_platform_read(struct snd_soc_platform *platform,
					unsigned int reg);
int snd_soc_platform_write(struct snd_soc_platform *platform,
					unsigned int reg, unsigned int val);
int soc_new_pcm(struct snd_soc_pcm_runtime *rtd, int num);
<<<<<<< HEAD
int soc_new_compress(struct snd_soc_pcm_runtime *rtd, int num);
=======
#ifdef CONFIG_SND_SOC_COMPRESS
int snd_soc_new_compress(struct snd_soc_pcm_runtime *rtd, int num);
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct snd_pcm_substream *snd_soc_get_dai_substream(struct snd_soc_card *card,
		const char *dai_link, int stream);
struct snd_soc_pcm_runtime *snd_soc_get_pcm_runtime(struct snd_soc_card *card,
		const char *dai_link);

<<<<<<< HEAD
=======
bool snd_soc_runtime_ignore_pmdown_time(struct snd_soc_pcm_runtime *rtd);
void snd_soc_runtime_activate(struct snd_soc_pcm_runtime *rtd, int stream);
void snd_soc_runtime_deactivate(struct snd_soc_pcm_runtime *rtd, int stream);

int snd_soc_runtime_set_dai_fmt(struct snd_soc_pcm_runtime *rtd,
	unsigned int dai_fmt);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Utility functions to get clock rates from various things */
int snd_soc_calc_frame_size(int sample_size, int channels, int tdm_slots);
int snd_soc_params_to_frame_size(struct snd_pcm_hw_params *params);
int snd_soc_calc_bclk(int fs, int sample_size, int channels, int tdm_slots);
int snd_soc_params_to_bclk(struct snd_pcm_hw_params *parms);

/* set runtime hw params */
int snd_soc_set_runtime_hwparams(struct snd_pcm_substream *substream,
	const struct snd_pcm_hardware *hw);

int snd_soc_platform_trigger(struct snd_pcm_substream *substream,
		int cmd, struct snd_soc_platform *platform);

<<<<<<< HEAD
/* Jack reporting */
int snd_soc_jack_new(struct snd_soc_codec *codec, const char *id, int type,
		     struct snd_soc_jack *jack);
=======
int soc_dai_hw_params(struct snd_pcm_substream *substream,
		      struct snd_pcm_hw_params *params,
		      struct snd_soc_dai *dai);

/* Jack reporting */
int snd_soc_card_jack_new(struct snd_soc_card *card, const char *id, int type,
	struct snd_soc_jack *jack, struct snd_soc_jack_pin *pins,
	unsigned int num_pins);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void snd_soc_jack_report(struct snd_soc_jack *jack, int status, int mask);
int snd_soc_jack_add_pins(struct snd_soc_jack *jack, int count,
			  struct snd_soc_jack_pin *pins);
void snd_soc_jack_notifier_register(struct snd_soc_jack *jack,
				    struct notifier_block *nb);
void snd_soc_jack_notifier_unregister(struct snd_soc_jack *jack,
				      struct notifier_block *nb);
int snd_soc_jack_add_zones(struct snd_soc_jack *jack, int count,
			  struct snd_soc_jack_zone *zones);
int snd_soc_jack_get_type(struct snd_soc_jack *jack, int micbias_voltage);
#ifdef CONFIG_GPIOLIB
int snd_soc_jack_add_gpios(struct snd_soc_jack *jack, int count,
			struct snd_soc_jack_gpio *gpios);
<<<<<<< HEAD
void snd_soc_jack_free_gpios(struct snd_soc_jack *jack, int count,
			struct snd_soc_jack_gpio *gpios);
#endif

/* codec register bit access */
int snd_soc_update_bits(struct snd_soc_codec *codec, unsigned short reg,
				unsigned int mask, unsigned int value);
int snd_soc_update_bits_locked(struct snd_soc_codec *codec,
			       unsigned short reg, unsigned int mask,
			       unsigned int value);
int snd_soc_test_bits(struct snd_soc_codec *codec, unsigned short reg,
				unsigned int mask, unsigned int value);

int snd_soc_new_ac97_codec(struct snd_soc_codec *codec,
	struct snd_ac97_bus_ops *ops, int num);
void snd_soc_free_ac97_codec(struct snd_soc_codec *codec);
=======
int snd_soc_jack_add_gpiods(struct device *gpiod_dev,
			    struct snd_soc_jack *jack,
			    int count, struct snd_soc_jack_gpio *gpios);
void snd_soc_jack_free_gpios(struct snd_soc_jack *jack, int count,
			struct snd_soc_jack_gpio *gpios);
#else
static inline int snd_soc_jack_add_gpios(struct snd_soc_jack *jack, int count,
					 struct snd_soc_jack_gpio *gpios)
{
	return 0;
}

static inline int snd_soc_jack_add_gpiods(struct device *gpiod_dev,
					  struct snd_soc_jack *jack,
					  int count,
					  struct snd_soc_jack_gpio *gpios)
{
	return 0;
}

static inline void snd_soc_jack_free_gpios(struct snd_soc_jack *jack, int count,
					   struct snd_soc_jack_gpio *gpios)
{
}
#endif

/* codec register bit access */
int snd_soc_update_bits(struct snd_soc_codec *codec, unsigned int reg,
				unsigned int mask, unsigned int value);
int snd_soc_update_bits_locked(struct snd_soc_codec *codec,
			       unsigned int reg, unsigned int mask,
			       unsigned int value);
int snd_soc_test_bits(struct snd_soc_codec *codec, unsigned int reg,
				unsigned int mask, unsigned int value);

#ifdef CONFIG_SND_SOC_AC97_BUS
struct snd_ac97 *snd_soc_alloc_ac97_codec(struct snd_soc_codec *codec);
struct snd_ac97 *snd_soc_new_ac97_codec(struct snd_soc_codec *codec,
	unsigned int id, unsigned int id_mask);
void snd_soc_free_ac97_codec(struct snd_ac97 *ac97);

int snd_soc_set_ac97_ops(struct snd_ac97_bus_ops *ops);
int snd_soc_set_ac97_ops_of_reset(struct snd_ac97_bus_ops *ops,
		struct platform_device *pdev);

extern struct snd_ac97_bus_ops *soc_ac97_ops;
#else
static inline int snd_soc_set_ac97_ops_of_reset(struct snd_ac97_bus_ops *ops,
	struct platform_device *pdev)
{
	return 0;
}

static inline int snd_soc_set_ac97_ops(struct snd_ac97_bus_ops *ops)
{
	return 0;
}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 *Controls
 */
struct snd_kcontrol *snd_soc_cnew(const struct snd_kcontrol_new *_template,
				  void *data, const char *long_name,
				  const char *prefix);
<<<<<<< HEAD
int snd_soc_add_codec_controls(struct snd_soc_codec *codec,
	const struct snd_kcontrol_new *controls, int num_controls);
int snd_soc_add_platform_controls(struct snd_soc_platform *platform,
	const struct snd_kcontrol_new *controls, int num_controls);
=======
struct snd_kcontrol *snd_soc_card_get_kcontrol(struct snd_soc_card *soc_card,
					       const char *name);
int snd_soc_add_component_controls(struct snd_soc_component *component,
	const struct snd_kcontrol_new *controls, unsigned int num_controls);
int snd_soc_add_codec_controls(struct snd_soc_codec *codec,
	const struct snd_kcontrol_new *controls, unsigned int num_controls);
int snd_soc_add_platform_controls(struct snd_soc_platform *platform,
	const struct snd_kcontrol_new *controls, unsigned int num_controls);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int snd_soc_add_card_controls(struct snd_soc_card *soc_card,
	const struct snd_kcontrol_new *controls, int num_controls);
int snd_soc_add_dai_controls(struct snd_soc_dai *dai,
	const struct snd_kcontrol_new *controls, int num_controls);
int snd_soc_info_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
<<<<<<< HEAD
int snd_soc_info_enum_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int snd_soc_get_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
<<<<<<< HEAD
int snd_soc_get_value_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_value_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_info_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_info_volsw_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
=======
int snd_soc_info_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_info_volsw_sx(struct snd_kcontrol *kcontrol,
			  struct snd_ctl_elem_info *uinfo);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define snd_soc_info_bool_ext		snd_ctl_boolean_mono_info
int snd_soc_get_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
#define snd_soc_get_volsw_2r snd_soc_get_volsw
#define snd_soc_put_volsw_2r snd_soc_put_volsw
int snd_soc_get_volsw_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
<<<<<<< HEAD
int snd_soc_info_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_get_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int snd_soc_info_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_put_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_get_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
<<<<<<< HEAD
int snd_soc_limit_volume(struct snd_soc_codec *codec,
=======
int snd_soc_limit_volume(struct snd_soc_card *card,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	const char *name, int max);
int snd_soc_bytes_info(struct snd_kcontrol *kcontrol,
		       struct snd_ctl_elem_info *uinfo);
int snd_soc_bytes_get(struct snd_kcontrol *kcontrol,
		      struct snd_ctl_elem_value *ucontrol);
int snd_soc_bytes_put(struct snd_kcontrol *kcontrol,
		      struct snd_ctl_elem_value *ucontrol);
<<<<<<< HEAD
=======
int snd_soc_bytes_info_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *ucontrol);
int snd_soc_bytes_tlv_callback(struct snd_kcontrol *kcontrol, int op_flag,
	unsigned int size, unsigned int __user *tlv);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int snd_soc_info_xr_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_get_xr_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_xr_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_get_strobe(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_strobe(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);

/**
<<<<<<< HEAD
 * struct snd_soc_reg_access - Describes whether a given register is
 * readable, writable or volatile.
 *
 * @reg: the register number
 * @read: whether this register is readable
 * @write: whether this register is writable
 * @vol: whether this register is volatile
 */
struct snd_soc_reg_access {
	u16 reg;
	u16 read;
	u16 write;
	u16 vol;
};

/**
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * struct snd_soc_jack_pin - Describes a pin to update based on jack detection
 *
 * @pin:    name of the pin to update
 * @mask:   bits to check for in reported jack status
 * @invert: if non-zero then pin is enabled when status is not reported
<<<<<<< HEAD
=======
 * @list:   internal list entry
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
struct snd_soc_jack_pin {
	struct list_head list;
	const char *pin;
	int mask;
	bool invert;
};

/**
 * struct snd_soc_jack_zone - Describes voltage zones of jack detection
 *
 * @min_mv: start voltage in mv
 * @max_mv: end voltage in mv
 * @jack_type: type of jack that is expected for this voltage
 * @debounce_time: debounce_time for jack, codec driver should wait for this
 *		duration before reading the adc for voltages
<<<<<<< HEAD
 * @:list: list container
=======
 * @list:   internal list entry
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
struct snd_soc_jack_zone {
	unsigned int min_mv;
	unsigned int max_mv;
	unsigned int jack_type;
	unsigned int debounce_time;
	struct list_head list;
};

/**
 * struct snd_soc_jack_gpio - Describes a gpio pin for jack detection
 *
<<<<<<< HEAD
 * @gpio:         gpio number
 * @name:         gpio name
 * @report:       value to report when jack detected
 * @invert:       report presence in low state
 * @debouce_time: debouce time in ms
=======
 * @gpio:         legacy gpio number
 * @idx:          gpio descriptor index within the function of the GPIO
 *                consumer device
 * @gpiod_dev:    GPIO consumer device
 * @name:         gpio name. Also as connection ID for the GPIO consumer
 *                device function name lookup
 * @report:       value to report when jack detected
 * @invert:       report presence in low state
 * @debounce_time: debounce time in ms
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @wake:	  enable as wake source
 * @jack_status_check: callback function which overrides the detection
 *		       to provide more complex checks (eg, reading an
 *		       ADC).
 */
<<<<<<< HEAD
#ifdef CONFIG_GPIOLIB
struct snd_soc_jack_gpio {
	unsigned int gpio;
=======
struct snd_soc_jack_gpio {
	unsigned int gpio;
	unsigned int idx;
	struct device *gpiod_dev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	const char *name;
	int report;
	int invert;
	int debounce_time;
	bool wake;

<<<<<<< HEAD
	struct snd_soc_jack *jack;
	struct delayed_work work;

	int (*jack_status_check)(void);
};
#endif
=======
	/* private: */
	struct snd_soc_jack *jack;
	struct delayed_work work;
	struct gpio_desc *desc;

	void *data;
	/* public: */
	int (*jack_status_check)(void *data);
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct snd_soc_jack {
	struct mutex mutex;
	struct snd_jack *jack;
<<<<<<< HEAD
	struct snd_soc_codec *codec;
=======
	struct snd_soc_card *card;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct list_head pins;
	int status;
	struct blocking_notifier_head notifier;
	struct list_head jack_zones;
};

/* SoC PCM stream information */
struct snd_soc_pcm_stream {
	const char *stream_name;
	u64 formats;			/* SNDRV_PCM_FMTBIT_* */
	unsigned int rates;		/* SNDRV_PCM_RATE_* */
	unsigned int rate_min;		/* min rate */
	unsigned int rate_max;		/* max rate */
	unsigned int channels_min;	/* min channels */
	unsigned int channels_max;	/* max channels */
	unsigned int sig_bits;		/* number of bits of content */
};

/* SoC audio ops */
struct snd_soc_ops {
	int (*startup)(struct snd_pcm_substream *);
	void (*shutdown)(struct snd_pcm_substream *);
	int (*hw_params)(struct snd_pcm_substream *, struct snd_pcm_hw_params *);
	int (*hw_free)(struct snd_pcm_substream *);
	int (*prepare)(struct snd_pcm_substream *);
	int (*trigger)(struct snd_pcm_substream *, int);
};

struct snd_soc_compr_ops {
	int (*startup)(struct snd_compr_stream *);
	void (*shutdown)(struct snd_compr_stream *);
	int (*set_params)(struct snd_compr_stream *);
	int (*trigger)(struct snd_compr_stream *);
};

<<<<<<< HEAD
/* SoC cache ops */
struct snd_soc_cache_ops {
	const char *name;
	enum snd_soc_compress_type id;
	int (*init)(struct snd_soc_codec *codec);
	int (*exit)(struct snd_soc_codec *codec);
	int (*read)(struct snd_soc_codec *codec, unsigned int reg,
		unsigned int *value);
	int (*write)(struct snd_soc_codec *codec, unsigned int reg,
		unsigned int value);
	int (*sync)(struct snd_soc_codec *codec);
};

/* SoC Audio Codec device */
struct snd_soc_codec {
	const char *name;
	const char *name_prefix;
	int id;
	struct device *dev;
	const struct snd_soc_codec_driver *driver;

	struct mutex mutex;
	struct snd_soc_card *card;
	struct list_head list;
	struct list_head card_list;
	int num_dai;
	enum snd_soc_compress_type compress_type;
	size_t reg_size;	/* reg_cache_size * reg_word_size */
	int (*volatile_register)(struct snd_soc_codec *, unsigned int);
	int (*readable_register)(struct snd_soc_codec *, unsigned int);
	int (*writable_register)(struct snd_soc_codec *, unsigned int);

	/* runtime */
	struct snd_ac97 *ac97;  /* for ad-hoc ac97 devices */
	unsigned int active;
	unsigned int cache_bypass:1; /* Suppress access to the cache */
	unsigned int suspended:1; /* Codec is in suspend PM state */
	unsigned int probed:1; /* Codec has been probed */
	unsigned int ac97_registered:1; /* Codec has been AC97 registered */
	unsigned int ac97_created:1; /* Codec has been created by SoC */
	unsigned int sysfs_registered:1; /* codec has been sysfs registered */
	unsigned int cache_init:1; /* codec cache has been initialized */
	unsigned int using_regmap:1; /* using regmap access */
	u32 cache_only;  /* Suppress writes to hardware */
	u32 cache_sync; /* Cache needs to be synced to hardware */

	/* codec IO */
	void *control_data; /* codec control (i2c/3wire) data */
	enum snd_soc_control_type control_type;
	hw_write_t hw_write;
	unsigned int (*hw_read)(struct snd_soc_codec *, unsigned int);
	unsigned int (*read)(struct snd_soc_codec *, unsigned int);
	int (*write)(struct snd_soc_codec *, unsigned int, unsigned int);
	int (*bulk_write_raw)(struct snd_soc_codec *, unsigned int, const void *, size_t);
	void *reg_cache;
	const void *reg_def_copy;
	const struct snd_soc_cache_ops *cache_ops;
	struct mutex cache_rw_mutex;
	int val_bytes;

	/* dapm */
	struct snd_soc_dapm_context dapm;
	unsigned int ignore_pmdown_time:1; /* pmdown_time is ignored at stop */

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_codec_root;
	struct dentry *debugfs_reg;
	struct dentry *debugfs_dapm;
=======
/* component interface */
struct snd_soc_component_driver {
	const char *name;

	/* Default control and setup, added after probe() is run */
	const struct snd_kcontrol_new *controls;
	unsigned int num_controls;
	const struct snd_soc_dapm_widget *dapm_widgets;
	unsigned int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	unsigned int num_dapm_routes;

	int (*probe)(struct snd_soc_component *);
	void (*remove)(struct snd_soc_component *);

	/* DT */
	int (*of_xlate_dai_name)(struct snd_soc_component *component,
				 struct of_phandle_args *args,
				 const char **dai_name);
	void (*seq_notifier)(struct snd_soc_component *, enum snd_soc_dapm_type,
		int subseq);
	int (*stream_event)(struct snd_soc_component *, int event);

	/* probe ordering - for components with runtime dependencies */
	int probe_order;
	int remove_order;
};

struct snd_soc_component {
	const char *name;
	int id;
	const char *name_prefix;
	struct device *dev;
	struct snd_soc_card *card;

	unsigned int active;

	unsigned int ignore_pmdown_time:1; /* pmdown_time is ignored at stop */
	unsigned int registered_as_component:1;

	struct list_head list;
	struct list_head list_aux; /* for auxiliary component of the card */

	struct snd_soc_dai_driver *dai_drv;
	int num_dai;

	const struct snd_soc_component_driver *driver;

	struct list_head dai_list;

	int (*read)(struct snd_soc_component *, unsigned int, unsigned int *);
	int (*write)(struct snd_soc_component *, unsigned int, unsigned int);

	struct regmap *regmap;
	int val_bytes;

	struct mutex io_mutex;

	/* attached dynamic objects */
	struct list_head dobj_list;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_root;
#endif

	/*
	* DO NOT use any of the fields below in drivers, they are temporary and
	* are going to be removed again soon. If you use them in driver code the
	* driver will be marked as BROKEN when these fields are removed.
	*/

	/* Don't use these, use snd_soc_component_get_dapm() */
	struct snd_soc_dapm_context dapm;

	const struct snd_kcontrol_new *controls;
	unsigned int num_controls;
	const struct snd_soc_dapm_widget *dapm_widgets;
	unsigned int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	unsigned int num_dapm_routes;
	struct snd_soc_codec *codec;

	int (*probe)(struct snd_soc_component *);
	void (*remove)(struct snd_soc_component *);

	/* machine specific init */
	int (*init)(struct snd_soc_component *component);

#ifdef CONFIG_DEBUG_FS
	void (*init_debugfs)(struct snd_soc_component *component);
	const char *debugfs_prefix;
#endif
};

/* SoC Audio Codec device */
struct snd_soc_codec {
	struct device *dev;
	const struct snd_soc_codec_driver *driver;

	struct list_head list;
	struct list_head card_list;

	/* runtime */
	unsigned int cache_bypass:1; /* Suppress access to the cache */
	unsigned int suspended:1; /* Codec is in suspend PM state */
	unsigned int cache_init:1; /* codec cache has been initialized */

	/* codec IO */
	void *control_data; /* codec control (i2c/3wire) data */
	hw_write_t hw_write;
	void *reg_cache;

	/* component */
	struct snd_soc_component component;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_reg;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
};

/* codec driver */
struct snd_soc_codec_driver {

	/* driver ops */
	int (*probe)(struct snd_soc_codec *);
	int (*remove)(struct snd_soc_codec *);
	int (*suspend)(struct snd_soc_codec *);
	int (*resume)(struct snd_soc_codec *);
<<<<<<< HEAD

	/* Default control and setup, added after probe() is run */
	const struct snd_kcontrol_new *controls;
	int num_controls;
	const struct snd_soc_dapm_widget *dapm_widgets;
	int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	int num_dapm_routes;
=======
	struct snd_soc_component_driver component_driver;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* codec wide operations */
	int (*set_sysclk)(struct snd_soc_codec *codec,
			  int clk_id, int source, unsigned int freq, int dir);
	int (*set_pll)(struct snd_soc_codec *codec, int pll_id, int source,
		unsigned int freq_in, unsigned int freq_out);

	/* codec IO */
<<<<<<< HEAD
	unsigned int (*read)(struct snd_soc_codec *, unsigned int);
	int (*write)(struct snd_soc_codec *, unsigned int, unsigned int);
	int (*display_register)(struct snd_soc_codec *, char *,
				size_t, unsigned int);
	int (*volatile_register)(struct snd_soc_codec *, unsigned int);
	int (*readable_register)(struct snd_soc_codec *, unsigned int);
	int (*writable_register)(struct snd_soc_codec *, unsigned int);
=======
	struct regmap *(*get_regmap)(struct device *);
	unsigned int (*read)(struct snd_soc_codec *, unsigned int);
	int (*write)(struct snd_soc_codec *, unsigned int, unsigned int);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int reg_cache_size;
	short reg_cache_step;
	short reg_word_size;
	const void *reg_cache_default;
<<<<<<< HEAD
	short reg_access_size;
	const struct snd_soc_reg_access *reg_access_default;
	enum snd_soc_compress_type compress_type;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* codec bias level */
	int (*set_bias_level)(struct snd_soc_codec *,
			      enum snd_soc_bias_level level);
	bool idle_bias_off;
<<<<<<< HEAD
=======
	bool suspend_bias_off;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	void (*seq_notifier)(struct snd_soc_dapm_context *,
			     enum snd_soc_dapm_type, int);

<<<<<<< HEAD
	/* codec stream completion event */
	int (*stream_event)(struct snd_soc_dapm_context *dapm, int event);

	bool ignore_pmdown_time;  /* Doesn't benefit from pmdown delay */

	/* probe ordering - for components with runtime dependencies */
	int probe_order;
	int remove_order;
=======
	bool ignore_pmdown_time;  /* Doesn't benefit from pmdown delay */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* SoC platform interface */
struct snd_soc_platform_driver {

	int (*probe)(struct snd_soc_platform *);
	int (*remove)(struct snd_soc_platform *);
<<<<<<< HEAD
	int (*suspend)(struct snd_soc_dai *dai);
	int (*resume)(struct snd_soc_dai *dai);
=======
	struct snd_soc_component_driver component_driver;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* pcm creation and destruction */
	int (*pcm_new)(struct snd_soc_pcm_runtime *);
	void (*pcm_free)(struct snd_pcm *);

<<<<<<< HEAD
	/* Default control and setup, added after probe() is run */
	const struct snd_kcontrol_new *controls;
	int num_controls;
	const struct snd_soc_dapm_widget *dapm_widgets;
	int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	int num_dapm_routes;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * For platform caused delay reporting.
	 * Optional.
	 */
	snd_pcm_sframes_t (*delay)(struct snd_pcm_substream *,
		struct snd_soc_dai *);

	/* platform stream pcm ops */
	const struct snd_pcm_ops *ops;

	/* platform stream compress ops */
	const struct snd_compr_ops *compr_ops;

<<<<<<< HEAD
	/* platform stream completion event */
	int (*stream_event)(struct snd_soc_dapm_context *dapm, int event);

	/* probe ordering - for components with runtime dependencies */
	int probe_order;
	int remove_order;

	/* platform IO - used for platform DAPM */
	unsigned int (*read)(struct snd_soc_platform *, unsigned int);
	int (*write)(struct snd_soc_platform *, unsigned int, unsigned int);
	int (*bespoke_trigger)(struct snd_pcm_substream *, int);
};

struct snd_soc_platform {
	const char *name;
	int id;
	struct device *dev;
	const struct snd_soc_platform_driver *driver;
	struct mutex mutex;

	unsigned int suspended:1; /* platform is suspended */
	unsigned int probed:1;

	struct snd_soc_card *card;
	struct list_head list;
	struct list_head card_list;

	struct snd_soc_dapm_context dapm;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_platform_root;
	struct dentry *debugfs_dapm;
#endif
};

struct snd_soc_component_driver {
	const char *name;
};

struct snd_soc_component {
	const char *name;
	int id;
	int num_dai;
	struct device *dev;
	struct list_head list;

	const struct snd_soc_component_driver *driver;
=======
	int (*bespoke_trigger)(struct snd_pcm_substream *, int);
};

struct snd_soc_dai_link_component {
	const char *name;
	struct device_node *of_node;
	const char *dai_name;
};

struct snd_soc_platform {
	struct device *dev;
	const struct snd_soc_platform_driver *driver;

	struct list_head list;

	struct snd_soc_component component;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct snd_soc_dai_link {
	/* config - must be set by machine driver */
	const char *name;			/* Codec name */
	const char *stream_name;		/* Stream name */
	/*
	 * You MAY specify the link's CPU-side device, either by device name,
	 * or by DT/OF node, but not both. If this information is omitted,
	 * the CPU-side DAI is matched using .cpu_dai_name only, which hence
	 * must be globally unique. These fields are currently typically used
	 * only for codec to codec links, or systems using device tree.
	 */
	const char *cpu_name;
<<<<<<< HEAD
	const struct device_node *cpu_of_node;
=======
	struct device_node *cpu_of_node;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * You MAY specify the DAI name of the CPU DAI. If this information is
	 * omitted, the CPU-side DAI is matched using .cpu_name/.cpu_of_node
	 * only, which only works well when that device exposes a single DAI.
	 */
	const char *cpu_dai_name;
	/*
	 * You MUST specify the link's codec, either by device name, or by
	 * DT/OF node, but not both.
	 */
	const char *codec_name;
<<<<<<< HEAD
	const struct device_node *codec_of_node;
	/* You MUST specify the DAI name within the codec */
	const char *codec_dai_name;
=======
	struct device_node *codec_of_node;
	/* You MUST specify the DAI name within the codec */
	const char *codec_dai_name;

	struct snd_soc_dai_link_component *codecs;
	unsigned int num_codecs;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * You MAY specify the link's platform/PCM/DMA driver, either by
	 * device name, or by DT/OF node, but not both. Some forms of link
	 * do not need a platform.
	 */
	const char *platform_name;
<<<<<<< HEAD
	const struct device_node *platform_of_node;
	int be_id;	/* optional ID for machine driver BE identification */

	const struct snd_soc_pcm_stream *params;
=======
	struct device_node *platform_of_node;
	int id;	/* optional ID for machine driver link identification */

	const struct snd_soc_pcm_stream *params;
	unsigned int num_params;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	unsigned int dai_fmt;           /* format to set on init */

	enum snd_soc_dpcm_trigger trigger[2]; /* trigger type for DPCM */

<<<<<<< HEAD
=======
	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_pcm_runtime *rtd);

	/* optional hw_params re-writing for BE and FE sync */
	int (*be_hw_params_fixup)(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params);

	/* machine stream operations */
	const struct snd_soc_ops *ops;
	const struct snd_soc_compr_ops *compr_ops;

	/* For unidirectional dai links */
	bool playback_only;
	bool capture_only;

	/* Mark this pcm with non atomic ops */
	bool nonatomic;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Keep DAI active over suspend */
	unsigned int ignore_suspend:1;

	/* Symmetry requirements */
	unsigned int symmetric_rates:1;
<<<<<<< HEAD
=======
	unsigned int symmetric_channels:1;
	unsigned int symmetric_samplebits:1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Do not create a PCM for this DAI link (Backend link) */
	unsigned int no_pcm:1;

	/* This DAI link can route to other DAI links at runtime (Frontend)*/
	unsigned int dynamic:1;

<<<<<<< HEAD
	/* pmdown_time is ignored at stop */
	unsigned int ignore_pmdown_time:1;

	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_pcm_runtime *rtd);

	/* optional hw_params re-writing for BE and FE sync */
	int (*be_hw_params_fixup)(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params);

	/* machine stream operations */
	const struct snd_soc_ops *ops;
	const struct snd_soc_compr_ops *compr_ops;
};

struct snd_soc_codec_conf {
	const char *dev_name;
=======
	/* DPCM capture and Playback support */
	unsigned int dpcm_capture:1;
	unsigned int dpcm_playback:1;

	/* DPCM used FE & BE merged format */
	unsigned int dpcm_merged_format:1;

	/* pmdown_time is ignored at stop */
	unsigned int ignore_pmdown_time:1;

	struct list_head list; /* DAI link list of the soc card */
	struct snd_soc_dobj dobj; /* For topology */
};

struct snd_soc_codec_conf {
	/*
	 * specify device either by device name, or by
	 * DT/OF node, but not both.
	 */
	const char *dev_name;
	struct device_node *of_node;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * optional map of kcontrol, widget and path name prefixes that are
	 * associated per device
	 */
	const char *name_prefix;
<<<<<<< HEAD

	/*
	 * set this to the desired compression type if you want to
	 * override the one supplied in codec->driver->compress_type
	 */
	enum snd_soc_compress_type compress_type;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct snd_soc_aux_dev {
	const char *name;		/* Codec name */
<<<<<<< HEAD
	const char *codec_name;		/* for multi-codec */

	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_dapm_context *dapm);
=======

	/*
	 * specify multi-codec either by device name, or by
	 * DT/OF node, but not both.
	 */
	const char *codec_name;
	struct device_node *codec_of_node;

	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_component *component);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* SoC card */
struct snd_soc_card {
	const char *name;
	const char *long_name;
	const char *driver_name;
	struct device *dev;
	struct snd_card *snd_card;
	struct module *owner;

<<<<<<< HEAD
	struct list_head list;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct mutex mutex;
	struct mutex dapm_mutex;

	bool instantiated;

	int (*probe)(struct snd_soc_card *card);
	int (*late_probe)(struct snd_soc_card *card);
	int (*remove)(struct snd_soc_card *card);

	/* the pre and post PM functions are used to do any PM work before and
	 * after the codec and DAI's do any PM work. */
	int (*suspend_pre)(struct snd_soc_card *card);
	int (*suspend_post)(struct snd_soc_card *card);
	int (*resume_pre)(struct snd_soc_card *card);
	int (*resume_post)(struct snd_soc_card *card);

	/* callbacks */
	int (*set_bias_level)(struct snd_soc_card *,
			      struct snd_soc_dapm_context *dapm,
			      enum snd_soc_bias_level level);
	int (*set_bias_level_post)(struct snd_soc_card *,
				   struct snd_soc_dapm_context *dapm,
				   enum snd_soc_bias_level level);

<<<<<<< HEAD
	long pmdown_time;

	/* CPU <--> Codec DAI links  */
	struct snd_soc_dai_link *dai_link;
	int num_links;
	struct snd_soc_pcm_runtime *rtd;
=======
	int (*add_dai_link)(struct snd_soc_card *,
			    struct snd_soc_dai_link *link);
	void (*remove_dai_link)(struct snd_soc_card *,
			    struct snd_soc_dai_link *link);

	long pmdown_time;

	/* CPU <--> Codec DAI links  */
	struct snd_soc_dai_link *dai_link;  /* predefined links only */
	int num_links;  /* predefined links only */
	struct list_head dai_link_list; /* all links */
	int num_dai_links;

	struct list_head rtd_list;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int num_rtd;

	/* optional codec specific configuration */
	struct snd_soc_codec_conf *codec_conf;
	int num_configs;

	/*
	 * optional auxiliary devices such as amplifiers or codecs with DAI
	 * link unused
	 */
	struct snd_soc_aux_dev *aux_dev;
	int num_aux_devs;
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd_aux;
	int num_aux_rtd;
=======
	struct list_head aux_comp_list;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	const struct snd_kcontrol_new *controls;
	int num_controls;

	/*
	 * Card-specific routes and widgets.
<<<<<<< HEAD
=======
	 * Note: of_dapm_xxx for Device Tree; Otherwise for driver build-in.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 */
	const struct snd_soc_dapm_widget *dapm_widgets;
	int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	int num_dapm_routes;
<<<<<<< HEAD
=======
	const struct snd_soc_dapm_widget *of_dapm_widgets;
	int num_of_dapm_widgets;
	const struct snd_soc_dapm_route *of_dapm_routes;
	int num_of_dapm_routes;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bool fully_routed;

	struct work_struct deferred_resume_work;

	/* lists of probed devices belonging to this card */
	struct list_head codec_dev_list;
<<<<<<< HEAD
	struct list_head platform_dev_list;
	struct list_head dai_dev_list;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	struct list_head widgets;
	struct list_head paths;
	struct list_head dapm_list;
	struct list_head dapm_dirty;

<<<<<<< HEAD
	/* Generic DAPM context for the card */
	struct snd_soc_dapm_context dapm;
	struct snd_soc_dapm_stats dapm_stats;
=======
	/* attached dynamic objects */
	struct list_head dobj_list;

	/* Generic DAPM context for the card */
	struct snd_soc_dapm_context dapm;
	struct snd_soc_dapm_stats dapm_stats;
	struct snd_soc_dapm_update *update;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_card_root;
	struct dentry *debugfs_pop_time;
#endif
	u32 pop_time;

	void *drvdata;
};

/* SoC machine DAI configuration, glues a codec and cpu DAI together */
struct snd_soc_pcm_runtime {
	struct device *dev;
	struct snd_soc_card *card;
	struct snd_soc_dai_link *dai_link;
	struct mutex pcm_mutex;
	enum snd_soc_pcm_subclass pcm_subclass;
	struct snd_pcm_ops ops;

	unsigned int dev_registered:1;

	/* Dynamic PCM BE runtime data */
	struct snd_soc_dpcm_runtime dpcm[2];
	int fe_compr;

	long pmdown_time;
	unsigned char pop_wait:1;

	/* runtime devices */
	struct snd_pcm *pcm;
	struct snd_compr *compr;
	struct snd_soc_codec *codec;
	struct snd_soc_platform *platform;
	struct snd_soc_dai *codec_dai;
	struct snd_soc_dai *cpu_dai;
<<<<<<< HEAD
=======
	struct snd_soc_component *component; /* Only valid for AUX dev rtds */

	struct snd_soc_dai **codec_dais;
	unsigned int num_codecs;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	struct delayed_work delayed_work;
#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_dpcm_root;
	struct dentry *debugfs_dpcm_state;
#endif
<<<<<<< HEAD
=======

	unsigned int num; /* 0-based and monotonic increasing */
	struct list_head list; /* rtd list of the soc card */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* mixer control */
struct soc_mixer_control {
	int min, max, platform_max;
<<<<<<< HEAD
	unsigned int reg, rreg, shift, rshift, invert;
=======
	int reg, rreg;
	unsigned int shift, rshift;
	unsigned int sign_bit;
	unsigned int invert:1;
	unsigned int autodisable:1;
	struct snd_soc_dobj dobj;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct soc_bytes {
	int base;
	int num_regs;
	u32 mask;
};

<<<<<<< HEAD
=======
struct soc_bytes_ext {
	int max;
	struct snd_soc_dobj dobj;

	/* used for TLV byte control */
	int (*get)(struct snd_kcontrol *kcontrol, unsigned int __user *bytes,
			unsigned int size);
	int (*put)(struct snd_kcontrol *kcontrol, const unsigned int __user *bytes,
			unsigned int size);
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* multi register control */
struct soc_mreg_control {
	long min, max;
	unsigned int regbase, regcount, nbits, invert;
};

/* enumerated kcontrol */
struct soc_enum {
<<<<<<< HEAD
	unsigned short reg;
	unsigned short reg2;
	unsigned char shift_l;
	unsigned char shift_r;
	unsigned int max;
	unsigned int mask;
	const char * const *texts;
	const unsigned int *values;
};

/* codec IO */
unsigned int snd_soc_read(struct snd_soc_codec *codec, unsigned int reg);
unsigned int snd_soc_write(struct snd_soc_codec *codec,
			   unsigned int reg, unsigned int val);
unsigned int snd_soc_bulk_write_raw(struct snd_soc_codec *codec,
				    unsigned int reg, const void *data, size_t len);
=======
	int reg;
	unsigned char shift_l;
	unsigned char shift_r;
	unsigned int items;
	unsigned int mask;
	const char * const *texts;
	const unsigned int *values;
	unsigned int autodisable:1;
	struct snd_soc_dobj dobj;
};

/**
 * snd_soc_component_to_codec() - Casts a component to the CODEC it is embedded in
 * @component: The component to cast to a CODEC
 *
 * This function must only be used on components that are known to be CODECs.
 * Otherwise the behavior is undefined.
 */
static inline struct snd_soc_codec *snd_soc_component_to_codec(
	struct snd_soc_component *component)
{
	return container_of(component, struct snd_soc_codec, component);
}

/**
 * snd_soc_component_to_platform() - Casts a component to the platform it is embedded in
 * @component: The component to cast to a platform
 *
 * This function must only be used on components that are known to be platforms.
 * Otherwise the behavior is undefined.
 */
static inline struct snd_soc_platform *snd_soc_component_to_platform(
	struct snd_soc_component *component)
{
	return container_of(component, struct snd_soc_platform, component);
}

/**
 * snd_soc_dapm_to_component() - Casts a DAPM context to the component it is
 *  embedded in
 * @dapm: The DAPM context to cast to the component
 *
 * This function must only be used on DAPM contexts that are known to be part of
 * a component (e.g. in a component driver). Otherwise the behavior is
 * undefined.
 */
static inline struct snd_soc_component *snd_soc_dapm_to_component(
	struct snd_soc_dapm_context *dapm)
{
	return container_of(dapm, struct snd_soc_component, dapm);
}

/**
 * snd_soc_dapm_to_codec() - Casts a DAPM context to the CODEC it is embedded in
 * @dapm: The DAPM context to cast to the CODEC
 *
 * This function must only be used on DAPM contexts that are known to be part of
 * a CODEC (e.g. in a CODEC driver). Otherwise the behavior is undefined.
 */
static inline struct snd_soc_codec *snd_soc_dapm_to_codec(
	struct snd_soc_dapm_context *dapm)
{
	return snd_soc_component_to_codec(snd_soc_dapm_to_component(dapm));
}

/**
 * snd_soc_dapm_to_platform() - Casts a DAPM context to the platform it is
 *  embedded in
 * @dapm: The DAPM context to cast to the platform.
 *
 * This function must only be used on DAPM contexts that are known to be part of
 * a platform (e.g. in a platform driver). Otherwise the behavior is undefined.
 */
static inline struct snd_soc_platform *snd_soc_dapm_to_platform(
	struct snd_soc_dapm_context *dapm)
{
	return snd_soc_component_to_platform(snd_soc_dapm_to_component(dapm));
}

/**
 * snd_soc_component_get_dapm() - Returns the DAPM context associated with a
 *  component
 * @component: The component for which to get the DAPM context
 */
static inline struct snd_soc_dapm_context *snd_soc_component_get_dapm(
	struct snd_soc_component *component)
{
	return &component->dapm;
}

/**
 * snd_soc_codec_get_dapm() - Returns the DAPM context for the CODEC
 * @codec: The CODEC for which to get the DAPM context
 *
 * Note: Use this function instead of directly accessing the CODEC's dapm field
 */
static inline struct snd_soc_dapm_context *snd_soc_codec_get_dapm(
	struct snd_soc_codec *codec)
{
	return snd_soc_component_get_dapm(&codec->component);
}

/**
 * snd_soc_dapm_init_bias_level() - Initialize CODEC DAPM bias level
 * @codec: The CODEC for which to initialize the DAPM bias level
 * @level: The DAPM level to initialize to
 *
 * Initializes the CODEC DAPM bias level. See snd_soc_dapm_init_bias_level().
 */
static inline void snd_soc_codec_init_bias_level(struct snd_soc_codec *codec,
	enum snd_soc_bias_level level)
{
	snd_soc_dapm_init_bias_level(snd_soc_codec_get_dapm(codec), level);
}

/**
 * snd_soc_dapm_get_bias_level() - Get current CODEC DAPM bias level
 * @codec: The CODEC for which to get the DAPM bias level
 *
 * Returns: The current DAPM bias level of the CODEC.
 */
static inline enum snd_soc_bias_level snd_soc_codec_get_bias_level(
	struct snd_soc_codec *codec)
{
	return snd_soc_dapm_get_bias_level(snd_soc_codec_get_dapm(codec));
}

/**
 * snd_soc_codec_force_bias_level() - Set the CODEC DAPM bias level
 * @codec: The CODEC for which to set the level
 * @level: The level to set to
 *
 * Forces the CODEC bias level to a specific state. See
 * snd_soc_dapm_force_bias_level().
 */
static inline int snd_soc_codec_force_bias_level(struct snd_soc_codec *codec,
	enum snd_soc_bias_level level)
{
	return snd_soc_dapm_force_bias_level(snd_soc_codec_get_dapm(codec),
		level);
}

/**
 * snd_soc_dapm_kcontrol_codec() - Returns the codec associated to a kcontrol
 * @kcontrol: The kcontrol
 *
 * This function must only be used on DAPM contexts that are known to be part of
 * a CODEC (e.g. in a CODEC driver). Otherwise the behavior is undefined.
 */
static inline struct snd_soc_codec *snd_soc_dapm_kcontrol_codec(
	struct snd_kcontrol *kcontrol)
{
	return snd_soc_dapm_to_codec(snd_soc_dapm_kcontrol_dapm(kcontrol));
}

/* codec IO */
unsigned int snd_soc_read(struct snd_soc_codec *codec, unsigned int reg);
int snd_soc_write(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int val);

/**
 * snd_soc_cache_sync() - Sync the register cache with the hardware
 * @codec: CODEC to sync
 *
 * Note: This function will call regcache_sync()
 */
static inline int snd_soc_cache_sync(struct snd_soc_codec *codec)
{
	return regcache_sync(codec->component.regmap);
}

/* component IO */
int snd_soc_component_read(struct snd_soc_component *component,
	unsigned int reg, unsigned int *val);
int snd_soc_component_write(struct snd_soc_component *component,
	unsigned int reg, unsigned int val);
int snd_soc_component_update_bits(struct snd_soc_component *component,
	unsigned int reg, unsigned int mask, unsigned int val);
int snd_soc_component_update_bits_async(struct snd_soc_component *component,
	unsigned int reg, unsigned int mask, unsigned int val);
void snd_soc_component_async_complete(struct snd_soc_component *component);
int snd_soc_component_test_bits(struct snd_soc_component *component,
	unsigned int reg, unsigned int mask, unsigned int value);

#ifdef CONFIG_REGMAP

void snd_soc_component_init_regmap(struct snd_soc_component *component,
	struct regmap *regmap);
void snd_soc_component_exit_regmap(struct snd_soc_component *component);

/**
 * snd_soc_codec_init_regmap() - Initialize regmap instance for the CODEC
 * @codec: The CODEC for which to initialize the regmap instance
 * @regmap: The regmap instance that should be used by the CODEC
 *
 * This function allows deferred assignment of the regmap instance that is
 * associated with the CODEC. Only use this if the regmap instance is not yet
 * ready when the CODEC is registered. The function must also be called before
 * the first IO attempt of the CODEC.
 */
static inline void snd_soc_codec_init_regmap(struct snd_soc_codec *codec,
	struct regmap *regmap)
{
	snd_soc_component_init_regmap(&codec->component, regmap);
}

/**
 * snd_soc_codec_exit_regmap() - De-initialize regmap instance for the CODEC
 * @codec: The CODEC for which to de-initialize the regmap instance
 *
 * Calls regmap_exit() on the regmap instance associated to the CODEC and
 * removes the regmap instance from the CODEC.
 *
 * This function should only be used if snd_soc_codec_init_regmap() was used to
 * initialize the regmap instance.
 */
static inline void snd_soc_codec_exit_regmap(struct snd_soc_codec *codec)
{
	snd_soc_component_exit_regmap(&codec->component);
}

#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* device driver data */

static inline void snd_soc_card_set_drvdata(struct snd_soc_card *card,
		void *data)
{
	card->drvdata = data;
}

static inline void *snd_soc_card_get_drvdata(struct snd_soc_card *card)
{
	return card->drvdata;
}

<<<<<<< HEAD
static inline void snd_soc_codec_set_drvdata(struct snd_soc_codec *codec,
		void *data)
{
	dev_set_drvdata(codec->dev, data);
}

static inline void *snd_soc_codec_get_drvdata(struct snd_soc_codec *codec)
{
	return dev_get_drvdata(codec->dev);
}

static inline void snd_soc_platform_set_drvdata(struct snd_soc_platform *platform,
		void *data)
{
	dev_set_drvdata(platform->dev, data);
}

static inline void *snd_soc_platform_get_drvdata(struct snd_soc_platform *platform)
{
	return dev_get_drvdata(platform->dev);
}

static inline void snd_soc_pcm_set_drvdata(struct snd_soc_pcm_runtime *rtd,
		void *data)
{
	dev_set_drvdata(rtd->dev, data);
}

static inline void *snd_soc_pcm_get_drvdata(struct snd_soc_pcm_runtime *rtd)
{
	return dev_get_drvdata(rtd->dev);
=======
static inline void snd_soc_component_set_drvdata(struct snd_soc_component *c,
		void *data)
{
	dev_set_drvdata(c->dev, data);
}

static inline void *snd_soc_component_get_drvdata(struct snd_soc_component *c)
{
	return dev_get_drvdata(c->dev);
}

static inline void snd_soc_codec_set_drvdata(struct snd_soc_codec *codec,
		void *data)
{
	snd_soc_component_set_drvdata(&codec->component, data);
}

static inline void *snd_soc_codec_get_drvdata(struct snd_soc_codec *codec)
{
	return snd_soc_component_get_drvdata(&codec->component);
}

static inline void snd_soc_platform_set_drvdata(struct snd_soc_platform *platform,
		void *data)
{
	snd_soc_component_set_drvdata(&platform->component, data);
}

static inline void *snd_soc_platform_get_drvdata(struct snd_soc_platform *platform)
{
	return snd_soc_component_get_drvdata(&platform->component);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void snd_soc_initialize_card_lists(struct snd_soc_card *card)
{
<<<<<<< HEAD
	INIT_LIST_HEAD(&card->dai_dev_list);
	INIT_LIST_HEAD(&card->codec_dev_list);
	INIT_LIST_HEAD(&card->platform_dev_list);
	INIT_LIST_HEAD(&card->widgets);
	INIT_LIST_HEAD(&card->paths);
	INIT_LIST_HEAD(&card->dapm_list);
=======
	INIT_LIST_HEAD(&card->codec_dev_list);
	INIT_LIST_HEAD(&card->widgets);
	INIT_LIST_HEAD(&card->paths);
	INIT_LIST_HEAD(&card->dapm_list);
	INIT_LIST_HEAD(&card->aux_comp_list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline bool snd_soc_volsw_is_stereo(struct soc_mixer_control *mc)
{
	if (mc->reg == mc->rreg && mc->shift == mc->rshift)
		return 0;
	/*
	 * mc->reg == mc->rreg && mc->shift != mc->rshift, or
	 * mc->reg != mc->rreg means that the control is
	 * stereo (bits in one register or in two registers)
	 */
	return 1;
}

<<<<<<< HEAD
=======
static inline unsigned int snd_soc_enum_val_to_item(struct soc_enum *e,
	unsigned int val)
{
	unsigned int i;

	if (!e->values)
		return val;

	for (i = 0; i < e->items; i++)
		if (val == e->values[i])
			return i;

	return 0;
}

static inline unsigned int snd_soc_enum_item_to_val(struct soc_enum *e,
	unsigned int item)
{
	if (!e->values)
		return item;

	return e->values[item];
}

static inline bool snd_soc_component_is_active(
	struct snd_soc_component *component)
{
	return component->active != 0;
}

static inline bool snd_soc_codec_is_active(struct snd_soc_codec *codec)
{
	return snd_soc_component_is_active(&codec->component);
}

/**
 * snd_soc_kcontrol_component() - Returns the component that registered the
 *  control
 * @kcontrol: The control for which to get the component
 *
 * Note: This function will work correctly if the control has been registered
 * for a component. Either with snd_soc_add_codec_controls() or
 * snd_soc_add_platform_controls() or via  table based setup for either a
 * CODEC, a platform or component driver. Otherwise the behavior is undefined.
 */
static inline struct snd_soc_component *snd_soc_kcontrol_component(
	struct snd_kcontrol *kcontrol)
{
	return snd_kcontrol_chip(kcontrol);
}

/**
 * snd_soc_kcontrol_codec() - Returns the CODEC that registered the control
 * @kcontrol: The control for which to get the CODEC
 *
 * Note: This function will only work correctly if the control has been
 * registered with snd_soc_add_codec_controls() or via table based setup of
 * snd_soc_codec_driver. Otherwise the behavior is undefined.
 */
static inline struct snd_soc_codec *snd_soc_kcontrol_codec(
	struct snd_kcontrol *kcontrol)
{
	return snd_soc_component_to_codec(snd_soc_kcontrol_component(kcontrol));
}

/**
 * snd_soc_kcontrol_platform() - Returns the platform that registered the control
 * @kcontrol: The control for which to get the platform
 *
 * Note: This function will only work correctly if the control has been
 * registered with snd_soc_add_platform_controls() or via table based setup of
 * a snd_soc_platform_driver. Otherwise the behavior is undefined.
 */
static inline struct snd_soc_platform *snd_soc_kcontrol_platform(
	struct snd_kcontrol *kcontrol)
{
	return snd_soc_component_to_platform(snd_soc_kcontrol_component(kcontrol));
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int snd_soc_util_init(void);
void snd_soc_util_exit(void);

int snd_soc_of_parse_card_name(struct snd_soc_card *card,
			       const char *propname);
<<<<<<< HEAD
int snd_soc_of_parse_audio_routing(struct snd_soc_card *card,
				   const char *propname);
unsigned int snd_soc_of_parse_daifmt(struct device_node *np,
				     const char *prefix);
=======
int snd_soc_of_parse_audio_simple_widgets(struct snd_soc_card *card,
					  const char *propname);
int snd_soc_of_parse_tdm_slot(struct device_node *np,
			      unsigned int *tx_mask,
			      unsigned int *rx_mask,
			      unsigned int *slots,
			      unsigned int *slot_width);
void snd_soc_of_parse_audio_prefix(struct snd_soc_card *card,
				   struct snd_soc_codec_conf *codec_conf,
				   struct device_node *of_node,
				   const char *propname);
int snd_soc_of_parse_audio_routing(struct snd_soc_card *card,
				   const char *propname);
unsigned int snd_soc_of_parse_daifmt(struct device_node *np,
				     const char *prefix,
				     struct device_node **bitclkmaster,
				     struct device_node **framemaster);
int snd_soc_of_get_dai_name(struct device_node *of_node,
			    const char **dai_name);
int snd_soc_of_get_dai_link_codecs(struct device *dev,
				   struct device_node *of_node,
				   struct snd_soc_dai_link *dai_link);

int snd_soc_add_dai_link(struct snd_soc_card *card,
				struct snd_soc_dai_link *dai_link);
void snd_soc_remove_dai_link(struct snd_soc_card *card,
			     struct snd_soc_dai_link *dai_link);

int snd_soc_register_dai(struct snd_soc_component *component,
	struct snd_soc_dai_driver *dai_drv);

struct snd_soc_dai *snd_soc_find_dai(
	const struct snd_soc_dai_link_component *dlc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <sound/soc-dai.h>

#ifdef CONFIG_DEBUG_FS
extern struct dentry *snd_soc_debugfs_root;
#endif

extern const struct dev_pm_ops snd_soc_pm_ops;

<<<<<<< HEAD
=======
/* Helper functions */
static inline void snd_soc_dapm_mutex_lock(struct snd_soc_dapm_context *dapm)
{
	mutex_lock_nested(&dapm->card->dapm_mutex, SND_SOC_DAPM_CLASS_RUNTIME);
}

static inline void snd_soc_dapm_mutex_unlock(struct snd_soc_dapm_context *dapm)
{
	mutex_unlock(&dapm->card->dapm_mutex);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
