/*
 * PCI Hot Plug Controller Driver for System z
 *
 * Copyright 2012 IBM Corp.
 *
 * Author(s):
 *   Jan Glauber <jang@linux.vnet.ibm.com>
 */

<<<<<<< HEAD
#define COMPONENT "zPCI hpc"
#define pr_fmt(fmt) COMPONENT ": " fmt
=======
#define KMSG_COMPONENT "zpci"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/pci.h>
#include <linux/pci_hotplug.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/pci_debug.h>
#include <asm/sclp.h>

#define SLOT_NAME_SIZE	10
static LIST_HEAD(s390_hotplug_slot_list);

MODULE_AUTHOR("Jan Glauber <jang@linux.vnet.ibm.com");
MODULE_DESCRIPTION("Hot Plug PCI Controller for System z");
MODULE_LICENSE("GPL");

static int zpci_fn_configured(enum zpci_state state)
{
	return state == ZPCI_FN_STATE_CONFIGURED ||
	       state == ZPCI_FN_STATE_ONLINE;
}

/*
 * struct slot - slot information for each *physical* slot
 */
struct slot {
	struct list_head slot_list;
	struct hotplug_slot *hotplug_slot;
	struct zpci_dev *zdev;
};

<<<<<<< HEAD
=======
static inline int slot_configure(struct slot *slot)
{
	int ret = sclp_pci_configure(slot->zdev->fid);

	zpci_dbg(3, "conf fid:%x, rc:%d\n", slot->zdev->fid, ret);
	if (!ret)
		slot->zdev->state = ZPCI_FN_STATE_CONFIGURED;

	return ret;
}

static inline int slot_deconfigure(struct slot *slot)
{
	int ret = sclp_pci_deconfigure(slot->zdev->fid);

	zpci_dbg(3, "deconf fid:%x, rc:%d\n", slot->zdev->fid, ret);
	if (!ret)
		slot->zdev->state = ZPCI_FN_STATE_STANDBY;

	return ret;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int enable_slot(struct hotplug_slot *hotplug_slot)
{
	struct slot *slot = hotplug_slot->private;
	int rc;

	if (slot->zdev->state != ZPCI_FN_STATE_STANDBY)
		return -EIO;

<<<<<<< HEAD
	rc = sclp_pci_configure(slot->zdev->fid);
	zpci_dbg(3, "conf fid:%x, rc:%d\n", slot->zdev->fid, rc);
	if (!rc) {
		slot->zdev->state = ZPCI_FN_STATE_CONFIGURED;
		/* automatically scan the device after is was configured */
		zpci_enable_device(slot->zdev);
		zpci_scan_device(slot->zdev);
	}
=======
	rc = slot_configure(slot);
	if (rc)
		return rc;

	rc = zpci_enable_device(slot->zdev);
	if (rc)
		goto out_deconfigure;

	pci_scan_slot(slot->zdev->bus, ZPCI_DEVFN);
	pci_lock_rescan_remove();
	pci_bus_add_devices(slot->zdev->bus);
	pci_unlock_rescan_remove();

	return rc;

out_deconfigure:
	slot_deconfigure(slot);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return rc;
}

static int disable_slot(struct hotplug_slot *hotplug_slot)
{
	struct slot *slot = hotplug_slot->private;
<<<<<<< HEAD
=======
	struct pci_dev *pdev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int rc;

	if (!zpci_fn_configured(slot->zdev->state))
		return -EIO;

<<<<<<< HEAD
	rc = zpci_disable_device(slot->zdev);
	if (rc)
		return rc;
	/* TODO: we rely on the user to unbind/remove the device, is that plausible
	 *	 or do we need to trigger that here?
	 */
	rc = sclp_pci_deconfigure(slot->zdev->fid);
	zpci_dbg(3, "deconf fid:%x, rc:%d\n", slot->zdev->fid, rc);
	if (!rc)
		slot->zdev->state = ZPCI_FN_STATE_STANDBY;
	return rc;
=======
	pdev = pci_get_slot(slot->zdev->bus, ZPCI_DEVFN);
	if (pdev) {
		pci_stop_and_remove_bus_device_locked(pdev);
		pci_dev_put(pdev);
	}

	rc = zpci_disable_device(slot->zdev);
	if (rc)
		return rc;

	return slot_deconfigure(slot);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int get_power_status(struct hotplug_slot *hotplug_slot, u8 *value)
{
	struct slot *slot = hotplug_slot->private;

	switch (slot->zdev->state) {
	case ZPCI_FN_STATE_STANDBY:
		*value = 0;
		break;
	default:
		*value = 1;
		break;
	}
	return 0;
}

static int get_adapter_status(struct hotplug_slot *hotplug_slot, u8 *value)
{
	/* if the slot exits it always contains a function */
	*value = 1;
	return 0;
}

static void release_slot(struct hotplug_slot *hotplug_slot)
{
	struct slot *slot = hotplug_slot->private;

<<<<<<< HEAD
	pr_debug("%s - physical_slot = %s\n", __func__, hotplug_slot_name(hotplug_slot));
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(slot->hotplug_slot->info);
	kfree(slot->hotplug_slot);
	kfree(slot);
}

static struct hotplug_slot_ops s390_hotplug_slot_ops = {
	.enable_slot =		enable_slot,
	.disable_slot =		disable_slot,
	.get_power_status =	get_power_status,
	.get_adapter_status =	get_adapter_status,
};

<<<<<<< HEAD
static int init_pci_slot(struct zpci_dev *zdev)
=======
int zpci_init_slot(struct zpci_dev *zdev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct hotplug_slot *hotplug_slot;
	struct hotplug_slot_info *info;
	char name[SLOT_NAME_SIZE];
	struct slot *slot;
	int rc;

	if (!zdev)
		return 0;

	slot = kzalloc(sizeof(*slot), GFP_KERNEL);
	if (!slot)
		goto error;

	hotplug_slot = kzalloc(sizeof(*hotplug_slot), GFP_KERNEL);
	if (!hotplug_slot)
		goto error_hp;
	hotplug_slot->private = slot;

	slot->hotplug_slot = hotplug_slot;
	slot->zdev = zdev;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		goto error_info;
	hotplug_slot->info = info;

	hotplug_slot->ops = &s390_hotplug_slot_ops;
	hotplug_slot->release = &release_slot;

	get_power_status(hotplug_slot, &info->power_status);
	get_adapter_status(hotplug_slot, &info->adapter_status);

	snprintf(name, SLOT_NAME_SIZE, "%08x", zdev->fid);
	rc = pci_hp_register(slot->hotplug_slot, zdev->bus,
			     ZPCI_DEVFN, name);
<<<<<<< HEAD
	if (rc) {
		pr_err("pci_hp_register failed with error %d\n", rc);
		goto error_reg;
	}
=======
	if (rc)
		goto error_reg;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	list_add(&slot->slot_list, &s390_hotplug_slot_list);
	return 0;

error_reg:
	kfree(info);
error_info:
	kfree(hotplug_slot);
error_hp:
	kfree(slot);
error:
	return -ENOMEM;
}

<<<<<<< HEAD
static void exit_pci_slot(struct zpci_dev *zdev)
{
	struct list_head *tmp, *n;
	struct slot *slot;

	list_for_each_safe(tmp, n, &s390_hotplug_slot_list) {
		slot = list_entry(tmp, struct slot, slot_list);
=======
void zpci_exit_slot(struct zpci_dev *zdev)
{
	struct slot *slot, *next;

	list_for_each_entry_safe(slot, next, &s390_hotplug_slot_list,
				 slot_list) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (slot->zdev != zdev)
			continue;
		list_del(&slot->slot_list);
		pci_hp_deregister(slot->hotplug_slot);
	}
}
<<<<<<< HEAD

static struct pci_hp_callback_ops hp_ops = {
	.create_slot = init_pci_slot,
	.remove_slot = exit_pci_slot,
};

static void __init init_pci_slots(void)
{
	struct zpci_dev *zdev;

	/*
	 * Create a structure for each slot, and register that slot
	 * with the pci_hotplug subsystem.
	 */
	mutex_lock(&zpci_list_lock);
	list_for_each_entry(zdev, &zpci_list, entry) {
		init_pci_slot(zdev);
	}
	mutex_unlock(&zpci_list_lock);
}

static void __exit exit_pci_slots(void)
{
	struct list_head *tmp, *n;
	struct slot *slot;

	/*
	 * Unregister all of our slots with the pci_hotplug subsystem.
	 * Memory will be freed in release_slot() callback after slot's
	 * lifespan is finished.
	 */
	list_for_each_safe(tmp, n, &s390_hotplug_slot_list) {
		slot = list_entry(tmp, struct slot, slot_list);
		list_del(&slot->slot_list);
		pci_hp_deregister(slot->hotplug_slot);
	}
}

static int __init pci_hotplug_s390_init(void)
{
	if (!s390_pci_probe)
		return -EOPNOTSUPP;

	zpci_register_hp_ops(&hp_ops);
	init_pci_slots();

	return 0;
}

static void __exit pci_hotplug_s390_exit(void)
{
	exit_pci_slots();
	zpci_deregister_hp_ops();
}

module_init(pci_hotplug_s390_init);
module_exit(pci_hotplug_s390_exit);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
