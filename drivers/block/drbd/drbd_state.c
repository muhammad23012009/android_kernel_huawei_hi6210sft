/*
   drbd_state.c

   This file is part of DRBD by Philipp Reisner and Lars Ellenberg.

   Copyright (C) 2001-2008, LINBIT Information Technologies GmbH.
   Copyright (C) 1999-2008, Philipp Reisner <philipp.reisner@linbit.com>.
   Copyright (C) 2002-2008, Lars Ellenberg <lars.ellenberg@linbit.com>.

   Thanks to Carter Burden, Bart Grantham and Gennadiy Nerubayev
   from Logicworks, Inc. for making SDP replication support possible.

   drbd is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   drbd is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with drbd; see the file COPYING.  If not, write to
   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/drbd_limits.h>
#include "drbd_int.h"
<<<<<<< HEAD
#include "drbd_req.h"

/* in drbd_main.c */
extern void tl_abort_disk_io(struct drbd_conf *mdev);

struct after_state_chg_work {
	struct drbd_work w;
=======
#include "drbd_protocol.h"
#include "drbd_req.h"
#include "drbd_state_change.h"

struct after_state_chg_work {
	struct drbd_work w;
	struct drbd_device *device;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	union drbd_state os;
	union drbd_state ns;
	enum chg_state_flags flags;
	struct completion *done;
<<<<<<< HEAD
=======
	struct drbd_state_change *state_change;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

enum sanitize_state_warnings {
	NO_WARNING,
	ABORTED_ONLINE_VERIFY,
	ABORTED_RESYNC,
	CONNECTION_LOST_NEGOTIATING,
	IMPLICITLY_UPGRADED_DISK,
	IMPLICITLY_UPGRADED_PDSK,
};

<<<<<<< HEAD
static int w_after_state_ch(struct drbd_work *w, int unused);
static void after_state_ch(struct drbd_conf *mdev, union drbd_state os,
			   union drbd_state ns, enum chg_state_flags flags);
static enum drbd_state_rv is_valid_state(struct drbd_conf *, union drbd_state);
static enum drbd_state_rv is_valid_soft_transition(union drbd_state, union drbd_state, struct drbd_tconn *);
static enum drbd_state_rv is_valid_transition(union drbd_state os, union drbd_state ns);
static union drbd_state sanitize_state(struct drbd_conf *mdev, union drbd_state ns,
				       enum sanitize_state_warnings *warn);
=======
static void count_objects(struct drbd_resource *resource,
			  unsigned int *n_devices,
			  unsigned int *n_connections)
{
	struct drbd_device *device;
	struct drbd_connection *connection;
	int vnr;

	*n_devices = 0;
	*n_connections = 0;

	idr_for_each_entry(&resource->devices, device, vnr)
		(*n_devices)++;
	for_each_connection(connection, resource)
		(*n_connections)++;
}

static struct drbd_state_change *alloc_state_change(unsigned int n_devices, unsigned int n_connections, gfp_t gfp)
{
	struct drbd_state_change *state_change;
	unsigned int size, n;

	size = sizeof(struct drbd_state_change) +
	       n_devices * sizeof(struct drbd_device_state_change) +
	       n_connections * sizeof(struct drbd_connection_state_change) +
	       n_devices * n_connections * sizeof(struct drbd_peer_device_state_change);
	state_change = kmalloc(size, gfp);
	if (!state_change)
		return NULL;
	state_change->n_devices = n_devices;
	state_change->n_connections = n_connections;
	state_change->devices = (void *)(state_change + 1);
	state_change->connections = (void *)&state_change->devices[n_devices];
	state_change->peer_devices = (void *)&state_change->connections[n_connections];
	state_change->resource->resource = NULL;
	for (n = 0; n < n_devices; n++)
		state_change->devices[n].device = NULL;
	for (n = 0; n < n_connections; n++)
		state_change->connections[n].connection = NULL;
	return state_change;
}

struct drbd_state_change *remember_old_state(struct drbd_resource *resource, gfp_t gfp)
{
	struct drbd_state_change *state_change;
	struct drbd_device *device;
	unsigned int n_devices;
	struct drbd_connection *connection;
	unsigned int n_connections;
	int vnr;

	struct drbd_device_state_change *device_state_change;
	struct drbd_peer_device_state_change *peer_device_state_change;
	struct drbd_connection_state_change *connection_state_change;

	/* Caller holds req_lock spinlock.
	 * No state, no device IDR, no connections lists can change. */
	count_objects(resource, &n_devices, &n_connections);
	state_change = alloc_state_change(n_devices, n_connections, gfp);
	if (!state_change)
		return NULL;

	kref_get(&resource->kref);
	state_change->resource->resource = resource;
	state_change->resource->role[OLD] =
		conn_highest_role(first_connection(resource));
	state_change->resource->susp[OLD] = resource->susp;
	state_change->resource->susp_nod[OLD] = resource->susp_nod;
	state_change->resource->susp_fen[OLD] = resource->susp_fen;

	connection_state_change = state_change->connections;
	for_each_connection(connection, resource) {
		kref_get(&connection->kref);
		connection_state_change->connection = connection;
		connection_state_change->cstate[OLD] =
			connection->cstate;
		connection_state_change->peer_role[OLD] =
			conn_highest_peer(connection);
		connection_state_change++;
	}

	device_state_change = state_change->devices;
	peer_device_state_change = state_change->peer_devices;
	idr_for_each_entry(&resource->devices, device, vnr) {
		kref_get(&device->kref);
		device_state_change->device = device;
		device_state_change->disk_state[OLD] = device->state.disk;

		/* The peer_devices for each device have to be enumerated in
		   the order of the connections. We may not use for_each_peer_device() here. */
		for_each_connection(connection, resource) {
			struct drbd_peer_device *peer_device;

			peer_device = conn_peer_device(connection, device->vnr);
			peer_device_state_change->peer_device = peer_device;
			peer_device_state_change->disk_state[OLD] =
				device->state.pdsk;
			peer_device_state_change->repl_state[OLD] =
				max_t(enum drbd_conns,
				      C_WF_REPORT_PARAMS, device->state.conn);
			peer_device_state_change->resync_susp_user[OLD] =
				device->state.user_isp;
			peer_device_state_change->resync_susp_peer[OLD] =
				device->state.peer_isp;
			peer_device_state_change->resync_susp_dependency[OLD] =
				device->state.aftr_isp;
			peer_device_state_change++;
		}
		device_state_change++;
	}

	return state_change;
}

static void remember_new_state(struct drbd_state_change *state_change)
{
	struct drbd_resource_state_change *resource_state_change;
	struct drbd_resource *resource;
	unsigned int n;

	if (!state_change)
		return;

	resource_state_change = &state_change->resource[0];
	resource = resource_state_change->resource;

	resource_state_change->role[NEW] =
		conn_highest_role(first_connection(resource));
	resource_state_change->susp[NEW] = resource->susp;
	resource_state_change->susp_nod[NEW] = resource->susp_nod;
	resource_state_change->susp_fen[NEW] = resource->susp_fen;

	for (n = 0; n < state_change->n_devices; n++) {
		struct drbd_device_state_change *device_state_change =
			&state_change->devices[n];
		struct drbd_device *device = device_state_change->device;

		device_state_change->disk_state[NEW] = device->state.disk;
	}

	for (n = 0; n < state_change->n_connections; n++) {
		struct drbd_connection_state_change *connection_state_change =
			&state_change->connections[n];
		struct drbd_connection *connection =
			connection_state_change->connection;

		connection_state_change->cstate[NEW] = connection->cstate;
		connection_state_change->peer_role[NEW] =
			conn_highest_peer(connection);
	}

	for (n = 0; n < state_change->n_devices * state_change->n_connections; n++) {
		struct drbd_peer_device_state_change *peer_device_state_change =
			&state_change->peer_devices[n];
		struct drbd_device *device =
			peer_device_state_change->peer_device->device;
		union drbd_dev_state state = device->state;

		peer_device_state_change->disk_state[NEW] = state.pdsk;
		peer_device_state_change->repl_state[NEW] =
			max_t(enum drbd_conns, C_WF_REPORT_PARAMS, state.conn);
		peer_device_state_change->resync_susp_user[NEW] =
			state.user_isp;
		peer_device_state_change->resync_susp_peer[NEW] =
			state.peer_isp;
		peer_device_state_change->resync_susp_dependency[NEW] =
			state.aftr_isp;
	}
}

void copy_old_to_new_state_change(struct drbd_state_change *state_change)
{
	struct drbd_resource_state_change *resource_state_change = &state_change->resource[0];
	unsigned int n_device, n_connection, n_peer_device, n_peer_devices;

#define OLD_TO_NEW(x) \
	(x[NEW] = x[OLD])

	OLD_TO_NEW(resource_state_change->role);
	OLD_TO_NEW(resource_state_change->susp);
	OLD_TO_NEW(resource_state_change->susp_nod);
	OLD_TO_NEW(resource_state_change->susp_fen);

	for (n_connection = 0; n_connection < state_change->n_connections; n_connection++) {
		struct drbd_connection_state_change *connection_state_change =
				&state_change->connections[n_connection];

		OLD_TO_NEW(connection_state_change->peer_role);
		OLD_TO_NEW(connection_state_change->cstate);
	}

	for (n_device = 0; n_device < state_change->n_devices; n_device++) {
		struct drbd_device_state_change *device_state_change =
			&state_change->devices[n_device];

		OLD_TO_NEW(device_state_change->disk_state);
	}

	n_peer_devices = state_change->n_devices * state_change->n_connections;
	for (n_peer_device = 0; n_peer_device < n_peer_devices; n_peer_device++) {
		struct drbd_peer_device_state_change *p =
			&state_change->peer_devices[n_peer_device];

		OLD_TO_NEW(p->disk_state);
		OLD_TO_NEW(p->repl_state);
		OLD_TO_NEW(p->resync_susp_user);
		OLD_TO_NEW(p->resync_susp_peer);
		OLD_TO_NEW(p->resync_susp_dependency);
	}

#undef OLD_TO_NEW
}

void forget_state_change(struct drbd_state_change *state_change)
{
	unsigned int n;

	if (!state_change)
		return;

	if (state_change->resource->resource)
		kref_put(&state_change->resource->resource->kref, drbd_destroy_resource);
	for (n = 0; n < state_change->n_devices; n++) {
		struct drbd_device *device = state_change->devices[n].device;

		if (device)
			kref_put(&device->kref, drbd_destroy_device);
	}
	for (n = 0; n < state_change->n_connections; n++) {
		struct drbd_connection *connection =
			state_change->connections[n].connection;

		if (connection)
			kref_put(&connection->kref, drbd_destroy_connection);
	}
	kfree(state_change);
}

static int w_after_state_ch(struct drbd_work *w, int unused);
static void after_state_ch(struct drbd_device *device, union drbd_state os,
			   union drbd_state ns, enum chg_state_flags flags,
			   struct drbd_state_change *);
static enum drbd_state_rv is_valid_state(struct drbd_device *, union drbd_state);
static enum drbd_state_rv is_valid_soft_transition(union drbd_state, union drbd_state, struct drbd_connection *);
static enum drbd_state_rv is_valid_transition(union drbd_state os, union drbd_state ns);
static union drbd_state sanitize_state(struct drbd_device *device, union drbd_state os,
				       union drbd_state ns, enum sanitize_state_warnings *warn);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline bool is_susp(union drbd_state s)
{
        return s.susp || s.susp_nod || s.susp_fen;
}

<<<<<<< HEAD
bool conn_all_vols_unconf(struct drbd_tconn *tconn)
{
	struct drbd_conf *mdev;
=======
bool conn_all_vols_unconf(struct drbd_connection *connection)
{
	struct drbd_peer_device *peer_device;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bool rv = true;
	int vnr;

	rcu_read_lock();
<<<<<<< HEAD
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		if (mdev->state.disk != D_DISKLESS ||
		    mdev->state.conn != C_STANDALONE ||
		    mdev->state.role != R_SECONDARY) {
=======
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		if (device->state.disk != D_DISKLESS ||
		    device->state.conn != C_STANDALONE ||
		    device->state.role != R_SECONDARY) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			rv = false;
			break;
		}
	}
	rcu_read_unlock();

	return rv;
}

/* Unfortunately the states where not correctly ordered, when
   they where defined. therefore can not use max_t() here. */
static enum drbd_role max_role(enum drbd_role role1, enum drbd_role role2)
{
	if (role1 == R_PRIMARY || role2 == R_PRIMARY)
		return R_PRIMARY;
	if (role1 == R_SECONDARY || role2 == R_SECONDARY)
		return R_SECONDARY;
	return R_UNKNOWN;
}
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static enum drbd_role min_role(enum drbd_role role1, enum drbd_role role2)
{
	if (role1 == R_UNKNOWN || role2 == R_UNKNOWN)
		return R_UNKNOWN;
	if (role1 == R_SECONDARY || role2 == R_SECONDARY)
		return R_SECONDARY;
	return R_PRIMARY;
}

<<<<<<< HEAD
enum drbd_role conn_highest_role(struct drbd_tconn *tconn)
{
	enum drbd_role role = R_UNKNOWN;
	struct drbd_conf *mdev;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr)
		role = max_role(role, mdev->state.role);
=======
enum drbd_role conn_highest_role(struct drbd_connection *connection)
{
	enum drbd_role role = R_UNKNOWN;
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		role = max_role(role, device->state.role);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_read_unlock();

	return role;
}

<<<<<<< HEAD
enum drbd_role conn_highest_peer(struct drbd_tconn *tconn)
{
	enum drbd_role peer = R_UNKNOWN;
	struct drbd_conf *mdev;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr)
		peer = max_role(peer, mdev->state.peer);
=======
enum drbd_role conn_highest_peer(struct drbd_connection *connection)
{
	enum drbd_role peer = R_UNKNOWN;
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		peer = max_role(peer, device->state.peer);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_read_unlock();

	return peer;
}

<<<<<<< HEAD
enum drbd_disk_state conn_highest_disk(struct drbd_tconn *tconn)
{
	enum drbd_disk_state ds = D_DISKLESS;
	struct drbd_conf *mdev;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr)
		ds = max_t(enum drbd_disk_state, ds, mdev->state.disk);
	rcu_read_unlock();

	return ds;
}

enum drbd_disk_state conn_lowest_disk(struct drbd_tconn *tconn)
{
	enum drbd_disk_state ds = D_MASK;
	struct drbd_conf *mdev;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr)
		ds = min_t(enum drbd_disk_state, ds, mdev->state.disk);
	rcu_read_unlock();

	return ds;
}

enum drbd_disk_state conn_highest_pdsk(struct drbd_tconn *tconn)
{
	enum drbd_disk_state ds = D_DISKLESS;
	struct drbd_conf *mdev;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr)
		ds = max_t(enum drbd_disk_state, ds, mdev->state.pdsk);
	rcu_read_unlock();

	return ds;
}

enum drbd_conns conn_lowest_conn(struct drbd_tconn *tconn)
{
	enum drbd_conns conn = C_MASK;
	struct drbd_conf *mdev;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr)
		conn = min_t(enum drbd_conns, conn, mdev->state.conn);
=======
enum drbd_disk_state conn_highest_disk(struct drbd_connection *connection)
{
	enum drbd_disk_state disk_state = D_DISKLESS;
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		disk_state = max_t(enum drbd_disk_state, disk_state, device->state.disk);
	}
	rcu_read_unlock();

	return disk_state;
}

enum drbd_disk_state conn_lowest_disk(struct drbd_connection *connection)
{
	enum drbd_disk_state disk_state = D_MASK;
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		disk_state = min_t(enum drbd_disk_state, disk_state, device->state.disk);
	}
	rcu_read_unlock();

	return disk_state;
}

enum drbd_disk_state conn_highest_pdsk(struct drbd_connection *connection)
{
	enum drbd_disk_state disk_state = D_DISKLESS;
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		disk_state = max_t(enum drbd_disk_state, disk_state, device->state.pdsk);
	}
	rcu_read_unlock();

	return disk_state;
}

enum drbd_conns conn_lowest_conn(struct drbd_connection *connection)
{
	enum drbd_conns conn = C_MASK;
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		conn = min_t(enum drbd_conns, conn, device->state.conn);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_read_unlock();

	return conn;
}

<<<<<<< HEAD
static bool no_peer_wf_report_params(struct drbd_tconn *tconn)
{
	struct drbd_conf *mdev;
=======
static bool no_peer_wf_report_params(struct drbd_connection *connection)
{
	struct drbd_peer_device *peer_device;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int vnr;
	bool rv = true;

	rcu_read_lock();
<<<<<<< HEAD
	idr_for_each_entry(&tconn->volumes, mdev, vnr)
		if (mdev->state.conn == C_WF_REPORT_PARAMS) {
=======
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr)
		if (peer_device->device->state.conn == C_WF_REPORT_PARAMS) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			rv = false;
			break;
		}
	rcu_read_unlock();

	return rv;
}

<<<<<<< HEAD

/**
 * cl_wide_st_chg() - true if the state change is a cluster wide one
 * @mdev:	DRBD device.
 * @os:		old (current) state.
 * @ns:		new (wanted) state.
 */
static int cl_wide_st_chg(struct drbd_conf *mdev,
=======
static void wake_up_all_devices(struct drbd_connection *connection)
{
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr)
		wake_up(&peer_device->device->state_wait);
	rcu_read_unlock();

}


/**
 * cl_wide_st_chg() - true if the state change is a cluster wide one
 * @device:	DRBD device.
 * @os:		old (current) state.
 * @ns:		new (wanted) state.
 */
static int cl_wide_st_chg(struct drbd_device *device,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			  union drbd_state os, union drbd_state ns)
{
	return (os.conn >= C_CONNECTED && ns.conn >= C_CONNECTED &&
		 ((os.role != R_PRIMARY && ns.role == R_PRIMARY) ||
		  (os.conn != C_STARTING_SYNC_T && ns.conn == C_STARTING_SYNC_T) ||
		  (os.conn != C_STARTING_SYNC_S && ns.conn == C_STARTING_SYNC_S) ||
		  (os.disk != D_FAILED && ns.disk == D_FAILED))) ||
		(os.conn >= C_CONNECTED && ns.conn == C_DISCONNECTING) ||
		(os.conn == C_CONNECTED && ns.conn == C_VERIFY_S) ||
		(os.conn == C_CONNECTED && ns.conn == C_WF_REPORT_PARAMS);
}

static union drbd_state
apply_mask_val(union drbd_state os, union drbd_state mask, union drbd_state val)
{
	union drbd_state ns;
	ns.i = (os.i & ~mask.i) | val.i;
	return ns;
}

enum drbd_state_rv
<<<<<<< HEAD
drbd_change_state(struct drbd_conf *mdev, enum chg_state_flags f,
=======
drbd_change_state(struct drbd_device *device, enum chg_state_flags f,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		  union drbd_state mask, union drbd_state val)
{
	unsigned long flags;
	union drbd_state ns;
	enum drbd_state_rv rv;

<<<<<<< HEAD
	spin_lock_irqsave(&mdev->tconn->req_lock, flags);
	ns = apply_mask_val(drbd_read_state(mdev), mask, val);
	rv = _drbd_set_state(mdev, ns, f, NULL);
	spin_unlock_irqrestore(&mdev->tconn->req_lock, flags);
=======
	spin_lock_irqsave(&device->resource->req_lock, flags);
	ns = apply_mask_val(drbd_read_state(device), mask, val);
	rv = _drbd_set_state(device, ns, f, NULL);
	spin_unlock_irqrestore(&device->resource->req_lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return rv;
}

/**
 * drbd_force_state() - Impose a change which happens outside our control on our state
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @mask:	mask of state bits to change.
 * @val:	value of new state bits.
 */
void drbd_force_state(struct drbd_conf *mdev,
	union drbd_state mask, union drbd_state val)
{
	drbd_change_state(mdev, CS_HARD, mask, val);
}

static enum drbd_state_rv
_req_st_cond(struct drbd_conf *mdev, union drbd_state mask,
=======
 * @device:	DRBD device.
 * @mask:	mask of state bits to change.
 * @val:	value of new state bits.
 */
void drbd_force_state(struct drbd_device *device,
	union drbd_state mask, union drbd_state val)
{
	drbd_change_state(device, CS_HARD, mask, val);
}

static enum drbd_state_rv
_req_st_cond(struct drbd_device *device, union drbd_state mask,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	     union drbd_state val)
{
	union drbd_state os, ns;
	unsigned long flags;
	enum drbd_state_rv rv;

<<<<<<< HEAD
	if (test_and_clear_bit(CL_ST_CHG_SUCCESS, &mdev->flags))
		return SS_CW_SUCCESS;

	if (test_and_clear_bit(CL_ST_CHG_FAIL, &mdev->flags))
		return SS_CW_FAILED_BY_PEER;

	spin_lock_irqsave(&mdev->tconn->req_lock, flags);
	os = drbd_read_state(mdev);
	ns = sanitize_state(mdev, apply_mask_val(os, mask, val), NULL);
=======
	if (test_and_clear_bit(CL_ST_CHG_SUCCESS, &device->flags))
		return SS_CW_SUCCESS;

	if (test_and_clear_bit(CL_ST_CHG_FAIL, &device->flags))
		return SS_CW_FAILED_BY_PEER;

	spin_lock_irqsave(&device->resource->req_lock, flags);
	os = drbd_read_state(device);
	ns = sanitize_state(device, os, apply_mask_val(os, mask, val), NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rv = is_valid_transition(os, ns);
	if (rv >= SS_SUCCESS)
		rv = SS_UNKNOWN_ERROR;  /* cont waiting, otherwise fail. */

<<<<<<< HEAD
	if (!cl_wide_st_chg(mdev, os, ns))
		rv = SS_CW_NO_NEED;
	if (rv == SS_UNKNOWN_ERROR) {
		rv = is_valid_state(mdev, ns);
		if (rv >= SS_SUCCESS) {
			rv = is_valid_soft_transition(os, ns, mdev->tconn);
=======
	if (!cl_wide_st_chg(device, os, ns))
		rv = SS_CW_NO_NEED;
	if (rv == SS_UNKNOWN_ERROR) {
		rv = is_valid_state(device, ns);
		if (rv >= SS_SUCCESS) {
			rv = is_valid_soft_transition(os, ns, first_peer_device(device)->connection);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (rv >= SS_SUCCESS)
				rv = SS_UNKNOWN_ERROR; /* cont waiting, otherwise fail. */
		}
	}
<<<<<<< HEAD
	spin_unlock_irqrestore(&mdev->tconn->req_lock, flags);
=======
	spin_unlock_irqrestore(&device->resource->req_lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return rv;
}

/**
 * drbd_req_state() - Perform an eventually cluster wide state change
<<<<<<< HEAD
 * @mdev:	DRBD device.
=======
 * @device:	DRBD device.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @mask:	mask of state bits to change.
 * @val:	value of new state bits.
 * @f:		flags
 *
 * Should not be called directly, use drbd_request_state() or
 * _drbd_request_state().
 */
static enum drbd_state_rv
<<<<<<< HEAD
drbd_req_state(struct drbd_conf *mdev, union drbd_state mask,
=======
drbd_req_state(struct drbd_device *device, union drbd_state mask,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	       union drbd_state val, enum chg_state_flags f)
{
	struct completion done;
	unsigned long flags;
	union drbd_state os, ns;
	enum drbd_state_rv rv;

	init_completion(&done);

	if (f & CS_SERIALIZE)
<<<<<<< HEAD
		mutex_lock(mdev->state_mutex);

	spin_lock_irqsave(&mdev->tconn->req_lock, flags);
	os = drbd_read_state(mdev);
	ns = sanitize_state(mdev, apply_mask_val(os, mask, val), NULL);
	rv = is_valid_transition(os, ns);
	if (rv < SS_SUCCESS) {
		spin_unlock_irqrestore(&mdev->tconn->req_lock, flags);
		goto abort;
	}

	if (cl_wide_st_chg(mdev, os, ns)) {
		rv = is_valid_state(mdev, ns);
		if (rv == SS_SUCCESS)
			rv = is_valid_soft_transition(os, ns, mdev->tconn);
		spin_unlock_irqrestore(&mdev->tconn->req_lock, flags);

		if (rv < SS_SUCCESS) {
			if (f & CS_VERBOSE)
				print_st_err(mdev, os, ns, rv);
			goto abort;
		}

		if (drbd_send_state_req(mdev, mask, val)) {
			rv = SS_CW_FAILED_BY_PEER;
			if (f & CS_VERBOSE)
				print_st_err(mdev, os, ns, rv);
			goto abort;
		}

		wait_event(mdev->state_wait,
			(rv = _req_st_cond(mdev, mask, val)));

		if (rv < SS_SUCCESS) {
			if (f & CS_VERBOSE)
				print_st_err(mdev, os, ns, rv);
			goto abort;
		}
		spin_lock_irqsave(&mdev->tconn->req_lock, flags);
		ns = apply_mask_val(drbd_read_state(mdev), mask, val);
		rv = _drbd_set_state(mdev, ns, f, &done);
	} else {
		rv = _drbd_set_state(mdev, ns, f, &done);
	}

	spin_unlock_irqrestore(&mdev->tconn->req_lock, flags);

	if (f & CS_WAIT_COMPLETE && rv == SS_SUCCESS) {
		D_ASSERT(current != mdev->tconn->worker.task);
=======
		mutex_lock(device->state_mutex);

	spin_lock_irqsave(&device->resource->req_lock, flags);
	os = drbd_read_state(device);
	ns = sanitize_state(device, os, apply_mask_val(os, mask, val), NULL);
	rv = is_valid_transition(os, ns);
	if (rv < SS_SUCCESS) {
		spin_unlock_irqrestore(&device->resource->req_lock, flags);
		goto abort;
	}

	if (cl_wide_st_chg(device, os, ns)) {
		rv = is_valid_state(device, ns);
		if (rv == SS_SUCCESS)
			rv = is_valid_soft_transition(os, ns, first_peer_device(device)->connection);
		spin_unlock_irqrestore(&device->resource->req_lock, flags);

		if (rv < SS_SUCCESS) {
			if (f & CS_VERBOSE)
				print_st_err(device, os, ns, rv);
			goto abort;
		}

		if (drbd_send_state_req(first_peer_device(device), mask, val)) {
			rv = SS_CW_FAILED_BY_PEER;
			if (f & CS_VERBOSE)
				print_st_err(device, os, ns, rv);
			goto abort;
		}

		wait_event(device->state_wait,
			(rv = _req_st_cond(device, mask, val)));

		if (rv < SS_SUCCESS) {
			if (f & CS_VERBOSE)
				print_st_err(device, os, ns, rv);
			goto abort;
		}
		spin_lock_irqsave(&device->resource->req_lock, flags);
		ns = apply_mask_val(drbd_read_state(device), mask, val);
		rv = _drbd_set_state(device, ns, f, &done);
	} else {
		rv = _drbd_set_state(device, ns, f, &done);
	}

	spin_unlock_irqrestore(&device->resource->req_lock, flags);

	if (f & CS_WAIT_COMPLETE && rv == SS_SUCCESS) {
		D_ASSERT(device, current != first_peer_device(device)->connection->worker.task);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		wait_for_completion(&done);
	}

abort:
	if (f & CS_SERIALIZE)
<<<<<<< HEAD
		mutex_unlock(mdev->state_mutex);
=======
		mutex_unlock(device->state_mutex);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return rv;
}

/**
 * _drbd_request_state() - Request a state change (with flags)
<<<<<<< HEAD
 * @mdev:	DRBD device.
=======
 * @device:	DRBD device.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @mask:	mask of state bits to change.
 * @val:	value of new state bits.
 * @f:		flags
 *
 * Cousin of drbd_request_state(), useful with the CS_WAIT_COMPLETE
 * flag, or when logging of failed state change requests is not desired.
 */
enum drbd_state_rv
<<<<<<< HEAD
_drbd_request_state(struct drbd_conf *mdev, union drbd_state mask,
=======
_drbd_request_state(struct drbd_device *device, union drbd_state mask,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		    union drbd_state val, enum chg_state_flags f)
{
	enum drbd_state_rv rv;

<<<<<<< HEAD
	wait_event(mdev->state_wait,
		   (rv = drbd_req_state(mdev, mask, val, f)) != SS_IN_TRANSIENT_STATE);
=======
	wait_event(device->state_wait,
		   (rv = drbd_req_state(device, mask, val, f)) != SS_IN_TRANSIENT_STATE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return rv;
}

<<<<<<< HEAD
static void print_st(struct drbd_conf *mdev, char *name, union drbd_state ns)
{
	dev_err(DEV, " %s = { cs:%s ro:%s/%s ds:%s/%s %c%c%c%c%c%c }\n",
=======
enum drbd_state_rv
_drbd_request_state_holding_state_mutex(struct drbd_device *device, union drbd_state mask,
		    union drbd_state val, enum chg_state_flags f)
{
	enum drbd_state_rv rv;

	BUG_ON(f & CS_SERIALIZE);

	wait_event_cmd(device->state_wait,
		       (rv = drbd_req_state(device, mask, val, f)) != SS_IN_TRANSIENT_STATE,
		       mutex_unlock(device->state_mutex),
		       mutex_lock(device->state_mutex));

	return rv;
}

static void print_st(struct drbd_device *device, const char *name, union drbd_state ns)
{
	drbd_err(device, " %s = { cs:%s ro:%s/%s ds:%s/%s %c%c%c%c%c%c }\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	    name,
	    drbd_conn_str(ns.conn),
	    drbd_role_str(ns.role),
	    drbd_role_str(ns.peer),
	    drbd_disk_str(ns.disk),
	    drbd_disk_str(ns.pdsk),
	    is_susp(ns) ? 's' : 'r',
	    ns.aftr_isp ? 'a' : '-',
	    ns.peer_isp ? 'p' : '-',
	    ns.user_isp ? 'u' : '-',
	    ns.susp_fen ? 'F' : '-',
	    ns.susp_nod ? 'N' : '-'
	    );
}

<<<<<<< HEAD
void print_st_err(struct drbd_conf *mdev, union drbd_state os,
=======
void print_st_err(struct drbd_device *device, union drbd_state os,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	          union drbd_state ns, enum drbd_state_rv err)
{
	if (err == SS_IN_TRANSIENT_STATE)
		return;
<<<<<<< HEAD
	dev_err(DEV, "State change failed: %s\n", drbd_set_st_err_str(err));
	print_st(mdev, " state", os);
	print_st(mdev, "wanted", ns);
=======
	drbd_err(device, "State change failed: %s\n", drbd_set_st_err_str(err));
	print_st(device, " state", os);
	print_st(device, "wanted", ns);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static long print_state_change(char *pb, union drbd_state os, union drbd_state ns,
			       enum chg_state_flags flags)
{
	char *pbp;
	pbp = pb;
	*pbp = 0;

	if (ns.role != os.role && flags & CS_DC_ROLE)
		pbp += sprintf(pbp, "role( %s -> %s ) ",
			       drbd_role_str(os.role),
			       drbd_role_str(ns.role));
	if (ns.peer != os.peer && flags & CS_DC_PEER)
		pbp += sprintf(pbp, "peer( %s -> %s ) ",
			       drbd_role_str(os.peer),
			       drbd_role_str(ns.peer));
	if (ns.conn != os.conn && flags & CS_DC_CONN)
		pbp += sprintf(pbp, "conn( %s -> %s ) ",
			       drbd_conn_str(os.conn),
			       drbd_conn_str(ns.conn));
	if (ns.disk != os.disk && flags & CS_DC_DISK)
		pbp += sprintf(pbp, "disk( %s -> %s ) ",
			       drbd_disk_str(os.disk),
			       drbd_disk_str(ns.disk));
	if (ns.pdsk != os.pdsk && flags & CS_DC_PDSK)
		pbp += sprintf(pbp, "pdsk( %s -> %s ) ",
			       drbd_disk_str(os.pdsk),
			       drbd_disk_str(ns.pdsk));

	return pbp - pb;
}

<<<<<<< HEAD
static void drbd_pr_state_change(struct drbd_conf *mdev, union drbd_state os, union drbd_state ns,
=======
static void drbd_pr_state_change(struct drbd_device *device, union drbd_state os, union drbd_state ns,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				 enum chg_state_flags flags)
{
	char pb[300];
	char *pbp = pb;

	pbp += print_state_change(pbp, os, ns, flags ^ CS_DC_MASK);

	if (ns.aftr_isp != os.aftr_isp)
		pbp += sprintf(pbp, "aftr_isp( %d -> %d ) ",
			       os.aftr_isp,
			       ns.aftr_isp);
	if (ns.peer_isp != os.peer_isp)
		pbp += sprintf(pbp, "peer_isp( %d -> %d ) ",
			       os.peer_isp,
			       ns.peer_isp);
	if (ns.user_isp != os.user_isp)
		pbp += sprintf(pbp, "user_isp( %d -> %d ) ",
			       os.user_isp,
			       ns.user_isp);

	if (pbp != pb)
<<<<<<< HEAD
		dev_info(DEV, "%s\n", pb);
}

static void conn_pr_state_change(struct drbd_tconn *tconn, union drbd_state os, union drbd_state ns,
=======
		drbd_info(device, "%s\n", pb);
}

static void conn_pr_state_change(struct drbd_connection *connection, union drbd_state os, union drbd_state ns,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				 enum chg_state_flags flags)
{
	char pb[300];
	char *pbp = pb;

	pbp += print_state_change(pbp, os, ns, flags);

	if (is_susp(ns) != is_susp(os) && flags & CS_DC_SUSP)
		pbp += sprintf(pbp, "susp( %d -> %d ) ",
			       is_susp(os),
			       is_susp(ns));

	if (pbp != pb)
<<<<<<< HEAD
		conn_info(tconn, "%s\n", pb);
=======
		drbd_info(connection, "%s\n", pb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}


/**
 * is_valid_state() - Returns an SS_ error code if ns is not valid
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @ns:		State to consider.
 */
static enum drbd_state_rv
is_valid_state(struct drbd_conf *mdev, union drbd_state ns)
=======
 * @device:	DRBD device.
 * @ns:		State to consider.
 */
static enum drbd_state_rv
is_valid_state(struct drbd_device *device, union drbd_state ns)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	/* See drbd_state_sw_errors in drbd_strings.c */

	enum drbd_fencing_p fp;
	enum drbd_state_rv rv = SS_SUCCESS;
	struct net_conf *nc;

	rcu_read_lock();
	fp = FP_DONT_CARE;
<<<<<<< HEAD
	if (get_ldev(mdev)) {
		fp = rcu_dereference(mdev->ldev->disk_conf)->fencing;
		put_ldev(mdev);
	}

	nc = rcu_dereference(mdev->tconn->net_conf);
=======
	if (get_ldev(device)) {
		fp = rcu_dereference(device->ldev->disk_conf)->fencing;
		put_ldev(device);
	}

	nc = rcu_dereference(first_peer_device(device)->connection->net_conf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (nc) {
		if (!nc->two_primaries && ns.role == R_PRIMARY) {
			if (ns.peer == R_PRIMARY)
				rv = SS_TWO_PRIMARIES;
<<<<<<< HEAD
			else if (conn_highest_peer(mdev->tconn) == R_PRIMARY)
=======
			else if (conn_highest_peer(first_peer_device(device)->connection) == R_PRIMARY)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				rv = SS_O_VOL_PEER_PRI;
		}
	}

	if (rv <= 0)
<<<<<<< HEAD
		/* already found a reason to abort */;
	else if (ns.role == R_SECONDARY && mdev->open_cnt)
=======
		goto out; /* already found a reason to abort */
	else if (ns.role == R_SECONDARY && device->open_cnt)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rv = SS_DEVICE_IN_USE;

	else if (ns.role == R_PRIMARY && ns.conn < C_CONNECTED && ns.disk < D_UP_TO_DATE)
		rv = SS_NO_UP_TO_DATE_DISK;

	else if (fp >= FP_RESOURCE &&
		 ns.role == R_PRIMARY && ns.conn < C_CONNECTED && ns.pdsk >= D_UNKNOWN)
		rv = SS_PRIMARY_NOP;

	else if (ns.role == R_PRIMARY && ns.disk <= D_INCONSISTENT && ns.pdsk <= D_INCONSISTENT)
		rv = SS_NO_UP_TO_DATE_DISK;

	else if (ns.conn > C_CONNECTED && ns.disk < D_INCONSISTENT)
		rv = SS_NO_LOCAL_DISK;

	else if (ns.conn > C_CONNECTED && ns.pdsk < D_INCONSISTENT)
		rv = SS_NO_REMOTE_DISK;

	else if (ns.conn > C_CONNECTED && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE)
		rv = SS_NO_UP_TO_DATE_DISK;

	else if ((ns.conn == C_CONNECTED ||
		  ns.conn == C_WF_BITMAP_S ||
		  ns.conn == C_SYNC_SOURCE ||
		  ns.conn == C_PAUSED_SYNC_S) &&
		  ns.disk == D_OUTDATED)
		rv = SS_CONNECTED_OUTDATES;

	else if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) &&
		 (nc->verify_alg[0] == 0))
		rv = SS_NO_VERIFY_ALG;

	else if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) &&
<<<<<<< HEAD
		  mdev->tconn->agreed_pro_version < 88)
=======
		  first_peer_device(device)->connection->agreed_pro_version < 88)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rv = SS_NOT_SUPPORTED;

	else if (ns.role == R_PRIMARY && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE)
		rv = SS_NO_UP_TO_DATE_DISK;

	else if ((ns.conn == C_STARTING_SYNC_S || ns.conn == C_STARTING_SYNC_T) &&
                 ns.pdsk == D_UNKNOWN)
		rv = SS_NEED_CONNECTION;

	else if (ns.conn >= C_CONNECTED && ns.pdsk == D_UNKNOWN)
		rv = SS_CONNECTED_OUTDATES;

<<<<<<< HEAD
=======
out:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_read_unlock();

	return rv;
}

/**
 * is_valid_soft_transition() - Returns an SS_ error code if the state transition is not possible
 * This function limits state transitions that may be declined by DRBD. I.e.
 * user requests (aka soft transitions).
<<<<<<< HEAD
 * @mdev:	DRBD device.
=======
 * @device:	DRBD device.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @ns:		new state.
 * @os:		old state.
 */
static enum drbd_state_rv
<<<<<<< HEAD
is_valid_soft_transition(union drbd_state os, union drbd_state ns, struct drbd_tconn *tconn)
=======
is_valid_soft_transition(union drbd_state os, union drbd_state ns, struct drbd_connection *connection)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	enum drbd_state_rv rv = SS_SUCCESS;

	if ((ns.conn == C_STARTING_SYNC_T || ns.conn == C_STARTING_SYNC_S) &&
	    os.conn > C_CONNECTED)
		rv = SS_RESYNC_RUNNING;

	if (ns.conn == C_DISCONNECTING && os.conn == C_STANDALONE)
		rv = SS_ALREADY_STANDALONE;

	if (ns.disk > D_ATTACHING && os.disk == D_DISKLESS)
		rv = SS_IS_DISKLESS;

	if (ns.conn == C_WF_CONNECTION && os.conn < C_UNCONNECTED)
		rv = SS_NO_NET_CONFIG;

	if (ns.disk == D_OUTDATED && os.disk < D_OUTDATED && os.disk != D_ATTACHING)
		rv = SS_LOWER_THAN_OUTDATED;

	if (ns.conn == C_DISCONNECTING && os.conn == C_UNCONNECTED)
		rv = SS_IN_TRANSIENT_STATE;

<<<<<<< HEAD
	/* if (ns.conn == os.conn && ns.conn == C_WF_REPORT_PARAMS)
	   rv = SS_IN_TRANSIENT_STATE; */

	/* While establishing a connection only allow cstate to change.
	   Delay/refuse role changes, detach attach etc... */
	if (test_bit(STATE_SENT, &tconn->flags) &&
	    !(os.conn == C_WF_REPORT_PARAMS ||
	      (ns.conn == C_WF_REPORT_PARAMS && os.conn == C_WF_CONNECTION)))
		rv = SS_IN_TRANSIENT_STATE;

=======
	/* While establishing a connection only allow cstate to change.
	   Delay/refuse role changes, detach attach etc... (they do not touch cstate) */
	if (test_bit(STATE_SENT, &connection->flags) &&
	    !((ns.conn == C_WF_REPORT_PARAMS && os.conn == C_WF_CONNECTION) ||
	      (ns.conn >= C_CONNECTED && os.conn == C_WF_REPORT_PARAMS)))
		rv = SS_IN_TRANSIENT_STATE;

	/* Do not promote during resync handshake triggered by "force primary".
	 * This is a hack. It should really be rejected by the peer during the
	 * cluster wide state change request. */
	if (os.role != R_PRIMARY && ns.role == R_PRIMARY
		&& ns.pdsk == D_UP_TO_DATE
		&& ns.disk != D_UP_TO_DATE && ns.disk != D_DISKLESS
		&& (ns.conn <= C_WF_SYNC_UUID || ns.conn != os.conn))
			rv = SS_IN_TRANSIENT_STATE;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) && os.conn < C_CONNECTED)
		rv = SS_NEED_CONNECTION;

	if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) &&
	    ns.conn != os.conn && os.conn > C_CONNECTED)
		rv = SS_RESYNC_RUNNING;

	if ((ns.conn == C_STARTING_SYNC_S || ns.conn == C_STARTING_SYNC_T) &&
	    os.conn < C_CONNECTED)
		rv = SS_NEED_CONNECTION;

	if ((ns.conn == C_SYNC_TARGET || ns.conn == C_SYNC_SOURCE)
	    && os.conn < C_WF_REPORT_PARAMS)
		rv = SS_NEED_CONNECTION; /* No NetworkFailure -> SyncTarget etc... */

	if (ns.conn == C_DISCONNECTING && ns.pdsk == D_OUTDATED &&
	    os.conn < C_CONNECTED && os.pdsk > D_OUTDATED)
		rv = SS_OUTDATE_WO_CONN;

	return rv;
}

static enum drbd_state_rv
is_valid_conn_transition(enum drbd_conns oc, enum drbd_conns nc)
{
	/* no change -> nothing to do, at least for the connection part */
	if (oc == nc)
		return SS_NOTHING_TO_DO;

	/* disconnect of an unconfigured connection does not make sense */
	if (oc == C_STANDALONE && nc == C_DISCONNECTING)
		return SS_ALREADY_STANDALONE;

	/* from C_STANDALONE, we start with C_UNCONNECTED */
	if (oc == C_STANDALONE && nc != C_UNCONNECTED)
		return SS_NEED_CONNECTION;

	/* When establishing a connection we need to go through WF_REPORT_PARAMS!
	   Necessary to do the right thing upon invalidate-remote on a disconnected resource */
	if (oc < C_WF_REPORT_PARAMS && nc >= C_CONNECTED)
		return SS_NEED_CONNECTION;

	/* After a network error only C_UNCONNECTED or C_DISCONNECTING may follow. */
	if (oc >= C_TIMEOUT && oc <= C_TEAR_DOWN && nc != C_UNCONNECTED && nc != C_DISCONNECTING)
		return SS_IN_TRANSIENT_STATE;

	/* After C_DISCONNECTING only C_STANDALONE may follow */
	if (oc == C_DISCONNECTING && nc != C_STANDALONE)
		return SS_IN_TRANSIENT_STATE;

	return SS_SUCCESS;
}


/**
 * is_valid_transition() - Returns an SS_ error code if the state transition is not possible
 * This limits hard state transitions. Hard state transitions are facts there are
 * imposed on DRBD by the environment. E.g. disk broke or network broke down.
 * But those hard state transitions are still not allowed to do everything.
 * @ns:		new state.
 * @os:		old state.
 */
static enum drbd_state_rv
is_valid_transition(union drbd_state os, union drbd_state ns)
{
	enum drbd_state_rv rv;

	rv = is_valid_conn_transition(os.conn, ns.conn);

	/* we cannot fail (again) if we already detached */
	if (ns.disk == D_FAILED && os.disk == D_DISKLESS)
		rv = SS_IS_DISKLESS;

	return rv;
}

<<<<<<< HEAD
static void print_sanitize_warnings(struct drbd_conf *mdev, enum sanitize_state_warnings warn)
=======
static void print_sanitize_warnings(struct drbd_device *device, enum sanitize_state_warnings warn)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	static const char *msg_table[] = {
		[NO_WARNING] = "",
		[ABORTED_ONLINE_VERIFY] = "Online-verify aborted.",
		[ABORTED_RESYNC] = "Resync aborted.",
		[CONNECTION_LOST_NEGOTIATING] = "Connection lost while negotiating, no data!",
		[IMPLICITLY_UPGRADED_DISK] = "Implicitly upgraded disk",
		[IMPLICITLY_UPGRADED_PDSK] = "Implicitly upgraded pdsk",
	};

	if (warn != NO_WARNING)
<<<<<<< HEAD
		dev_warn(DEV, "%s\n", msg_table[warn]);
=======
		drbd_warn(device, "%s\n", msg_table[warn]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * sanitize_state() - Resolves implicitly necessary additional changes to a state transition
<<<<<<< HEAD
 * @mdev:	DRBD device.
=======
 * @device:	DRBD device.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @os:		old state.
 * @ns:		new state.
 * @warn_sync_abort:
 *
 * When we loose connection, we have to set the state of the peers disk (pdsk)
 * to D_UNKNOWN. This rule and many more along those lines are in this function.
 */
<<<<<<< HEAD
static union drbd_state sanitize_state(struct drbd_conf *mdev, union drbd_state ns,
				       enum sanitize_state_warnings *warn)
=======
static union drbd_state sanitize_state(struct drbd_device *device, union drbd_state os,
				       union drbd_state ns, enum sanitize_state_warnings *warn)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	enum drbd_fencing_p fp;
	enum drbd_disk_state disk_min, disk_max, pdsk_min, pdsk_max;

	if (warn)
		*warn = NO_WARNING;

	fp = FP_DONT_CARE;
<<<<<<< HEAD
	if (get_ldev(mdev)) {
		rcu_read_lock();
		fp = rcu_dereference(mdev->ldev->disk_conf)->fencing;
		rcu_read_unlock();
		put_ldev(mdev);
=======
	if (get_ldev(device)) {
		rcu_read_lock();
		fp = rcu_dereference(device->ldev->disk_conf)->fencing;
		rcu_read_unlock();
		put_ldev(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Implications from connection to peer and peer_isp */
	if (ns.conn < C_CONNECTED) {
		ns.peer_isp = 0;
		ns.peer = R_UNKNOWN;
		if (ns.pdsk > D_UNKNOWN || ns.pdsk < D_INCONSISTENT)
			ns.pdsk = D_UNKNOWN;
	}

	/* Clear the aftr_isp when becoming unconfigured */
	if (ns.conn == C_STANDALONE && ns.disk == D_DISKLESS && ns.role == R_SECONDARY)
		ns.aftr_isp = 0;

	/* An implication of the disk states onto the connection state */
	/* Abort resync if a disk fails/detaches */
	if (ns.conn > C_CONNECTED && (ns.disk <= D_FAILED || ns.pdsk <= D_FAILED)) {
		if (warn)
			*warn = ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T ?
				ABORTED_ONLINE_VERIFY : ABORTED_RESYNC;
		ns.conn = C_CONNECTED;
	}

	/* Connection breaks down before we finished "Negotiating" */
	if (ns.conn < C_CONNECTED && ns.disk == D_NEGOTIATING &&
<<<<<<< HEAD
	    get_ldev_if_state(mdev, D_NEGOTIATING)) {
		if (mdev->ed_uuid == mdev->ldev->md.uuid[UI_CURRENT]) {
			ns.disk = mdev->new_state_tmp.disk;
			ns.pdsk = mdev->new_state_tmp.pdsk;
=======
	    get_ldev_if_state(device, D_NEGOTIATING)) {
		if (device->ed_uuid == device->ldev->md.uuid[UI_CURRENT]) {
			ns.disk = device->new_state_tmp.disk;
			ns.pdsk = device->new_state_tmp.pdsk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		} else {
			if (warn)
				*warn = CONNECTION_LOST_NEGOTIATING;
			ns.disk = D_DISKLESS;
			ns.pdsk = D_UNKNOWN;
		}
<<<<<<< HEAD
		put_ldev(mdev);
=======
		put_ldev(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* D_CONSISTENT and D_OUTDATED vanish when we get connected */
	if (ns.conn >= C_CONNECTED && ns.conn < C_AHEAD) {
		if (ns.disk == D_CONSISTENT || ns.disk == D_OUTDATED)
			ns.disk = D_UP_TO_DATE;
		if (ns.pdsk == D_CONSISTENT || ns.pdsk == D_OUTDATED)
			ns.pdsk = D_UP_TO_DATE;
	}

	/* Implications of the connection stat on the disk states */
	disk_min = D_DISKLESS;
	disk_max = D_UP_TO_DATE;
	pdsk_min = D_INCONSISTENT;
	pdsk_max = D_UNKNOWN;
	switch ((enum drbd_conns)ns.conn) {
	case C_WF_BITMAP_T:
	case C_PAUSED_SYNC_T:
	case C_STARTING_SYNC_T:
	case C_WF_SYNC_UUID:
	case C_BEHIND:
		disk_min = D_INCONSISTENT;
		disk_max = D_OUTDATED;
		pdsk_min = D_UP_TO_DATE;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_VERIFY_S:
	case C_VERIFY_T:
		disk_min = D_UP_TO_DATE;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_UP_TO_DATE;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_CONNECTED:
		disk_min = D_DISKLESS;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_DISKLESS;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_WF_BITMAP_S:
	case C_PAUSED_SYNC_S:
	case C_STARTING_SYNC_S:
	case C_AHEAD:
		disk_min = D_UP_TO_DATE;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_INCONSISTENT;
		pdsk_max = D_CONSISTENT; /* D_OUTDATED would be nice. But explicit outdate necessary*/
		break;
	case C_SYNC_TARGET:
		disk_min = D_INCONSISTENT;
		disk_max = D_INCONSISTENT;
		pdsk_min = D_UP_TO_DATE;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_SYNC_SOURCE:
		disk_min = D_UP_TO_DATE;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_INCONSISTENT;
		pdsk_max = D_INCONSISTENT;
		break;
	case C_STANDALONE:
	case C_DISCONNECTING:
	case C_UNCONNECTED:
	case C_TIMEOUT:
	case C_BROKEN_PIPE:
	case C_NETWORK_FAILURE:
	case C_PROTOCOL_ERROR:
	case C_TEAR_DOWN:
	case C_WF_CONNECTION:
	case C_WF_REPORT_PARAMS:
	case C_MASK:
		break;
	}
	if (ns.disk > disk_max)
		ns.disk = disk_max;

	if (ns.disk < disk_min) {
		if (warn)
			*warn = IMPLICITLY_UPGRADED_DISK;
		ns.disk = disk_min;
	}
	if (ns.pdsk > pdsk_max)
		ns.pdsk = pdsk_max;

	if (ns.pdsk < pdsk_min) {
		if (warn)
			*warn = IMPLICITLY_UPGRADED_PDSK;
		ns.pdsk = pdsk_min;
	}

	if (fp == FP_STONITH &&
<<<<<<< HEAD
	    (ns.role == R_PRIMARY && ns.conn < C_CONNECTED && ns.pdsk > D_OUTDATED))
		ns.susp_fen = 1; /* Suspend IO while fence-peer handler runs (peer lost) */

	if (mdev->tconn->res_opts.on_no_data == OND_SUSPEND_IO &&
	    (ns.role == R_PRIMARY && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE))
=======
	    (ns.role == R_PRIMARY && ns.conn < C_CONNECTED && ns.pdsk > D_OUTDATED) &&
	    !(os.role == R_PRIMARY && os.conn < C_CONNECTED && os.pdsk > D_OUTDATED))
		ns.susp_fen = 1; /* Suspend IO while fence-peer handler runs (peer lost) */

	if (device->resource->res_opts.on_no_data == OND_SUSPEND_IO &&
	    (ns.role == R_PRIMARY && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE) &&
	    !(os.role == R_PRIMARY && os.disk < D_UP_TO_DATE && os.pdsk < D_UP_TO_DATE))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ns.susp_nod = 1; /* Suspend IO while no data available (no accessible data available) */

	if (ns.aftr_isp || ns.peer_isp || ns.user_isp) {
		if (ns.conn == C_SYNC_SOURCE)
			ns.conn = C_PAUSED_SYNC_S;
		if (ns.conn == C_SYNC_TARGET)
			ns.conn = C_PAUSED_SYNC_T;
	} else {
		if (ns.conn == C_PAUSED_SYNC_S)
			ns.conn = C_SYNC_SOURCE;
		if (ns.conn == C_PAUSED_SYNC_T)
			ns.conn = C_SYNC_TARGET;
	}

	return ns;
}

<<<<<<< HEAD
void drbd_resume_al(struct drbd_conf *mdev)
{
	if (test_and_clear_bit(AL_SUSPENDED, &mdev->flags))
		dev_info(DEV, "Resumed AL updates\n");
}

/* helper for __drbd_set_state */
static void set_ov_position(struct drbd_conf *mdev, enum drbd_conns cs)
{
	if (mdev->tconn->agreed_pro_version < 90)
		mdev->ov_start_sector = 0;
	mdev->rs_total = drbd_bm_bits(mdev);
	mdev->ov_position = 0;
=======
void drbd_resume_al(struct drbd_device *device)
{
	if (test_and_clear_bit(AL_SUSPENDED, &device->flags))
		drbd_info(device, "Resumed AL updates\n");
}

/* helper for _drbd_set_state */
static void set_ov_position(struct drbd_device *device, enum drbd_conns cs)
{
	if (first_peer_device(device)->connection->agreed_pro_version < 90)
		device->ov_start_sector = 0;
	device->rs_total = drbd_bm_bits(device);
	device->ov_position = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (cs == C_VERIFY_T) {
		/* starting online verify from an arbitrary position
		 * does not fit well into the existing protocol.
		 * on C_VERIFY_T, we initialize ov_left and friends
		 * implicitly in receive_DataRequest once the
		 * first P_OV_REQUEST is received */
<<<<<<< HEAD
		mdev->ov_start_sector = ~(sector_t)0;
	} else {
		unsigned long bit = BM_SECT_TO_BIT(mdev->ov_start_sector);
		if (bit >= mdev->rs_total) {
			mdev->ov_start_sector =
				BM_BIT_TO_SECT(mdev->rs_total - 1);
			mdev->rs_total = 1;
		} else
			mdev->rs_total -= bit;
		mdev->ov_position = mdev->ov_start_sector;
	}
	mdev->ov_left = mdev->rs_total;
}

/**
 * __drbd_set_state() - Set a new DRBD state
 * @mdev:	DRBD device.
=======
		device->ov_start_sector = ~(sector_t)0;
	} else {
		unsigned long bit = BM_SECT_TO_BIT(device->ov_start_sector);
		if (bit >= device->rs_total) {
			device->ov_start_sector =
				BM_BIT_TO_SECT(device->rs_total - 1);
			device->rs_total = 1;
		} else
			device->rs_total -= bit;
		device->ov_position = device->ov_start_sector;
	}
	device->ov_left = device->rs_total;
}

/**
 * _drbd_set_state() - Set a new DRBD state
 * @device:	DRBD device.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @ns:		new state.
 * @flags:	Flags
 * @done:	Optional completion, that will get completed after the after_state_ch() finished
 *
<<<<<<< HEAD
 * Caller needs to hold req_lock, and global_state_lock. Do not call directly.
 */
enum drbd_state_rv
__drbd_set_state(struct drbd_conf *mdev, union drbd_state ns,
	         enum chg_state_flags flags, struct completion *done)
{
=======
 * Caller needs to hold req_lock. Do not call directly.
 */
enum drbd_state_rv
_drbd_set_state(struct drbd_device *device, union drbd_state ns,
	        enum chg_state_flags flags, struct completion *done)
{
	struct drbd_peer_device *peer_device = first_peer_device(device);
	struct drbd_connection *connection = peer_device ? peer_device->connection : NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	union drbd_state os;
	enum drbd_state_rv rv = SS_SUCCESS;
	enum sanitize_state_warnings ssw;
	struct after_state_chg_work *ascw;
<<<<<<< HEAD
	bool did_remote, should_do_remote;

	os = drbd_read_state(mdev);

	ns = sanitize_state(mdev, ns, &ssw);
=======
	struct drbd_state_change *state_change;

	os = drbd_read_state(device);

	ns = sanitize_state(device, os, ns, &ssw);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ns.i == os.i)
		return SS_NOTHING_TO_DO;

	rv = is_valid_transition(os, ns);
	if (rv < SS_SUCCESS)
		return rv;

	if (!(flags & CS_HARD)) {
		/*  pre-state-change checks ; only look at ns  */
		/* See drbd_state_sw_errors in drbd_strings.c */

<<<<<<< HEAD
		rv = is_valid_state(mdev, ns);
=======
		rv = is_valid_state(device, ns);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (rv < SS_SUCCESS) {
			/* If the old state was illegal as well, then let
			   this happen...*/

<<<<<<< HEAD
			if (is_valid_state(mdev, os) == rv)
				rv = is_valid_soft_transition(os, ns, mdev->tconn);
		} else
			rv = is_valid_soft_transition(os, ns, mdev->tconn);
=======
			if (is_valid_state(device, os) == rv)
				rv = is_valid_soft_transition(os, ns, connection);
		} else
			rv = is_valid_soft_transition(os, ns, connection);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (rv < SS_SUCCESS) {
		if (flags & CS_VERBOSE)
<<<<<<< HEAD
			print_st_err(mdev, os, ns, rv);
		return rv;
	}

	print_sanitize_warnings(mdev, ssw);

	drbd_pr_state_change(mdev, os, ns, flags);
=======
			print_st_err(device, os, ns, rv);
		return rv;
	}

	print_sanitize_warnings(device, ssw);

	drbd_pr_state_change(device, os, ns, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Display changes to the susp* flags that where caused by the call to
	   sanitize_state(). Only display it here if we where not called from
	   _conn_request_state() */
	if (!(flags & CS_DC_SUSP))
<<<<<<< HEAD
		conn_pr_state_change(mdev->tconn, os, ns, (flags & ~CS_DC_MASK) | CS_DC_SUSP);
=======
		conn_pr_state_change(connection, os, ns,
				     (flags & ~CS_DC_MASK) | CS_DC_SUSP);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* if we are going -> D_FAILED or D_DISKLESS, grab one extra reference
	 * on the ldev here, to be sure the transition -> D_DISKLESS resp.
	 * drbd_ldev_destroy() won't happen before our corresponding
	 * after_state_ch works run, where we put_ldev again. */
	if ((os.disk != D_FAILED && ns.disk == D_FAILED) ||
	    (os.disk != D_DISKLESS && ns.disk == D_DISKLESS))
<<<<<<< HEAD
		atomic_inc(&mdev->local_cnt);

	did_remote = drbd_should_do_remote(mdev->state);
	mdev->state.i = ns.i;
	should_do_remote = drbd_should_do_remote(mdev->state);
	mdev->tconn->susp = ns.susp;
	mdev->tconn->susp_nod = ns.susp_nod;
	mdev->tconn->susp_fen = ns.susp_fen;

	/* put replicated vs not-replicated requests in seperate epochs */
	if (did_remote != should_do_remote)
		start_new_tl_epoch(mdev->tconn);

	if (os.disk == D_ATTACHING && ns.disk >= D_NEGOTIATING)
		drbd_print_uuids(mdev, "attached to UUIDs");

	/* Wake up role changes, that were delayed because of connection establishing */
	if (os.conn == C_WF_REPORT_PARAMS && ns.conn != C_WF_REPORT_PARAMS &&
	    no_peer_wf_report_params(mdev->tconn))
		clear_bit(STATE_SENT, &mdev->tconn->flags);

	wake_up(&mdev->misc_wait);
	wake_up(&mdev->state_wait);
	wake_up(&mdev->tconn->ping_wait);
=======
		atomic_inc(&device->local_cnt);

	if (!is_sync_state(os.conn) && is_sync_state(ns.conn))
		clear_bit(RS_DONE, &device->flags);

	/* FIXME: Have any flags been set earlier in this function already? */
	state_change = remember_old_state(device->resource, GFP_ATOMIC);

	/* changes to local_cnt and device flags should be visible before
	 * changes to state, which again should be visible before anything else
	 * depending on that change happens. */
	smp_wmb();
	device->state.i = ns.i;
	device->resource->susp = ns.susp;
	device->resource->susp_nod = ns.susp_nod;
	device->resource->susp_fen = ns.susp_fen;
	smp_wmb();

	remember_new_state(state_change);

	/* put replicated vs not-replicated requests in seperate epochs */
	if (drbd_should_do_remote((union drbd_dev_state)os.i) !=
	    drbd_should_do_remote((union drbd_dev_state)ns.i))
		start_new_tl_epoch(connection);

	if (os.disk == D_ATTACHING && ns.disk >= D_NEGOTIATING)
		drbd_print_uuids(device, "attached to UUIDs");

	/* Wake up role changes, that were delayed because of connection establishing */
	if (os.conn == C_WF_REPORT_PARAMS && ns.conn != C_WF_REPORT_PARAMS &&
	    no_peer_wf_report_params(connection)) {
		clear_bit(STATE_SENT, &connection->flags);
		wake_up_all_devices(connection);
	}

	wake_up(&device->misc_wait);
	wake_up(&device->state_wait);
	wake_up(&connection->ping_wait);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Aborted verify run, or we reached the stop sector.
	 * Log the last position, unless end-of-device. */
	if ((os.conn == C_VERIFY_S || os.conn == C_VERIFY_T) &&
	    ns.conn <= C_CONNECTED) {
<<<<<<< HEAD
		mdev->ov_start_sector =
			BM_BIT_TO_SECT(drbd_bm_bits(mdev) - mdev->ov_left);
		if (mdev->ov_left)
			dev_info(DEV, "Online Verify reached sector %llu\n",
				(unsigned long long)mdev->ov_start_sector);
=======
		device->ov_start_sector =
			BM_BIT_TO_SECT(drbd_bm_bits(device) - device->ov_left);
		if (device->ov_left)
			drbd_info(device, "Online Verify reached sector %llu\n",
				(unsigned long long)device->ov_start_sector);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if ((os.conn == C_PAUSED_SYNC_T || os.conn == C_PAUSED_SYNC_S) &&
	    (ns.conn == C_SYNC_TARGET  || ns.conn == C_SYNC_SOURCE)) {
<<<<<<< HEAD
		dev_info(DEV, "Syncer continues.\n");
		mdev->rs_paused += (long)jiffies
				  -(long)mdev->rs_mark_time[mdev->rs_last_mark];
		if (ns.conn == C_SYNC_TARGET)
			mod_timer(&mdev->resync_timer, jiffies);
=======
		drbd_info(device, "Syncer continues.\n");
		device->rs_paused += (long)jiffies
				  -(long)device->rs_mark_time[device->rs_last_mark];
		if (ns.conn == C_SYNC_TARGET)
			mod_timer(&device->resync_timer, jiffies);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if ((os.conn == C_SYNC_TARGET  || os.conn == C_SYNC_SOURCE) &&
	    (ns.conn == C_PAUSED_SYNC_T || ns.conn == C_PAUSED_SYNC_S)) {
<<<<<<< HEAD
		dev_info(DEV, "Resync suspended\n");
		mdev->rs_mark_time[mdev->rs_last_mark] = jiffies;
=======
		drbd_info(device, "Resync suspended\n");
		device->rs_mark_time[device->rs_last_mark] = jiffies;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (os.conn == C_CONNECTED &&
	    (ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T)) {
		unsigned long now = jiffies;
		int i;

<<<<<<< HEAD
		set_ov_position(mdev, ns.conn);
		mdev->rs_start = now;
		mdev->rs_last_events = 0;
		mdev->rs_last_sect_ev = 0;
		mdev->ov_last_oos_size = 0;
		mdev->ov_last_oos_start = 0;

		for (i = 0; i < DRBD_SYNC_MARKS; i++) {
			mdev->rs_mark_left[i] = mdev->ov_left;
			mdev->rs_mark_time[i] = now;
		}

		drbd_rs_controller_reset(mdev);

		if (ns.conn == C_VERIFY_S) {
			dev_info(DEV, "Starting Online Verify from sector %llu\n",
					(unsigned long long)mdev->ov_position);
			mod_timer(&mdev->resync_timer, jiffies);
		}
	}

	if (get_ldev(mdev)) {
		u32 mdf = mdev->ldev->md.flags & ~(MDF_CONSISTENT|MDF_PRIMARY_IND|
=======
		set_ov_position(device, ns.conn);
		device->rs_start = now;
		device->rs_last_sect_ev = 0;
		device->ov_last_oos_size = 0;
		device->ov_last_oos_start = 0;

		for (i = 0; i < DRBD_SYNC_MARKS; i++) {
			device->rs_mark_left[i] = device->ov_left;
			device->rs_mark_time[i] = now;
		}

		drbd_rs_controller_reset(device);

		if (ns.conn == C_VERIFY_S) {
			drbd_info(device, "Starting Online Verify from sector %llu\n",
					(unsigned long long)device->ov_position);
			mod_timer(&device->resync_timer, jiffies);
		}
	}

	if (get_ldev(device)) {
		u32 mdf = device->ldev->md.flags & ~(MDF_CONSISTENT|MDF_PRIMARY_IND|
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						 MDF_CONNECTED_IND|MDF_WAS_UP_TO_DATE|
						 MDF_PEER_OUT_DATED|MDF_CRASHED_PRIMARY);

		mdf &= ~MDF_AL_CLEAN;
<<<<<<< HEAD
		if (test_bit(CRASHED_PRIMARY, &mdev->flags))
			mdf |= MDF_CRASHED_PRIMARY;
		if (mdev->state.role == R_PRIMARY ||
		    (mdev->state.pdsk < D_INCONSISTENT && mdev->state.peer == R_PRIMARY))
			mdf |= MDF_PRIMARY_IND;
		if (mdev->state.conn > C_WF_REPORT_PARAMS)
			mdf |= MDF_CONNECTED_IND;
		if (mdev->state.disk > D_INCONSISTENT)
			mdf |= MDF_CONSISTENT;
		if (mdev->state.disk > D_OUTDATED)
			mdf |= MDF_WAS_UP_TO_DATE;
		if (mdev->state.pdsk <= D_OUTDATED && mdev->state.pdsk >= D_INCONSISTENT)
			mdf |= MDF_PEER_OUT_DATED;
		if (mdf != mdev->ldev->md.flags) {
			mdev->ldev->md.flags = mdf;
			drbd_md_mark_dirty(mdev);
		}
		if (os.disk < D_CONSISTENT && ns.disk >= D_CONSISTENT)
			drbd_set_ed_uuid(mdev, mdev->ldev->md.uuid[UI_CURRENT]);
		put_ldev(mdev);
=======
		if (test_bit(CRASHED_PRIMARY, &device->flags))
			mdf |= MDF_CRASHED_PRIMARY;
		if (device->state.role == R_PRIMARY ||
		    (device->state.pdsk < D_INCONSISTENT && device->state.peer == R_PRIMARY))
			mdf |= MDF_PRIMARY_IND;
		if (device->state.conn > C_WF_REPORT_PARAMS)
			mdf |= MDF_CONNECTED_IND;
		if (device->state.disk > D_INCONSISTENT)
			mdf |= MDF_CONSISTENT;
		if (device->state.disk > D_OUTDATED)
			mdf |= MDF_WAS_UP_TO_DATE;
		if (device->state.pdsk <= D_OUTDATED && device->state.pdsk >= D_INCONSISTENT)
			mdf |= MDF_PEER_OUT_DATED;
		if (mdf != device->ldev->md.flags) {
			device->ldev->md.flags = mdf;
			drbd_md_mark_dirty(device);
		}
		if (os.disk < D_CONSISTENT && ns.disk >= D_CONSISTENT)
			drbd_set_ed_uuid(device, device->ldev->md.uuid[UI_CURRENT]);
		put_ldev(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Peer was forced D_UP_TO_DATE & R_PRIMARY, consider to resync */
	if (os.disk == D_INCONSISTENT && os.pdsk == D_INCONSISTENT &&
	    os.peer == R_SECONDARY && ns.peer == R_PRIMARY)
<<<<<<< HEAD
		set_bit(CONSIDER_RESYNC, &mdev->flags);

	/* Receiver should clean up itself */
	if (os.conn != C_DISCONNECTING && ns.conn == C_DISCONNECTING)
		drbd_thread_stop_nowait(&mdev->tconn->receiver);

	/* Now the receiver finished cleaning up itself, it should die */
	if (os.conn != C_STANDALONE && ns.conn == C_STANDALONE)
		drbd_thread_stop_nowait(&mdev->tconn->receiver);
=======
		set_bit(CONSIDER_RESYNC, &device->flags);

	/* Receiver should clean up itself */
	if (os.conn != C_DISCONNECTING && ns.conn == C_DISCONNECTING)
		drbd_thread_stop_nowait(&connection->receiver);

	/* Now the receiver finished cleaning up itself, it should die */
	if (os.conn != C_STANDALONE && ns.conn == C_STANDALONE)
		drbd_thread_stop_nowait(&connection->receiver);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Upon network failure, we need to restart the receiver. */
	if (os.conn > C_WF_CONNECTION &&
	    ns.conn <= C_TEAR_DOWN && ns.conn >= C_TIMEOUT)
<<<<<<< HEAD
		drbd_thread_restart_nowait(&mdev->tconn->receiver);

	/* Resume AL writing if we get a connection */
	if (os.conn < C_CONNECTED && ns.conn >= C_CONNECTED)
		drbd_resume_al(mdev);
=======
		drbd_thread_restart_nowait(&connection->receiver);

	/* Resume AL writing if we get a connection */
	if (os.conn < C_CONNECTED && ns.conn >= C_CONNECTED) {
		drbd_resume_al(device);
		connection->connect_cnt++;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* remember last attach time so request_timer_fn() won't
	 * kill newly established sessions while we are still trying to thaw
	 * previously frozen IO */
	if ((os.disk == D_ATTACHING || os.disk == D_NEGOTIATING) &&
	    ns.disk > D_NEGOTIATING)
<<<<<<< HEAD
		mdev->last_reattach_jif = jiffies;
=======
		device->last_reattach_jif = jiffies;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ascw = kmalloc(sizeof(*ascw), GFP_ATOMIC);
	if (ascw) {
		ascw->os = os;
		ascw->ns = ns;
		ascw->flags = flags;
		ascw->w.cb = w_after_state_ch;
<<<<<<< HEAD
		ascw->w.mdev = mdev;
		ascw->done = done;
		drbd_queue_work(&mdev->tconn->sender_work, &ascw->w);
	} else {
		dev_err(DEV, "Could not kmalloc an ascw\n");
=======
		ascw->device = device;
		ascw->done = done;
		ascw->state_change = state_change;
		drbd_queue_work(&connection->sender_work,
				&ascw->w);
	} else {
		drbd_err(device, "Could not kmalloc an ascw\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return rv;
}

static int w_after_state_ch(struct drbd_work *w, int unused)
{
	struct after_state_chg_work *ascw =
		container_of(w, struct after_state_chg_work, w);
<<<<<<< HEAD
	struct drbd_conf *mdev = w->mdev;

	after_state_ch(mdev, ascw->os, ascw->ns, ascw->flags);
	if (ascw->flags & CS_WAIT_COMPLETE) {
		D_ASSERT(ascw->done != NULL);
		complete(ascw->done);
	}
=======
	struct drbd_device *device = ascw->device;

	after_state_ch(device, ascw->os, ascw->ns, ascw->flags, ascw->state_change);
	forget_state_change(ascw->state_change);
	if (ascw->flags & CS_WAIT_COMPLETE)
		complete(ascw->done);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(ascw);

	return 0;
}

<<<<<<< HEAD
static void abw_start_sync(struct drbd_conf *mdev, int rv)
{
	if (rv) {
		dev_err(DEV, "Writing the bitmap failed not starting resync.\n");
		_drbd_request_state(mdev, NS(conn, C_CONNECTED), CS_VERBOSE);
		return;
	}

	switch (mdev->state.conn) {
	case C_STARTING_SYNC_T:
		_drbd_request_state(mdev, NS(conn, C_WF_SYNC_UUID), CS_VERBOSE);
		break;
	case C_STARTING_SYNC_S:
		drbd_start_resync(mdev, C_SYNC_SOURCE);
=======
static void abw_start_sync(struct drbd_device *device, int rv)
{
	if (rv) {
		drbd_err(device, "Writing the bitmap failed not starting resync.\n");
		_drbd_request_state(device, NS(conn, C_CONNECTED), CS_VERBOSE);
		return;
	}

	switch (device->state.conn) {
	case C_STARTING_SYNC_T:
		_drbd_request_state(device, NS(conn, C_WF_SYNC_UUID), CS_VERBOSE);
		break;
	case C_STARTING_SYNC_S:
		drbd_start_resync(device, C_SYNC_SOURCE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	}
}

<<<<<<< HEAD
int drbd_bitmap_io_from_worker(struct drbd_conf *mdev,
		int (*io_fn)(struct drbd_conf *),
=======
int drbd_bitmap_io_from_worker(struct drbd_device *device,
		int (*io_fn)(struct drbd_device *),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		char *why, enum bm_flag flags)
{
	int rv;

<<<<<<< HEAD
	D_ASSERT(current == mdev->tconn->worker.task);

	/* open coded non-blocking drbd_suspend_io(mdev); */
	set_bit(SUSPEND_IO, &mdev->flags);

	drbd_bm_lock(mdev, why, flags);
	rv = io_fn(mdev);
	drbd_bm_unlock(mdev);

	drbd_resume_io(mdev);
=======
	D_ASSERT(device, current == first_peer_device(device)->connection->worker.task);

	/* open coded non-blocking drbd_suspend_io(device); */
	atomic_inc(&device->suspend_cnt);

	drbd_bm_lock(device, why, flags);
	rv = io_fn(device);
	drbd_bm_unlock(device);

	drbd_resume_io(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return rv;
}

<<<<<<< HEAD
/**
 * after_state_ch() - Perform after state change actions that may sleep
 * @mdev:	DRBD device.
=======
void notify_resource_state_change(struct sk_buff *skb,
				  unsigned int seq,
				  struct drbd_resource_state_change *resource_state_change,
				  enum drbd_notification_type type)
{
	struct drbd_resource *resource = resource_state_change->resource;
	struct resource_info resource_info = {
		.res_role = resource_state_change->role[NEW],
		.res_susp = resource_state_change->susp[NEW],
		.res_susp_nod = resource_state_change->susp_nod[NEW],
		.res_susp_fen = resource_state_change->susp_fen[NEW],
	};

	notify_resource_state(skb, seq, resource, &resource_info, type);
}

void notify_connection_state_change(struct sk_buff *skb,
				    unsigned int seq,
				    struct drbd_connection_state_change *connection_state_change,
				    enum drbd_notification_type type)
{
	struct drbd_connection *connection = connection_state_change->connection;
	struct connection_info connection_info = {
		.conn_connection_state = connection_state_change->cstate[NEW],
		.conn_role = connection_state_change->peer_role[NEW],
	};

	notify_connection_state(skb, seq, connection, &connection_info, type);
}

void notify_device_state_change(struct sk_buff *skb,
				unsigned int seq,
				struct drbd_device_state_change *device_state_change,
				enum drbd_notification_type type)
{
	struct drbd_device *device = device_state_change->device;
	struct device_info device_info = {
		.dev_disk_state = device_state_change->disk_state[NEW],
	};

	notify_device_state(skb, seq, device, &device_info, type);
}

void notify_peer_device_state_change(struct sk_buff *skb,
				     unsigned int seq,
				     struct drbd_peer_device_state_change *p,
				     enum drbd_notification_type type)
{
	struct drbd_peer_device *peer_device = p->peer_device;
	struct peer_device_info peer_device_info = {
		.peer_repl_state = p->repl_state[NEW],
		.peer_disk_state = p->disk_state[NEW],
		.peer_resync_susp_user = p->resync_susp_user[NEW],
		.peer_resync_susp_peer = p->resync_susp_peer[NEW],
		.peer_resync_susp_dependency = p->resync_susp_dependency[NEW],
	};

	notify_peer_device_state(skb, seq, peer_device, &peer_device_info, type);
}

static void broadcast_state_change(struct drbd_state_change *state_change)
{
	struct drbd_resource_state_change *resource_state_change = &state_change->resource[0];
	bool resource_state_has_changed;
	unsigned int n_device, n_connection, n_peer_device, n_peer_devices;
	void (*last_func)(struct sk_buff *, unsigned int, void *,
			  enum drbd_notification_type) = NULL;
	void *uninitialized_var(last_arg);

#define HAS_CHANGED(state) ((state)[OLD] != (state)[NEW])
#define FINAL_STATE_CHANGE(type) \
	({ if (last_func) \
		last_func(NULL, 0, last_arg, type); \
	})
#define REMEMBER_STATE_CHANGE(func, arg, type) \
	({ FINAL_STATE_CHANGE(type | NOTIFY_CONTINUES); \
	   last_func = (typeof(last_func))func; \
	   last_arg = arg; \
	 })

	mutex_lock(&notification_mutex);

	resource_state_has_changed =
	    HAS_CHANGED(resource_state_change->role) ||
	    HAS_CHANGED(resource_state_change->susp) ||
	    HAS_CHANGED(resource_state_change->susp_nod) ||
	    HAS_CHANGED(resource_state_change->susp_fen);

	if (resource_state_has_changed)
		REMEMBER_STATE_CHANGE(notify_resource_state_change,
				      resource_state_change, NOTIFY_CHANGE);

	for (n_connection = 0; n_connection < state_change->n_connections; n_connection++) {
		struct drbd_connection_state_change *connection_state_change =
				&state_change->connections[n_connection];

		if (HAS_CHANGED(connection_state_change->peer_role) ||
		    HAS_CHANGED(connection_state_change->cstate))
			REMEMBER_STATE_CHANGE(notify_connection_state_change,
					      connection_state_change, NOTIFY_CHANGE);
	}

	for (n_device = 0; n_device < state_change->n_devices; n_device++) {
		struct drbd_device_state_change *device_state_change =
			&state_change->devices[n_device];

		if (HAS_CHANGED(device_state_change->disk_state))
			REMEMBER_STATE_CHANGE(notify_device_state_change,
					      device_state_change, NOTIFY_CHANGE);
	}

	n_peer_devices = state_change->n_devices * state_change->n_connections;
	for (n_peer_device = 0; n_peer_device < n_peer_devices; n_peer_device++) {
		struct drbd_peer_device_state_change *p =
			&state_change->peer_devices[n_peer_device];

		if (HAS_CHANGED(p->disk_state) ||
		    HAS_CHANGED(p->repl_state) ||
		    HAS_CHANGED(p->resync_susp_user) ||
		    HAS_CHANGED(p->resync_susp_peer) ||
		    HAS_CHANGED(p->resync_susp_dependency))
			REMEMBER_STATE_CHANGE(notify_peer_device_state_change,
					      p, NOTIFY_CHANGE);
	}

	FINAL_STATE_CHANGE(NOTIFY_CHANGE);
	mutex_unlock(&notification_mutex);

#undef HAS_CHANGED
#undef FINAL_STATE_CHANGE
#undef REMEMBER_STATE_CHANGE
}

/* takes old and new peer disk state */
static bool lost_contact_to_peer_data(enum drbd_disk_state os, enum drbd_disk_state ns)
{
	if ((os >= D_INCONSISTENT && os != D_UNKNOWN && os != D_OUTDATED)
	&&  (ns < D_INCONSISTENT || ns == D_UNKNOWN || ns == D_OUTDATED))
		return true;

	/* Scenario, starting with normal operation
	 * Connected Primary/Secondary UpToDate/UpToDate
	 * NetworkFailure Primary/Unknown UpToDate/DUnknown (frozen)
	 * ...
	 * Connected Primary/Secondary UpToDate/Diskless (resumed; needs to bump uuid!)
	 */
	if (os == D_UNKNOWN
	&&  (ns == D_DISKLESS || ns == D_FAILED || ns == D_OUTDATED))
		return true;

	return false;
}

/**
 * after_state_ch() - Perform after state change actions that may sleep
 * @device:	DRBD device.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @os:		old state.
 * @ns:		new state.
 * @flags:	Flags
 */
<<<<<<< HEAD
static void after_state_ch(struct drbd_conf *mdev, union drbd_state os,
			   union drbd_state ns, enum chg_state_flags flags)
{
	struct sib_info sib;

=======
static void after_state_ch(struct drbd_device *device, union drbd_state os,
			   union drbd_state ns, enum chg_state_flags flags,
			   struct drbd_state_change *state_change)
{
	struct drbd_resource *resource = device->resource;
	struct drbd_peer_device *peer_device = first_peer_device(device);
	struct drbd_connection *connection = peer_device ? peer_device->connection : NULL;
	struct sib_info sib;

	broadcast_state_change(state_change);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sib.sib_reason = SIB_STATE_CHANGE;
	sib.os = os;
	sib.ns = ns;

<<<<<<< HEAD
	if (os.conn != C_CONNECTED && ns.conn == C_CONNECTED) {
		clear_bit(CRASHED_PRIMARY, &mdev->flags);
		if (mdev->p_uuid)
			mdev->p_uuid[UI_FLAGS] &= ~((u64)2);
	}

	/* Inform userspace about the change... */
	drbd_bcast_event(mdev, &sib);

	if (!(os.role == R_PRIMARY && os.disk < D_UP_TO_DATE && os.pdsk < D_UP_TO_DATE) &&
	    (ns.role == R_PRIMARY && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE))
		drbd_khelper(mdev, "pri-on-incon-degr");
=======
	if ((os.disk != D_UP_TO_DATE || os.pdsk != D_UP_TO_DATE)
	&&  (ns.disk == D_UP_TO_DATE && ns.pdsk == D_UP_TO_DATE)) {
		clear_bit(CRASHED_PRIMARY, &device->flags);
		if (device->p_uuid)
			device->p_uuid[UI_FLAGS] &= ~((u64)2);
	}

	/* Inform userspace about the change... */
	drbd_bcast_event(device, &sib);

	if (!(os.role == R_PRIMARY && os.disk < D_UP_TO_DATE && os.pdsk < D_UP_TO_DATE) &&
	    (ns.role == R_PRIMARY && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE))
		drbd_khelper(device, "pri-on-incon-degr");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Here we have the actions that are performed after a
	   state change. This function might sleep */

	if (ns.susp_nod) {
<<<<<<< HEAD
		struct drbd_tconn *tconn = mdev->tconn;
		enum drbd_req_event what = NOTHING;

		spin_lock_irq(&tconn->req_lock);
		if (os.conn < C_CONNECTED && conn_lowest_conn(tconn) >= C_CONNECTED)
			what = RESEND;

		if ((os.disk == D_ATTACHING || os.disk == D_NEGOTIATING) &&
		    conn_lowest_disk(tconn) > D_NEGOTIATING)
			what = RESTART_FROZEN_DISK_IO;

		if (tconn->susp_nod && what != NOTHING) {
			_tl_restart(tconn, what);
			_conn_request_state(tconn,
=======
		enum drbd_req_event what = NOTHING;

		spin_lock_irq(&device->resource->req_lock);
		if (os.conn < C_CONNECTED && conn_lowest_conn(connection) >= C_CONNECTED)
			what = RESEND;

		if ((os.disk == D_ATTACHING || os.disk == D_NEGOTIATING) &&
		    conn_lowest_disk(connection) == D_UP_TO_DATE)
			what = RESTART_FROZEN_DISK_IO;

		if (resource->susp_nod && what != NOTHING) {
			_tl_restart(connection, what);
			_conn_request_state(connection,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					    (union drbd_state) { { .susp_nod = 1 } },
					    (union drbd_state) { { .susp_nod = 0 } },
					    CS_VERBOSE);
		}
<<<<<<< HEAD
		spin_unlock_irq(&tconn->req_lock);
	}

	if (ns.susp_fen) {
		struct drbd_tconn *tconn = mdev->tconn;

		spin_lock_irq(&tconn->req_lock);
		if (tconn->susp_fen && conn_lowest_conn(tconn) >= C_CONNECTED) {
			/* case2: The connection was established again: */
			struct drbd_conf *odev;
			int vnr;

			rcu_read_lock();
			idr_for_each_entry(&tconn->volumes, odev, vnr)
				clear_bit(NEW_CUR_UUID, &odev->flags);
			rcu_read_unlock();
			_tl_restart(tconn, RESEND);
			_conn_request_state(tconn,
=======
		spin_unlock_irq(&device->resource->req_lock);
	}

	if (ns.susp_fen) {
		spin_lock_irq(&device->resource->req_lock);
		if (resource->susp_fen && conn_lowest_conn(connection) >= C_CONNECTED) {
			/* case2: The connection was established again: */
			struct drbd_peer_device *peer_device;
			int vnr;

			rcu_read_lock();
			idr_for_each_entry(&connection->peer_devices, peer_device, vnr)
				clear_bit(NEW_CUR_UUID, &peer_device->device->flags);
			rcu_read_unlock();

			/* We should actively create a new uuid, _before_
			 * we resume/resent, if the peer is diskless
			 * (recovery from a multiple error scenario).
			 * Currently, this happens with a slight delay
			 * below when checking lost_contact_to_peer_data() ...
			 */
			_tl_restart(connection, RESEND);
			_conn_request_state(connection,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					    (union drbd_state) { { .susp_fen = 1 } },
					    (union drbd_state) { { .susp_fen = 0 } },
					    CS_VERBOSE);
		}
<<<<<<< HEAD
		spin_unlock_irq(&tconn->req_lock);
=======
		spin_unlock_irq(&device->resource->req_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Became sync source.  With protocol >= 96, we still need to send out
	 * the sync uuid now. Need to do that before any drbd_send_state, or
	 * the other side may go "paused sync" before receiving the sync uuids,
	 * which is unexpected. */
	if ((os.conn != C_SYNC_SOURCE && os.conn != C_PAUSED_SYNC_S) &&
	    (ns.conn == C_SYNC_SOURCE || ns.conn == C_PAUSED_SYNC_S) &&
<<<<<<< HEAD
	    mdev->tconn->agreed_pro_version >= 96 && get_ldev(mdev)) {
		drbd_gen_and_send_sync_uuid(mdev);
		put_ldev(mdev);
=======
	    connection->agreed_pro_version >= 96 && get_ldev(device)) {
		drbd_gen_and_send_sync_uuid(peer_device);
		put_ldev(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Do not change the order of the if above and the two below... */
	if (os.pdsk == D_DISKLESS &&
	    ns.pdsk > D_DISKLESS && ns.pdsk != D_UNKNOWN) {      /* attach on the peer */
		/* we probably will start a resync soon.
		 * make sure those things are properly reset. */
<<<<<<< HEAD
		mdev->rs_total = 0;
		mdev->rs_failed = 0;
		atomic_set(&mdev->rs_pending_cnt, 0);
		drbd_rs_cancel_all(mdev);

		drbd_send_uuids(mdev);
		drbd_send_state(mdev, ns);
=======
		device->rs_total = 0;
		device->rs_failed = 0;
		atomic_set(&device->rs_pending_cnt, 0);
		drbd_rs_cancel_all(device);

		drbd_send_uuids(peer_device);
		drbd_send_state(peer_device, ns);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	/* No point in queuing send_bitmap if we don't have a connection
	 * anymore, so check also the _current_ state, not only the new state
	 * at the time this work was queued. */
	if (os.conn != C_WF_BITMAP_S && ns.conn == C_WF_BITMAP_S &&
<<<<<<< HEAD
	    mdev->state.conn == C_WF_BITMAP_S)
		drbd_queue_bitmap_io(mdev, &drbd_send_bitmap, NULL,
=======
	    device->state.conn == C_WF_BITMAP_S)
		drbd_queue_bitmap_io(device, &drbd_send_bitmap, NULL,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				"send_bitmap (WFBitMapS)",
				BM_LOCKED_TEST_ALLOWED);

	/* Lost contact to peer's copy of the data */
<<<<<<< HEAD
	if ((os.pdsk >= D_INCONSISTENT &&
	     os.pdsk != D_UNKNOWN &&
	     os.pdsk != D_OUTDATED)
	&&  (ns.pdsk < D_INCONSISTENT ||
	     ns.pdsk == D_UNKNOWN ||
	     ns.pdsk == D_OUTDATED)) {
		if (get_ldev(mdev)) {
			if ((ns.role == R_PRIMARY || ns.peer == R_PRIMARY) &&
			    mdev->ldev->md.uuid[UI_BITMAP] == 0 && ns.disk >= D_UP_TO_DATE) {
				if (drbd_suspended(mdev)) {
					set_bit(NEW_CUR_UUID, &mdev->flags);
				} else {
					drbd_uuid_new_current(mdev);
					drbd_send_uuids(mdev);
				}
			}
			put_ldev(mdev);
		}
	}

	if (ns.pdsk < D_INCONSISTENT && get_ldev(mdev)) {
		if (os.peer == R_SECONDARY && ns.peer == R_PRIMARY &&
		    mdev->ldev->md.uuid[UI_BITMAP] == 0 && ns.disk >= D_UP_TO_DATE) {
			drbd_uuid_new_current(mdev);
			drbd_send_uuids(mdev);
=======
	if (lost_contact_to_peer_data(os.pdsk, ns.pdsk)) {
		if (get_ldev(device)) {
			if ((ns.role == R_PRIMARY || ns.peer == R_PRIMARY) &&
			    device->ldev->md.uuid[UI_BITMAP] == 0 && ns.disk >= D_UP_TO_DATE) {
				if (drbd_suspended(device)) {
					set_bit(NEW_CUR_UUID, &device->flags);
				} else {
					drbd_uuid_new_current(device);
					drbd_send_uuids(peer_device);
				}
			}
			put_ldev(device);
		}
	}

	if (ns.pdsk < D_INCONSISTENT && get_ldev(device)) {
		if (os.peer != R_PRIMARY && ns.peer == R_PRIMARY &&
		    device->ldev->md.uuid[UI_BITMAP] == 0 && ns.disk >= D_UP_TO_DATE) {
			drbd_uuid_new_current(device);
			drbd_send_uuids(peer_device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		/* D_DISKLESS Peer becomes secondary */
		if (os.peer == R_PRIMARY && ns.peer == R_SECONDARY)
			/* We may still be Primary ourselves.
			 * No harm done if the bitmap still changes,
			 * redirtied pages will follow later. */
<<<<<<< HEAD
			drbd_bitmap_io_from_worker(mdev, &drbd_bm_write,
				"demote diskless peer", BM_LOCKED_SET_ALLOWED);
		put_ldev(mdev);
=======
			drbd_bitmap_io_from_worker(device, &drbd_bm_write,
				"demote diskless peer", BM_LOCKED_SET_ALLOWED);
		put_ldev(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Write out all changed bits on demote.
	 * Though, no need to da that just yet
	 * if there is a resync going on still */
	if (os.role == R_PRIMARY && ns.role == R_SECONDARY &&
<<<<<<< HEAD
		mdev->state.conn <= C_CONNECTED && get_ldev(mdev)) {
		/* No changes to the bitmap expected this time, so assert that,
		 * even though no harm was done if it did change. */
		drbd_bitmap_io_from_worker(mdev, &drbd_bm_write,
				"demote", BM_LOCKED_TEST_ALLOWED);
		put_ldev(mdev);
=======
		device->state.conn <= C_CONNECTED && get_ldev(device)) {
		/* No changes to the bitmap expected this time, so assert that,
		 * even though no harm was done if it did change. */
		drbd_bitmap_io_from_worker(device, &drbd_bm_write,
				"demote", BM_LOCKED_TEST_ALLOWED);
		put_ldev(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Last part of the attaching process ... */
	if (ns.conn >= C_CONNECTED &&
	    os.disk == D_ATTACHING && ns.disk == D_NEGOTIATING) {
<<<<<<< HEAD
		drbd_send_sizes(mdev, 0, 0);  /* to start sync... */
		drbd_send_uuids(mdev);
		drbd_send_state(mdev, ns);
=======
		drbd_send_sizes(peer_device, 0, 0);  /* to start sync... */
		drbd_send_uuids(peer_device);
		drbd_send_state(peer_device, ns);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* We want to pause/continue resync, tell peer. */
	if (ns.conn >= C_CONNECTED &&
	     ((os.aftr_isp != ns.aftr_isp) ||
	      (os.user_isp != ns.user_isp)))
<<<<<<< HEAD
		drbd_send_state(mdev, ns);
=======
		drbd_send_state(peer_device, ns);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* In case one of the isp bits got set, suspend other devices. */
	if ((!os.aftr_isp && !os.peer_isp && !os.user_isp) &&
	    (ns.aftr_isp || ns.peer_isp || ns.user_isp))
<<<<<<< HEAD
		suspend_other_sg(mdev);
=======
		suspend_other_sg(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Make sure the peer gets informed about eventual state
	   changes (ISP bits) while we were in WFReportParams. */
	if (os.conn == C_WF_REPORT_PARAMS && ns.conn >= C_CONNECTED)
<<<<<<< HEAD
		drbd_send_state(mdev, ns);

	if (os.conn != C_AHEAD && ns.conn == C_AHEAD)
		drbd_send_state(mdev, ns);
=======
		drbd_send_state(peer_device, ns);

	if (os.conn != C_AHEAD && ns.conn == C_AHEAD)
		drbd_send_state(peer_device, ns);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* We are in the progress to start a full sync... */
	if ((os.conn != C_STARTING_SYNC_T && ns.conn == C_STARTING_SYNC_T) ||
	    (os.conn != C_STARTING_SYNC_S && ns.conn == C_STARTING_SYNC_S))
		/* no other bitmap changes expected during this phase */
<<<<<<< HEAD
		drbd_queue_bitmap_io(mdev,
=======
		drbd_queue_bitmap_io(device,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			&drbd_bmio_set_n_write, &abw_start_sync,
			"set_n_write from StartingSync", BM_LOCKED_TEST_ALLOWED);

	/* first half of local IO error, failure to attach,
	 * or administrative detach */
	if (os.disk != D_FAILED && ns.disk == D_FAILED) {
		enum drbd_io_error_p eh = EP_PASS_ON;
		int was_io_error = 0;
<<<<<<< HEAD
		/* corresponding get_ldev was in __drbd_set_state, to serialize
		 * our cleanup here with the transition to D_DISKLESS.
		 * But is is still not save to dreference ldev here, since
		 * we might come from an failed Attach before ldev was set. */
		if (mdev->ldev) {
			rcu_read_lock();
			eh = rcu_dereference(mdev->ldev->disk_conf)->on_io_error;
			rcu_read_unlock();

			was_io_error = test_and_clear_bit(WAS_IO_ERROR, &mdev->flags);

			if (was_io_error && eh == EP_CALL_HELPER)
				drbd_khelper(mdev, "local-io-error");
=======
		/* corresponding get_ldev was in _drbd_set_state, to serialize
		 * our cleanup here with the transition to D_DISKLESS.
		 * But is is still not save to dreference ldev here, since
		 * we might come from an failed Attach before ldev was set. */
		if (device->ldev) {
			rcu_read_lock();
			eh = rcu_dereference(device->ldev->disk_conf)->on_io_error;
			rcu_read_unlock();

			was_io_error = test_and_clear_bit(WAS_IO_ERROR, &device->flags);

			/* Intentionally call this handler first, before drbd_send_state().
			 * See: 2932204 drbd: call local-io-error handler early
			 * People may chose to hard-reset the box from this handler.
			 * It is useful if this looks like a "regular node crash". */
			if (was_io_error && eh == EP_CALL_HELPER)
				drbd_khelper(device, "local-io-error");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			/* Immediately allow completion of all application IO,
			 * that waits for completion from the local disk,
			 * if this was a force-detach due to disk_timeout
			 * or administrator request (drbdsetup detach --force).
			 * Do NOT abort otherwise.
			 * Aborting local requests may cause serious problems,
			 * if requests are completed to upper layers already,
			 * and then later the already submitted local bio completes.
			 * This can cause DMA into former bio pages that meanwhile
			 * have been re-used for other things.
			 * So aborting local requests may cause crashes,
			 * or even worse, silent data corruption.
			 */
<<<<<<< HEAD
			if (test_and_clear_bit(FORCE_DETACH, &mdev->flags))
				tl_abort_disk_io(mdev);
=======
			if (test_and_clear_bit(FORCE_DETACH, &device->flags))
				tl_abort_disk_io(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			/* current state still has to be D_FAILED,
			 * there is only one way out: to D_DISKLESS,
			 * and that may only happen after our put_ldev below. */
<<<<<<< HEAD
			if (mdev->state.disk != D_FAILED)
				dev_err(DEV,
					"ASSERT FAILED: disk is %s during detach\n",
					drbd_disk_str(mdev->state.disk));

			if (ns.conn >= C_CONNECTED)
				drbd_send_state(mdev, ns);

			drbd_rs_cancel_all(mdev);
=======
			if (device->state.disk != D_FAILED)
				drbd_err(device,
					"ASSERT FAILED: disk is %s during detach\n",
					drbd_disk_str(device->state.disk));

			if (ns.conn >= C_CONNECTED)
				drbd_send_state(peer_device, ns);

			drbd_rs_cancel_all(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			/* In case we want to get something to stable storage still,
			 * this may be the last chance.
			 * Following put_ldev may transition to D_DISKLESS. */
<<<<<<< HEAD
			drbd_md_sync(mdev);
		}
		put_ldev(mdev);
	}

        /* second half of local IO error, failure to attach,
         * or administrative detach,
         * after local_cnt references have reached zero again */
        if (os.disk != D_DISKLESS && ns.disk == D_DISKLESS) {
                /* We must still be diskless,
                 * re-attach has to be serialized with this! */
                if (mdev->state.disk != D_DISKLESS)
                        dev_err(DEV,
                                "ASSERT FAILED: disk is %s while going diskless\n",
                                drbd_disk_str(mdev->state.disk));

		if (ns.conn >= C_CONNECTED)
			drbd_send_state(mdev, ns);
		/* corresponding get_ldev in __drbd_set_state
		 * this may finally trigger drbd_ldev_destroy. */
		put_ldev(mdev);
=======
			drbd_md_sync(device);
		}
		put_ldev(device);
	}

	/* second half of local IO error, failure to attach,
	 * or administrative detach,
	 * after local_cnt references have reached zero again */
	if (os.disk != D_DISKLESS && ns.disk == D_DISKLESS) {
		/* We must still be diskless,
		 * re-attach has to be serialized with this! */
		if (device->state.disk != D_DISKLESS)
			drbd_err(device,
				 "ASSERT FAILED: disk is %s while going diskless\n",
				 drbd_disk_str(device->state.disk));

		if (ns.conn >= C_CONNECTED)
			drbd_send_state(peer_device, ns);
		/* corresponding get_ldev in __drbd_set_state
		 * this may finally trigger drbd_ldev_destroy. */
		put_ldev(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Notify peer that I had a local IO error, and did not detached.. */
	if (os.disk == D_UP_TO_DATE && ns.disk == D_INCONSISTENT && ns.conn >= C_CONNECTED)
<<<<<<< HEAD
		drbd_send_state(mdev, ns);

	/* Disks got bigger while they were detached */
	if (ns.disk > D_NEGOTIATING && ns.pdsk > D_NEGOTIATING &&
	    test_and_clear_bit(RESYNC_AFTER_NEG, &mdev->flags)) {
		if (ns.conn == C_CONNECTED)
			resync_after_online_grow(mdev);
=======
		drbd_send_state(peer_device, ns);

	/* Disks got bigger while they were detached */
	if (ns.disk > D_NEGOTIATING && ns.pdsk > D_NEGOTIATING &&
	    test_and_clear_bit(RESYNC_AFTER_NEG, &device->flags)) {
		if (ns.conn == C_CONNECTED)
			resync_after_online_grow(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* A resync finished or aborted, wake paused devices... */
	if ((os.conn > C_CONNECTED && ns.conn <= C_CONNECTED) ||
	    (os.peer_isp && !ns.peer_isp) ||
	    (os.user_isp && !ns.user_isp))
<<<<<<< HEAD
		resume_next_sg(mdev);
=======
		resume_next_sg(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* sync target done with resync.  Explicitly notify peer, even though
	 * it should (at least for non-empty resyncs) already know itself. */
	if (os.disk < D_UP_TO_DATE && os.conn >= C_SYNC_SOURCE && ns.conn == C_CONNECTED)
<<<<<<< HEAD
		drbd_send_state(mdev, ns);
=======
		drbd_send_state(peer_device, ns);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Verify finished, or reached stop sector.  Peer did not know about
	 * the stop sector, and we may even have changed the stop sector during
	 * verify to interrupt/stop early.  Send the new state. */
	if (os.conn == C_VERIFY_S && ns.conn == C_CONNECTED
<<<<<<< HEAD
	&& verify_can_do_stop_sector(mdev))
		drbd_send_state(mdev, ns);

	/* This triggers bitmap writeout of potentially still unwritten pages
	 * if the resync finished cleanly, or aborted because of peer disk
	 * failure, or because of connection loss.
	 * For resync aborted because of local disk failure, we cannot do
	 * any bitmap writeout anymore.
	 * No harm done if some bits change during this phase.
	 */
	if (os.conn > C_CONNECTED && ns.conn <= C_CONNECTED && get_ldev(mdev)) {
		drbd_queue_bitmap_io(mdev, &drbd_bm_write_copy_pages, NULL,
			"write from resync_finished", BM_LOCKED_CHANGE_ALLOWED);
		put_ldev(mdev);
=======
	&& verify_can_do_stop_sector(device))
		drbd_send_state(peer_device, ns);

	/* This triggers bitmap writeout of potentially still unwritten pages
	 * if the resync finished cleanly, or aborted because of peer disk
	 * failure, or on transition from resync back to AHEAD/BEHIND.
	 *
	 * Connection loss is handled in drbd_disconnected() by the receiver.
	 *
	 * For resync aborted because of local disk failure, we cannot do
	 * any bitmap writeout anymore.
	 *
	 * No harm done if some bits change during this phase.
	 */
	if ((os.conn > C_CONNECTED && os.conn < C_AHEAD) &&
	    (ns.conn == C_CONNECTED || ns.conn >= C_AHEAD) && get_ldev(device)) {
		drbd_queue_bitmap_io(device, &drbd_bm_write_copy_pages, NULL,
			"write from resync_finished", BM_LOCKED_CHANGE_ALLOWED);
		put_ldev(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (ns.disk == D_DISKLESS &&
	    ns.conn == C_STANDALONE &&
	    ns.role == R_SECONDARY) {
		if (os.aftr_isp != ns.aftr_isp)
<<<<<<< HEAD
			resume_next_sg(mdev);
	}

	drbd_md_sync(mdev);
=======
			resume_next_sg(device);
	}

	drbd_md_sync(device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

struct after_conn_state_chg_work {
	struct drbd_work w;
	enum drbd_conns oc;
	union drbd_state ns_min;
<<<<<<< HEAD
	union drbd_state ns_max; /* new, max state, over all mdevs */
	enum chg_state_flags flags;
=======
	union drbd_state ns_max; /* new, max state, over all devices */
	enum chg_state_flags flags;
	struct drbd_connection *connection;
	struct drbd_state_change *state_change;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int w_after_conn_state_ch(struct drbd_work *w, int unused)
{
	struct after_conn_state_chg_work *acscw =
		container_of(w, struct after_conn_state_chg_work, w);
<<<<<<< HEAD
	struct drbd_tconn *tconn = w->tconn;
	enum drbd_conns oc = acscw->oc;
	union drbd_state ns_max = acscw->ns_max;
	struct drbd_conf *mdev;
	int vnr;

=======
	struct drbd_connection *connection = acscw->connection;
	enum drbd_conns oc = acscw->oc;
	union drbd_state ns_max = acscw->ns_max;
	struct drbd_peer_device *peer_device;
	int vnr;

	broadcast_state_change(acscw->state_change);
	forget_state_change(acscw->state_change);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(acscw);

	/* Upon network configuration, we need to start the receiver */
	if (oc == C_STANDALONE && ns_max.conn == C_UNCONNECTED)
<<<<<<< HEAD
		drbd_thread_start(&tconn->receiver);
=======
		drbd_thread_start(&connection->receiver);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (oc == C_DISCONNECTING && ns_max.conn == C_STANDALONE) {
		struct net_conf *old_conf;

<<<<<<< HEAD
		mutex_lock(&tconn->conf_update);
		old_conf = tconn->net_conf;
		tconn->my_addr_len = 0;
		tconn->peer_addr_len = 0;
		rcu_assign_pointer(tconn->net_conf, NULL);
		conn_free_crypto(tconn);
		mutex_unlock(&tconn->conf_update);
=======
		mutex_lock(&notification_mutex);
		idr_for_each_entry(&connection->peer_devices, peer_device, vnr)
			notify_peer_device_state(NULL, 0, peer_device, NULL,
						 NOTIFY_DESTROY | NOTIFY_CONTINUES);
		notify_connection_state(NULL, 0, connection, NULL, NOTIFY_DESTROY);
		mutex_unlock(&notification_mutex);

		mutex_lock(&connection->resource->conf_update);
		old_conf = connection->net_conf;
		connection->my_addr_len = 0;
		connection->peer_addr_len = 0;
		RCU_INIT_POINTER(connection->net_conf, NULL);
		conn_free_crypto(connection);
		mutex_unlock(&connection->resource->conf_update);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		synchronize_rcu();
		kfree(old_conf);
	}

	if (ns_max.susp_fen) {
		/* case1: The outdate peer handler is successful: */
		if (ns_max.pdsk <= D_OUTDATED) {
			rcu_read_lock();
<<<<<<< HEAD
			idr_for_each_entry(&tconn->volumes, mdev, vnr) {
				if (test_bit(NEW_CUR_UUID, &mdev->flags)) {
					drbd_uuid_new_current(mdev);
					clear_bit(NEW_CUR_UUID, &mdev->flags);
				}
			}
			rcu_read_unlock();
			spin_lock_irq(&tconn->req_lock);
			_tl_restart(tconn, CONNECTION_LOST_WHILE_PENDING);
			_conn_request_state(tconn,
					    (union drbd_state) { { .susp_fen = 1 } },
					    (union drbd_state) { { .susp_fen = 0 } },
					    CS_VERBOSE);
			spin_unlock_irq(&tconn->req_lock);
		}
	}
	kref_put(&tconn->kref, &conn_destroy);

	conn_md_sync(tconn);
=======
			idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
				struct drbd_device *device = peer_device->device;
				if (test_bit(NEW_CUR_UUID, &device->flags)) {
					drbd_uuid_new_current(device);
					clear_bit(NEW_CUR_UUID, &device->flags);
				}
			}
			rcu_read_unlock();
			spin_lock_irq(&connection->resource->req_lock);
			_tl_restart(connection, CONNECTION_LOST_WHILE_PENDING);
			_conn_request_state(connection,
					    (union drbd_state) { { .susp_fen = 1 } },
					    (union drbd_state) { { .susp_fen = 0 } },
					    CS_VERBOSE);
			spin_unlock_irq(&connection->resource->req_lock);
		}
	}
	kref_put(&connection->kref, drbd_destroy_connection);

	conn_md_sync(connection);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
void conn_old_common_state(struct drbd_tconn *tconn, union drbd_state *pcs, enum chg_state_flags *pf)
{
	enum chg_state_flags flags = ~0;
	struct drbd_conf *mdev;
=======
static void conn_old_common_state(struct drbd_connection *connection, union drbd_state *pcs, enum chg_state_flags *pf)
{
	enum chg_state_flags flags = ~0;
	struct drbd_peer_device *peer_device;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int vnr, first_vol = 1;
	union drbd_dev_state os, cs = {
		{ .role = R_SECONDARY,
		  .peer = R_UNKNOWN,
<<<<<<< HEAD
		  .conn = tconn->cstate,
=======
		  .conn = connection->cstate,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		  .disk = D_DISKLESS,
		  .pdsk = D_UNKNOWN,
		} };

	rcu_read_lock();
<<<<<<< HEAD
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		os = mdev->state;
=======
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		os = device->state;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (first_vol) {
			cs = os;
			first_vol = 0;
			continue;
		}

		if (cs.role != os.role)
			flags &= ~CS_DC_ROLE;

		if (cs.peer != os.peer)
			flags &= ~CS_DC_PEER;

		if (cs.conn != os.conn)
			flags &= ~CS_DC_CONN;

		if (cs.disk != os.disk)
			flags &= ~CS_DC_DISK;

		if (cs.pdsk != os.pdsk)
			flags &= ~CS_DC_PDSK;
	}
	rcu_read_unlock();

	*pf |= CS_DC_MASK;
	*pf &= flags;
	(*pcs).i = cs.i;
}

static enum drbd_state_rv
<<<<<<< HEAD
conn_is_valid_transition(struct drbd_tconn *tconn, union drbd_state mask, union drbd_state val,
=======
conn_is_valid_transition(struct drbd_connection *connection, union drbd_state mask, union drbd_state val,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			 enum chg_state_flags flags)
{
	enum drbd_state_rv rv = SS_SUCCESS;
	union drbd_state ns, os;
<<<<<<< HEAD
	struct drbd_conf *mdev;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		os = drbd_read_state(mdev);
		ns = sanitize_state(mdev, apply_mask_val(os, mask, val), NULL);
=======
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		os = drbd_read_state(device);
		ns = sanitize_state(device, os, apply_mask_val(os, mask, val), NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (flags & CS_IGN_OUTD_FAIL && ns.disk == D_OUTDATED && os.disk < D_OUTDATED)
			ns.disk = os.disk;

		if (ns.i == os.i)
			continue;

		rv = is_valid_transition(os, ns);
<<<<<<< HEAD
		if (rv < SS_SUCCESS)
			break;

		if (!(flags & CS_HARD)) {
			rv = is_valid_state(mdev, ns);
			if (rv < SS_SUCCESS) {
				if (is_valid_state(mdev, os) == rv)
					rv = is_valid_soft_transition(os, ns, tconn);
			} else
				rv = is_valid_soft_transition(os, ns, tconn);
		}
		if (rv < SS_SUCCESS)
			break;
	}
	rcu_read_unlock();

	if (rv < SS_SUCCESS && flags & CS_VERBOSE)
		print_st_err(mdev, os, ns, rv);

	return rv;
}

void
conn_set_state(struct drbd_tconn *tconn, union drbd_state mask, union drbd_state val,
=======

		if (rv >= SS_SUCCESS && !(flags & CS_HARD)) {
			rv = is_valid_state(device, ns);
			if (rv < SS_SUCCESS) {
				if (is_valid_state(device, os) == rv)
					rv = is_valid_soft_transition(os, ns, connection);
			} else
				rv = is_valid_soft_transition(os, ns, connection);
		}

		if (rv < SS_SUCCESS) {
			if (flags & CS_VERBOSE)
				print_st_err(device, os, ns, rv);
			break;
		}
	}
	rcu_read_unlock();

	return rv;
}

static void
conn_set_state(struct drbd_connection *connection, union drbd_state mask, union drbd_state val,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	       union drbd_state *pns_min, union drbd_state *pns_max, enum chg_state_flags flags)
{
	union drbd_state ns, os, ns_max = { };
	union drbd_state ns_min = {
		{ .role = R_MASK,
		  .peer = R_MASK,
		  .conn = val.conn,
		  .disk = D_MASK,
		  .pdsk = D_MASK
		} };
<<<<<<< HEAD
	struct drbd_conf *mdev;
=======
	struct drbd_peer_device *peer_device;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	enum drbd_state_rv rv;
	int vnr, number_of_volumes = 0;

	if (mask.conn == C_MASK) {
		/* remember last connect time so request_timer_fn() won't
		 * kill newly established sessions while we are still trying to thaw
		 * previously frozen IO */
<<<<<<< HEAD
		if (tconn->cstate != C_WF_REPORT_PARAMS && val.conn == C_WF_REPORT_PARAMS)
			tconn->last_reconnect_jif = jiffies;

		tconn->cstate = val.conn;
	}

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		number_of_volumes++;
		os = drbd_read_state(mdev);
		ns = apply_mask_val(os, mask, val);
		ns = sanitize_state(mdev, ns, NULL);
=======
		if (connection->cstate != C_WF_REPORT_PARAMS && val.conn == C_WF_REPORT_PARAMS)
			connection->last_reconnect_jif = jiffies;

		connection->cstate = val.conn;
	}

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		number_of_volumes++;
		os = drbd_read_state(device);
		ns = apply_mask_val(os, mask, val);
		ns = sanitize_state(device, os, ns, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (flags & CS_IGN_OUTD_FAIL && ns.disk == D_OUTDATED && os.disk < D_OUTDATED)
			ns.disk = os.disk;

<<<<<<< HEAD
		rv = __drbd_set_state(mdev, ns, flags, NULL);
		if (rv < SS_SUCCESS)
			BUG();

		ns.i = mdev->state.i;
=======
		rv = _drbd_set_state(device, ns, flags, NULL);
		BUG_ON(rv < SS_SUCCESS);
		ns.i = device->state.i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ns_max.role = max_role(ns.role, ns_max.role);
		ns_max.peer = max_role(ns.peer, ns_max.peer);
		ns_max.conn = max_t(enum drbd_conns, ns.conn, ns_max.conn);
		ns_max.disk = max_t(enum drbd_disk_state, ns.disk, ns_max.disk);
		ns_max.pdsk = max_t(enum drbd_disk_state, ns.pdsk, ns_max.pdsk);

		ns_min.role = min_role(ns.role, ns_min.role);
		ns_min.peer = min_role(ns.peer, ns_min.peer);
		ns_min.conn = min_t(enum drbd_conns, ns.conn, ns_min.conn);
		ns_min.disk = min_t(enum drbd_disk_state, ns.disk, ns_min.disk);
		ns_min.pdsk = min_t(enum drbd_disk_state, ns.pdsk, ns_min.pdsk);
	}
	rcu_read_unlock();

	if (number_of_volumes == 0) {
		ns_min = ns_max = (union drbd_state) { {
				.role = R_SECONDARY,
				.peer = R_UNKNOWN,
				.conn = val.conn,
				.disk = D_DISKLESS,
				.pdsk = D_UNKNOWN
			} };
	}

<<<<<<< HEAD
	ns_min.susp = ns_max.susp = tconn->susp;
	ns_min.susp_nod = ns_max.susp_nod = tconn->susp_nod;
	ns_min.susp_fen = ns_max.susp_fen = tconn->susp_fen;
=======
	ns_min.susp = ns_max.susp = connection->resource->susp;
	ns_min.susp_nod = ns_max.susp_nod = connection->resource->susp_nod;
	ns_min.susp_fen = ns_max.susp_fen = connection->resource->susp_fen;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	*pns_min = ns_min;
	*pns_max = ns_max;
}

static enum drbd_state_rv
<<<<<<< HEAD
_conn_rq_cond(struct drbd_tconn *tconn, union drbd_state mask, union drbd_state val)
{
	enum drbd_state_rv rv;

	if (test_and_clear_bit(CONN_WD_ST_CHG_OKAY, &tconn->flags))
		return SS_CW_SUCCESS;

	if (test_and_clear_bit(CONN_WD_ST_CHG_FAIL, &tconn->flags))
		return SS_CW_FAILED_BY_PEER;

	rv = conn_is_valid_transition(tconn, mask, val, 0);
	if (rv == SS_SUCCESS && tconn->cstate == C_WF_REPORT_PARAMS)
		rv = SS_UNKNOWN_ERROR; /* continue waiting */

	return rv;
}

enum drbd_state_rv
_conn_request_state(struct drbd_tconn *tconn, union drbd_state mask, union drbd_state val,
=======
_conn_rq_cond(struct drbd_connection *connection, union drbd_state mask, union drbd_state val)
{
	enum drbd_state_rv err, rv = SS_UNKNOWN_ERROR; /* continue waiting */;

	if (test_and_clear_bit(CONN_WD_ST_CHG_OKAY, &connection->flags))
		rv = SS_CW_SUCCESS;

	if (test_and_clear_bit(CONN_WD_ST_CHG_FAIL, &connection->flags))
		rv = SS_CW_FAILED_BY_PEER;

	err = conn_is_valid_transition(connection, mask, val, 0);
	if (err == SS_SUCCESS && connection->cstate == C_WF_REPORT_PARAMS)
		return rv;

	return err;
}

enum drbd_state_rv
_conn_request_state(struct drbd_connection *connection, union drbd_state mask, union drbd_state val,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		    enum chg_state_flags flags)
{
	enum drbd_state_rv rv = SS_SUCCESS;
	struct after_conn_state_chg_work *acscw;
<<<<<<< HEAD
	enum drbd_conns oc = tconn->cstate;
	union drbd_state ns_max, ns_min, os;
	bool have_mutex = false;
=======
	enum drbd_conns oc = connection->cstate;
	union drbd_state ns_max, ns_min, os;
	bool have_mutex = false;
	struct drbd_state_change *state_change;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (mask.conn) {
		rv = is_valid_conn_transition(oc, val.conn);
		if (rv < SS_SUCCESS)
			goto abort;
	}

<<<<<<< HEAD
	rv = conn_is_valid_transition(tconn, mask, val, flags);
=======
	rv = conn_is_valid_transition(connection, mask, val, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (rv < SS_SUCCESS)
		goto abort;

	if (oc == C_WF_REPORT_PARAMS && val.conn == C_DISCONNECTING &&
	    !(flags & (CS_LOCAL_ONLY | CS_HARD))) {

		/* This will be a cluster-wide state change.
		 * Need to give up the spinlock, grab the mutex,
		 * then send the state change request, ... */
<<<<<<< HEAD
		spin_unlock_irq(&tconn->req_lock);
		mutex_lock(&tconn->cstate_mutex);
		have_mutex = true;

		set_bit(CONN_WD_ST_CHG_REQ, &tconn->flags);
		if (conn_send_state_req(tconn, mask, val)) {
			/* sending failed. */
			clear_bit(CONN_WD_ST_CHG_REQ, &tconn->flags);
=======
		spin_unlock_irq(&connection->resource->req_lock);
		mutex_lock(&connection->cstate_mutex);
		have_mutex = true;

		set_bit(CONN_WD_ST_CHG_REQ, &connection->flags);
		if (conn_send_state_req(connection, mask, val)) {
			/* sending failed. */
			clear_bit(CONN_WD_ST_CHG_REQ, &connection->flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			rv = SS_CW_FAILED_BY_PEER;
			/* need to re-aquire the spin lock, though */
			goto abort_unlocked;
		}

		if (val.conn == C_DISCONNECTING)
<<<<<<< HEAD
			set_bit(DISCONNECT_SENT, &tconn->flags);
=======
			set_bit(DISCONNECT_SENT, &connection->flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* ... and re-aquire the spinlock.
		 * If _conn_rq_cond() returned >= SS_SUCCESS, we must call
		 * conn_set_state() within the same spinlock. */
<<<<<<< HEAD
		spin_lock_irq(&tconn->req_lock);
		wait_event_lock_irq(tconn->ping_wait,
				(rv = _conn_rq_cond(tconn, mask, val)),
				tconn->req_lock);
		clear_bit(CONN_WD_ST_CHG_REQ, &tconn->flags);
=======
		spin_lock_irq(&connection->resource->req_lock);
		wait_event_lock_irq(connection->ping_wait,
				(rv = _conn_rq_cond(connection, mask, val)),
				connection->resource->req_lock);
		clear_bit(CONN_WD_ST_CHG_REQ, &connection->flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (rv < SS_SUCCESS)
			goto abort;
	}

<<<<<<< HEAD
	conn_old_common_state(tconn, &os, &flags);
	flags |= CS_DC_SUSP;
	conn_set_state(tconn, mask, val, &ns_min, &ns_max, flags);
	conn_pr_state_change(tconn, os, ns_max, flags);
=======
	state_change = remember_old_state(connection->resource, GFP_ATOMIC);
	conn_old_common_state(connection, &os, &flags);
	flags |= CS_DC_SUSP;
	conn_set_state(connection, mask, val, &ns_min, &ns_max, flags);
	conn_pr_state_change(connection, os, ns_max, flags);
	remember_new_state(state_change);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	acscw = kmalloc(sizeof(*acscw), GFP_ATOMIC);
	if (acscw) {
		acscw->oc = os.conn;
		acscw->ns_min = ns_min;
		acscw->ns_max = ns_max;
		acscw->flags = flags;
		acscw->w.cb = w_after_conn_state_ch;
<<<<<<< HEAD
		kref_get(&tconn->kref);
		acscw->w.tconn = tconn;
		drbd_queue_work(&tconn->sender_work, &acscw->w);
	} else {
		conn_err(tconn, "Could not kmalloc an acscw\n");
=======
		kref_get(&connection->kref);
		acscw->connection = connection;
		acscw->state_change = state_change;
		drbd_queue_work(&connection->sender_work, &acscw->w);
	} else {
		drbd_err(connection, "Could not kmalloc an acscw\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

 abort:
	if (have_mutex) {
		/* mutex_unlock() "... must not be used in interrupt context.",
		 * so give up the spinlock, then re-aquire it */
<<<<<<< HEAD
		spin_unlock_irq(&tconn->req_lock);
 abort_unlocked:
		mutex_unlock(&tconn->cstate_mutex);
		spin_lock_irq(&tconn->req_lock);
	}
	if (rv < SS_SUCCESS && flags & CS_VERBOSE) {
		conn_err(tconn, "State change failed: %s\n", drbd_set_st_err_str(rv));
		conn_err(tconn, " mask = 0x%x val = 0x%x\n", mask.i, val.i);
		conn_err(tconn, " old_conn:%s wanted_conn:%s\n", drbd_conn_str(oc), drbd_conn_str(val.conn));
=======
		spin_unlock_irq(&connection->resource->req_lock);
 abort_unlocked:
		mutex_unlock(&connection->cstate_mutex);
		spin_lock_irq(&connection->resource->req_lock);
	}
	if (rv < SS_SUCCESS && flags & CS_VERBOSE) {
		drbd_err(connection, "State change failed: %s\n", drbd_set_st_err_str(rv));
		drbd_err(connection, " mask = 0x%x val = 0x%x\n", mask.i, val.i);
		drbd_err(connection, " old_conn:%s wanted_conn:%s\n", drbd_conn_str(oc), drbd_conn_str(val.conn));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return rv;
}

enum drbd_state_rv
<<<<<<< HEAD
conn_request_state(struct drbd_tconn *tconn, union drbd_state mask, union drbd_state val,
=======
conn_request_state(struct drbd_connection *connection, union drbd_state mask, union drbd_state val,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		   enum chg_state_flags flags)
{
	enum drbd_state_rv rv;

<<<<<<< HEAD
	spin_lock_irq(&tconn->req_lock);
	rv = _conn_request_state(tconn, mask, val, flags);
	spin_unlock_irq(&tconn->req_lock);
=======
	spin_lock_irq(&connection->resource->req_lock);
	rv = _conn_request_state(connection, mask, val, flags);
	spin_unlock_irq(&connection->resource->req_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return rv;
}
