#ifndef ACPI_BUTTON_H
#define ACPI_BUTTON_H

#include <linux/notifier.h>

<<<<<<< HEAD
#if defined(CONFIG_ACPI_BUTTON) || defined(CONFIG_ACPI_BUTTON_MODULE)
=======
#if IS_ENABLED(CONFIG_ACPI_BUTTON)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int acpi_lid_notifier_register(struct notifier_block *nb);
extern int acpi_lid_notifier_unregister(struct notifier_block *nb);
extern int acpi_lid_open(void);
#else
static inline int acpi_lid_notifier_register(struct notifier_block *nb)
{
	return 0;
}
static inline int acpi_lid_notifier_unregister(struct notifier_block *nb)
{
	return 0;
}
static inline int acpi_lid_open(void)
{
	return 1;
}
<<<<<<< HEAD
#endif /* defined(CONFIG_ACPI_BUTTON) || defined(CONFIG_ACPI_BUTTON_MODULE) */
=======
#endif /* IS_ENABLED(CONFIG_ACPI_BUTTON) */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* ACPI_BUTTON_H */
