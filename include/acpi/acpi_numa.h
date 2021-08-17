#ifndef __ACPI_NUMA_H
#define __ACPI_NUMA_H

#ifdef CONFIG_ACPI_NUMA
#include <linux/kernel.h>

/* Proximity bitmap length */
#if MAX_NUMNODES > 256
#define MAX_PXM_DOMAINS MAX_NUMNODES
#else
#define MAX_PXM_DOMAINS (256)	/* Old pxm spec is defined 8 bit */
#endif

extern int pxm_to_node(int);
extern int node_to_pxm(int);
<<<<<<< HEAD
extern void __acpi_map_pxm_to_node(int, int);
extern int acpi_map_pxm_to_node(int);
extern unsigned char acpi_srat_revision;
=======
extern int acpi_map_pxm_to_node(int);
extern unsigned char acpi_srat_revision;
extern int acpi_numa __initdata;

extern void bad_srat(void);
extern int srat_disabled(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif				/* CONFIG_ACPI_NUMA */
#endif				/* __ACP_NUMA_H */
