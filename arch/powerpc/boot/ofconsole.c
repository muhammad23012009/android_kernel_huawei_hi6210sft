/*
 * OF console routines
 *
 * Copyright (C) Paul Mackerras 1997.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <stddef.h>
#include "types.h"
#include "elf.h"
#include "string.h"
#include "stdio.h"
#include "page.h"
#include "ops.h"

#include "of.h"

<<<<<<< HEAD
static void *of_stdout_handle;
=======
static unsigned int of_stdout_handle;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int of_console_open(void)
{
	void *devp;

	if (((devp = of_finddevice("/chosen")) != NULL)
	    && (of_getprop(devp, "stdout", &of_stdout_handle,
			   sizeof(of_stdout_handle))
<<<<<<< HEAD
		== sizeof(of_stdout_handle)))
		return 0;
=======
		== sizeof(of_stdout_handle))) {
		of_stdout_handle = be32_to_cpu(of_stdout_handle);
		return 0;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return -1;
}

static void of_console_write(const char *buf, int len)
{
	of_call_prom("write", 3, 1, of_stdout_handle, buf, len);
}

void of_console_init(void)
{
	console_ops.open = of_console_open;
	console_ops.write = of_console_write;
}
