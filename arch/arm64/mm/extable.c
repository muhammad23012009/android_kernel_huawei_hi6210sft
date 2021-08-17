/*
 * Based on arch/arm/mm/extable.c
 */

<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/extable.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/uaccess.h>

int fixup_exception(struct pt_regs *regs)
{
	const struct exception_table_entry *fixup;

	fixup = search_exception_tables(instruction_pointer(regs));
	if (fixup)
<<<<<<< HEAD
		regs->pc = fixup->fixup;
=======
		regs->pc = (unsigned long)&fixup->fixup + fixup->fixup;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return fixup != NULL;
}
