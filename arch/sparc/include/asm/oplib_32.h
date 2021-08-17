/*
 * oplib.h:  Describes the interface and available routines in the
 *           Linux Prom library.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef __SPARC_OPLIB_H
#define __SPARC_OPLIB_H

#include <asm/openprom.h>
#include <linux/spinlock.h>
#include <linux/compiler.h>

/* The master romvec pointer... */
extern struct linux_romvec *romvec;

/* Enumeration to describe the prom major version we have detected. */
enum prom_major_version {
	PROM_V0,      /* Original sun4c V0 prom */
	PROM_V2,      /* sun4c and early sun4m V2 prom */
	PROM_V3,      /* sun4m and later, up to sun4d/sun4e machines V3 */
	PROM_P1275,   /* IEEE compliant ISA based Sun PROM, only sun4u */
};

extern enum prom_major_version prom_vers;
/* Revision, and firmware revision. */
extern unsigned int prom_rev, prom_prev;

/* Root node of the prom device tree, this stays constant after
 * initialization is complete.
 */
extern phandle prom_root_node;

/* Pointer to prom structure containing the device tree traversal
 * and usage utility functions.  Only prom-lib should use these,
 * users use the interface defined by the library only!
 */
extern struct linux_nodeops *prom_nodeops;

/* The functions... */

/* You must call prom_init() before using any of the library services,
 * preferably as early as possible.  Pass it the romvec pointer.
 */
<<<<<<< HEAD
extern void prom_init(struct linux_romvec *rom_ptr);

/* Boot argument acquisition, returns the boot command line string. */
extern char *prom_getbootargs(void);
=======
void prom_init(struct linux_romvec *rom_ptr);

/* Boot argument acquisition, returns the boot command line string. */
char *prom_getbootargs(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Miscellaneous routines, don't really fit in any category per se. */

/* Reboot the machine with the command line passed. */
<<<<<<< HEAD
extern void prom_reboot(char *boot_command);

/* Evaluate the forth string passed. */
extern void prom_feval(char *forth_string);
=======
void prom_reboot(char *boot_command);

/* Evaluate the forth string passed. */
void prom_feval(char *forth_string);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Enter the prom, with possibility of continuation with the 'go'
 * command in newer proms.
 */
<<<<<<< HEAD
extern void prom_cmdline(void);
=======
void prom_cmdline(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Enter the prom, with no chance of continuation for the stand-alone
 * which calls this.
 */
<<<<<<< HEAD
extern void __noreturn prom_halt(void);
=======
void __noreturn prom_halt(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Set the PROM 'sync' callback function to the passed function pointer.
 * When the user gives the 'sync' command at the prom prompt while the
 * kernel is still active, the prom will call this routine.
 *
 * XXX The arguments are different on V0 vs. V2->higher proms, grrr! XXX
 */
typedef void (*sync_func_t)(void);
<<<<<<< HEAD
extern void prom_setsync(sync_func_t func_ptr);
=======
void prom_setsync(sync_func_t func_ptr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Acquire the IDPROM of the root node in the prom device tree.  This
 * gets passed a buffer where you would like it stuffed.  The return value
 * is the format type of this idprom or 0xff on error.
 */
<<<<<<< HEAD
extern unsigned char prom_get_idprom(char *idp_buffer, int idpbuf_size);

/* Get the prom major version. */
extern int prom_version(void);

/* Get the prom plugin revision. */
extern int prom_getrev(void);

/* Get the prom firmware revision. */
extern int prom_getprev(void);

/* Write a buffer of characters to the console. */
extern void prom_console_write_buf(const char *buf, int len);

/* Prom's internal routines, don't use in kernel/boot code. */
extern __printf(1, 2) void prom_printf(const char *fmt, ...);
extern void prom_write(const char *buf, unsigned int len);
=======
unsigned char prom_get_idprom(char *idp_buffer, int idpbuf_size);

/* Get the prom major version. */
int prom_version(void);

/* Get the prom plugin revision. */
int prom_getrev(void);

/* Get the prom firmware revision. */
int prom_getprev(void);

/* Write a buffer of characters to the console. */
void prom_console_write_buf(const char *buf, int len);

/* Prom's internal routines, don't use in kernel/boot code. */
__printf(1, 2) void prom_printf(const char *fmt, ...);
void prom_write(const char *buf, unsigned int len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Multiprocessor operations... */

/* Start the CPU with the given device tree node, context table, and context
 * at the passed program counter.
 */
<<<<<<< HEAD
extern int prom_startcpu(int cpunode, struct linux_prom_registers *context_table,
			 int context, char *program_counter);
=======
int prom_startcpu(int cpunode, struct linux_prom_registers *context_table,
		  int context, char *program_counter);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Initialize the memory lists based upon the prom version. */
void prom_meminit(void);

/* PROM device tree traversal functions... */

/* Get the child node of the given node, or zero if no child exists. */
<<<<<<< HEAD
extern phandle prom_getchild(phandle parent_node);
=======
phandle prom_getchild(phandle parent_node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Get the next sibling node of the given node, or zero if no further
 * siblings exist.
 */
<<<<<<< HEAD
extern phandle prom_getsibling(phandle node);
=======
phandle prom_getsibling(phandle node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Get the length, at the passed node, of the given property type.
 * Returns -1 on error (ie. no such property at this node).
 */
<<<<<<< HEAD
extern int prom_getproplen(phandle thisnode, const char *property);
=======
int prom_getproplen(phandle thisnode, const char *property);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Fetch the requested property using the given buffer.  Returns
 * the number of bytes the prom put into your buffer or -1 on error.
 */
<<<<<<< HEAD
extern int __must_check prom_getproperty(phandle thisnode, const char *property,
					 char *prop_buffer, int propbuf_size);

/* Acquire an integer property. */
extern int prom_getint(phandle node, char *property);

/* Acquire an integer property, with a default value. */
extern int prom_getintdefault(phandle node, char *property, int defval);

/* Acquire a boolean property, 0=FALSE 1=TRUE. */
extern int prom_getbool(phandle node, char *prop);

/* Acquire a string property, null string on error. */
extern void prom_getstring(phandle node, char *prop, char *buf, int bufsize);
=======
int __must_check prom_getproperty(phandle thisnode, const char *property,
				  char *prop_buffer, int propbuf_size);

/* Acquire an integer property. */
int prom_getint(phandle node, char *property);

/* Acquire an integer property, with a default value. */
int prom_getintdefault(phandle node, char *property, int defval);

/* Acquire a boolean property, 0=FALSE 1=TRUE. */
int prom_getbool(phandle node, char *prop);

/* Acquire a string property, null string on error. */
void prom_getstring(phandle node, char *prop, char *buf, int bufsize);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Search all siblings starting at the passed node for "name" matching
 * the given string.  Returns the node on success, zero on failure.
 */
<<<<<<< HEAD
extern phandle prom_searchsiblings(phandle node_start, char *name);
=======
phandle prom_searchsiblings(phandle node_start, char *name);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Returns the next property after the passed property for the given
 * node.  Returns null string on failure.
 */
<<<<<<< HEAD
extern char *prom_nextprop(phandle node, char *prev_property, char *buffer);

/* Returns phandle of the path specified */
extern phandle prom_finddevice(char *name);
=======
char *prom_nextprop(phandle node, char *prev_property, char *buffer);

/* Returns phandle of the path specified */
phandle prom_finddevice(char *name);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Set the indicated property at the given node with the passed value.
 * Returns the number of bytes of your value that the prom took.
 */
<<<<<<< HEAD
extern int prom_setprop(phandle node, const char *prop_name, char *prop_value,
			int value_size);

extern phandle prom_inst2pkg(int);
=======
int prom_setprop(phandle node, const char *prop_name, char *prop_value,
		 int value_size);

phandle prom_inst2pkg(int);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Dorking with Bus ranges... */

/* Apply promlib probes OBIO ranges to registers. */
<<<<<<< HEAD
extern void prom_apply_obio_ranges(struct linux_prom_registers *obioregs, int nregs);

/* Apply ranges of any prom node (and optionally parent node as well) to registers. */
extern void prom_apply_generic_ranges(phandle node, phandle parent,
				      struct linux_prom_registers *sbusregs, int nregs);
=======
void prom_apply_obio_ranges(struct linux_prom_registers *obioregs, int nregs);

/* Apply ranges of any prom node (and optionally parent node as well) to registers. */
void prom_apply_generic_ranges(phandle node, phandle parent,
			       struct linux_prom_registers *sbusregs, int nregs);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void prom_ranges_init(void);

/* CPU probing helpers.  */
int cpu_find_by_instance(int instance, phandle *prom_node, int *mid);
int cpu_find_by_mid(int mid, phandle *prom_node);
int cpu_get_hwmid(phandle prom_node);

extern spinlock_t prom_lock;

#endif /* !(__SPARC_OPLIB_H) */
