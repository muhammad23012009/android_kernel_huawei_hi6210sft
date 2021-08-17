/*
 * AppArmor security module
 *
 * This file contains AppArmor security identifier (sid) definitions
 *
 * Copyright 2009-2010 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 */

#ifndef __AA_SID_H
#define __AA_SID_H

#include <linux/types.h>

<<<<<<< HEAD
struct aa_profile;
=======
/* sid value that will not be allocated */
#define AA_SID_INVALID 0
#define AA_SID_ALLOC AA_SID_INVALID
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

u32 aa_alloc_sid(void);
void aa_free_sid(u32 sid);

#endif /* __AA_SID_H */
