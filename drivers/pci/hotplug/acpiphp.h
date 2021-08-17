/*
 * ACPI PCI Hot Plug Controller Driver
 *
 * Copyright (C) 1995,2001 Compaq Computer Corporation
 * Copyright (C) 2001 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (C) 2001 IBM Corp.
 * Copyright (C) 2002 Hiroshi Aono (h-aono@ap.jp.nec.com)
 * Copyright (C) 2002,2003 Takayoshi Kochi (t-kochi@bq.jp.nec.com)
 * Copyright (C) 2002,2003 NEC Corporation
 * Copyright (C) 2003-2005 Matthew Wilcox (matthew.wilcox@hp.com)
 * Copyright (C) 2003-2005 Hewlett Packard
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Send feedback to <gregkh@us.ibm.com>,
 *		    <t-kochi@bq.jp.nec.com>
 *
 */

#ifndef _ACPIPHP_H
#define _ACPIPHP_H

#include <linux/acpi.h>
#include <linux/mutex.h>
#include <linux/pci_hotplug.h>

<<<<<<< HEAD
#define dbg(format, arg...)					\
	do {							\
		if (acpiphp_debug)				\
			printk(KERN_DEBUG "%s: " format,	\
				MY_NAME , ## arg);		\
	} while (0)
#define err(format, arg...) printk(KERN_ERR "%s: " format, MY_NAME , ## arg)
#define info(format, arg...) printk(KERN_INFO "%s: " format, MY_NAME , ## arg)
#define warn(format, arg...) printk(KERN_WARNING "%s: " format, MY_NAME , ## arg)

=======
struct acpiphp_context;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct acpiphp_bridge;
struct acpiphp_slot;

/*
 * struct slot - slot information for each *physical* slot
 */
struct slot {
	struct hotplug_slot	*hotplug_slot;
	struct acpiphp_slot	*acpi_slot;
	struct hotplug_slot_info info;
<<<<<<< HEAD
=======
	unsigned int sun;	/* ACPI _SUN (Slot User Number) value */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static inline const char *slot_name(struct slot *slot)
{
	return hotplug_slot_name(slot->hotplug_slot);
}

/*
 * struct acpiphp_bridge - PCI bridge information
 *
 * for each bridge device in ACPI namespace
 */
struct acpiphp_bridge {
	struct list_head list;
	struct list_head slots;
	struct kref ref;
<<<<<<< HEAD
	acpi_handle handle;

	/* Ejectable PCI-to-PCI bridge (PCI bridge and PCI function) */
	struct acpiphp_func *func;

	int nr_slots;

	u32 flags;

=======

	struct acpiphp_context *context;

	int nr_slots;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* This bus (host bridge) or Secondary bus (PCI-to-PCI bridge) */
	struct pci_bus *pci_bus;

	/* PCI-to-PCI bridge device */
	struct pci_dev *pci_dev;
<<<<<<< HEAD
=======

	bool is_going_away;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};


/*
 * struct acpiphp_slot - PCI slot information
 *
 * PCI slot information for each *physical* PCI slot
 */
struct acpiphp_slot {
	struct list_head node;
<<<<<<< HEAD
	struct acpiphp_bridge *bridge;	/* parent */
	struct list_head funcs;		/* one slot may have different
					   objects (i.e. for each function) */
	struct slot *slot;
	struct mutex crit_sect;

	u8		device;		/* pci device# */

	unsigned long long sun;		/* ACPI _SUN (slot unique number) */
=======
	struct pci_bus *bus;
	struct list_head funcs;		/* one slot may have different
					   objects (i.e. for each function) */
	struct slot *slot;

	u8		device;		/* pci device# */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32		flags;		/* see below */
};


/*
 * struct acpiphp_func - PCI function information
 *
 * PCI function information for each object in ACPI namespace
 * typically 8 objects per slot (i.e. for each PCI function)
 */
struct acpiphp_func {
<<<<<<< HEAD
	struct acpiphp_slot *slot;	/* parent */

	struct list_head sibling;
	struct notifier_block nb;
	acpi_handle	handle;
=======
	struct acpiphp_bridge *parent;
	struct acpiphp_slot *slot;

	struct list_head sibling;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	u8		function;	/* pci function# */
	u32		flags;		/* see below */
};

<<<<<<< HEAD
=======
struct acpiphp_context {
	struct acpi_hotplug_context hp;
	struct acpiphp_func func;
	struct acpiphp_bridge *bridge;
	unsigned int refcount;
};

static inline struct acpiphp_context *to_acpiphp_context(struct acpi_hotplug_context *hp)
{
	return container_of(hp, struct acpiphp_context, hp);
}

static inline struct acpiphp_context *func_to_context(struct acpiphp_func *func)
{
	return container_of(func, struct acpiphp_context, func);
}

static inline struct acpi_device *func_to_acpi_device(struct acpiphp_func *func)
{
	return func_to_context(func)->hp.self;
}

static inline acpi_handle func_to_handle(struct acpiphp_func *func)
{
	return func_to_acpi_device(func)->handle;
}

struct acpiphp_root_context {
	struct acpi_hotplug_context hp;
	struct acpiphp_bridge *root_bridge;
};

static inline struct acpiphp_root_context *to_acpiphp_root_context(struct acpi_hotplug_context *hp)
{
	return container_of(hp, struct acpiphp_root_context, hp);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * struct acpiphp_attention_info - device specific attention registration
 *
 * ACPI has no generic method of setting/getting attention status
 * this allows for device specific driver registration
 */
struct acpiphp_attention_info
{
	int (*set_attn)(struct hotplug_slot *slot, u8 status);
	int (*get_attn)(struct hotplug_slot *slot, u8 *status);
	struct module *owner;
};

<<<<<<< HEAD
/* PCI bus bridge HID */
#define ACPI_PCI_HOST_HID		"PNP0A03"

/* ACPI _STA method value (ignore bit 4; battery present) */
#define ACPI_STA_ALL			(0x0000000f)

/* bridge flags */
#define BRIDGE_HAS_EJ0		(0x00000001)

/* slot flags */

#define SLOT_POWEREDON		(0x00000001)
#define SLOT_ENABLED		(0x00000002)
#define SLOT_MULTIFUNCTION	(0x00000004)
=======
/* ACPI _STA method value (ignore bit 4; battery present) */
#define ACPI_STA_ALL			(0x0000000f)

/* slot flags */

#define SLOT_ENABLED		(0x00000001)
#define SLOT_IS_GOING_AWAY	(0x00000002)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* function flags */

#define FUNC_HAS_STA		(0x00000001)
#define FUNC_HAS_EJ0		(0x00000002)
<<<<<<< HEAD
#define FUNC_HAS_PS0		(0x00000010)
#define FUNC_HAS_PS1		(0x00000020)
#define FUNC_HAS_PS2		(0x00000040)
#define FUNC_HAS_PS3		(0x00000080)
#define FUNC_HAS_DCK            (0x00000100)
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* function prototypes */

/* acpiphp_core.c */
<<<<<<< HEAD
int acpiphp_register_attention(struct acpiphp_attention_info*info);
int acpiphp_unregister_attention(struct acpiphp_attention_info *info);
int acpiphp_register_hotplug_slot(struct acpiphp_slot *slot);
=======
int acpiphp_register_attention(struct acpiphp_attention_info *info);
int acpiphp_unregister_attention(struct acpiphp_attention_info *info);
int acpiphp_register_hotplug_slot(struct acpiphp_slot *slot, unsigned int sun);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void acpiphp_unregister_hotplug_slot(struct acpiphp_slot *slot);

/* acpiphp_glue.c */
typedef int (*acpiphp_callback)(struct acpiphp_slot *slot, void *data);

int acpiphp_enable_slot(struct acpiphp_slot *slot);
int acpiphp_disable_slot(struct acpiphp_slot *slot);
<<<<<<< HEAD
int acpiphp_eject_slot(struct acpiphp_slot *slot);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
u8 acpiphp_get_power_status(struct acpiphp_slot *slot);
u8 acpiphp_get_attention_status(struct acpiphp_slot *slot);
u8 acpiphp_get_latch_status(struct acpiphp_slot *slot);
u8 acpiphp_get_adapter_status(struct acpiphp_slot *slot);

/* variables */
<<<<<<< HEAD
extern bool acpiphp_debug;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern bool acpiphp_disabled;

#endif /* _ACPIPHP_H */
