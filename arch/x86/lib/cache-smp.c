#include <linux/smp.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void __wbinvd(void *dummy)
{
	wbinvd();
}

void wbinvd_on_cpu(int cpu)
{
	smp_call_function_single(cpu, __wbinvd, NULL, 1);
}
EXPORT_SYMBOL(wbinvd_on_cpu);

int wbinvd_on_all_cpus(void)
{
	return on_each_cpu(__wbinvd, NULL, 1);
}
EXPORT_SYMBOL(wbinvd_on_all_cpus);
