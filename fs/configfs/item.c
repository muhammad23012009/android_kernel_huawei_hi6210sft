/* -*- mode: c; c-basic-offset: 8; -*-
 * vim: noexpandtab sw=8 ts=8 sts=0:
 *
 * item.c - library routines for handling generic config items
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 *
 * Based on kobject:
<<<<<<< HEAD
 * 	kobject is Copyright (c) 2002-2003 Patrick Mochel
=======
 *	kobject is Copyright (c) 2002-2003 Patrick Mochel
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * configfs Copyright (C) 2005 Oracle.  All rights reserved.
 *
 * Please see the file Documentation/filesystems/configfs/configfs.txt for
 * critical information about using the config_item interface.
 */

#include <linux/string.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/slab.h>

#include <linux/configfs.h>


<<<<<<< HEAD
static inline struct config_item * to_item(struct list_head * entry)
{
	return container_of(entry,struct config_item,ci_entry);
=======
static inline struct config_item *to_item(struct list_head *entry)
{
	return container_of(entry, struct config_item, ci_entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Evil kernel */
static void config_item_release(struct kref *kref);

/**
 *	config_item_init - initialize item.
 *	@item:	item in question.
 */
<<<<<<< HEAD
void config_item_init(struct config_item * item)
=======
static void config_item_init(struct config_item *item)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	kref_init(&item->ci_kref);
	INIT_LIST_HEAD(&item->ci_entry);
}

/**
 *	config_item_set_name - Set the name of an item
 *	@item:	item.
<<<<<<< HEAD
 *	@name:	name.
=======
 *	@fmt:  The vsnprintf()'s format string.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 *	If strlen(name) >= CONFIGFS_ITEM_NAME_LEN, then use a
 *	dynamically allocated string that @item->ci_name points to.
 *	Otherwise, use the static @item->ci_namebuf array.
 */
<<<<<<< HEAD
int config_item_set_name(struct config_item * item, const char * fmt, ...)
=======
int config_item_set_name(struct config_item *item, const char *fmt, ...)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int error = 0;
	int limit = CONFIGFS_ITEM_NAME_LEN;
	int need;
	va_list args;
<<<<<<< HEAD
	char * name;
=======
	char *name;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * First, try the static array
	 */
<<<<<<< HEAD
	va_start(args,fmt);
	need = vsnprintf(item->ci_namebuf,limit,fmt,args);
=======
	va_start(args, fmt);
	need = vsnprintf(item->ci_namebuf, limit, fmt, args);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	va_end(args);
	if (need < limit)
		name = item->ci_namebuf;
	else {
		/*
		 * Need more space? Allocate it and try again
		 */
		limit = need + 1;
<<<<<<< HEAD
		name = kmalloc(limit,GFP_KERNEL);
=======
		name = kmalloc(limit, GFP_KERNEL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!name) {
			error = -ENOMEM;
			goto Done;
		}
<<<<<<< HEAD
		va_start(args,fmt);
		need = vsnprintf(name,limit,fmt,args);
=======
		va_start(args, fmt);
		need = vsnprintf(name, limit, fmt, args);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		va_end(args);

		/* Still? Give up. */
		if (need >= limit) {
			kfree(name);
			error = -EFAULT;
			goto Done;
		}
	}

	/* Free the old name, if necessary. */
	if (item->ci_name && item->ci_name != item->ci_namebuf)
		kfree(item->ci_name);

	/* Now, set the new name */
	item->ci_name = name;
 Done:
	return error;
}
<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL(config_item_set_name);

void config_item_init_type_name(struct config_item *item,
				const char *name,
				struct config_item_type *type)
{
<<<<<<< HEAD
	config_item_set_name(item, name);
=======
	config_item_set_name(item, "%s", name);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	item->ci_type = type;
	config_item_init(item);
}
EXPORT_SYMBOL(config_item_init_type_name);

void config_group_init_type_name(struct config_group *group, const char *name,
			 struct config_item_type *type)
{
<<<<<<< HEAD
	config_item_set_name(&group->cg_item, name);
=======
	config_item_set_name(&group->cg_item, "%s", name);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	group->cg_item.ci_type = type;
	config_group_init(group);
}
EXPORT_SYMBOL(config_group_init_type_name);

<<<<<<< HEAD
struct config_item * config_item_get(struct config_item * item)
=======
struct config_item *config_item_get(struct config_item *item)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (item)
		kref_get(&item->ci_kref);
	return item;
}
<<<<<<< HEAD

static void config_item_cleanup(struct config_item * item)
{
	struct config_item_type * t = item->ci_type;
	struct config_group * s = item->ci_group;
	struct config_item * parent = item->ci_parent;

	pr_debug("config_item %s: cleaning up\n",config_item_name(item));
=======
EXPORT_SYMBOL(config_item_get);

static void config_item_cleanup(struct config_item *item)
{
	struct config_item_type *t = item->ci_type;
	struct config_group *s = item->ci_group;
	struct config_item *parent = item->ci_parent;

	pr_debug("config_item %s: cleaning up\n", config_item_name(item));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (item->ci_name != item->ci_namebuf)
		kfree(item->ci_name);
	item->ci_name = NULL;
	if (t && t->ct_item_ops && t->ct_item_ops->release)
		t->ct_item_ops->release(item);
	if (s)
		config_group_put(s);
	if (parent)
		config_item_put(parent);
}

static void config_item_release(struct kref *kref)
{
	config_item_cleanup(container_of(kref, struct config_item, ci_kref));
}

/**
 *	config_item_put - decrement refcount for item.
 *	@item:	item.
 *
 *	Decrement the refcount, and if 0, call config_item_cleanup().
 */
<<<<<<< HEAD
void config_item_put(struct config_item * item)
=======
void config_item_put(struct config_item *item)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (item)
		kref_put(&item->ci_kref, config_item_release);
}
<<<<<<< HEAD

/**
 *	config_group_init - initialize a group for use
 *	@k:	group
=======
EXPORT_SYMBOL(config_item_put);

/**
 *	config_group_init - initialize a group for use
 *	@group:	config_group
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
void config_group_init(struct config_group *group)
{
	config_item_init(&group->cg_item);
	INIT_LIST_HEAD(&group->cg_children);
<<<<<<< HEAD
}
=======
	INIT_LIST_HEAD(&group->default_groups);
}
EXPORT_SYMBOL(config_group_init);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 *	config_group_find_item - search for item in group.
 *	@group:	group we're looking in.
 *	@name:	item's name.
 *
 *	Iterate over @group->cg_list, looking for a matching config_item.
 *	If matching item is found take a reference and return the item.
 *	Caller must have locked group via @group->cg_subsys->su_mtx.
 */
struct config_item *config_group_find_item(struct config_group *group,
					   const char *name)
{
<<<<<<< HEAD
	struct list_head * entry;
	struct config_item * ret = NULL;

	list_for_each(entry,&group->cg_children) {
		struct config_item * item = to_item(entry);
=======
	struct list_head *entry;
	struct config_item *ret = NULL;

	list_for_each(entry, &group->cg_children) {
		struct config_item *item = to_item(entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (config_item_name(item) &&
		    !strcmp(config_item_name(item), name)) {
			ret = config_item_get(item);
			break;
		}
	}
	return ret;
}
<<<<<<< HEAD

EXPORT_SYMBOL(config_item_init);
EXPORT_SYMBOL(config_group_init);
EXPORT_SYMBOL(config_item_get);
EXPORT_SYMBOL(config_item_put);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL(config_group_find_item);
