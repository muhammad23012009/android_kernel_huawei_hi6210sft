/*
 * Copyright (c) 2008, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
<<<<<<< HEAD
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
=======
 * this program; if not, see <http://www.gnu.org/licenses/>.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Author: Alexander Duyck <alexander.h.duyck@intel.com>
 */

#ifndef __NET_TC_SKBEDIT_H
#define __NET_TC_SKBEDIT_H

#include <net/act_api.h>
<<<<<<< HEAD

struct tcf_skbedit {
	struct tcf_common	common;
	u32			flags;
	u32     		priority;
	u32     		mark;
	u16			queue_mapping;
	/* XXX: 16-bit pad here? */
};
#define to_skbedit(pc) \
	container_of(pc, struct tcf_skbedit, common)
=======
#include <linux/tc_act/tc_skbedit.h>

struct tcf_skbedit {
	struct tc_action	common;
	u32		flags;
	u32		priority;
	u32		mark;
	u16		queue_mapping;
	u16		ptype;
};
#define to_skbedit(a) ((struct tcf_skbedit *)a)

/* Return true iff action is mark */
static inline bool is_tcf_skbedit_mark(const struct tc_action *a)
{
#ifdef CONFIG_NET_CLS_ACT
	if (a->ops && a->ops->type == TCA_ACT_SKBEDIT)
		return to_skbedit(a)->flags == SKBEDIT_F_MARK;
#endif
	return false;
}

static inline u32 tcf_skbedit_mark(const struct tc_action *a)
{
	return to_skbedit(a)->mark;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __NET_TC_SKBEDIT_H */
