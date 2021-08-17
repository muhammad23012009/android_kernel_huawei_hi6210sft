/*
 * Copyright (C) 2003 Christoph Hellwig.
 *	Released under GPL v2.
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sysctl.h>

#include "scsi_logging.h"
#include "scsi_priv.h"


<<<<<<< HEAD
static ctl_table scsi_table[] = {
=======
static struct ctl_table scsi_table[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ .procname	= "logging_level",
	  .data		= &scsi_logging_level,
	  .maxlen	= sizeof(scsi_logging_level),
	  .mode		= 0644,
	  .proc_handler	= proc_dointvec },
	{ }
};

<<<<<<< HEAD
static ctl_table scsi_dir_table[] = {
=======
static struct ctl_table scsi_dir_table[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ .procname	= "scsi",
	  .mode		= 0555,
	  .child	= scsi_table },
	{ }
};

<<<<<<< HEAD
static ctl_table scsi_root_table[] = {
=======
static struct ctl_table scsi_root_table[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ .procname	= "dev",
	  .mode		= 0555,
	  .child	= scsi_dir_table },
	{ }
};

static struct ctl_table_header *scsi_table_header;

int __init scsi_init_sysctl(void)
{
	scsi_table_header = register_sysctl_table(scsi_root_table);
	if (!scsi_table_header)
		return -ENOMEM;
	return 0;
}

void scsi_exit_sysctl(void)
{
	unregister_sysctl_table(scsi_table_header);
}
