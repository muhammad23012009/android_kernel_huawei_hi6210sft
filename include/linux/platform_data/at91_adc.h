/*
 * Copyright (C) 2011 Free Electrons
 *
 * Licensed under the GPLv2 or later.
 */

#ifndef _AT91_ADC_H_
#define _AT91_ADC_H_

<<<<<<< HEAD
/**
 * struct at91_adc_reg_desc - Various informations relative to registers
 * @channel_base:	Base offset for the channel data registers
 * @drdy_mask:		Mask of the DRDY field in the relevant registers
			(Interruptions registers mostly)
 * @status_register:	Offset of the Interrupt Status Register
 * @trigger_register:	Offset of the Trigger setup register
 */
struct at91_adc_reg_desc {
	u8	channel_base;
	u32	drdy_mask;
	u8	status_register;
	u8	trigger_register;
=======
enum atmel_adc_ts_type {
	ATMEL_ADC_TOUCHSCREEN_NONE = 0,
	ATMEL_ADC_TOUCHSCREEN_4WIRE = 4,
	ATMEL_ADC_TOUCHSCREEN_5WIRE = 5,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/**
 * struct at91_adc_trigger - description of triggers
 * @name:		name of the trigger advertised to the user
 * @value:		value to set in the ADC's trigger setup register
			to enable the trigger
 * @is_external:	Does the trigger rely on an external pin?
 */
struct at91_adc_trigger {
	const char	*name;
	u8		value;
	bool		is_external;
};

/**
 * struct at91_adc_data - platform data for ADC driver
 * @channels_used:		channels in use on the board as a bitmask
<<<<<<< HEAD
 * @num_channels:		global number of channels available on the board
 * @registers:			Registers definition on the board
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @startup_time:		startup time of the ADC in microseconds
 * @trigger_list:		Triggers available in the ADC
 * @trigger_number:		Number of triggers available in the ADC
 * @use_external_triggers:	does the board has external triggers availables
 * @vref:			Reference voltage for the ADC in millivolts
<<<<<<< HEAD
 */
struct at91_adc_data {
	unsigned long			channels_used;
	u8				num_channels;
	struct at91_adc_reg_desc	*registers;
=======
 * @touchscreen_type:		If a touchscreen is connected, its type (4 or 5 wires)
 */
struct at91_adc_data {
	unsigned long			channels_used;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u8				startup_time;
	struct at91_adc_trigger		*trigger_list;
	u8				trigger_number;
	bool				use_external_triggers;
	u16				vref;
<<<<<<< HEAD
=======
	enum atmel_adc_ts_type		touchscreen_type;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

extern void __init at91_add_device_adc(struct at91_adc_data *data);
#endif
