/*
 * Copyright 2010 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */

<<<<<<< HEAD
#if __tile_chip__ == 0
#include <arch/chip_tile64.h>
#elif __tile_chip__ == 1
=======
#if __tile_chip__ == 1
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <arch/chip_tilepro.h>
#elif defined(__tilegx__)
#include <arch/chip_tilegx.h>
#else
#error Unexpected Tilera chip type
#endif
