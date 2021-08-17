#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "debugfs.h"

static struct dentry *d_xen_debug;

struct dentry * __init xen_init_debugfs(void)
{
	if (!d_xen_debug) {
		d_xen_debug = debugfs_create_dir("xen", NULL);

		if (!d_xen_debug)
			pr_warning("Could not create 'xen' debugfs directory\n");
	}

	return d_xen_debug;
}

