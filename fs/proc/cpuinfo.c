#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

extern const struct seq_operations cpuinfo_op;
static int cpuinfo_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &cpuinfo_op);
}

static const struct file_operations proc_cpuinfo_operations = {
	.open		= cpuinfo_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static int __init proc_cpuinfo_init(void)
{
	proc_create("cpuinfo", 0, NULL, &proc_cpuinfo_operations);
	return 0;
}
<<<<<<< HEAD
module_init(proc_cpuinfo_init);
=======
fs_initcall(proc_cpuinfo_init);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
