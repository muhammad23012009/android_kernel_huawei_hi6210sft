/*
 * BCH Error Location Module
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef __ELM_H
#define __ELM_H

enum bch_ecc {
	BCH4_ECC = 0,
	BCH8_ECC,
<<<<<<< HEAD
=======
	BCH16_ECC,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* ELM support 8 error syndrome process */
#define ERROR_VECTOR_MAX		8

<<<<<<< HEAD
#define BCH8_ECC_OOB_BYTES		13
#define BCH4_ECC_OOB_BYTES		7
/* RBL requires 14 byte even though BCH8 uses only 13 byte */
#define BCH8_SIZE			(BCH8_ECC_OOB_BYTES + 1)
/* Uses 1 extra byte to handle erased pages */
#define BCH4_SIZE			(BCH4_ECC_OOB_BYTES + 1)

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * struct elm_errorvec - error vector for elm
 * @error_reported:		set true for vectors error is reported
 * @error_uncorrectable:	number of uncorrectable errors
 * @error_count:		number of correctable errors in the sector
 * @error_loc:			buffer for error location
 *
 */
struct elm_errorvec {
	bool error_reported;
	bool error_uncorrectable;
	int error_count;
<<<<<<< HEAD
	int error_loc[ERROR_VECTOR_MAX];
};

void elm_decode_bch_error_page(struct device *dev, u8 *ecc_calc,
		struct elm_errorvec *err_vec);
int elm_config(struct device *dev, enum bch_ecc bch_type);
=======
	int error_loc[16];
};

#if IS_ENABLED(CONFIG_MTD_NAND_OMAP_BCH)
void elm_decode_bch_error_page(struct device *dev, u8 *ecc_calc,
		struct elm_errorvec *err_vec);
int elm_config(struct device *dev, enum bch_ecc bch_type,
	int ecc_steps, int ecc_step_size, int ecc_syndrome_size);
#else
static inline void
elm_decode_bch_error_page(struct device *dev, u8 *ecc_calc,
			  struct elm_errorvec *err_vec)
{
}

static inline int elm_config(struct device *dev, enum bch_ecc bch_type,
			     int ecc_steps, int ecc_step_size,
			     int ecc_syndrome_size)
{
	return -ENOSYS;
}
#endif /* CONFIG_MTD_NAND_ECC_BCH */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __ELM_H */
