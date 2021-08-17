/*
  drbd_int.h

  This file is part of DRBD by Philipp Reisner and Lars Ellenberg.

  Copyright (C) 2001-2008, LINBIT Information Technologies GmbH.
  Copyright (C) 1999-2008, Philipp Reisner <philipp.reisner@linbit.com>.
  Copyright (C) 2002-2008, Lars Ellenberg <lars.ellenberg@linbit.com>.

  drbd is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  drbd is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drbd; see the file COPYING.  If not, write to
  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef _DRBD_INT_H
#define _DRBD_INT_H

<<<<<<< HEAD
=======
#include <crypto/hash.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/bitops.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/crypto.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/ratelimit.h>
#include <linux/tcp.h>
#include <linux/mutex.h>
#include <linux/major.h>
#include <linux/blkdev.h>
<<<<<<< HEAD
#include <linux/genhd.h>
#include <linux/idr.h>
=======
#include <linux/backing-dev.h>
#include <linux/genhd.h>
#include <linux/idr.h>
#include <linux/dynamic_debug.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <net/tcp.h>
#include <linux/lru_cache.h>
#include <linux/prefetch.h>
#include <linux/drbd_genl_api.h>
#include <linux/drbd.h>
<<<<<<< HEAD
#include "drbd_state.h"
=======
#include "drbd_strings.h"
#include "drbd_state.h"
#include "drbd_protocol.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef __CHECKER__
# define __protected_by(x)       __attribute__((require_context(x,1,999,"rdwr")))
# define __protected_read_by(x)  __attribute__((require_context(x,1,999,"read")))
# define __protected_write_by(x) __attribute__((require_context(x,1,999,"write")))
# define __must_hold(x)       __attribute__((context(x,1,1), require_context(x,1,999,"call")))
#else
# define __protected_by(x)
# define __protected_read_by(x)
# define __protected_write_by(x)
# define __must_hold(x)
#endif

<<<<<<< HEAD
#define __no_warn(lock, stmt) do { __acquire(lock); stmt; __release(lock); } while (0)

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* module parameter, defined in drbd_main.c */
extern unsigned int minor_count;
extern bool disable_sendpage;
extern bool allow_oos;
<<<<<<< HEAD
=======
void tl_abort_disk_io(struct drbd_device *device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_DRBD_FAULT_INJECTION
extern int enable_faults;
extern int fault_rate;
extern int fault_devs;
#endif

extern char usermode_helper[];


<<<<<<< HEAD
/* I don't remember why XCPU ...
 * This is used to wake the asender,
 * and to interrupt sending the sending task
 * on disconnect.
 */
#define DRBD_SIG SIGXCPU

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* This is used to stop/restart our threads.
 * Cannot use SIGTERM nor SIGKILL, since these
 * are sent out by init on runlevel changes
 * I choose SIGHUP for now.
 */
#define DRBD_SIGKILL SIGHUP

#define ID_IN_SYNC      (4711ULL)
#define ID_OUT_OF_SYNC  (4712ULL)
#define ID_SYNCER (-1ULL)

#define UUID_NEW_BM_OFFSET ((u64)0x0001000000000000ULL)

<<<<<<< HEAD
struct drbd_conf;
struct drbd_tconn;


/* to shorten dev_warn(DEV, "msg"); and relatives statements */
#define DEV (disk_to_dev(mdev->vdisk))

#define conn_printk(LEVEL, TCONN, FMT, ARGS...) \
	printk(LEVEL "d-con %s: " FMT, TCONN->name , ## ARGS)
#define conn_alert(TCONN, FMT, ARGS...)  conn_printk(KERN_ALERT, TCONN, FMT, ## ARGS)
#define conn_crit(TCONN, FMT, ARGS...)   conn_printk(KERN_CRIT, TCONN, FMT, ## ARGS)
#define conn_err(TCONN, FMT, ARGS...)    conn_printk(KERN_ERR, TCONN, FMT, ## ARGS)
#define conn_warn(TCONN, FMT, ARGS...)   conn_printk(KERN_WARNING, TCONN, FMT, ## ARGS)
#define conn_notice(TCONN, FMT, ARGS...) conn_printk(KERN_NOTICE, TCONN, FMT, ## ARGS)
#define conn_info(TCONN, FMT, ARGS...)   conn_printk(KERN_INFO, TCONN, FMT, ## ARGS)
#define conn_dbg(TCONN, FMT, ARGS...)    conn_printk(KERN_DEBUG, TCONN, FMT, ## ARGS)

#define D_ASSERT(exp)	if (!(exp)) \
	 dev_err(DEV, "ASSERT( " #exp " ) in %s:%d\n", __FILE__, __LINE__)
=======
struct drbd_device;
struct drbd_connection;

#define __drbd_printk_device(level, device, fmt, args...) \
	dev_printk(level, disk_to_dev((device)->vdisk), fmt, ## args)
#define __drbd_printk_peer_device(level, peer_device, fmt, args...) \
	dev_printk(level, disk_to_dev((peer_device)->device->vdisk), fmt, ## args)
#define __drbd_printk_resource(level, resource, fmt, args...) \
	printk(level "drbd %s: " fmt, (resource)->name, ## args)
#define __drbd_printk_connection(level, connection, fmt, args...) \
	printk(level "drbd %s: " fmt, (connection)->resource->name, ## args)

void drbd_printk_with_wrong_object_type(void);

#define __drbd_printk_if_same_type(obj, type, func, level, fmt, args...) \
	(__builtin_types_compatible_p(typeof(obj), type) || \
	 __builtin_types_compatible_p(typeof(obj), const type)), \
	func(level, (const type)(obj), fmt, ## args)

#define drbd_printk(level, obj, fmt, args...) \
	__builtin_choose_expr( \
	  __drbd_printk_if_same_type(obj, struct drbd_device *, \
			     __drbd_printk_device, level, fmt, ## args), \
	  __builtin_choose_expr( \
	    __drbd_printk_if_same_type(obj, struct drbd_resource *, \
			       __drbd_printk_resource, level, fmt, ## args), \
	    __builtin_choose_expr( \
	      __drbd_printk_if_same_type(obj, struct drbd_connection *, \
				 __drbd_printk_connection, level, fmt, ## args), \
	      __builtin_choose_expr( \
		__drbd_printk_if_same_type(obj, struct drbd_peer_device *, \
				 __drbd_printk_peer_device, level, fmt, ## args), \
		drbd_printk_with_wrong_object_type()))))

#define drbd_dbg(obj, fmt, args...) \
	drbd_printk(KERN_DEBUG, obj, fmt, ## args)
#define drbd_alert(obj, fmt, args...) \
	drbd_printk(KERN_ALERT, obj, fmt, ## args)
#define drbd_err(obj, fmt, args...) \
	drbd_printk(KERN_ERR, obj, fmt, ## args)
#define drbd_warn(obj, fmt, args...) \
	drbd_printk(KERN_WARNING, obj, fmt, ## args)
#define drbd_info(obj, fmt, args...) \
	drbd_printk(KERN_INFO, obj, fmt, ## args)
#define drbd_emerg(obj, fmt, args...) \
	drbd_printk(KERN_EMERG, obj, fmt, ## args)

#define dynamic_drbd_dbg(device, fmt, args...) \
	dynamic_dev_dbg(disk_to_dev(device->vdisk), fmt, ## args)

#define D_ASSERT(device, exp)	do { \
	if (!(exp)) \
		drbd_err(device, "ASSERT( " #exp " ) in %s:%d\n", __FILE__, __LINE__); \
	} while (0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * expect  -  Make an assertion
 *
 * Unlike the assert macro, this macro returns a boolean result.
 */
#define expect(exp) ({								\
		bool _bool = (exp);						\
		if (!_bool)							\
<<<<<<< HEAD
			dev_err(DEV, "ASSERTION %s FAILED in %s\n",		\
=======
			drbd_err(device, "ASSERTION %s FAILED in %s\n",		\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			        #exp, __func__);				\
		_bool;								\
		})

/* Defines to control fault insertion */
enum {
	DRBD_FAULT_MD_WR = 0,	/* meta data write */
	DRBD_FAULT_MD_RD = 1,	/*           read  */
	DRBD_FAULT_RS_WR = 2,	/* resync          */
	DRBD_FAULT_RS_RD = 3,
	DRBD_FAULT_DT_WR = 4,	/* data            */
	DRBD_FAULT_DT_RD = 5,
	DRBD_FAULT_DT_RA = 6,	/* data read ahead */
	DRBD_FAULT_BM_ALLOC = 7,	/* bitmap allocation */
	DRBD_FAULT_AL_EE = 8,	/* alloc ee */
	DRBD_FAULT_RECEIVE = 9, /* Changes some bytes upon receiving a [rs]data block */

	DRBD_FAULT_MAX,
};

extern unsigned int
<<<<<<< HEAD
_drbd_insert_fault(struct drbd_conf *mdev, unsigned int type);

static inline int
drbd_insert_fault(struct drbd_conf *mdev, unsigned int type) {
#ifdef CONFIG_DRBD_FAULT_INJECTION
	return fault_rate &&
		(enable_faults & (1<<type)) &&
		_drbd_insert_fault(mdev, type);
=======
_drbd_insert_fault(struct drbd_device *device, unsigned int type);

static inline int
drbd_insert_fault(struct drbd_device *device, unsigned int type) {
#ifdef CONFIG_DRBD_FAULT_INJECTION
	return fault_rate &&
		(enable_faults & (1<<type)) &&
		_drbd_insert_fault(device, type);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
	return 0;
#endif
}

/* integer division, round _UP_ to the next integer */
#define div_ceil(A, B) ((A)/(B) + ((A)%(B) ? 1 : 0))
/* usual integer division */
#define div_floor(A, B) ((A)/(B))

extern struct ratelimit_state drbd_ratelimit_state;
<<<<<<< HEAD
extern struct idr minors; /* RCU, updates: genl_lock() */
extern struct list_head drbd_tconns; /* RCU, updates: genl_lock() */

/* on the wire */
enum drbd_packet {
	/* receiver (data socket) */
	P_DATA		      = 0x00,
	P_DATA_REPLY	      = 0x01, /* Response to P_DATA_REQUEST */
	P_RS_DATA_REPLY	      = 0x02, /* Response to P_RS_DATA_REQUEST */
	P_BARRIER	      = 0x03,
	P_BITMAP	      = 0x04,
	P_BECOME_SYNC_TARGET  = 0x05,
	P_BECOME_SYNC_SOURCE  = 0x06,
	P_UNPLUG_REMOTE	      = 0x07, /* Used at various times to hint the peer */
	P_DATA_REQUEST	      = 0x08, /* Used to ask for a data block */
	P_RS_DATA_REQUEST     = 0x09, /* Used to ask for a data block for resync */
	P_SYNC_PARAM	      = 0x0a,
	P_PROTOCOL	      = 0x0b,
	P_UUIDS		      = 0x0c,
	P_SIZES		      = 0x0d,
	P_STATE		      = 0x0e,
	P_SYNC_UUID	      = 0x0f,
	P_AUTH_CHALLENGE      = 0x10,
	P_AUTH_RESPONSE	      = 0x11,
	P_STATE_CHG_REQ	      = 0x12,

	/* asender (meta socket */
	P_PING		      = 0x13,
	P_PING_ACK	      = 0x14,
	P_RECV_ACK	      = 0x15, /* Used in protocol B */
	P_WRITE_ACK	      = 0x16, /* Used in protocol C */
	P_RS_WRITE_ACK	      = 0x17, /* Is a P_WRITE_ACK, additionally call set_in_sync(). */
	P_SUPERSEDED	      = 0x18, /* Used in proto C, two-primaries conflict detection */
	P_NEG_ACK	      = 0x19, /* Sent if local disk is unusable */
	P_NEG_DREPLY	      = 0x1a, /* Local disk is broken... */
	P_NEG_RS_DREPLY	      = 0x1b, /* Local disk is broken... */
	P_BARRIER_ACK	      = 0x1c,
	P_STATE_CHG_REPLY     = 0x1d,

	/* "new" commands, no longer fitting into the ordering scheme above */

	P_OV_REQUEST	      = 0x1e, /* data socket */
	P_OV_REPLY	      = 0x1f,
	P_OV_RESULT	      = 0x20, /* meta socket */
	P_CSUM_RS_REQUEST     = 0x21, /* data socket */
	P_RS_IS_IN_SYNC	      = 0x22, /* meta socket */
	P_SYNC_PARAM89	      = 0x23, /* data socket, protocol version 89 replacement for P_SYNC_PARAM */
	P_COMPRESSED_BITMAP   = 0x24, /* compressed or otherwise encoded bitmap transfer */
	/* P_CKPT_FENCE_REQ      = 0x25, * currently reserved for protocol D */
	/* P_CKPT_DISABLE_REQ    = 0x26, * currently reserved for protocol D */
	P_DELAY_PROBE         = 0x27, /* is used on BOTH sockets */
	P_OUT_OF_SYNC         = 0x28, /* Mark as out of sync (Outrunning), data socket */
	P_RS_CANCEL           = 0x29, /* meta: Used to cancel RS_DATA_REQUEST packet by SyncSource */
	P_CONN_ST_CHG_REQ     = 0x2a, /* data sock: Connection wide state request */
	P_CONN_ST_CHG_REPLY   = 0x2b, /* meta sock: Connection side state req reply */
	P_RETRY_WRITE	      = 0x2c, /* Protocol C: retry conflicting write request */
	P_PROTOCOL_UPDATE     = 0x2d, /* data sock: is used in established connections */

	P_MAY_IGNORE	      = 0x100, /* Flag to test if (cmd > P_MAY_IGNORE) ... */
	P_MAX_OPT_CMD	      = 0x101,

	/* special command ids for handshake */

	P_INITIAL_META	      = 0xfff1, /* First Packet on the MetaSock */
	P_INITIAL_DATA	      = 0xfff2, /* First Packet on the Socket */

	P_CONNECTION_FEATURES = 0xfffe	/* FIXED for the next century! */
};
=======
extern struct idr drbd_devices; /* RCU, updates: genl_lock() */
extern struct list_head drbd_resources; /* RCU, updates: genl_lock() */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern const char *cmdname(enum drbd_packet cmd);

/* for sending/receiving the bitmap,
 * possibly in some encoding scheme */
struct bm_xfer_ctx {
	/* "const"
	 * stores total bits and long words
	 * of the bitmap, so we don't need to
	 * call the accessor functions over and again. */
	unsigned long bm_bits;
	unsigned long bm_words;
	/* during xfer, current position within the bitmap */
	unsigned long bit_offset;
	unsigned long word_offset;

	/* statistics; index: (h->command == P_BITMAP) */
	unsigned packets[2];
	unsigned bytes[2];
};

<<<<<<< HEAD
extern void INFO_bm_xfer_stats(struct drbd_conf *mdev,
=======
extern void INFO_bm_xfer_stats(struct drbd_device *device,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		const char *direction, struct bm_xfer_ctx *c);

static inline void bm_xfer_ctx_bit_to_word_offset(struct bm_xfer_ctx *c)
{
	/* word_offset counts "native long words" (32 or 64 bit),
	 * aligned at 64 bit.
	 * Encoded packet may end at an unaligned bit offset.
	 * In case a fallback clear text packet is transmitted in
	 * between, we adjust this offset back to the last 64bit
	 * aligned "native long word", which makes coding and decoding
	 * the plain text bitmap much more convenient.  */
#if BITS_PER_LONG == 64
	c->word_offset = c->bit_offset >> 6;
#elif BITS_PER_LONG == 32
	c->word_offset = c->bit_offset >> 5;
	c->word_offset &= ~(1UL);
#else
# error "unsupported BITS_PER_LONG"
#endif
}

<<<<<<< HEAD
#ifndef __packed
#define __packed __attribute__((packed))
#endif

/* This is the layout for a packet on the wire.
 * The byteorder is the network byte order.
 *     (except block_id and barrier fields.
 *	these are pointers to local structs
 *	and have no relevance for the partner,
 *	which just echoes them as received.)
 *
 * NOTE that the payload starts at a long aligned offset,
 * regardless of 32 or 64 bit arch!
 */
struct p_header80 {
	u32	  magic;
	u16	  command;
	u16	  length;	/* bytes of data after this header */
} __packed;

/* Header for big packets, Used for data packets exceeding 64kB */
struct p_header95 {
	u16	  magic;	/* use DRBD_MAGIC_BIG here */
	u16	  command;
	u32	  length;
} __packed;

struct p_header100 {
	u32	  magic;
	u16	  volume;
	u16	  command;
	u32	  length;
	u32	  pad;
} __packed;

extern unsigned int drbd_header_size(struct drbd_tconn *tconn);

/* these defines must not be changed without changing the protocol version */
#define DP_HARDBARRIER	      1 /* depricated */
#define DP_RW_SYNC	      2 /* equals REQ_SYNC    */
#define DP_MAY_SET_IN_SYNC    4
#define DP_UNPLUG             8 /* not used anymore   */
#define DP_FUA               16 /* equals REQ_FUA     */
#define DP_FLUSH             32 /* equals REQ_FLUSH   */
#define DP_DISCARD           64 /* equals REQ_DISCARD */
#define DP_SEND_RECEIVE_ACK 128 /* This is a proto B write request */
#define DP_SEND_WRITE_ACK   256 /* This is a proto C write request */

struct p_data {
	u64	    sector;    /* 64 bits sector number */
	u64	    block_id;  /* to identify the request in protocol B&C */
	u32	    seq_num;
	u32	    dp_flags;
} __packed;

/*
 * commands which share a struct:
 *  p_block_ack:
 *   P_RECV_ACK (proto B), P_WRITE_ACK (proto C),
 *   P_SUPERSEDED (proto C, two-primaries conflict detection)
 *  p_block_req:
 *   P_DATA_REQUEST, P_RS_DATA_REQUEST
 */
struct p_block_ack {
	u64	    sector;
	u64	    block_id;
	u32	    blksize;
	u32	    seq_num;
} __packed;

struct p_block_req {
	u64 sector;
	u64 block_id;
	u32 blksize;
	u32 pad;	/* to multiple of 8 Byte */
} __packed;

/*
 * commands with their own struct for additional fields:
 *   P_CONNECTION_FEATURES
 *   P_BARRIER
 *   P_BARRIER_ACK
 *   P_SYNC_PARAM
 *   ReportParams
 */

struct p_connection_features {
	u32 protocol_min;
	u32 feature_flags;
	u32 protocol_max;

	/* should be more than enough for future enhancements
	 * for now, feature_flags and the reserved array shall be zero.
	 */

	u32 _pad;
	u64 reserved[7];
} __packed;

struct p_barrier {
	u32 barrier;	/* barrier number _handle_ only */
	u32 pad;	/* to multiple of 8 Byte */
} __packed;

struct p_barrier_ack {
	u32 barrier;
	u32 set_size;
} __packed;

struct p_rs_param {
	u32 resync_rate;

	      /* Since protocol version 88 and higher. */
	char verify_alg[0];
} __packed;

struct p_rs_param_89 {
	u32 resync_rate;
        /* protocol version 89: */
	char verify_alg[SHARED_SECRET_MAX];
	char csums_alg[SHARED_SECRET_MAX];
} __packed;

struct p_rs_param_95 {
	u32 resync_rate;
	char verify_alg[SHARED_SECRET_MAX];
	char csums_alg[SHARED_SECRET_MAX];
	u32 c_plan_ahead;
	u32 c_delay_target;
	u32 c_fill_target;
	u32 c_max_rate;
} __packed;

enum drbd_conn_flags {
	CF_DISCARD_MY_DATA = 1,
	CF_DRY_RUN = 2,
};

struct p_protocol {
	u32 protocol;
	u32 after_sb_0p;
	u32 after_sb_1p;
	u32 after_sb_2p;
	u32 conn_flags;
	u32 two_primaries;

              /* Since protocol version 87 and higher. */
	char integrity_alg[0];

} __packed;

struct p_uuids {
	u64 uuid[UI_EXTENDED_SIZE];
} __packed;

struct p_rs_uuid {
	u64	    uuid;
} __packed;

struct p_sizes {
	u64	    d_size;  /* size of disk */
	u64	    u_size;  /* user requested size */
	u64	    c_size;  /* current exported size */
	u32	    max_bio_size;  /* Maximal size of a BIO */
	u16	    queue_order_type;  /* not yet implemented in DRBD*/
	u16	    dds_flags; /* use enum dds_flags here. */
} __packed;

struct p_state {
	u32	    state;
} __packed;

struct p_req_state {
	u32	    mask;
	u32	    val;
} __packed;

struct p_req_state_reply {
	u32	    retcode;
} __packed;

struct p_drbd06_param {
	u64	  size;
	u32	  state;
	u32	  blksize;
	u32	  protocol;
	u32	  version;
	u32	  gen_cnt[5];
	u32	  bit_map_gen[5];
} __packed;

struct p_block_desc {
	u64 sector;
	u32 blksize;
	u32 pad;	/* to multiple of 8 Byte */
} __packed;

/* Valid values for the encoding field.
 * Bump proto version when changing this. */
enum drbd_bitmap_code {
	/* RLE_VLI_Bytes = 0,
	 * and other bit variants had been defined during
	 * algorithm evaluation. */
	RLE_VLI_Bits = 2,
};

struct p_compressed_bm {
	/* (encoding & 0x0f): actual encoding, see enum drbd_bitmap_code
	 * (encoding & 0x80): polarity (set/unset) of first runlength
	 * ((encoding >> 4) & 0x07): pad_bits, number of trailing zero bits
	 * used to pad up to head.length bytes
	 */
	u8 encoding;

	u8 code[0];
} __packed;

struct p_delay_probe93 {
	u32     seq_num; /* sequence number to match the two probe packets */
	u32     offset;  /* usecs the probe got sent after the reference time point */
} __packed;

/*
 * Bitmap packets need to fit within a single page on the sender and receiver,
 * so we are limited to 4 KiB (and not to PAGE_SIZE, which can be bigger).
 */
#define DRBD_SOCKET_BUFFER_SIZE 4096
=======
extern unsigned int drbd_header_size(struct drbd_connection *connection);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**********************************************************************/
enum drbd_thread_state {
	NONE,
	RUNNING,
	EXITING,
	RESTARTING
};

struct drbd_thread {
	spinlock_t t_lock;
	struct task_struct *task;
	struct completion stop;
	enum drbd_thread_state t_state;
	int (*function) (struct drbd_thread *);
<<<<<<< HEAD
	struct drbd_tconn *tconn;
	int reset_cpu_mask;
	char name[9];
=======
	struct drbd_resource *resource;
	struct drbd_connection *connection;
	int reset_cpu_mask;
	const char *name;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static inline enum drbd_thread_state get_t_state(struct drbd_thread *thi)
{
	/* THINK testing the t_state seems to be uncritical in all cases
	 * (but thread_{start,stop}), so we can read it *without* the lock.
	 *	--lge */

	smp_rmb();
	return thi->t_state;
}

struct drbd_work {
	struct list_head list;
	int (*cb)(struct drbd_work *, int cancel);
<<<<<<< HEAD
	union {
		struct drbd_conf *mdev;
		struct drbd_tconn *tconn;
	};
=======
};

struct drbd_device_work {
	struct drbd_work w;
	struct drbd_device *device;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#include "drbd_interval.h"

<<<<<<< HEAD
extern int drbd_wait_misc(struct drbd_conf *, struct drbd_interval *);

struct drbd_request {
	struct drbd_work w;
=======
extern int drbd_wait_misc(struct drbd_device *, struct drbd_interval *);

extern void lock_all_resources(void);
extern void unlock_all_resources(void);

struct drbd_request {
	struct drbd_work w;
	struct drbd_device *device;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* if local IO is not allowed, will be NULL.
	 * if local IO _is_ allowed, holds the locally submitted bio clone,
	 * or, after local IO completion, the ERR_PTR(error).
	 * see drbd_request_endio(). */
	struct bio *private_bio;

	struct drbd_interval i;

	/* epoch: used to check on "completion" whether this req was in
	 * the current epoch, and we therefore have to close it,
	 * causing a p_barrier packet to be send, starting a new epoch.
	 *
	 * This corresponds to "barrier" in struct p_barrier[_ack],
	 * and to "barrier_nr" in struct drbd_epoch (and various
	 * comments/function parameters/local variable names).
	 */
	unsigned int epoch;

	struct list_head tl_requests; /* ring list in the transfer log */
	struct bio *master_bio;       /* master bio pointer */
<<<<<<< HEAD
	unsigned long start_time;
=======

	/* see struct drbd_device */
	struct list_head req_pending_master_completion;
	struct list_head req_pending_local;

	/* for generic IO accounting */
	unsigned long start_jif;

	/* for DRBD internal statistics */

	/* Minimal set of time stamps to determine if we wait for activity log
	 * transactions, local disk or peer.  32 bit "jiffies" are good enough,
	 * we don't expect a DRBD request to be stalled for several month.
	 */

	/* before actual request processing */
	unsigned long in_actlog_jif;

	/* local disk */
	unsigned long pre_submit_jif;

	/* per connection */
	unsigned long pre_send_jif;
	unsigned long acked_jif;
	unsigned long net_done_jif;

	/* Possibly even more detail to track each phase:
	 *  master_completion_jif
	 *      how long did it take to complete the master bio
	 *      (application visible latency)
	 *  allocated_jif
	 *      how long the master bio was blocked until we finally allocated
	 *      a tracking struct
	 *  in_actlog_jif
	 *      how long did we wait for activity log transactions
	 *
	 *  net_queued_jif
	 *      when did we finally queue it for sending
	 *  pre_send_jif
	 *      when did we start sending it
	 *  post_send_jif
	 *      how long did we block in the network stack trying to send it
	 *  acked_jif
	 *      when did we receive (or fake, in protocol A) a remote ACK
	 *  net_done_jif
	 *      when did we receive final acknowledgement (P_BARRIER_ACK),
	 *      or decide, e.g. on connection loss, that we do no longer expect
	 *      anything from this peer for this request.
	 *
	 *  pre_submit_jif
	 *  post_sub_jif
	 *      when did we start submiting to the lower level device,
	 *      and how long did we block in that submit function
	 *  local_completion_jif
	 *      how long did it take the lower level device to complete this request
	 */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* once it hits 0, we may complete the master_bio */
	atomic_t completion_ref;
	/* once it hits 0, we may destroy this drbd_request object */
	struct kref kref;

	unsigned rq_state; /* see comments above _req_mod() */
};

struct drbd_epoch {
<<<<<<< HEAD
	struct drbd_tconn *tconn;
=======
	struct drbd_connection *connection;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct list_head list;
	unsigned int barrier_nr;
	atomic_t epoch_size; /* increased on every request added. */
	atomic_t active;     /* increased on every req. added, and dec on every finished. */
	unsigned long flags;
};

<<<<<<< HEAD
=======
/* Prototype declaration of function defined in drbd_receiver.c */
int drbdd_init(struct drbd_thread *);
int drbd_asender(struct drbd_thread *);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* drbd_epoch flag bits */
enum {
	DE_HAVE_BARRIER_NUMBER,
};

enum epoch_event {
	EV_PUT,
	EV_GOT_BARRIER_NR,
	EV_BECAME_LAST,
	EV_CLEANUP = 32, /* used as flag */
};

<<<<<<< HEAD
struct drbd_wq_barrier {
	struct drbd_work w;
	struct completion done;
};

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct digest_info {
	int digest_size;
	void *digest;
};

struct drbd_peer_request {
	struct drbd_work w;
<<<<<<< HEAD
=======
	struct drbd_peer_device *peer_device;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct drbd_epoch *epoch; /* for writes */
	struct page *pages;
	atomic_t pending_bios;
	struct drbd_interval i;
	/* see comments on ee flag bits below */
	unsigned long flags;
<<<<<<< HEAD
=======
	unsigned long submit_jif;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	union {
		u64 block_id;
		struct digest_info *digest;
	};
};

/* ee flag bits.
 * While corresponding bios are in flight, the only modification will be
 * set_bit WAS_ERROR, which has to be atomic.
 * If no bios are in flight yet, or all have been completed,
 * non-atomic modification to ee->flags is ok.
 */
enum {
	__EE_CALL_AL_COMPLETE_IO,
	__EE_MAY_SET_IN_SYNC,

<<<<<<< HEAD
=======
	/* is this a TRIM aka REQ_DISCARD? */
	__EE_IS_TRIM,
	/* our lower level cannot handle trim,
	 * and we want to fall back to zeroout instead */
	__EE_IS_TRIM_USE_ZEROOUT,

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* In case a barrier failed,
	 * we need to resubmit without the barrier flag. */
	__EE_RESUBMITTED,

	/* we may have several bios per peer request.
	 * if any of those fail, we set this flag atomically
	 * from the endio callback */
	__EE_WAS_ERROR,

	/* This ee has a pointer to a digest instead of a block id */
	__EE_HAS_DIGEST,

	/* Conflicting local requests need to be restarted after this request */
	__EE_RESTART_REQUESTS,

	/* The peer wants a write ACK for this (wire proto C) */
	__EE_SEND_WRITE_ACK,

	/* Is set when net_conf had two_primaries set while creating this peer_req */
	__EE_IN_INTERVAL_TREE,
<<<<<<< HEAD
};
#define EE_CALL_AL_COMPLETE_IO (1<<__EE_CALL_AL_COMPLETE_IO)
#define EE_MAY_SET_IN_SYNC     (1<<__EE_MAY_SET_IN_SYNC)
#define	EE_RESUBMITTED         (1<<__EE_RESUBMITTED)
=======

	/* for debugfs: */
	/* has this been submitted, or does it still wait for something else? */
	__EE_SUBMITTED,

	/* this is/was a write request */
	__EE_WRITE,

	/* this is/was a write same request */
	__EE_WRITE_SAME,

	/* this originates from application on peer
	 * (not some resync or verify or other DRBD internal request) */
	__EE_APPLICATION,

	/* If it contains only 0 bytes, send back P_RS_DEALLOCATED */
	__EE_RS_THIN_REQ,
};
#define EE_CALL_AL_COMPLETE_IO (1<<__EE_CALL_AL_COMPLETE_IO)
#define EE_MAY_SET_IN_SYNC     (1<<__EE_MAY_SET_IN_SYNC)
#define EE_IS_TRIM             (1<<__EE_IS_TRIM)
#define EE_IS_TRIM_USE_ZEROOUT (1<<__EE_IS_TRIM_USE_ZEROOUT)
#define EE_RESUBMITTED         (1<<__EE_RESUBMITTED)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define EE_WAS_ERROR           (1<<__EE_WAS_ERROR)
#define EE_HAS_DIGEST          (1<<__EE_HAS_DIGEST)
#define EE_RESTART_REQUESTS	(1<<__EE_RESTART_REQUESTS)
#define EE_SEND_WRITE_ACK	(1<<__EE_SEND_WRITE_ACK)
#define EE_IN_INTERVAL_TREE	(1<<__EE_IN_INTERVAL_TREE)
<<<<<<< HEAD

/* flag bits per mdev */
=======
#define EE_SUBMITTED		(1<<__EE_SUBMITTED)
#define EE_WRITE		(1<<__EE_WRITE)
#define EE_WRITE_SAME		(1<<__EE_WRITE_SAME)
#define EE_APPLICATION		(1<<__EE_APPLICATION)
#define EE_RS_THIN_REQ		(1<<__EE_RS_THIN_REQ)

/* flag bits per device */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
enum {
	UNPLUG_REMOTE,		/* sending a "UnplugRemote" could help */
	MD_DIRTY,		/* current uuids and flags not yet on disk */
	USE_DEGR_WFC_T,		/* degr-wfc-timeout instead of wfc-timeout. */
	CL_ST_CHG_SUCCESS,
	CL_ST_CHG_FAIL,
	CRASHED_PRIMARY,	/* This node was a crashed primary.
				 * Gets cleared when the state.conn
				 * goes into C_CONNECTED state. */
	CONSIDER_RESYNC,

	MD_NO_FUA,		/* Users wants us to not use FUA/FLUSH on meta data dev */
<<<<<<< HEAD
	SUSPEND_IO,		/* suspend application io */
	BITMAP_IO,		/* suspend application io;
				   once no more io in flight, start bitmap io */
	BITMAP_IO_QUEUED,       /* Started bitmap IO */
	GO_DISKLESS,		/* Disk is being detached, on io-error or admin request. */
=======

	BITMAP_IO,		/* suspend application io;
				   once no more io in flight, start bitmap io */
	BITMAP_IO_QUEUED,       /* Started bitmap IO */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	WAS_IO_ERROR,		/* Local disk failed, returned IO error */
	WAS_READ_ERROR,		/* Local disk READ failed (set additionally to the above) */
	FORCE_DETACH,		/* Force-detach from local disk, aborting any pending local IO */
	RESYNC_AFTER_NEG,       /* Resync after online grow after the attach&negotiate finished. */
	RESIZE_PENDING,		/* Size change detected locally, waiting for the response from
				 * the peer, if it changed there as well. */
	NEW_CUR_UUID,		/* Create new current UUID when thawing IO */
	AL_SUSPENDED,		/* Activity logging is currently suspended. */
	AHEAD_TO_SYNC_SOURCE,   /* Ahead -> SyncSource queued */
	B_RS_H_DONE,		/* Before resync handler done (already executed) */
	DISCARD_MY_DATA,	/* discard_my_data flag per volume */
	READ_BALANCE_RR,
<<<<<<< HEAD
};

struct drbd_bitmap; /* opaque for drbd_conf */
=======

	FLUSH_PENDING,		/* if set, device->flush_jif is when we submitted that flush
				 * from drbd_flush_after_epoch() */

	/* cleared only after backing device related structures have been destroyed. */
	GOING_DISKLESS,		/* Disk is being detached, because of io-error, or admin request. */

	/* to be used in drbd_device_post_work() */
	GO_DISKLESS,		/* tell worker to schedule cleanup before detach */
	DESTROY_DISK,		/* tell worker to close backing devices and destroy related structures. */
	MD_SYNC,		/* tell worker to call drbd_md_sync() */
	RS_START,		/* tell worker to start resync/OV */
	RS_PROGRESS,		/* tell worker that resync made significant progress */
	RS_DONE,		/* tell worker that resync is done */
};

struct drbd_bitmap; /* opaque for drbd_device */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* definition of bits in bm_flags to be used in drbd_bm_lock
 * and drbd_bitmap_io and friends. */
enum bm_flag {
<<<<<<< HEAD
	/* do we need to kfree, or vfree bm_pages? */
	BM_P_VMALLOCED = 0x10000, /* internal use only, will be masked out */

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* currently locked for bulk operation */
	BM_LOCKED_MASK = 0xf,

	/* in detail, that is: */
	BM_DONT_CLEAR = 0x1,
	BM_DONT_SET   = 0x2,
	BM_DONT_TEST  = 0x4,

	/* so we can mark it locked for bulk operation,
	 * and still allow all non-bulk operations */
	BM_IS_LOCKED  = 0x8,

	/* (test bit, count bit) allowed (common case) */
	BM_LOCKED_TEST_ALLOWED = BM_DONT_CLEAR | BM_DONT_SET | BM_IS_LOCKED,

	/* testing bits, as well as setting new bits allowed, but clearing bits
	 * would be unexpected.  Used during bitmap receive.  Setting new bits
	 * requires sending of "out-of-sync" information, though. */
	BM_LOCKED_SET_ALLOWED = BM_DONT_CLEAR | BM_IS_LOCKED,

	/* for drbd_bm_write_copy_pages, everything is allowed,
	 * only concurrent bulk operations are locked out. */
	BM_LOCKED_CHANGE_ALLOWED = BM_IS_LOCKED,
};

struct drbd_work_queue {
	struct list_head q;
	spinlock_t q_lock;  /* to protect the list. */
	wait_queue_head_t q_wait;
};

struct drbd_socket {
	struct mutex mutex;
	struct socket    *socket;
	/* this way we get our
	 * send/receive buffers off the stack */
	void *sbuf;
	void *rbuf;
};

struct drbd_md {
	u64 md_offset;		/* sector offset to 'super' block */

	u64 la_size_sect;	/* last agreed size, unit sectors */
	spinlock_t uuid_lock;
	u64 uuid[UI_SIZE];
	u64 device_uuid;
	u32 flags;
	u32 md_size_sect;

	s32 al_offset;	/* signed relative sector offset to activity log */
	s32 bm_offset;	/* signed relative sector offset to bitmap */

	/* cached value of bdev->disk_conf->meta_dev_idx (see below) */
	s32 meta_dev_idx;

	/* see al_tr_number_to_on_disk_sector() */
	u32 al_stripes;
	u32 al_stripe_size_4k;
	u32 al_size_4k; /* cached product of the above */
};

struct drbd_backing_dev {
	struct block_device *backing_bdev;
	struct block_device *md_bdev;
	struct drbd_md md;
<<<<<<< HEAD
	struct disk_conf *disk_conf; /* RCU, for updates: mdev->tconn->conf_update */
=======
	struct disk_conf *disk_conf; /* RCU, for updates: resource->conf_update */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sector_t known_size; /* last known size of that backing device */
};

struct drbd_md_io {
<<<<<<< HEAD
=======
	struct page *page;
	unsigned long start_jif;	/* last call to drbd_md_get_buffer */
	unsigned long submit_jif;	/* last _drbd_md_sync_page_io() submit */
	const char *current_use;
	atomic_t in_use;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int done;
	int error;
};

struct bm_io_work {
	struct drbd_work w;
	char *why;
	enum bm_flag flags;
<<<<<<< HEAD
	int (*io_fn)(struct drbd_conf *mdev);
	void (*done)(struct drbd_conf *mdev, int rv);
};

enum write_ordering_e {
	WO_none,
	WO_drain_io,
	WO_bdev_flush,
=======
	int (*io_fn)(struct drbd_device *device);
	void (*done)(struct drbd_device *device, int rv);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct fifo_buffer {
	unsigned int head_index;
	unsigned int size;
	int total; /* sum of all values */
	int values[0];
};
extern struct fifo_buffer *fifo_alloc(int fifo_size);

<<<<<<< HEAD
/* flag bits per tconn */
enum {
	NET_CONGESTED,		/* The data socket is congested */
	RESOLVE_CONFLICTS,	/* Set on one node, cleared on the peer! */
	SEND_PING,		/* whether asender should send a ping asap */
	SIGNAL_ASENDER,		/* whether asender wants to be interrupted */
=======
/* flag bits per connection */
enum {
	NET_CONGESTED,		/* The data socket is congested */
	RESOLVE_CONFLICTS,	/* Set on one node, cleared on the peer! */
	SEND_PING,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	GOT_PING_ACK,		/* set when we receive a ping_ack packet, ping_wait gets woken */
	CONN_WD_ST_CHG_REQ,	/* A cluster wide state change on the connection is active */
	CONN_WD_ST_CHG_OKAY,
	CONN_WD_ST_CHG_FAIL,
	CONN_DRY_RUN,		/* Expect disconnect after resync handshake. */
	CREATE_BARRIER,		/* next P_DATA is preceded by a P_BARRIER */
	STATE_SENT,		/* Do not change state/UUIDs while this is set */
	CALLBACK_PENDING,	/* Whether we have a call_usermodehelper(, UMH_WAIT_PROC)
				 * pending, from drbd worker context.
				 * If set, bdi_write_congested() returns true,
				 * so shrink_page_list() would not recurse into,
				 * and potentially deadlock on, this drbd worker.
				 */
	DISCONNECT_SENT,
<<<<<<< HEAD
};

struct drbd_tconn {			/* is a resource from the config file */
	char *name;			/* Resource name */
	struct list_head all_tconn;	/* linked on global drbd_tconns */
	struct kref kref;
	struct idr volumes;		/* <tconn, vnr> to mdev mapping */
	enum drbd_conns cstate;		/* Only C_STANDALONE to C_WF_REPORT_PARAMS */
	unsigned susp:1;		/* IO suspended by user */
	unsigned susp_nod:1;		/* IO suspended because no data */
	unsigned susp_fen:1;		/* IO suspended because fence peer handler runs */
	struct mutex cstate_mutex;	/* Protects graceful disconnects */

	unsigned long flags;
	struct net_conf *net_conf;	/* content protected by rcu */
	struct mutex conf_update;	/* mutex for ready-copy-update of net_conf and disk_conf */
	wait_queue_head_t ping_wait;	/* Woken upon reception of a ping, and a state change */
	struct res_opts res_opts;
=======

	DEVICE_WORK_PENDING,	/* tell worker that some device has pending work */
};

enum which_state { NOW, OLD = NOW, NEW };

struct drbd_resource {
	char *name;
#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_res;
	struct dentry *debugfs_res_volumes;
	struct dentry *debugfs_res_connections;
	struct dentry *debugfs_res_in_flight_summary;
#endif
	struct kref kref;
	struct idr devices;		/* volume number to device mapping */
	struct list_head connections;
	struct list_head resources;
	struct res_opts res_opts;
	struct mutex conf_update;	/* mutex for ready-copy-update of net_conf and disk_conf */
	struct mutex adm_mutex;		/* mutex to serialize administrative requests */
	spinlock_t req_lock;

	unsigned susp:1;		/* IO suspended by user */
	unsigned susp_nod:1;		/* IO suspended because no data */
	unsigned susp_fen:1;		/* IO suspended because fence peer handler runs */

	enum write_ordering_e write_ordering;

	cpumask_var_t cpu_mask;
};

struct drbd_thread_timing_details
{
	unsigned long start_jif;
	void *cb_addr;
	const char *caller_fn;
	unsigned int line;
	unsigned int cb_nr;
};

struct drbd_connection {
	struct list_head connections;
	struct drbd_resource *resource;
#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_conn;
	struct dentry *debugfs_conn_callback_history;
	struct dentry *debugfs_conn_oldest_requests;
#endif
	struct kref kref;
	struct idr peer_devices;	/* volume number to peer device mapping */
	enum drbd_conns cstate;		/* Only C_STANDALONE to C_WF_REPORT_PARAMS */
	struct mutex cstate_mutex;	/* Protects graceful disconnects */
	unsigned int connect_cnt;	/* Inc each time a connection is established */

	unsigned long flags;
	struct net_conf *net_conf;	/* content protected by rcu */
	wait_queue_head_t ping_wait;	/* Woken upon reception of a ping, and a state change */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	struct sockaddr_storage my_addr;
	int my_addr_len;
	struct sockaddr_storage peer_addr;
	int peer_addr_len;

	struct drbd_socket data;	/* data/barrier/cstate/parameter packets */
	struct drbd_socket meta;	/* ping/ack (metadata) packets */
	int agreed_pro_version;		/* actually used protocol version */
<<<<<<< HEAD
	unsigned long last_received;	/* in jiffies, either socket */
	unsigned int ko_count;

	spinlock_t req_lock;

	struct list_head transfer_log;	/* all requests not yet fully processed */

	struct crypto_hash *cram_hmac_tfm;
	struct crypto_hash *integrity_tfm;  /* checksums we compute, updates protected by tconn->data->mutex */
	struct crypto_hash *peer_integrity_tfm;  /* checksums we verify, only accessed from receiver thread  */
	struct crypto_hash *csums_tfm;
	struct crypto_hash *verify_tfm;
=======
	u32 agreed_features;
	unsigned long last_received;	/* in jiffies, either socket */
	unsigned int ko_count;

	struct list_head transfer_log;	/* all requests not yet fully processed */

	struct crypto_shash *cram_hmac_tfm;
	struct crypto_ahash *integrity_tfm;  /* checksums we compute, updates protected by connection->data->mutex */
	struct crypto_ahash *peer_integrity_tfm;  /* checksums we verify, only accessed from receiver thread  */
	struct crypto_ahash *csums_tfm;
	struct crypto_ahash *verify_tfm;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	void *int_dig_in;
	void *int_dig_vv;

	/* receiver side */
	struct drbd_epoch *current_epoch;
	spinlock_t epoch_lock;
	unsigned int epochs;
<<<<<<< HEAD
	enum write_ordering_e write_ordering;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	atomic_t current_tle_nr;	/* transfer log epoch number */
	unsigned current_tle_writes;	/* writes seen within this tl epoch */

	unsigned long last_reconnect_jif;
	struct drbd_thread receiver;
	struct drbd_thread worker;
<<<<<<< HEAD
	struct drbd_thread asender;
	cpumask_var_t cpu_mask;
=======
	struct drbd_thread ack_receiver;
	struct workqueue_struct *ack_sender;

	/* cached pointers,
	 * so we can look up the oldest pending requests more quickly.
	 * protected by resource->req_lock */
	struct drbd_request *req_next; /* DRBD 9: todo.req_next */
	struct drbd_request *req_ack_pending;
	struct drbd_request *req_not_net_done;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* sender side */
	struct drbd_work_queue sender_work;

<<<<<<< HEAD
	struct {
=======
#define DRBD_THREAD_DETAILS_HIST	16
	unsigned int w_cb_nr; /* keeps counting up */
	unsigned int r_cb_nr; /* keeps counting up */
	struct drbd_thread_timing_details w_timing_details[DRBD_THREAD_DETAILS_HIST];
	struct drbd_thread_timing_details r_timing_details[DRBD_THREAD_DETAILS_HIST];

	struct {
		unsigned long last_sent_barrier_jif;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* whether this sender thread
		 * has processed a single write yet. */
		bool seen_any_write_yet;

		/* Which barrier number to send with the next P_BARRIER */
		int current_epoch_nr;

		/* how many write requests have been sent
		 * with req->epoch == current_epoch_nr.
		 * If none, no P_BARRIER will be sent. */
		unsigned current_epoch_writes;
	} send;
};

<<<<<<< HEAD
=======
static inline bool has_net_conf(struct drbd_connection *connection)
{
	bool has_net_conf;

	rcu_read_lock();
	has_net_conf = rcu_dereference(connection->net_conf);
	rcu_read_unlock();

	return has_net_conf;
}

void __update_timing_details(
		struct drbd_thread_timing_details *tdp,
		unsigned int *cb_nr,
		void *cb,
		const char *fn, const unsigned int line);

#define update_worker_timing_details(c, cb) \
	__update_timing_details(c->w_timing_details, &c->w_cb_nr, cb, __func__ , __LINE__ )
#define update_receiver_timing_details(c, cb) \
	__update_timing_details(c->r_timing_details, &c->r_cb_nr, cb, __func__ , __LINE__ )

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct submit_worker {
	struct workqueue_struct *wq;
	struct work_struct worker;

<<<<<<< HEAD
	spinlock_t lock;
	struct list_head writes;
};

struct drbd_conf {
	struct drbd_tconn *tconn;
	int vnr;			/* volume number within the connection */
=======
	/* protected by ..->resource->req_lock */
	struct list_head writes;
};

struct drbd_peer_device {
	struct list_head peer_devices;
	struct drbd_device *device;
	struct drbd_connection *connection;
	struct work_struct send_acks_work;
#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_peer_dev;
#endif
};

struct drbd_device {
	struct drbd_resource *resource;
	struct list_head peer_devices;
	struct list_head pending_bitmap_io;

	unsigned long flush_jif;
#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_minor;
	struct dentry *debugfs_vol;
	struct dentry *debugfs_vol_oldest_requests;
	struct dentry *debugfs_vol_act_log_extents;
	struct dentry *debugfs_vol_resync_extents;
	struct dentry *debugfs_vol_data_gen_id;
	struct dentry *debugfs_vol_ed_gen_id;
#endif

	unsigned int vnr;	/* volume number within the connection */
	unsigned int minor;	/* device minor number */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct kref kref;

	/* things that are stored as / read from meta data on disk */
	unsigned long flags;

	/* configured by drbdsetup */
	struct drbd_backing_dev *ldev __protected_by(local);

	sector_t p_size;     /* partner's disk size */
	struct request_queue *rq_queue;
	struct block_device *this_bdev;
	struct gendisk	    *vdisk;

	unsigned long last_reattach_jif;
<<<<<<< HEAD
	struct drbd_work  resync_work,
			  unplug_work,
			  go_diskless,
			  md_sync_work,
			  start_resync_work;
=======
	struct drbd_work resync_work;
	struct drbd_work unplug_work;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct timer_list resync_timer;
	struct timer_list md_sync_timer;
	struct timer_list start_resync_timer;
	struct timer_list request_timer;
<<<<<<< HEAD
#ifdef DRBD_DEBUG_MD_SYNC
	struct {
		unsigned int line;
		const char* func;
	} last_md_mark_dirty;
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Used after attach while negotiating new disk state. */
	union drbd_state new_state_tmp;

	union drbd_dev_state state;
	wait_queue_head_t misc_wait;
	wait_queue_head_t state_wait;  /* upon each state change. */
	unsigned int send_cnt;
	unsigned int recv_cnt;
	unsigned int read_cnt;
	unsigned int writ_cnt;
	unsigned int al_writ_cnt;
	unsigned int bm_writ_cnt;
	atomic_t ap_bio_cnt;	 /* Requests we need to complete */
<<<<<<< HEAD
=======
	atomic_t ap_actlog_cnt;  /* Requests waiting for activity log */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	atomic_t ap_pending_cnt; /* AP data packets on the wire, ack expected */
	atomic_t rs_pending_cnt; /* RS request/data packets on the wire */
	atomic_t unacked_cnt;	 /* Need to send replies for */
	atomic_t local_cnt;	 /* Waiting for local completion */
<<<<<<< HEAD
=======
	atomic_t suspend_cnt;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Interval tree of pending local requests */
	struct rb_root read_requests;
	struct rb_root write_requests;

<<<<<<< HEAD
=======
	/* for statistics and timeouts */
	/* [0] read, [1] write */
	struct list_head pending_master_completion[2];
	struct list_head pending_completion[2];

	/* use checksums for *this* resync */
	bool use_csums;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* blocks to resync in this run [unit BM_BLOCK_SIZE] */
	unsigned long rs_total;
	/* number of resync blocks that failed in this run */
	unsigned long rs_failed;
	/* Syncer's start time [unit jiffies] */
	unsigned long rs_start;
	/* cumulated time in PausedSyncX state [unit jiffies] */
	unsigned long rs_paused;
	/* skipped because csum was equal [unit BM_BLOCK_SIZE] */
	unsigned long rs_same_csum;
#define DRBD_SYNC_MARKS 8
#define DRBD_SYNC_MARK_STEP (3*HZ)
	/* block not up-to-date at mark [unit BM_BLOCK_SIZE] */
	unsigned long rs_mark_left[DRBD_SYNC_MARKS];
	/* marks's time [unit jiffies] */
	unsigned long rs_mark_time[DRBD_SYNC_MARKS];
	/* current index into rs_mark_{left,time} */
	int rs_last_mark;
	unsigned long rs_last_bcast; /* [unit jiffies] */

	/* where does the admin want us to start? (sector) */
	sector_t ov_start_sector;
	sector_t ov_stop_sector;
	/* where are we now? (sector) */
	sector_t ov_position;
	/* Start sector of out of sync range (to merge printk reporting). */
	sector_t ov_last_oos_start;
	/* size of out-of-sync range in sectors. */
	sector_t ov_last_oos_size;
	unsigned long ov_left; /* in bits */

	struct drbd_bitmap *bitmap;
	unsigned long bm_resync_fo; /* bit offset for drbd_bm_find_next */

	/* Used to track operations of resync... */
	struct lru_cache *resync;
	/* Number of locked elements in resync LRU */
	unsigned int resync_locked;
	/* resync extent number waiting for application requests */
	unsigned int resync_wenr;

	int open_cnt;
	u64 *p_uuid;

	struct list_head active_ee; /* IO in progress (P_DATA gets written to disk) */
	struct list_head sync_ee;   /* IO in progress (P_RS_DATA_REPLY gets written to disk) */
	struct list_head done_ee;   /* need to send P_WRITE_ACK */
	struct list_head read_ee;   /* [RS]P_DATA_REQUEST being read */
	struct list_head net_ee;    /* zero-copy network send in progress */

	int next_barrier_nr;
	struct list_head resync_reads;
	atomic_t pp_in_use;		/* allocated from page pool */
	atomic_t pp_in_use_by_net;	/* sendpage()d, still referenced by tcp */
	wait_queue_head_t ee_wait;
<<<<<<< HEAD
	struct page *md_io_page;	/* one page buffer for md_io */
	struct drbd_md_io md_io;
	atomic_t md_io_in_use;		/* protects the md_io, md_io_page and md_io_tmpp */
=======
	struct drbd_md_io md_io;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spinlock_t al_lock;
	wait_queue_head_t al_wait;
	struct lru_cache *act_log;	/* activity log */
	unsigned int al_tr_number;
	int al_tr_cycle;
	wait_queue_head_t seq_wait;
	atomic_t packet_seq;
	unsigned int peer_seq;
	spinlock_t peer_seq_lock;
<<<<<<< HEAD
	unsigned int minor;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long comm_bm_set; /* communicated number of set bits. */
	struct bm_io_work bm_io_work;
	u64 ed_uuid; /* UUID of the exposed data */
	struct mutex own_state_mutex;
<<<<<<< HEAD
	struct mutex *state_mutex; /* either own_state_mutex or mdev->tconn->cstate_mutex */
=======
	struct mutex *state_mutex; /* either own_state_mutex or first_peer_device(device)->connection->cstate_mutex */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	char congestion_reason;  /* Why we where congested... */
	atomic_t rs_sect_in; /* for incoming resync data rate, SyncTarget */
	atomic_t rs_sect_ev; /* for submitted resync data rate, both */
	int rs_last_sect_ev; /* counter to compare with */
	int rs_last_events;  /* counter of read or write "events" (unit sectors)
			      * on the lower level device when we last looked. */
	int c_sync_rate; /* current resync rate after syncer throttle magic */
<<<<<<< HEAD
	struct fifo_buffer *rs_plan_s; /* correction values of resync planer (RCU, tconn->conn_update) */
=======
	struct fifo_buffer *rs_plan_s; /* correction values of resync planer (RCU, connection->conn_update) */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int rs_in_flight; /* resync sectors in flight (to proxy, in proxy and from proxy) */
	atomic_t ap_in_flight; /* App sectors in flight (waiting for ack) */
	unsigned int peer_max_bio_size;
	unsigned int local_max_bio_size;

	/* any requests that would block in drbd_make_request()
	 * are deferred to this single-threaded work queue */
	struct submit_worker submit;
};

<<<<<<< HEAD
static inline struct drbd_conf *minor_to_mdev(unsigned int minor)
{
	return (struct drbd_conf *)idr_find(&minors, minor);
}

static inline unsigned int mdev_to_minor(struct drbd_conf *mdev)
{
	return mdev->minor;
}

static inline struct drbd_conf *vnr_to_mdev(struct drbd_tconn *tconn, int vnr)
{
	return (struct drbd_conf *)idr_find(&tconn->volumes, vnr);
=======
struct drbd_bm_aio_ctx {
	struct drbd_device *device;
	struct list_head list; /* on device->pending_bitmap_io */;
	unsigned long start_jif;
	atomic_t in_flight;
	unsigned int done;
	unsigned flags;
#define BM_AIO_COPY_PAGES	1
#define BM_AIO_WRITE_HINTED	2
#define BM_AIO_WRITE_ALL_PAGES	4
#define BM_AIO_READ		8
	int error;
	struct kref kref;
};

struct drbd_config_context {
	/* assigned from drbd_genlmsghdr */
	unsigned int minor;
	/* assigned from request attributes, if present */
	unsigned int volume;
#define VOLUME_UNSPECIFIED		(-1U)
	/* pointer into the request skb,
	 * limited lifetime! */
	char *resource_name;
	struct nlattr *my_addr;
	struct nlattr *peer_addr;

	/* reply buffer */
	struct sk_buff *reply_skb;
	/* pointer into reply buffer */
	struct drbd_genlmsghdr *reply_dh;
	/* resolved from attributes, if possible */
	struct drbd_device *device;
	struct drbd_resource *resource;
	struct drbd_connection *connection;
};

static inline struct drbd_device *minor_to_device(unsigned int minor)
{
	return (struct drbd_device *)idr_find(&drbd_devices, minor);
}

static inline struct drbd_peer_device *first_peer_device(struct drbd_device *device)
{
	return list_first_entry_or_null(&device->peer_devices, struct drbd_peer_device, peer_devices);
}

static inline struct drbd_peer_device *
conn_peer_device(struct drbd_connection *connection, int volume_number)
{
	return idr_find(&connection->peer_devices, volume_number);
}

#define for_each_resource(resource, _resources) \
	list_for_each_entry(resource, _resources, resources)

#define for_each_resource_rcu(resource, _resources) \
	list_for_each_entry_rcu(resource, _resources, resources)

#define for_each_resource_safe(resource, tmp, _resources) \
	list_for_each_entry_safe(resource, tmp, _resources, resources)

#define for_each_connection(connection, resource) \
	list_for_each_entry(connection, &resource->connections, connections)

#define for_each_connection_rcu(connection, resource) \
	list_for_each_entry_rcu(connection, &resource->connections, connections)

#define for_each_connection_safe(connection, tmp, resource) \
	list_for_each_entry_safe(connection, tmp, &resource->connections, connections)

#define for_each_peer_device(peer_device, device) \
	list_for_each_entry(peer_device, &device->peer_devices, peer_devices)

#define for_each_peer_device_rcu(peer_device, device) \
	list_for_each_entry_rcu(peer_device, &device->peer_devices, peer_devices)

#define for_each_peer_device_safe(peer_device, tmp, device) \
	list_for_each_entry_safe(peer_device, tmp, &device->peer_devices, peer_devices)

static inline unsigned int device_to_minor(struct drbd_device *device)
{
	return device->minor;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * function declarations
 *************************/

/* drbd_main.c */

enum dds_flags {
	DDSF_FORCED    = 1,
	DDSF_NO_RESYNC = 2, /* Do not run a resync for the new space */
};

<<<<<<< HEAD
extern void drbd_init_set_defaults(struct drbd_conf *mdev);
extern int  drbd_thread_start(struct drbd_thread *thi);
extern void _drbd_thread_stop(struct drbd_thread *thi, int restart, int wait);
extern char *drbd_task_to_thread_name(struct drbd_tconn *tconn, struct task_struct *task);
#ifdef CONFIG_SMP
extern void drbd_thread_current_set_cpu(struct drbd_thread *thi);
extern void drbd_calc_cpu_mask(struct drbd_tconn *tconn);
#else
#define drbd_thread_current_set_cpu(A) ({})
#define drbd_calc_cpu_mask(A) ({})
#endif
extern void tl_release(struct drbd_tconn *, unsigned int barrier_nr,
		       unsigned int set_size);
extern void tl_clear(struct drbd_tconn *);
extern void drbd_free_sock(struct drbd_tconn *tconn);
extern int drbd_send(struct drbd_tconn *tconn, struct socket *sock,
		     void *buf, size_t size, unsigned msg_flags);
extern int drbd_send_all(struct drbd_tconn *, struct socket *, void *, size_t,
			 unsigned);

extern int __drbd_send_protocol(struct drbd_tconn *tconn, enum drbd_packet cmd);
extern int drbd_send_protocol(struct drbd_tconn *tconn);
extern int drbd_send_uuids(struct drbd_conf *mdev);
extern int drbd_send_uuids_skip_initial_sync(struct drbd_conf *mdev);
extern void drbd_gen_and_send_sync_uuid(struct drbd_conf *mdev);
extern int drbd_send_sizes(struct drbd_conf *mdev, int trigger_reply, enum dds_flags flags);
extern int drbd_send_state(struct drbd_conf *mdev, union drbd_state s);
extern int drbd_send_current_state(struct drbd_conf *mdev);
extern int drbd_send_sync_param(struct drbd_conf *mdev);
extern void drbd_send_b_ack(struct drbd_tconn *tconn, u32 barrier_nr,
			    u32 set_size);
extern int drbd_send_ack(struct drbd_conf *, enum drbd_packet,
			 struct drbd_peer_request *);
extern void drbd_send_ack_rp(struct drbd_conf *mdev, enum drbd_packet cmd,
			     struct p_block_req *rp);
extern void drbd_send_ack_dp(struct drbd_conf *mdev, enum drbd_packet cmd,
			     struct p_data *dp, int data_size);
extern int drbd_send_ack_ex(struct drbd_conf *mdev, enum drbd_packet cmd,
			    sector_t sector, int blksize, u64 block_id);
extern int drbd_send_out_of_sync(struct drbd_conf *, struct drbd_request *);
extern int drbd_send_block(struct drbd_conf *, enum drbd_packet,
			   struct drbd_peer_request *);
extern int drbd_send_dblock(struct drbd_conf *mdev, struct drbd_request *req);
extern int drbd_send_drequest(struct drbd_conf *mdev, int cmd,
			      sector_t sector, int size, u64 block_id);
extern int drbd_send_drequest_csum(struct drbd_conf *mdev, sector_t sector,
				   int size, void *digest, int digest_size,
				   enum drbd_packet cmd);
extern int drbd_send_ov_request(struct drbd_conf *mdev,sector_t sector,int size);

extern int drbd_send_bitmap(struct drbd_conf *mdev);
extern void drbd_send_sr_reply(struct drbd_conf *mdev, enum drbd_state_rv retcode);
extern void conn_send_sr_reply(struct drbd_tconn *tconn, enum drbd_state_rv retcode);
extern void drbd_free_bc(struct drbd_backing_dev *ldev);
extern void drbd_mdev_cleanup(struct drbd_conf *mdev);
void drbd_print_uuids(struct drbd_conf *mdev, const char *text);

extern void conn_md_sync(struct drbd_tconn *tconn);
extern void drbd_md_sync(struct drbd_conf *mdev);
extern int  drbd_md_read(struct drbd_conf *mdev, struct drbd_backing_dev *bdev);
extern void drbd_uuid_set(struct drbd_conf *mdev, int idx, u64 val) __must_hold(local);
extern void _drbd_uuid_set(struct drbd_conf *mdev, int idx, u64 val) __must_hold(local);
extern void drbd_uuid_new_current(struct drbd_conf *mdev) __must_hold(local);
extern void drbd_uuid_set_bm(struct drbd_conf *mdev, u64 val) __must_hold(local);
extern void drbd_uuid_move_history(struct drbd_conf *mdev) __must_hold(local);
extern void __drbd_uuid_set(struct drbd_conf *mdev, int idx, u64 val) __must_hold(local);
extern void drbd_md_set_flag(struct drbd_conf *mdev, int flags) __must_hold(local);
extern void drbd_md_clear_flag(struct drbd_conf *mdev, int flags)__must_hold(local);
extern int drbd_md_test_flag(struct drbd_backing_dev *, int);
#ifndef DRBD_DEBUG_MD_SYNC
extern void drbd_md_mark_dirty(struct drbd_conf *mdev);
#else
#define drbd_md_mark_dirty(m)	drbd_md_mark_dirty_(m, __LINE__ , __func__ )
extern void drbd_md_mark_dirty_(struct drbd_conf *mdev,
		unsigned int line, const char *func);
#endif
extern void drbd_queue_bitmap_io(struct drbd_conf *mdev,
				 int (*io_fn)(struct drbd_conf *),
				 void (*done)(struct drbd_conf *, int),
				 char *why, enum bm_flag flags);
extern int drbd_bitmap_io(struct drbd_conf *mdev,
		int (*io_fn)(struct drbd_conf *),
		char *why, enum bm_flag flags);
extern int drbd_bitmap_io_from_worker(struct drbd_conf *mdev,
		int (*io_fn)(struct drbd_conf *),
		char *why, enum bm_flag flags);
extern int drbd_bmio_set_n_write(struct drbd_conf *mdev);
extern int drbd_bmio_clear_n_write(struct drbd_conf *mdev);
extern void drbd_ldev_destroy(struct drbd_conf *mdev);
=======
extern void drbd_init_set_defaults(struct drbd_device *device);
extern int  drbd_thread_start(struct drbd_thread *thi);
extern void _drbd_thread_stop(struct drbd_thread *thi, int restart, int wait);
#ifdef CONFIG_SMP
extern void drbd_thread_current_set_cpu(struct drbd_thread *thi);
#else
#define drbd_thread_current_set_cpu(A) ({})
#endif
extern void tl_release(struct drbd_connection *, unsigned int barrier_nr,
		       unsigned int set_size);
extern void tl_clear(struct drbd_connection *);
extern void drbd_free_sock(struct drbd_connection *connection);
extern int drbd_send(struct drbd_connection *connection, struct socket *sock,
		     void *buf, size_t size, unsigned msg_flags);
extern int drbd_send_all(struct drbd_connection *, struct socket *, void *, size_t,
			 unsigned);

extern int __drbd_send_protocol(struct drbd_connection *connection, enum drbd_packet cmd);
extern int drbd_send_protocol(struct drbd_connection *connection);
extern int drbd_send_uuids(struct drbd_peer_device *);
extern int drbd_send_uuids_skip_initial_sync(struct drbd_peer_device *);
extern void drbd_gen_and_send_sync_uuid(struct drbd_peer_device *);
extern int drbd_send_sizes(struct drbd_peer_device *, int trigger_reply, enum dds_flags flags);
extern int drbd_send_state(struct drbd_peer_device *, union drbd_state s);
extern int drbd_send_current_state(struct drbd_peer_device *);
extern int drbd_send_sync_param(struct drbd_peer_device *);
extern void drbd_send_b_ack(struct drbd_connection *connection, u32 barrier_nr,
			    u32 set_size);
extern int drbd_send_ack(struct drbd_peer_device *, enum drbd_packet,
			 struct drbd_peer_request *);
extern void drbd_send_ack_rp(struct drbd_peer_device *, enum drbd_packet,
			     struct p_block_req *rp);
extern void drbd_send_ack_dp(struct drbd_peer_device *, enum drbd_packet,
			     struct p_data *dp, int data_size);
extern int drbd_send_ack_ex(struct drbd_peer_device *, enum drbd_packet,
			    sector_t sector, int blksize, u64 block_id);
extern int drbd_send_out_of_sync(struct drbd_peer_device *, struct drbd_request *);
extern int drbd_send_block(struct drbd_peer_device *, enum drbd_packet,
			   struct drbd_peer_request *);
extern int drbd_send_dblock(struct drbd_peer_device *, struct drbd_request *req);
extern int drbd_send_drequest(struct drbd_peer_device *, int cmd,
			      sector_t sector, int size, u64 block_id);
extern int drbd_send_drequest_csum(struct drbd_peer_device *, sector_t sector,
				   int size, void *digest, int digest_size,
				   enum drbd_packet cmd);
extern int drbd_send_ov_request(struct drbd_peer_device *, sector_t sector, int size);

extern int drbd_send_bitmap(struct drbd_device *device);
extern void drbd_send_sr_reply(struct drbd_peer_device *, enum drbd_state_rv retcode);
extern void conn_send_sr_reply(struct drbd_connection *connection, enum drbd_state_rv retcode);
extern int drbd_send_rs_deallocated(struct drbd_peer_device *, struct drbd_peer_request *);
extern void drbd_backing_dev_free(struct drbd_device *device, struct drbd_backing_dev *ldev);
extern void drbd_device_cleanup(struct drbd_device *device);
void drbd_print_uuids(struct drbd_device *device, const char *text);

extern void conn_md_sync(struct drbd_connection *connection);
extern void drbd_md_write(struct drbd_device *device, void *buffer);
extern void drbd_md_sync(struct drbd_device *device);
extern int  drbd_md_read(struct drbd_device *device, struct drbd_backing_dev *bdev);
extern void drbd_uuid_set(struct drbd_device *device, int idx, u64 val) __must_hold(local);
extern void _drbd_uuid_set(struct drbd_device *device, int idx, u64 val) __must_hold(local);
extern void drbd_uuid_new_current(struct drbd_device *device) __must_hold(local);
extern void drbd_uuid_set_bm(struct drbd_device *device, u64 val) __must_hold(local);
extern void drbd_uuid_move_history(struct drbd_device *device) __must_hold(local);
extern void __drbd_uuid_set(struct drbd_device *device, int idx, u64 val) __must_hold(local);
extern void drbd_md_set_flag(struct drbd_device *device, int flags) __must_hold(local);
extern void drbd_md_clear_flag(struct drbd_device *device, int flags)__must_hold(local);
extern int drbd_md_test_flag(struct drbd_backing_dev *, int);
extern void drbd_md_mark_dirty(struct drbd_device *device);
extern void drbd_queue_bitmap_io(struct drbd_device *device,
				 int (*io_fn)(struct drbd_device *),
				 void (*done)(struct drbd_device *, int),
				 char *why, enum bm_flag flags);
extern int drbd_bitmap_io(struct drbd_device *device,
		int (*io_fn)(struct drbd_device *),
		char *why, enum bm_flag flags);
extern int drbd_bitmap_io_from_worker(struct drbd_device *device,
		int (*io_fn)(struct drbd_device *),
		char *why, enum bm_flag flags);
extern int drbd_bmio_set_n_write(struct drbd_device *device) __must_hold(local);
extern int drbd_bmio_clear_n_write(struct drbd_device *device) __must_hold(local);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Meta data layout
 *
 * We currently have two possible layouts.
 * Offsets in (512 byte) sectors.
 * external:
 *   |----------- md_size_sect ------------------|
 *   [ 4k superblock ][ activity log ][  Bitmap  ]
 *   | al_offset == 8 |
 *   | bm_offset = al_offset + X      |
 *  ==> bitmap sectors = md_size_sect - bm_offset
 *
 *  Variants:
 *     old, indexed fixed size meta data:
 *
 * internal:
 *            |----------- md_size_sect ------------------|
 * [data.....][  Bitmap  ][ activity log ][ 4k superblock ][padding*]
 *                        | al_offset < 0 |
 *            | bm_offset = al_offset - Y |
 *  ==> bitmap sectors = Y = al_offset - bm_offset
 *
 *  [padding*] are zero or up to 7 unused 512 Byte sectors to the
 *  end of the device, so that the [4k superblock] will be 4k aligned.
 *
 *  The activity log consists of 4k transaction blocks,
 *  which are written in a ring-buffer, or striped ring-buffer like fashion,
 *  which are writtensize used to be fixed 32kB,
 *  but is about to become configurable.
 */

/* Our old fixed size meta data layout
 * allows up to about 3.8TB, so if you want more,
 * you need to use the "flexible" meta data format. */
#define MD_128MB_SECT (128LLU << 11)  /* 128 MB, unit sectors */
#define MD_4kB_SECT	 8
#define MD_32kB_SECT	64

/* One activity log extent represents 4M of storage */
#define AL_EXTENT_SHIFT 22
#define AL_EXTENT_SIZE (1<<AL_EXTENT_SHIFT)

/* We could make these currently hardcoded constants configurable
 * variables at create-md time (or even re-configurable at runtime?).
 * Which will require some more changes to the DRBD "super block"
 * and attach code.
 *
 * updates per transaction:
 *   This many changes to the active set can be logged with one transaction.
 *   This number is arbitrary.
 * context per transaction:
 *   This many context extent numbers are logged with each transaction.
 *   This number is resulting from the transaction block size (4k), the layout
 *   of the transaction header, and the number of updates per transaction.
 *   See drbd_actlog.c:struct al_transaction_on_disk
 * */
#define AL_UPDATES_PER_TRANSACTION	 64	// arbitrary
#define AL_CONTEXT_PER_TRANSACTION	919	// (4096 - 36 - 6*64)/4

#if BITS_PER_LONG == 32
#define LN2_BPL 5
#define cpu_to_lel(A) cpu_to_le32(A)
#define lel_to_cpu(A) le32_to_cpu(A)
#elif BITS_PER_LONG == 64
#define LN2_BPL 6
#define cpu_to_lel(A) cpu_to_le64(A)
#define lel_to_cpu(A) le64_to_cpu(A)
#else
#error "LN2 of BITS_PER_LONG unknown!"
#endif

/* resync bitmap */
/* 16MB sized 'bitmap extent' to track syncer usage */
struct bm_extent {
	int rs_left; /* number of bits set (out of sync) in this extent. */
	int rs_failed; /* number of failed resync requests in this extent. */
	unsigned long flags;
	struct lc_element lce;
};

#define BME_NO_WRITES  0  /* bm_extent.flags: no more requests on this one! */
#define BME_LOCKED     1  /* bm_extent.flags: syncer active on this one. */
#define BME_PRIORITY   2  /* finish resync IO on this extent ASAP! App IO waiting! */

/* drbd_bitmap.c */
/*
 * We need to store one bit for a block.
 * Example: 1GB disk @ 4096 byte blocks ==> we need 32 KB bitmap.
 * Bit 0 ==> local node thinks this block is binary identical on both nodes
 * Bit 1 ==> local node thinks this block needs to be synced.
 */

#define SLEEP_TIME (HZ/10)

/* We do bitmap IO in units of 4k blocks.
 * We also still have a hardcoded 4k per bit relation. */
#define BM_BLOCK_SHIFT	12			 /* 4k per bit */
#define BM_BLOCK_SIZE	 (1<<BM_BLOCK_SHIFT)
/* mostly arbitrarily set the represented size of one bitmap extent,
 * aka resync extent, to 16 MiB (which is also 512 Byte worth of bitmap
 * at 4k per bit resolution) */
#define BM_EXT_SHIFT	 24	/* 16 MiB per resync extent */
#define BM_EXT_SIZE	 (1<<BM_EXT_SHIFT)

#if (BM_EXT_SHIFT != 24) || (BM_BLOCK_SHIFT != 12)
#error "HAVE YOU FIXED drbdmeta AS WELL??"
#endif

/* thus many _storage_ sectors are described by one bit */
#define BM_SECT_TO_BIT(x)   ((x)>>(BM_BLOCK_SHIFT-9))
#define BM_BIT_TO_SECT(x)   ((sector_t)(x)<<(BM_BLOCK_SHIFT-9))
#define BM_SECT_PER_BIT     BM_BIT_TO_SECT(1)

/* bit to represented kilo byte conversion */
#define Bit2KB(bits) ((bits)<<(BM_BLOCK_SHIFT-10))

/* in which _bitmap_ extent (resp. sector) the bit for a certain
 * _storage_ sector is located in */
#define BM_SECT_TO_EXT(x)   ((x)>>(BM_EXT_SHIFT-9))
<<<<<<< HEAD

/* how much _storage_ sectors we have per bitmap sector */
#define BM_EXT_TO_SECT(x)   ((sector_t)(x) << (BM_EXT_SHIFT-9))
#define BM_SECT_PER_EXT     BM_EXT_TO_SECT(1)
=======
#define BM_BIT_TO_EXT(x)    ((x) >> (BM_EXT_SHIFT - BM_BLOCK_SHIFT))

/* first storage sector a bitmap extent corresponds to */
#define BM_EXT_TO_SECT(x)   ((sector_t)(x) << (BM_EXT_SHIFT-9))
/* how much _storage_ sectors we have per bitmap extent */
#define BM_SECT_PER_EXT     BM_EXT_TO_SECT(1)
/* how many bits are covered by one bitmap extent (resync extent) */
#define BM_BITS_PER_EXT     (1UL << (BM_EXT_SHIFT - BM_BLOCK_SHIFT))

#define BM_BLOCKS_PER_BM_EXT_MASK  (BM_BITS_PER_EXT - 1)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* in one sector of the bitmap, we have this many activity_log extents. */
#define AL_EXT_PER_BM_SECT  (1 << (BM_EXT_SHIFT - AL_EXTENT_SHIFT))

<<<<<<< HEAD
#define BM_BLOCKS_PER_BM_EXT_B (BM_EXT_SHIFT - BM_BLOCK_SHIFT)
#define BM_BLOCKS_PER_BM_EXT_MASK  ((1<<BM_BLOCKS_PER_BM_EXT_B) - 1)

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* the extent in "PER_EXTENT" below is an activity log extent
 * we need that many (long words/bytes) to store the bitmap
 *		     of one AL_EXTENT_SIZE chunk of storage.
 * we can store the bitmap for that many AL_EXTENTS within
 * one sector of the _on_disk_ bitmap:
 * bit	 0	  bit 37   bit 38	     bit (512*8)-1
 *	     ...|........|........|.. // ..|........|
 * sect. 0	 `296	  `304			   ^(512*8*8)-1
 *
#define BM_WORDS_PER_EXT    ( (AL_EXT_SIZE/BM_BLOCK_SIZE) / BITS_PER_LONG )
#define BM_BYTES_PER_EXT    ( (AL_EXT_SIZE/BM_BLOCK_SIZE) / 8 )  // 128
#define BM_EXT_PER_SECT	    ( 512 / BM_BYTES_PER_EXTENT )	 //   4
 */

#define DRBD_MAX_SECTORS_32 (0xffffffffLU)
/* we have a certain meta data variant that has a fixed on-disk size of 128
 * MiB, of which 4k are our "superblock", and 32k are the fixed size activity
 * log, leaving this many sectors for the bitmap.
 */

#define DRBD_MAX_SECTORS_FIXED_BM \
	  ((MD_128MB_SECT - MD_32kB_SECT - MD_4kB_SECT) * (1LL<<(BM_EXT_SHIFT-9)))
#if !defined(CONFIG_LBDAF) && BITS_PER_LONG == 32
#define DRBD_MAX_SECTORS      DRBD_MAX_SECTORS_32
#define DRBD_MAX_SECTORS_FLEX DRBD_MAX_SECTORS_32
#else
#define DRBD_MAX_SECTORS      DRBD_MAX_SECTORS_FIXED_BM
/* 16 TB in units of sectors */
#if BITS_PER_LONG == 32
/* adjust by one page worth of bitmap,
 * so we won't wrap around in drbd_bm_find_next_bit.
 * you should use 64bit OS for that much storage, anyways. */
#define DRBD_MAX_SECTORS_FLEX BM_BIT_TO_SECT(0xffff7fff)
#else
/* we allow up to 1 PiB now on 64bit architecture with "flexible" meta data */
#define DRBD_MAX_SECTORS_FLEX (1UL << 51)
/* corresponds to (1UL << 38) bits right now. */
#endif
#endif

<<<<<<< HEAD
/* BIO_MAX_SIZE is 256 * PAGE_CACHE_SIZE,
 * so for typical PAGE_CACHE_SIZE of 4k, that is (1<<20) Byte.
=======
/* Estimate max bio size as 256 * PAGE_SIZE,
 * so for typical PAGE_SIZE of 4k, that is (1<<20) Byte.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Since we may live in a mixed-platform cluster,
 * we limit us to a platform agnostic constant here for now.
 * A followup commit may allow even bigger BIO sizes,
 * once we thought that through. */
#define DRBD_MAX_BIO_SIZE (1U << 20)
<<<<<<< HEAD
#if DRBD_MAX_BIO_SIZE > BIO_MAX_SIZE
=======
#if DRBD_MAX_BIO_SIZE > (BIO_MAX_PAGES << PAGE_SHIFT)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#error Architecture not supported: DRBD_MAX_BIO_SIZE > BIO_MAX_SIZE
#endif
#define DRBD_MAX_BIO_SIZE_SAFE (1U << 12)       /* Works always = 4k */

#define DRBD_MAX_SIZE_H80_PACKET (1U << 15) /* Header 80 only allows packets up to 32KiB data */
#define DRBD_MAX_BIO_SIZE_P95    (1U << 17) /* Protocol 95 to 99 allows bios up to 128KiB */

<<<<<<< HEAD
extern int  drbd_bm_init(struct drbd_conf *mdev);
extern int  drbd_bm_resize(struct drbd_conf *mdev, sector_t sectors, int set_new_bits);
extern void drbd_bm_cleanup(struct drbd_conf *mdev);
extern void drbd_bm_set_all(struct drbd_conf *mdev);
extern void drbd_bm_clear_all(struct drbd_conf *mdev);
/* set/clear/test only a few bits at a time */
extern int  drbd_bm_set_bits(
		struct drbd_conf *mdev, unsigned long s, unsigned long e);
extern int  drbd_bm_clear_bits(
		struct drbd_conf *mdev, unsigned long s, unsigned long e);
extern int drbd_bm_count_bits(
	struct drbd_conf *mdev, const unsigned long s, const unsigned long e);
/* bm_set_bits variant for use while holding drbd_bm_lock,
 * may process the whole bitmap in one go */
extern void _drbd_bm_set_bits(struct drbd_conf *mdev,
		const unsigned long s, const unsigned long e);
extern int  drbd_bm_test_bit(struct drbd_conf *mdev, unsigned long bitnr);
extern int  drbd_bm_e_weight(struct drbd_conf *mdev, unsigned long enr);
extern int  drbd_bm_write_page(struct drbd_conf *mdev, unsigned int idx) __must_hold(local);
extern int  drbd_bm_read(struct drbd_conf *mdev) __must_hold(local);
extern void drbd_bm_mark_for_writeout(struct drbd_conf *mdev, int page_nr);
extern int  drbd_bm_write(struct drbd_conf *mdev) __must_hold(local);
extern int  drbd_bm_write_hinted(struct drbd_conf *mdev) __must_hold(local);
extern int drbd_bm_write_all(struct drbd_conf *mdev) __must_hold(local);
extern int  drbd_bm_write_copy_pages(struct drbd_conf *mdev) __must_hold(local);
extern size_t	     drbd_bm_words(struct drbd_conf *mdev);
extern unsigned long drbd_bm_bits(struct drbd_conf *mdev);
extern sector_t      drbd_bm_capacity(struct drbd_conf *mdev);

#define DRBD_END_OF_BITMAP	(~(unsigned long)0)
extern unsigned long drbd_bm_find_next(struct drbd_conf *mdev, unsigned long bm_fo);
/* bm_find_next variants for use while you hold drbd_bm_lock() */
extern unsigned long _drbd_bm_find_next(struct drbd_conf *mdev, unsigned long bm_fo);
extern unsigned long _drbd_bm_find_next_zero(struct drbd_conf *mdev, unsigned long bm_fo);
extern unsigned long _drbd_bm_total_weight(struct drbd_conf *mdev);
extern unsigned long drbd_bm_total_weight(struct drbd_conf *mdev);
extern int drbd_bm_rs_done(struct drbd_conf *mdev);
/* for receive_bitmap */
extern void drbd_bm_merge_lel(struct drbd_conf *mdev, size_t offset,
		size_t number, unsigned long *buffer);
/* for _drbd_send_bitmap */
extern void drbd_bm_get_lel(struct drbd_conf *mdev, size_t offset,
		size_t number, unsigned long *buffer);

extern void drbd_bm_lock(struct drbd_conf *mdev, char *why, enum bm_flag flags);
extern void drbd_bm_unlock(struct drbd_conf *mdev);
=======
/* For now, don't allow more than half of what we can "activate" in one
 * activity log transaction to be discarded in one go. We may need to rework
 * drbd_al_begin_io() to allow for even larger discard ranges */
#define DRBD_MAX_BATCH_BIO_SIZE	 (AL_UPDATES_PER_TRANSACTION/2*AL_EXTENT_SIZE)
#define DRBD_MAX_BBIO_SECTORS    (DRBD_MAX_BATCH_BIO_SIZE >> 9)

extern int  drbd_bm_init(struct drbd_device *device);
extern int  drbd_bm_resize(struct drbd_device *device, sector_t sectors, int set_new_bits);
extern void drbd_bm_cleanup(struct drbd_device *device);
extern void drbd_bm_set_all(struct drbd_device *device);
extern void drbd_bm_clear_all(struct drbd_device *device);
/* set/clear/test only a few bits at a time */
extern int  drbd_bm_set_bits(
		struct drbd_device *device, unsigned long s, unsigned long e);
extern int  drbd_bm_clear_bits(
		struct drbd_device *device, unsigned long s, unsigned long e);
extern int drbd_bm_count_bits(
	struct drbd_device *device, const unsigned long s, const unsigned long e);
/* bm_set_bits variant for use while holding drbd_bm_lock,
 * may process the whole bitmap in one go */
extern void _drbd_bm_set_bits(struct drbd_device *device,
		const unsigned long s, const unsigned long e);
extern int  drbd_bm_test_bit(struct drbd_device *device, unsigned long bitnr);
extern int  drbd_bm_e_weight(struct drbd_device *device, unsigned long enr);
extern int  drbd_bm_read(struct drbd_device *device) __must_hold(local);
extern void drbd_bm_mark_for_writeout(struct drbd_device *device, int page_nr);
extern int  drbd_bm_write(struct drbd_device *device) __must_hold(local);
extern void drbd_bm_reset_al_hints(struct drbd_device *device) __must_hold(local);
extern int  drbd_bm_write_hinted(struct drbd_device *device) __must_hold(local);
extern int  drbd_bm_write_lazy(struct drbd_device *device, unsigned upper_idx) __must_hold(local);
extern int drbd_bm_write_all(struct drbd_device *device) __must_hold(local);
extern int  drbd_bm_write_copy_pages(struct drbd_device *device) __must_hold(local);
extern size_t	     drbd_bm_words(struct drbd_device *device);
extern unsigned long drbd_bm_bits(struct drbd_device *device);
extern sector_t      drbd_bm_capacity(struct drbd_device *device);

#define DRBD_END_OF_BITMAP	(~(unsigned long)0)
extern unsigned long drbd_bm_find_next(struct drbd_device *device, unsigned long bm_fo);
/* bm_find_next variants for use while you hold drbd_bm_lock() */
extern unsigned long _drbd_bm_find_next(struct drbd_device *device, unsigned long bm_fo);
extern unsigned long _drbd_bm_find_next_zero(struct drbd_device *device, unsigned long bm_fo);
extern unsigned long _drbd_bm_total_weight(struct drbd_device *device);
extern unsigned long drbd_bm_total_weight(struct drbd_device *device);
/* for receive_bitmap */
extern void drbd_bm_merge_lel(struct drbd_device *device, size_t offset,
		size_t number, unsigned long *buffer);
/* for _drbd_send_bitmap */
extern void drbd_bm_get_lel(struct drbd_device *device, size_t offset,
		size_t number, unsigned long *buffer);

extern void drbd_bm_lock(struct drbd_device *device, char *why, enum bm_flag flags);
extern void drbd_bm_unlock(struct drbd_device *device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* drbd_main.c */

extern struct kmem_cache *drbd_request_cache;
extern struct kmem_cache *drbd_ee_cache;	/* peer requests */
extern struct kmem_cache *drbd_bm_ext_cache;	/* bitmap extents */
extern struct kmem_cache *drbd_al_ext_cache;	/* activity log extents */
extern mempool_t *drbd_request_mempool;
extern mempool_t *drbd_ee_mempool;

/* drbd's page pool, used to buffer data received from the peer,
 * or data requested by the peer.
 *
 * This does not have an emergency reserve.
 *
 * When allocating from this pool, it first takes pages from the pool.
 * Only if the pool is depleted will try to allocate from the system.
 *
 * The assumption is that pages taken from this pool will be processed,
 * and given back, "quickly", and then can be recycled, so we can avoid
 * frequent calls to alloc_page(), and still will be able to make progress even
 * under memory pressure.
 */
extern struct page *drbd_pp_pool;
extern spinlock_t   drbd_pp_lock;
extern int	    drbd_pp_vacant;
extern wait_queue_head_t drbd_pp_wait;

/* We also need a standard (emergency-reserve backed) page pool
 * for meta data IO (activity log, bitmap).
 * We can keep it global, as long as it is used as "N pages at a time".
 * 128 should be plenty, currently we probably can get away with as few as 1.
 */
#define DRBD_MIN_POOL_PAGES	128
extern mempool_t *drbd_md_io_page_pool;

/* We also need to make sure we get a bio
 * when we need it for housekeeping purposes */
extern struct bio_set *drbd_md_io_bio_set;
/* to allocate from that set */
extern struct bio *bio_alloc_drbd(gfp_t gfp_mask);

<<<<<<< HEAD
extern rwlock_t global_state_lock;

extern int conn_lowest_minor(struct drbd_tconn *tconn);
enum drbd_ret_code conn_new_minor(struct drbd_tconn *tconn, unsigned int minor, int vnr);
extern void drbd_minor_destroy(struct kref *kref);

extern int set_resource_options(struct drbd_tconn *tconn, struct res_opts *res_opts);
extern struct drbd_tconn *conn_create(const char *name, struct res_opts *res_opts);
extern void conn_destroy(struct kref *kref);
struct drbd_tconn *conn_get_by_name(const char *name);
extern struct drbd_tconn *conn_get_by_addrs(void *my_addr, int my_addr_len,
					    void *peer_addr, int peer_addr_len);
extern void conn_free_crypto(struct drbd_tconn *tconn);
=======
extern struct mutex resources_mutex;

extern int conn_lowest_minor(struct drbd_connection *connection);
extern enum drbd_ret_code drbd_create_device(struct drbd_config_context *adm_ctx, unsigned int minor);
extern void drbd_destroy_device(struct kref *kref);
extern void drbd_delete_device(struct drbd_device *device);

extern struct drbd_resource *drbd_create_resource(const char *name);
extern void drbd_free_resource(struct drbd_resource *resource);

extern int set_resource_options(struct drbd_resource *resource, struct res_opts *res_opts);
extern struct drbd_connection *conn_create(const char *name, struct res_opts *res_opts);
extern void drbd_destroy_connection(struct kref *kref);
extern struct drbd_connection *conn_get_by_addrs(void *my_addr, int my_addr_len,
					    void *peer_addr, int peer_addr_len);
extern struct drbd_resource *drbd_find_resource(const char *name);
extern void drbd_destroy_resource(struct kref *kref);
extern void conn_free_crypto(struct drbd_connection *connection);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern int proc_details;

/* drbd_req */
extern void do_submit(struct work_struct *ws);
<<<<<<< HEAD
extern void __drbd_make_request(struct drbd_conf *, struct bio *, unsigned long);
extern void drbd_make_request(struct request_queue *q, struct bio *bio);
extern int drbd_read_remote(struct drbd_conf *mdev, struct drbd_request *req);
extern int drbd_merge_bvec(struct request_queue *q, struct bvec_merge_data *bvm, struct bio_vec *bvec);
=======
extern void __drbd_make_request(struct drbd_device *, struct bio *, unsigned long);
extern blk_qc_t drbd_make_request(struct request_queue *q, struct bio *bio);
extern int drbd_read_remote(struct drbd_device *device, struct drbd_request *req);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int is_valid_ar_handle(struct drbd_request *, sector_t);


/* drbd_nl.c */
<<<<<<< HEAD
extern int drbd_msg_put_info(const char *info);
extern void drbd_suspend_io(struct drbd_conf *mdev);
extern void drbd_resume_io(struct drbd_conf *mdev);
extern char *ppsize(char *buf, unsigned long long size);
extern sector_t drbd_new_dev_size(struct drbd_conf *, struct drbd_backing_dev *, sector_t, int);
enum determine_dev_size { dev_size_error = -1, unchanged = 0, shrunk = 1, grew = 2 };
extern enum determine_dev_size drbd_determine_dev_size(struct drbd_conf *, enum dds_flags) __must_hold(local);
extern void resync_after_online_grow(struct drbd_conf *);
extern void drbd_reconsider_max_bio_size(struct drbd_conf *mdev);
extern enum drbd_state_rv drbd_set_role(struct drbd_conf *mdev,
					enum drbd_role new_role,
					int force);
extern bool conn_try_outdate_peer(struct drbd_tconn *tconn);
extern void conn_try_outdate_peer_async(struct drbd_tconn *tconn);
extern int drbd_khelper(struct drbd_conf *mdev, char *cmd);

/* drbd_worker.c */
extern int drbd_worker(struct drbd_thread *thi);
enum drbd_ret_code drbd_resync_after_valid(struct drbd_conf *mdev, int o_minor);
void drbd_resync_after_changed(struct drbd_conf *mdev);
extern void drbd_start_resync(struct drbd_conf *mdev, enum drbd_conns side);
extern void resume_next_sg(struct drbd_conf *mdev);
extern void suspend_other_sg(struct drbd_conf *mdev);
extern int drbd_resync_finished(struct drbd_conf *mdev);
/* maybe rather drbd_main.c ? */
extern void *drbd_md_get_buffer(struct drbd_conf *mdev);
extern void drbd_md_put_buffer(struct drbd_conf *mdev);
extern int drbd_md_sync_page_io(struct drbd_conf *mdev,
		struct drbd_backing_dev *bdev, sector_t sector, int rw);
extern void drbd_ov_out_of_sync_found(struct drbd_conf *, sector_t, int);
extern void wait_until_done_or_force_detached(struct drbd_conf *mdev,
		struct drbd_backing_dev *bdev, unsigned int *done);
extern void drbd_rs_controller_reset(struct drbd_conf *mdev);

static inline void ov_out_of_sync_print(struct drbd_conf *mdev)
{
	if (mdev->ov_last_oos_size) {
		dev_err(DEV, "Out of sync: start=%llu, size=%lu (sectors)\n",
		     (unsigned long long)mdev->ov_last_oos_start,
		     (unsigned long)mdev->ov_last_oos_size);
	}
	mdev->ov_last_oos_size=0;
}


extern void drbd_csum_bio(struct drbd_conf *, struct crypto_hash *, struct bio *, void *);
extern void drbd_csum_ee(struct drbd_conf *, struct crypto_hash *,
			 struct drbd_peer_request *, void *);
=======

extern struct mutex notification_mutex;

extern void drbd_suspend_io(struct drbd_device *device);
extern void drbd_resume_io(struct drbd_device *device);
extern char *ppsize(char *buf, unsigned long long size);
extern sector_t drbd_new_dev_size(struct drbd_device *, struct drbd_backing_dev *, sector_t, int);
enum determine_dev_size {
	DS_ERROR_SHRINK = -3,
	DS_ERROR_SPACE_MD = -2,
	DS_ERROR = -1,
	DS_UNCHANGED = 0,
	DS_SHRUNK = 1,
	DS_GREW = 2,
	DS_GREW_FROM_ZERO = 3,
};
extern enum determine_dev_size
drbd_determine_dev_size(struct drbd_device *, enum dds_flags, struct resize_parms *) __must_hold(local);
extern void resync_after_online_grow(struct drbd_device *);
extern void drbd_reconsider_queue_parameters(struct drbd_device *device,
			struct drbd_backing_dev *bdev, struct o_qlim *o);
extern enum drbd_state_rv drbd_set_role(struct drbd_device *device,
					enum drbd_role new_role,
					int force);
extern bool conn_try_outdate_peer(struct drbd_connection *connection);
extern void conn_try_outdate_peer_async(struct drbd_connection *connection);
extern enum drbd_peer_state conn_khelper(struct drbd_connection *connection, char *cmd);
extern int drbd_khelper(struct drbd_device *device, char *cmd);

/* drbd_worker.c */
/* bi_end_io handlers */
extern void drbd_md_endio(struct bio *bio);
extern void drbd_peer_request_endio(struct bio *bio);
extern void drbd_request_endio(struct bio *bio);
extern int drbd_worker(struct drbd_thread *thi);
enum drbd_ret_code drbd_resync_after_valid(struct drbd_device *device, int o_minor);
void drbd_resync_after_changed(struct drbd_device *device);
extern void drbd_start_resync(struct drbd_device *device, enum drbd_conns side);
extern void resume_next_sg(struct drbd_device *device);
extern void suspend_other_sg(struct drbd_device *device);
extern int drbd_resync_finished(struct drbd_device *device);
/* maybe rather drbd_main.c ? */
extern void *drbd_md_get_buffer(struct drbd_device *device, const char *intent);
extern void drbd_md_put_buffer(struct drbd_device *device);
extern int drbd_md_sync_page_io(struct drbd_device *device,
		struct drbd_backing_dev *bdev, sector_t sector, int op);
extern void drbd_ov_out_of_sync_found(struct drbd_device *, sector_t, int);
extern void wait_until_done_or_force_detached(struct drbd_device *device,
		struct drbd_backing_dev *bdev, unsigned int *done);
extern void drbd_rs_controller_reset(struct drbd_device *device);

static inline void ov_out_of_sync_print(struct drbd_device *device)
{
	if (device->ov_last_oos_size) {
		drbd_err(device, "Out of sync: start=%llu, size=%lu (sectors)\n",
		     (unsigned long long)device->ov_last_oos_start,
		     (unsigned long)device->ov_last_oos_size);
	}
	device->ov_last_oos_size = 0;
}


extern void drbd_csum_bio(struct crypto_ahash *, struct bio *, void *);
extern void drbd_csum_ee(struct crypto_ahash *, struct drbd_peer_request *, void *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* worker callbacks */
extern int w_e_end_data_req(struct drbd_work *, int);
extern int w_e_end_rsdata_req(struct drbd_work *, int);
extern int w_e_end_csum_rs_req(struct drbd_work *, int);
extern int w_e_end_ov_reply(struct drbd_work *, int);
extern int w_e_end_ov_req(struct drbd_work *, int);
extern int w_ov_finished(struct drbd_work *, int);
extern int w_resync_timer(struct drbd_work *, int);
extern int w_send_write_hint(struct drbd_work *, int);
<<<<<<< HEAD
extern int w_make_resync_request(struct drbd_work *, int);
extern int w_send_dblock(struct drbd_work *, int);
extern int w_send_read_req(struct drbd_work *, int);
extern int w_prev_work_done(struct drbd_work *, int);
=======
extern int w_send_dblock(struct drbd_work *, int);
extern int w_send_read_req(struct drbd_work *, int);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int w_e_reissue(struct drbd_work *, int);
extern int w_restart_disk_io(struct drbd_work *, int);
extern int w_send_out_of_sync(struct drbd_work *, int);
extern int w_start_resync(struct drbd_work *, int);

extern void resync_timer_fn(unsigned long data);
extern void start_resync_timer_fn(unsigned long data);

<<<<<<< HEAD
/* drbd_receiver.c */
extern int drbd_rs_should_slow_down(struct drbd_conf *mdev, sector_t sector);
extern int drbd_submit_peer_request(struct drbd_conf *,
				    struct drbd_peer_request *, const unsigned,
				    const int);
extern int drbd_free_peer_reqs(struct drbd_conf *, struct list_head *);
extern struct drbd_peer_request *drbd_alloc_peer_req(struct drbd_conf *, u64,
						     sector_t, unsigned int,
						     gfp_t) __must_hold(local);
extern void __drbd_free_peer_req(struct drbd_conf *, struct drbd_peer_request *,
				 int);
#define drbd_free_peer_req(m,e) __drbd_free_peer_req(m, e, 0)
#define drbd_free_net_peer_req(m,e) __drbd_free_peer_req(m, e, 1)
extern struct page *drbd_alloc_pages(struct drbd_conf *, unsigned int, bool);
extern void drbd_set_recv_tcq(struct drbd_conf *mdev, int tcq_enabled);
extern void _drbd_clear_done_ee(struct drbd_conf *mdev, struct list_head *to_be_freed);
extern void conn_flush_workqueue(struct drbd_tconn *tconn);
extern int drbd_connected(struct drbd_conf *mdev);
static inline void drbd_flush_workqueue(struct drbd_conf *mdev)
{
	conn_flush_workqueue(mdev->tconn);
}

/* Yes, there is kernel_setsockopt, but only since 2.6.18.
 * So we have our own copy of it here. */
static inline int drbd_setsockopt(struct socket *sock, int level, int optname,
				  char *optval, int optlen)
{
	mm_segment_t oldfs = get_fs();
	char __user *uoptval;
	int err;

	uoptval = (char __user __force *)optval;

	set_fs(KERNEL_DS);
	if (level == SOL_SOCKET)
		err = sock_setsockopt(sock, level, optname, uoptval, optlen);
	else
		err = sock->ops->setsockopt(sock, level, optname, uoptval,
					    optlen);
	set_fs(oldfs);
	return err;
}
=======
extern void drbd_endio_write_sec_final(struct drbd_peer_request *peer_req);

/* drbd_receiver.c */
extern int drbd_issue_discard_or_zero_out(struct drbd_device *device,
		sector_t start, unsigned int nr_sectors, bool discard);
extern int drbd_receiver(struct drbd_thread *thi);
extern int drbd_ack_receiver(struct drbd_thread *thi);
extern void drbd_send_ping_wf(struct work_struct *ws);
extern void drbd_send_acks_wf(struct work_struct *ws);
extern bool drbd_rs_c_min_rate_throttle(struct drbd_device *device);
extern bool drbd_rs_should_slow_down(struct drbd_device *device, sector_t sector,
		bool throttle_if_app_is_waiting);
extern int drbd_submit_peer_request(struct drbd_device *,
				    struct drbd_peer_request *, const unsigned,
				    const unsigned, const int);
extern int drbd_free_peer_reqs(struct drbd_device *, struct list_head *);
extern struct drbd_peer_request *drbd_alloc_peer_req(struct drbd_peer_device *, u64,
						     sector_t, unsigned int,
						     unsigned int,
						     gfp_t) __must_hold(local);
extern void __drbd_free_peer_req(struct drbd_device *, struct drbd_peer_request *,
				 int);
#define drbd_free_peer_req(m,e) __drbd_free_peer_req(m, e, 0)
#define drbd_free_net_peer_req(m,e) __drbd_free_peer_req(m, e, 1)
extern struct page *drbd_alloc_pages(struct drbd_peer_device *, unsigned int, bool);
extern void drbd_set_recv_tcq(struct drbd_device *device, int tcq_enabled);
extern void _drbd_clear_done_ee(struct drbd_device *device, struct list_head *to_be_freed);
extern int drbd_connected(struct drbd_peer_device *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline void drbd_tcp_cork(struct socket *sock)
{
	int val = 1;
<<<<<<< HEAD
	(void) drbd_setsockopt(sock, SOL_TCP, TCP_CORK,
=======
	(void) kernel_setsockopt(sock, SOL_TCP, TCP_CORK,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			(char*)&val, sizeof(val));
}

static inline void drbd_tcp_uncork(struct socket *sock)
{
	int val = 0;
<<<<<<< HEAD
	(void) drbd_setsockopt(sock, SOL_TCP, TCP_CORK,
=======
	(void) kernel_setsockopt(sock, SOL_TCP, TCP_CORK,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			(char*)&val, sizeof(val));
}

static inline void drbd_tcp_nodelay(struct socket *sock)
{
	int val = 1;
<<<<<<< HEAD
	(void) drbd_setsockopt(sock, SOL_TCP, TCP_NODELAY,
=======
	(void) kernel_setsockopt(sock, SOL_TCP, TCP_NODELAY,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			(char*)&val, sizeof(val));
}

static inline void drbd_tcp_quickack(struct socket *sock)
{
	int val = 2;
<<<<<<< HEAD
	(void) drbd_setsockopt(sock, SOL_TCP, TCP_QUICKACK,
			(char*)&val, sizeof(val));
}

void drbd_bump_write_ordering(struct drbd_tconn *tconn, enum write_ordering_e wo);
=======
	(void) kernel_setsockopt(sock, SOL_TCP, TCP_QUICKACK,
			(char*)&val, sizeof(val));
}

/* sets the number of 512 byte sectors of our virtual device */
static inline void drbd_set_my_capacity(struct drbd_device *device,
					sector_t size)
{
	/* set_capacity(device->this_bdev->bd_disk, size); */
	set_capacity(device->vdisk, size);
	device->this_bdev->bd_inode->i_size = (loff_t)size << 9;
}

/*
 * used to submit our private bio
 */
static inline void drbd_generic_make_request(struct drbd_device *device,
					     int fault_type, struct bio *bio)
{
	__release(local);
	if (!bio->bi_bdev) {
		drbd_err(device, "drbd_generic_make_request: bio->bi_bdev == NULL\n");
		bio->bi_error = -ENODEV;
		bio_endio(bio);
		return;
	}

	if (drbd_insert_fault(device, fault_type))
		bio_io_error(bio);
	else
		generic_make_request(bio);
}

void drbd_bump_write_ordering(struct drbd_resource *resource, struct drbd_backing_dev *bdev,
			      enum write_ordering_e wo);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* drbd_proc.c */
extern struct proc_dir_entry *drbd_proc;
extern const struct file_operations drbd_proc_fops;
<<<<<<< HEAD
extern const char *drbd_conn_str(enum drbd_conns s);
extern const char *drbd_role_str(enum drbd_role s);

/* drbd_actlog.c */
extern int drbd_al_begin_io_nonblock(struct drbd_conf *mdev, struct drbd_interval *i);
extern void drbd_al_begin_io_commit(struct drbd_conf *mdev, bool delegate);
extern bool drbd_al_begin_io_fastpath(struct drbd_conf *mdev, struct drbd_interval *i);
extern void drbd_al_begin_io(struct drbd_conf *mdev, struct drbd_interval *i, bool delegate);
extern void drbd_al_complete_io(struct drbd_conf *mdev, struct drbd_interval *i);
extern void drbd_rs_complete_io(struct drbd_conf *mdev, sector_t sector);
extern int drbd_rs_begin_io(struct drbd_conf *mdev, sector_t sector);
extern int drbd_try_rs_begin_io(struct drbd_conf *mdev, sector_t sector);
extern void drbd_rs_cancel_all(struct drbd_conf *mdev);
extern int drbd_rs_del_all(struct drbd_conf *mdev);
extern void drbd_rs_failed_io(struct drbd_conf *mdev,
		sector_t sector, int size);
extern void drbd_advance_rs_marks(struct drbd_conf *mdev, unsigned long still_to_go);
extern void __drbd_set_in_sync(struct drbd_conf *mdev, sector_t sector,
		int size, const char *file, const unsigned int line);
#define drbd_set_in_sync(mdev, sector, size) \
	__drbd_set_in_sync(mdev, sector, size, __FILE__, __LINE__)
extern int __drbd_set_out_of_sync(struct drbd_conf *mdev, sector_t sector,
		int size, const char *file, const unsigned int line);
#define drbd_set_out_of_sync(mdev, sector, size) \
	__drbd_set_out_of_sync(mdev, sector, size, __FILE__, __LINE__)
extern void drbd_al_shrink(struct drbd_conf *mdev);
=======

/* drbd_actlog.c */
extern bool drbd_al_begin_io_prepare(struct drbd_device *device, struct drbd_interval *i);
extern int drbd_al_begin_io_nonblock(struct drbd_device *device, struct drbd_interval *i);
extern void drbd_al_begin_io_commit(struct drbd_device *device);
extern bool drbd_al_begin_io_fastpath(struct drbd_device *device, struct drbd_interval *i);
extern void drbd_al_begin_io(struct drbd_device *device, struct drbd_interval *i);
extern void drbd_al_complete_io(struct drbd_device *device, struct drbd_interval *i);
extern void drbd_rs_complete_io(struct drbd_device *device, sector_t sector);
extern int drbd_rs_begin_io(struct drbd_device *device, sector_t sector);
extern int drbd_try_rs_begin_io(struct drbd_device *device, sector_t sector);
extern void drbd_rs_cancel_all(struct drbd_device *device);
extern int drbd_rs_del_all(struct drbd_device *device);
extern void drbd_rs_failed_io(struct drbd_device *device,
		sector_t sector, int size);
extern void drbd_advance_rs_marks(struct drbd_device *device, unsigned long still_to_go);

enum update_sync_bits_mode { RECORD_RS_FAILED, SET_OUT_OF_SYNC, SET_IN_SYNC };
extern int __drbd_change_sync(struct drbd_device *device, sector_t sector, int size,
		enum update_sync_bits_mode mode);
#define drbd_set_in_sync(device, sector, size) \
	__drbd_change_sync(device, sector, size, SET_IN_SYNC)
#define drbd_set_out_of_sync(device, sector, size) \
	__drbd_change_sync(device, sector, size, SET_OUT_OF_SYNC)
#define drbd_rs_failed_io(device, sector, size) \
	__drbd_change_sync(device, sector, size, RECORD_RS_FAILED)
extern void drbd_al_shrink(struct drbd_device *device);
extern int drbd_al_initialize(struct drbd_device *, void *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* drbd_nl.c */
/* state info broadcast */
struct sib_info {
	enum drbd_state_info_bcast_reason sib_reason;
	union {
		struct {
			char *helper_name;
			unsigned helper_exit_code;
		};
		struct {
			union drbd_state os;
			union drbd_state ns;
		};
	};
};
<<<<<<< HEAD
void drbd_bcast_event(struct drbd_conf *mdev, const struct sib_info *sib);
=======
void drbd_bcast_event(struct drbd_device *device, const struct sib_info *sib);

extern void notify_resource_state(struct sk_buff *,
				  unsigned int,
				  struct drbd_resource *,
				  struct resource_info *,
				  enum drbd_notification_type);
extern void notify_device_state(struct sk_buff *,
				unsigned int,
				struct drbd_device *,
				struct device_info *,
				enum drbd_notification_type);
extern void notify_connection_state(struct sk_buff *,
				    unsigned int,
				    struct drbd_connection *,
				    struct connection_info *,
				    enum drbd_notification_type);
extern void notify_peer_device_state(struct sk_buff *,
				     unsigned int,
				     struct drbd_peer_device *,
				     struct peer_device_info *,
				     enum drbd_notification_type);
extern void notify_helper(enum drbd_notification_type, struct drbd_device *,
			  struct drbd_connection *, const char *, int);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * inline helper functions
 *************************/

/* see also page_chain_add and friends in drbd_receiver.c */
static inline struct page *page_chain_next(struct page *page)
{
	return (struct page *)page_private(page);
}
#define page_chain_for_each(page) \
	for (; page && ({ prefetch(page_chain_next(page)); 1; }); \
			page = page_chain_next(page))
#define page_chain_for_each_safe(page, n) \
	for (; page && ({ n = page_chain_next(page); 1; }); page = n)


static inline int drbd_peer_req_has_active_page(struct drbd_peer_request *peer_req)
{
	struct page *page = peer_req->pages;
	page_chain_for_each(page) {
		if (page_count(page) > 1)
			return 1;
	}
	return 0;
}

<<<<<<< HEAD
static inline enum drbd_state_rv
_drbd_set_state(struct drbd_conf *mdev, union drbd_state ns,
		enum chg_state_flags flags, struct completion *done)
{
	enum drbd_state_rv rv;

	read_lock(&global_state_lock);
	rv = __drbd_set_state(mdev, ns, flags, done);
	read_unlock(&global_state_lock);

	return rv;
}

static inline union drbd_state drbd_read_state(struct drbd_conf *mdev)
{
	union drbd_state rv;

	rv.i = mdev->state.i;
	rv.susp = mdev->tconn->susp;
	rv.susp_nod = mdev->tconn->susp_nod;
	rv.susp_fen = mdev->tconn->susp_fen;
=======
static inline union drbd_state drbd_read_state(struct drbd_device *device)
{
	struct drbd_resource *resource = device->resource;
	union drbd_state rv;

	rv.i = device->state.i;
	rv.susp = resource->susp;
	rv.susp_nod = resource->susp_nod;
	rv.susp_fen = resource->susp_fen;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return rv;
}

enum drbd_force_detach_flags {
	DRBD_READ_ERROR,
	DRBD_WRITE_ERROR,
	DRBD_META_IO_ERROR,
	DRBD_FORCE_DETACH,
};

#define __drbd_chk_io_error(m,f) __drbd_chk_io_error_(m,f, __func__)
<<<<<<< HEAD
static inline void __drbd_chk_io_error_(struct drbd_conf *mdev,
=======
static inline void __drbd_chk_io_error_(struct drbd_device *device,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		enum drbd_force_detach_flags df,
		const char *where)
{
	enum drbd_io_error_p ep;

	rcu_read_lock();
<<<<<<< HEAD
	ep = rcu_dereference(mdev->ldev->disk_conf)->on_io_error;
=======
	ep = rcu_dereference(device->ldev->disk_conf)->on_io_error;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_read_unlock();
	switch (ep) {
	case EP_PASS_ON: /* FIXME would this be better named "Ignore"? */
		if (df == DRBD_READ_ERROR || df == DRBD_WRITE_ERROR) {
			if (__ratelimit(&drbd_ratelimit_state))
<<<<<<< HEAD
				dev_err(DEV, "Local IO failed in %s.\n", where);
			if (mdev->state.disk > D_INCONSISTENT)
				_drbd_set_state(_NS(mdev, disk, D_INCONSISTENT), CS_HARD, NULL);
=======
				drbd_err(device, "Local IO failed in %s.\n", where);
			if (device->state.disk > D_INCONSISTENT)
				_drbd_set_state(_NS(device, disk, D_INCONSISTENT), CS_HARD, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		}
		/* NOTE fall through for DRBD_META_IO_ERROR or DRBD_FORCE_DETACH */
	case EP_DETACH:
	case EP_CALL_HELPER:
		/* Remember whether we saw a READ or WRITE error.
		 *
		 * Recovery of the affected area for WRITE failure is covered
		 * by the activity log.
		 * READ errors may fall outside that area though. Certain READ
		 * errors can be "healed" by writing good data to the affected
		 * blocks, which triggers block re-allocation in lower layers.
		 *
		 * If we can not write the bitmap after a READ error,
		 * we may need to trigger a full sync (see w_go_diskless()).
		 *
		 * Force-detach is not really an IO error, but rather a
		 * desperate measure to try to deal with a completely
		 * unresponsive lower level IO stack.
		 * Still it should be treated as a WRITE error.
		 *
		 * Meta IO error is always WRITE error:
		 * we read meta data only once during attach,
		 * which will fail in case of errors.
		 */
<<<<<<< HEAD
		set_bit(WAS_IO_ERROR, &mdev->flags);
		if (df == DRBD_READ_ERROR)
			set_bit(WAS_READ_ERROR, &mdev->flags);
		if (df == DRBD_FORCE_DETACH)
			set_bit(FORCE_DETACH, &mdev->flags);
		if (mdev->state.disk > D_FAILED) {
			_drbd_set_state(_NS(mdev, disk, D_FAILED), CS_HARD, NULL);
			dev_err(DEV,
=======
		set_bit(WAS_IO_ERROR, &device->flags);
		if (df == DRBD_READ_ERROR)
			set_bit(WAS_READ_ERROR, &device->flags);
		if (df == DRBD_FORCE_DETACH)
			set_bit(FORCE_DETACH, &device->flags);
		if (device->state.disk > D_FAILED) {
			_drbd_set_state(_NS(device, disk, D_FAILED), CS_HARD, NULL);
			drbd_err(device,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				"Local IO failed in %s. Detaching...\n", where);
		}
		break;
	}
}

/**
 * drbd_chk_io_error: Handle the on_io_error setting, should be called from all io completion handlers
<<<<<<< HEAD
 * @mdev:	 DRBD device.
=======
 * @device:	 DRBD device.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @error:	 Error code passed to the IO completion callback
 * @forcedetach: Force detach. I.e. the error happened while accessing the meta data
 *
 * See also drbd_main.c:after_state_ch() if (os.disk > D_FAILED && ns.disk == D_FAILED)
 */
#define drbd_chk_io_error(m,e,f) drbd_chk_io_error_(m,e,f, __func__)
<<<<<<< HEAD
static inline void drbd_chk_io_error_(struct drbd_conf *mdev,
=======
static inline void drbd_chk_io_error_(struct drbd_device *device,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int error, enum drbd_force_detach_flags forcedetach, const char *where)
{
	if (error) {
		unsigned long flags;
<<<<<<< HEAD
		spin_lock_irqsave(&mdev->tconn->req_lock, flags);
		__drbd_chk_io_error_(mdev, forcedetach, where);
		spin_unlock_irqrestore(&mdev->tconn->req_lock, flags);
=======
		spin_lock_irqsave(&device->resource->req_lock, flags);
		__drbd_chk_io_error_(device, forcedetach, where);
		spin_unlock_irqrestore(&device->resource->req_lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}


/**
 * drbd_md_first_sector() - Returns the first sector number of the meta data area
 * @bdev:	Meta data block device.
 *
 * BTW, for internal meta data, this happens to be the maximum capacity
 * we could agree upon with our peer node.
 */
static inline sector_t drbd_md_first_sector(struct drbd_backing_dev *bdev)
{
	switch (bdev->md.meta_dev_idx) {
	case DRBD_MD_INDEX_INTERNAL:
	case DRBD_MD_INDEX_FLEX_INT:
		return bdev->md.md_offset + bdev->md.bm_offset;
	case DRBD_MD_INDEX_FLEX_EXT:
	default:
		return bdev->md.md_offset;
	}
}

/**
 * drbd_md_last_sector() - Return the last sector number of the meta data area
 * @bdev:	Meta data block device.
 */
static inline sector_t drbd_md_last_sector(struct drbd_backing_dev *bdev)
{
	switch (bdev->md.meta_dev_idx) {
	case DRBD_MD_INDEX_INTERNAL:
	case DRBD_MD_INDEX_FLEX_INT:
		return bdev->md.md_offset + MD_4kB_SECT -1;
	case DRBD_MD_INDEX_FLEX_EXT:
	default:
		return bdev->md.md_offset + bdev->md.md_size_sect -1;
	}
}

/* Returns the number of 512 byte sectors of the device */
static inline sector_t drbd_get_capacity(struct block_device *bdev)
{
	/* return bdev ? get_capacity(bdev->bd_disk) : 0; */
	return bdev ? i_size_read(bdev->bd_inode) >> 9 : 0;
}

/**
 * drbd_get_max_capacity() - Returns the capacity we announce to out peer
 * @bdev:	Meta data block device.
 *
 * returns the capacity we announce to out peer.  we clip ourselves at the
 * various MAX_SECTORS, because if we don't, current implementation will
 * oops sooner or later
 */
static inline sector_t drbd_get_max_capacity(struct drbd_backing_dev *bdev)
{
	sector_t s;

	switch (bdev->md.meta_dev_idx) {
	case DRBD_MD_INDEX_INTERNAL:
	case DRBD_MD_INDEX_FLEX_INT:
		s = drbd_get_capacity(bdev->backing_bdev)
			? min_t(sector_t, DRBD_MAX_SECTORS_FLEX,
				drbd_md_first_sector(bdev))
			: 0;
		break;
	case DRBD_MD_INDEX_FLEX_EXT:
		s = min_t(sector_t, DRBD_MAX_SECTORS_FLEX,
				drbd_get_capacity(bdev->backing_bdev));
		/* clip at maximum size the meta device can support */
		s = min_t(sector_t, s,
			BM_EXT_TO_SECT(bdev->md.md_size_sect
				     - bdev->md.bm_offset));
		break;
	default:
		s = min_t(sector_t, DRBD_MAX_SECTORS,
				drbd_get_capacity(bdev->backing_bdev));
	}
	return s;
}

/**
 * drbd_md_ss() - Return the sector number of our meta data super block
 * @bdev:	Meta data block device.
 */
static inline sector_t drbd_md_ss(struct drbd_backing_dev *bdev)
{
	const int meta_dev_idx = bdev->md.meta_dev_idx;

	if (meta_dev_idx == DRBD_MD_INDEX_FLEX_EXT)
		return 0;

	/* Since drbd08, internal meta data is always "flexible".
	 * position: last 4k aligned block of 4k size */
	if (meta_dev_idx == DRBD_MD_INDEX_INTERNAL ||
	    meta_dev_idx == DRBD_MD_INDEX_FLEX_INT)
		return (drbd_get_capacity(bdev->backing_bdev) & ~7ULL) - 8;

	/* external, some index; this is the old fixed size layout */
	return MD_128MB_SECT * bdev->md.meta_dev_idx;
}

static inline void
<<<<<<< HEAD
drbd_queue_work_front(struct drbd_work_queue *q, struct drbd_work *w)
{
	unsigned long flags;
	spin_lock_irqsave(&q->q_lock, flags);
	list_add(&w->list, &q->q);
=======
drbd_queue_work(struct drbd_work_queue *q, struct drbd_work *w)
{
	unsigned long flags;
	spin_lock_irqsave(&q->q_lock, flags);
	list_add_tail(&w->list, &q->q);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_unlock_irqrestore(&q->q_lock, flags);
	wake_up(&q->q_wait);
}

static inline void
<<<<<<< HEAD
drbd_queue_work(struct drbd_work_queue *q, struct drbd_work *w)
{
	unsigned long flags;
	spin_lock_irqsave(&q->q_lock, flags);
	list_add_tail(&w->list, &q->q);
=======
drbd_queue_work_if_unqueued(struct drbd_work_queue *q, struct drbd_work *w)
{
	unsigned long flags;
	spin_lock_irqsave(&q->q_lock, flags);
	if (list_empty_careful(&w->list))
		list_add_tail(&w->list, &q->q);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_unlock_irqrestore(&q->q_lock, flags);
	wake_up(&q->q_wait);
}

<<<<<<< HEAD
static inline void wake_asender(struct drbd_tconn *tconn)
{
	if (test_bit(SIGNAL_ASENDER, &tconn->flags))
		force_sig(DRBD_SIG, tconn->asender.task);
}

static inline void request_ping(struct drbd_tconn *tconn)
{
	set_bit(SEND_PING, &tconn->flags);
	wake_asender(tconn);
}

extern void *conn_prepare_command(struct drbd_tconn *, struct drbd_socket *);
extern void *drbd_prepare_command(struct drbd_conf *, struct drbd_socket *);
extern int conn_send_command(struct drbd_tconn *, struct drbd_socket *,
			     enum drbd_packet, unsigned int, void *,
			     unsigned int);
extern int drbd_send_command(struct drbd_conf *, struct drbd_socket *,
			     enum drbd_packet, unsigned int, void *,
			     unsigned int);

extern int drbd_send_ping(struct drbd_tconn *tconn);
extern int drbd_send_ping_ack(struct drbd_tconn *tconn);
extern int drbd_send_state_req(struct drbd_conf *, union drbd_state, union drbd_state);
extern int conn_send_state_req(struct drbd_tconn *, union drbd_state, union drbd_state);
=======
static inline void
drbd_device_post_work(struct drbd_device *device, int work_bit)
{
	if (!test_and_set_bit(work_bit, &device->flags)) {
		struct drbd_connection *connection =
			first_peer_device(device)->connection;
		struct drbd_work_queue *q = &connection->sender_work;
		if (!test_and_set_bit(DEVICE_WORK_PENDING, &connection->flags))
			wake_up(&q->q_wait);
	}
}

extern void drbd_flush_workqueue(struct drbd_work_queue *work_queue);

/* To get the ack_receiver out of the blocking network stack,
 * so it can change its sk_rcvtimeo from idle- to ping-timeout,
 * and send a ping, we need to send a signal.
 * Which signal we send is irrelevant. */
static inline void wake_ack_receiver(struct drbd_connection *connection)
{
	struct task_struct *task = connection->ack_receiver.task;
	if (task && get_t_state(&connection->ack_receiver) == RUNNING)
		force_sig(SIGXCPU, task);
}

static inline void request_ping(struct drbd_connection *connection)
{
	set_bit(SEND_PING, &connection->flags);
	wake_ack_receiver(connection);
}

extern void *conn_prepare_command(struct drbd_connection *, struct drbd_socket *);
extern void *drbd_prepare_command(struct drbd_peer_device *, struct drbd_socket *);
extern int conn_send_command(struct drbd_connection *, struct drbd_socket *,
			     enum drbd_packet, unsigned int, void *,
			     unsigned int);
extern int drbd_send_command(struct drbd_peer_device *, struct drbd_socket *,
			     enum drbd_packet, unsigned int, void *,
			     unsigned int);

extern int drbd_send_ping(struct drbd_connection *connection);
extern int drbd_send_ping_ack(struct drbd_connection *connection);
extern int drbd_send_state_req(struct drbd_peer_device *, union drbd_state, union drbd_state);
extern int conn_send_state_req(struct drbd_connection *, union drbd_state, union drbd_state);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline void drbd_thread_stop(struct drbd_thread *thi)
{
	_drbd_thread_stop(thi, false, true);
}

static inline void drbd_thread_stop_nowait(struct drbd_thread *thi)
{
	_drbd_thread_stop(thi, false, false);
}

static inline void drbd_thread_restart_nowait(struct drbd_thread *thi)
{
	_drbd_thread_stop(thi, true, false);
}

/* counts how many answer packets packets we expect from our peer,
 * for either explicit application requests,
 * or implicit barrier packets as necessary.
 * increased:
 *  w_send_barrier
 *  _req_mod(req, QUEUE_FOR_NET_WRITE or QUEUE_FOR_NET_READ);
 *    it is much easier and equally valid to count what we queue for the
 *    worker, even before it actually was queued or send.
 *    (drbd_make_request_common; recovery path on read io-error)
 * decreased:
 *  got_BarrierAck (respective tl_clear, tl_clear_barrier)
 *  _req_mod(req, DATA_RECEIVED)
 *     [from receive_DataReply]
 *  _req_mod(req, WRITE_ACKED_BY_PEER or RECV_ACKED_BY_PEER or NEG_ACKED)
 *     [from got_BlockAck (P_WRITE_ACK, P_RECV_ACK)]
 *     for some reason it is NOT decreased in got_NegAck,
 *     but in the resulting cleanup code from report_params.
 *     we should try to remember the reason for that...
 *  _req_mod(req, SEND_FAILED or SEND_CANCELED)
 *  _req_mod(req, CONNECTION_LOST_WHILE_PENDING)
 *     [from tl_clear_barrier]
 */
<<<<<<< HEAD
static inline void inc_ap_pending(struct drbd_conf *mdev)
{
	atomic_inc(&mdev->ap_pending_cnt);
}

#define ERR_IF_CNT_IS_NEGATIVE(which, func, line)			\
	if (atomic_read(&mdev->which) < 0)				\
		dev_err(DEV, "in %s:%d: " #which " = %d < 0 !\n",	\
			func, line,					\
			atomic_read(&mdev->which))

#define dec_ap_pending(mdev) _dec_ap_pending(mdev, __FUNCTION__, __LINE__)
static inline void _dec_ap_pending(struct drbd_conf *mdev, const char *func, int line)
{
	if (atomic_dec_and_test(&mdev->ap_pending_cnt))
		wake_up(&mdev->misc_wait);
=======
static inline void inc_ap_pending(struct drbd_device *device)
{
	atomic_inc(&device->ap_pending_cnt);
}

#define ERR_IF_CNT_IS_NEGATIVE(which, func, line)			\
	if (atomic_read(&device->which) < 0)				\
		drbd_err(device, "in %s:%d: " #which " = %d < 0 !\n",	\
			func, line,					\
			atomic_read(&device->which))

#define dec_ap_pending(device) _dec_ap_pending(device, __func__, __LINE__)
static inline void _dec_ap_pending(struct drbd_device *device, const char *func, int line)
{
	if (atomic_dec_and_test(&device->ap_pending_cnt))
		wake_up(&device->misc_wait);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ERR_IF_CNT_IS_NEGATIVE(ap_pending_cnt, func, line);
}

/* counts how many resync-related answers we still expect from the peer
 *		     increase			decrease
 * C_SYNC_TARGET sends P_RS_DATA_REQUEST (and expects P_RS_DATA_REPLY)
 * C_SYNC_SOURCE sends P_RS_DATA_REPLY   (and expects P_WRITE_ACK with ID_SYNCER)
 *					   (or P_NEG_ACK with ID_SYNCER)
 */
<<<<<<< HEAD
static inline void inc_rs_pending(struct drbd_conf *mdev)
{
	atomic_inc(&mdev->rs_pending_cnt);
}

#define dec_rs_pending(mdev) _dec_rs_pending(mdev, __FUNCTION__, __LINE__)
static inline void _dec_rs_pending(struct drbd_conf *mdev, const char *func, int line)
{
	atomic_dec(&mdev->rs_pending_cnt);
=======
static inline void inc_rs_pending(struct drbd_device *device)
{
	atomic_inc(&device->rs_pending_cnt);
}

#define dec_rs_pending(device) _dec_rs_pending(device, __func__, __LINE__)
static inline void _dec_rs_pending(struct drbd_device *device, const char *func, int line)
{
	atomic_dec(&device->rs_pending_cnt);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ERR_IF_CNT_IS_NEGATIVE(rs_pending_cnt, func, line);
}

/* counts how many answers we still need to send to the peer.
 * increased on
 *  receive_Data	unless protocol A;
 *			we need to send a P_RECV_ACK (proto B)
 *			or P_WRITE_ACK (proto C)
 *  receive_RSDataReply (recv_resync_read) we need to send a P_WRITE_ACK
 *  receive_DataRequest (receive_RSDataRequest) we need to send back P_DATA
 *  receive_Barrier_*	we need to send a P_BARRIER_ACK
 */
<<<<<<< HEAD
static inline void inc_unacked(struct drbd_conf *mdev)
{
	atomic_inc(&mdev->unacked_cnt);
}

#define dec_unacked(mdev) _dec_unacked(mdev, __FUNCTION__, __LINE__)
static inline void _dec_unacked(struct drbd_conf *mdev, const char *func, int line)
{
	atomic_dec(&mdev->unacked_cnt);
	ERR_IF_CNT_IS_NEGATIVE(unacked_cnt, func, line);
}

#define sub_unacked(mdev, n) _sub_unacked(mdev, n, __FUNCTION__, __LINE__)
static inline void _sub_unacked(struct drbd_conf *mdev, int n, const char *func, int line)
{
	atomic_sub(n, &mdev->unacked_cnt);
	ERR_IF_CNT_IS_NEGATIVE(unacked_cnt, func, line);
}

/**
 * get_ldev() - Increase the ref count on mdev->ldev. Returns 0 if there is no ldev
 * @M:		DRBD device.
 *
 * You have to call put_ldev() when finished working with mdev->ldev.
 */
#define get_ldev(M) __cond_lock(local, _get_ldev_if_state(M,D_INCONSISTENT))
#define get_ldev_if_state(M,MINS) __cond_lock(local, _get_ldev_if_state(M,MINS))

static inline void put_ldev(struct drbd_conf *mdev)
{
	int i = atomic_dec_return(&mdev->local_cnt);
=======
static inline void inc_unacked(struct drbd_device *device)
{
	atomic_inc(&device->unacked_cnt);
}

#define dec_unacked(device) _dec_unacked(device, __func__, __LINE__)
static inline void _dec_unacked(struct drbd_device *device, const char *func, int line)
{
	atomic_dec(&device->unacked_cnt);
	ERR_IF_CNT_IS_NEGATIVE(unacked_cnt, func, line);
}

#define sub_unacked(device, n) _sub_unacked(device, n, __func__, __LINE__)
static inline void _sub_unacked(struct drbd_device *device, int n, const char *func, int line)
{
	atomic_sub(n, &device->unacked_cnt);
	ERR_IF_CNT_IS_NEGATIVE(unacked_cnt, func, line);
}

static inline bool is_sync_target_state(enum drbd_conns connection_state)
{
	return	connection_state == C_SYNC_TARGET ||
		connection_state == C_PAUSED_SYNC_T;
}

static inline bool is_sync_source_state(enum drbd_conns connection_state)
{
	return	connection_state == C_SYNC_SOURCE ||
		connection_state == C_PAUSED_SYNC_S;
}

static inline bool is_sync_state(enum drbd_conns connection_state)
{
	return	is_sync_source_state(connection_state) ||
		is_sync_target_state(connection_state);
}

/**
 * get_ldev() - Increase the ref count on device->ldev. Returns 0 if there is no ldev
 * @_device:		DRBD device.
 * @_min_state:		Minimum device state required for success.
 *
 * You have to call put_ldev() when finished working with device->ldev.
 */
#define get_ldev_if_state(_device, _min_state)				\
	(_get_ldev_if_state((_device), (_min_state)) ?			\
	 ({ __acquire(x); true; }) : false)
#define get_ldev(_device) get_ldev_if_state(_device, D_INCONSISTENT)

static inline void put_ldev(struct drbd_device *device)
{
	enum drbd_disk_state disk_state = device->state.disk;
	/* We must check the state *before* the atomic_dec becomes visible,
	 * or we have a theoretical race where someone hitting zero,
	 * while state still D_FAILED, will then see D_DISKLESS in the
	 * condition below and calling into destroy, where he must not, yet. */
	int i = atomic_dec_return(&device->local_cnt);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* This may be called from some endio handler,
	 * so we must not sleep here. */

	__release(local);
<<<<<<< HEAD
	D_ASSERT(i >= 0);
	if (i == 0) {
		if (mdev->state.disk == D_DISKLESS)
			/* even internal references gone, safe to destroy */
			drbd_ldev_destroy(mdev);
		if (mdev->state.disk == D_FAILED) {
			/* all application IO references gone. */
			if (!test_and_set_bit(GO_DISKLESS, &mdev->flags))
				drbd_queue_work(&mdev->tconn->sender_work, &mdev->go_diskless);
		}
		wake_up(&mdev->misc_wait);
=======
	D_ASSERT(device, i >= 0);
	if (i == 0) {
		if (disk_state == D_DISKLESS)
			/* even internal references gone, safe to destroy */
			drbd_device_post_work(device, DESTROY_DISK);
		if (disk_state == D_FAILED)
			/* all application IO references gone. */
			if (!test_and_set_bit(GOING_DISKLESS, &device->flags))
				drbd_device_post_work(device, GO_DISKLESS);
		wake_up(&device->misc_wait);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

#ifndef __CHECKER__
<<<<<<< HEAD
static inline int _get_ldev_if_state(struct drbd_conf *mdev, enum drbd_disk_state mins)
=======
static inline int _get_ldev_if_state(struct drbd_device *device, enum drbd_disk_state mins)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int io_allowed;

	/* never get a reference while D_DISKLESS */
<<<<<<< HEAD
	if (mdev->state.disk == D_DISKLESS)
		return 0;

	atomic_inc(&mdev->local_cnt);
	io_allowed = (mdev->state.disk >= mins);
	if (!io_allowed)
		put_ldev(mdev);
	return io_allowed;
}
#else
extern int _get_ldev_if_state(struct drbd_conf *mdev, enum drbd_disk_state mins);
#endif

/* you must have an "get_ldev" reference */
static inline void drbd_get_syncer_progress(struct drbd_conf *mdev,
		unsigned long *bits_left, unsigned int *per_mil_done)
{
	/* this is to break it at compile time when we change that, in case we
	 * want to support more than (1<<32) bits on a 32bit arch. */
	typecheck(unsigned long, mdev->rs_total);

	/* note: both rs_total and rs_left are in bits, i.e. in
	 * units of BM_BLOCK_SIZE.
	 * for the percentage, we don't care. */

	if (mdev->state.conn == C_VERIFY_S || mdev->state.conn == C_VERIFY_T)
		*bits_left = mdev->ov_left;
	else
		*bits_left = drbd_bm_total_weight(mdev) - mdev->rs_failed;
	/* >> 10 to prevent overflow,
	 * +1 to prevent division by zero */
	if (*bits_left > mdev->rs_total) {
		/* doh. maybe a logic bug somewhere.
		 * may also be just a race condition
		 * between this and a disconnect during sync.
		 * for now, just prevent in-kernel buffer overflow.
		 */
		smp_rmb();
		dev_warn(DEV, "cs:%s rs_left=%lu > rs_total=%lu (rs_failed %lu)\n",
				drbd_conn_str(mdev->state.conn),
				*bits_left, mdev->rs_total, mdev->rs_failed);
		*per_mil_done = 0;
	} else {
		/* Make sure the division happens in long context.
		 * We allow up to one petabyte storage right now,
		 * at a granularity of 4k per bit that is 2**38 bits.
		 * After shift right and multiplication by 1000,
		 * this should still fit easily into a 32bit long,
		 * so we don't need a 64bit division on 32bit arch.
		 * Note: currently we don't support such large bitmaps on 32bit
		 * arch anyways, but no harm done to be prepared for it here.
		 */
		unsigned int shift = mdev->rs_total > UINT_MAX ? 16 : 10;
		unsigned long left = *bits_left >> shift;
		unsigned long total = 1UL + (mdev->rs_total >> shift);
		unsigned long tmp = 1000UL - left * 1000UL/total;
		*per_mil_done = tmp;
	}
}


/* this throttles on-the-fly application requests
 * according to max_buffers settings;
 * maybe re-implement using semaphores? */
static inline int drbd_get_max_buffers(struct drbd_conf *mdev)
=======
	if (device->state.disk == D_DISKLESS)
		return 0;

	atomic_inc(&device->local_cnt);
	io_allowed = (device->state.disk >= mins);
	if (!io_allowed)
		put_ldev(device);
	return io_allowed;
}
#else
extern int _get_ldev_if_state(struct drbd_device *device, enum drbd_disk_state mins);
#endif

/* this throttles on-the-fly application requests
 * according to max_buffers settings;
 * maybe re-implement using semaphores? */
static inline int drbd_get_max_buffers(struct drbd_device *device)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct net_conf *nc;
	int mxb;

	rcu_read_lock();
<<<<<<< HEAD
	nc = rcu_dereference(mdev->tconn->net_conf);
=======
	nc = rcu_dereference(first_peer_device(device)->connection->net_conf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mxb = nc ? nc->max_buffers : 1000000;  /* arbitrary limit on open requests */
	rcu_read_unlock();

	return mxb;
}

<<<<<<< HEAD
static inline int drbd_state_is_stable(struct drbd_conf *mdev)
{
	union drbd_dev_state s = mdev->state;
=======
static inline int drbd_state_is_stable(struct drbd_device *device)
{
	union drbd_dev_state s = device->state;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* DO NOT add a default clause, we want the compiler to warn us
	 * for any newly introduced state we may have forgotten to add here */

	switch ((enum drbd_conns)s.conn) {
	/* new io only accepted when there is no connection, ... */
	case C_STANDALONE:
	case C_WF_CONNECTION:
	/* ... or there is a well established connection. */
	case C_CONNECTED:
	case C_SYNC_SOURCE:
	case C_SYNC_TARGET:
	case C_VERIFY_S:
	case C_VERIFY_T:
	case C_PAUSED_SYNC_S:
	case C_PAUSED_SYNC_T:
	case C_AHEAD:
	case C_BEHIND:
		/* transitional states, IO allowed */
	case C_DISCONNECTING:
	case C_UNCONNECTED:
	case C_TIMEOUT:
	case C_BROKEN_PIPE:
	case C_NETWORK_FAILURE:
	case C_PROTOCOL_ERROR:
	case C_TEAR_DOWN:
	case C_WF_REPORT_PARAMS:
	case C_STARTING_SYNC_S:
	case C_STARTING_SYNC_T:
		break;

		/* Allow IO in BM exchange states with new protocols */
	case C_WF_BITMAP_S:
<<<<<<< HEAD
		if (mdev->tconn->agreed_pro_version < 96)
=======
		if (first_peer_device(device)->connection->agreed_pro_version < 96)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return 0;
		break;

		/* no new io accepted in these states */
	case C_WF_BITMAP_T:
	case C_WF_SYNC_UUID:
	case C_MASK:
		/* not "stable" */
		return 0;
	}

	switch ((enum drbd_disk_state)s.disk) {
	case D_DISKLESS:
	case D_INCONSISTENT:
	case D_OUTDATED:
	case D_CONSISTENT:
	case D_UP_TO_DATE:
	case D_FAILED:
		/* disk state is stable as well. */
		break;

	/* no new io accepted during transitional states */
	case D_ATTACHING:
	case D_NEGOTIATING:
	case D_UNKNOWN:
	case D_MASK:
		/* not "stable" */
		return 0;
	}

	return 1;
}

<<<<<<< HEAD
static inline int drbd_suspended(struct drbd_conf *mdev)
{
	struct drbd_tconn *tconn = mdev->tconn;

	return tconn->susp || tconn->susp_fen || tconn->susp_nod;
}

static inline bool may_inc_ap_bio(struct drbd_conf *mdev)
{
	int mxb = drbd_get_max_buffers(mdev);

	if (drbd_suspended(mdev))
		return false;
	if (test_bit(SUSPEND_IO, &mdev->flags))
=======
static inline int drbd_suspended(struct drbd_device *device)
{
	struct drbd_resource *resource = device->resource;

	return resource->susp || resource->susp_fen || resource->susp_nod;
}

static inline bool may_inc_ap_bio(struct drbd_device *device)
{
	int mxb = drbd_get_max_buffers(device);

	if (drbd_suspended(device))
		return false;
	if (atomic_read(&device->suspend_cnt))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return false;

	/* to avoid potential deadlock or bitmap corruption,
	 * in various places, we only allow new application io
	 * to start during "stable" states. */

	/* no new io accepted when attaching or detaching the disk */
<<<<<<< HEAD
	if (!drbd_state_is_stable(mdev))
=======
	if (!drbd_state_is_stable(device))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return false;

	/* since some older kernels don't have atomic_add_unless,
	 * and we are within the spinlock anyways, we have this workaround.  */
<<<<<<< HEAD
	if (atomic_read(&mdev->ap_bio_cnt) > mxb)
		return false;
	if (test_bit(BITMAP_IO, &mdev->flags))
=======
	if (atomic_read(&device->ap_bio_cnt) > mxb)
		return false;
	if (test_bit(BITMAP_IO, &device->flags))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return false;
	return true;
}

<<<<<<< HEAD
static inline bool inc_ap_bio_cond(struct drbd_conf *mdev)
{
	bool rv = false;

	spin_lock_irq(&mdev->tconn->req_lock);
	rv = may_inc_ap_bio(mdev);
	if (rv)
		atomic_inc(&mdev->ap_bio_cnt);
	spin_unlock_irq(&mdev->tconn->req_lock);
=======
static inline bool inc_ap_bio_cond(struct drbd_device *device)
{
	bool rv = false;

	spin_lock_irq(&device->resource->req_lock);
	rv = may_inc_ap_bio(device);
	if (rv)
		atomic_inc(&device->ap_bio_cnt);
	spin_unlock_irq(&device->resource->req_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return rv;
}

<<<<<<< HEAD
static inline void inc_ap_bio(struct drbd_conf *mdev)
=======
static inline void inc_ap_bio(struct drbd_device *device)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	/* we wait here
	 *    as long as the device is suspended
	 *    until the bitmap is no longer on the fly during connection
	 *    handshake as long as we would exceed the max_buffer limit.
	 *
	 * to avoid races with the reconnect code,
	 * we need to atomic_inc within the spinlock. */

<<<<<<< HEAD
	wait_event(mdev->misc_wait, inc_ap_bio_cond(mdev));
}

static inline void dec_ap_bio(struct drbd_conf *mdev)
{
	int mxb = drbd_get_max_buffers(mdev);
	int ap_bio = atomic_dec_return(&mdev->ap_bio_cnt);

	D_ASSERT(ap_bio >= 0);

	if (ap_bio == 0 && test_bit(BITMAP_IO, &mdev->flags)) {
		if (!test_and_set_bit(BITMAP_IO_QUEUED, &mdev->flags))
			drbd_queue_work(&mdev->tconn->sender_work, &mdev->bm_io_work.w);
=======
	wait_event(device->misc_wait, inc_ap_bio_cond(device));
}

static inline void dec_ap_bio(struct drbd_device *device)
{
	int mxb = drbd_get_max_buffers(device);
	int ap_bio = atomic_dec_return(&device->ap_bio_cnt);

	D_ASSERT(device, ap_bio >= 0);

	if (ap_bio == 0 && test_bit(BITMAP_IO, &device->flags)) {
		if (!test_and_set_bit(BITMAP_IO_QUEUED, &device->flags))
			drbd_queue_work(&first_peer_device(device)->
				connection->sender_work,
				&device->bm_io_work.w);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* this currently does wake_up for every dec_ap_bio!
	 * maybe rather introduce some type of hysteresis?
	 * e.g. (ap_bio == mxb/2 || ap_bio == 0) ? */
	if (ap_bio < mxb)
<<<<<<< HEAD
		wake_up(&mdev->misc_wait);
}

static inline bool verify_can_do_stop_sector(struct drbd_conf *mdev)
{
	return mdev->tconn->agreed_pro_version >= 97 &&
		mdev->tconn->agreed_pro_version != 100;
}

static inline int drbd_set_ed_uuid(struct drbd_conf *mdev, u64 val)
{
	int changed = mdev->ed_uuid != val;
	mdev->ed_uuid = val;
	return changed;
}

static inline int drbd_queue_order_type(struct drbd_conf *mdev)
=======
		wake_up(&device->misc_wait);
}

static inline bool verify_can_do_stop_sector(struct drbd_device *device)
{
	return first_peer_device(device)->connection->agreed_pro_version >= 97 &&
		first_peer_device(device)->connection->agreed_pro_version != 100;
}

static inline int drbd_set_ed_uuid(struct drbd_device *device, u64 val)
{
	int changed = device->ed_uuid != val;
	device->ed_uuid = val;
	return changed;
}

static inline int drbd_queue_order_type(struct drbd_device *device)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	/* sorry, we currently have no working implementation
	 * of distributed TCQ stuff */
#ifndef QUEUE_ORDERED_NONE
#define QUEUE_ORDERED_NONE 0
#endif
	return QUEUE_ORDERED_NONE;
}

<<<<<<< HEAD
static inline void drbd_md_flush(struct drbd_conf *mdev)
{
	int r;

	if (mdev->ldev == NULL) {
		dev_warn(DEV, "mdev->ldev == NULL in drbd_md_flush\n");
		return;
	}

	if (test_bit(MD_NO_FUA, &mdev->flags))
		return;

	r = blkdev_issue_flush(mdev->ldev->md_bdev, GFP_NOIO, NULL);
	if (r) {
		set_bit(MD_NO_FUA, &mdev->flags);
		dev_err(DEV, "meta data flush failed with status %d, disabling md-flushes\n", r);
	}
=======
static inline struct drbd_connection *first_connection(struct drbd_resource *resource)
{
	return list_first_entry_or_null(&resource->connections,
				struct drbd_connection, connections);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif
