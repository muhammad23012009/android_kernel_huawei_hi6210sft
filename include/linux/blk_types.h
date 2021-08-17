/*
 * Block data types and constants.  Directly include this file only to
 * break include dependency loop.
 */
#ifndef __LINUX_BLK_TYPES_H
#define __LINUX_BLK_TYPES_H

<<<<<<< HEAD
#ifdef CONFIG_BLOCK

#include <linux/types.h>
=======
#include <linux/types.h>
#include <linux/bvec.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct bio_set;
struct bio;
struct bio_integrity_payload;
struct page;
struct block_device;
struct io_context;
struct cgroup_subsys_state;
<<<<<<< HEAD
typedef void (bio_end_io_t) (struct bio *, int);
typedef void (bio_destructor_t) (struct bio *);

/*
 * was unsigned short, but we might as well be ready for > 64kB I/O pages
 */
struct bio_vec {
	struct page	*bv_page;
	unsigned int	bv_len;
	unsigned int	bv_offset;
};

=======
typedef void (bio_end_io_t) (struct bio *);

#ifdef CONFIG_BLOCK
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * main unit of I/O for the block layer and lower layers (ie drivers and
 * stacking drivers)
 */
struct bio {
<<<<<<< HEAD
	sector_t		bi_sector;	/* device address in 512 byte
						   sectors */
	struct bio		*bi_next;	/* request queue link */
	struct block_device	*bi_bdev;
	unsigned long		bi_flags;	/* status, command, etc */
	unsigned long		bi_rw;		/* bottom bits READ/WRITE,
						 * top bits priority
						 */

	unsigned short		bi_vcnt;	/* how many bio_vec's */
	unsigned short		bi_idx;		/* current index into bvl_vec */
=======
	struct bio		*bi_next;	/* request queue link */
	struct block_device	*bi_bdev;
	int			bi_error;
	unsigned int		bi_opf;		/* bottom bits req flags,
						 * top bits REQ_OP. Use
						 * accessors.
						 */
	unsigned short		bi_flags;	/* status, command, etc */
	unsigned short		bi_ioprio;

	struct bvec_iter	bi_iter;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Number of segments in this BIO after
	 * physical address coalescing is performed.
	 */
	unsigned int		bi_phys_segments;

<<<<<<< HEAD
	unsigned int		bi_size;	/* residual I/O count */

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * To keep track of the max segment size, we account for the
	 * sizes of the first and last mergeable segments in this bio.
	 */
	unsigned int		bi_seg_front_size;
	unsigned int		bi_seg_back_size;

<<<<<<< HEAD
=======
	atomic_t		__bi_remaining;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bio_end_io_t		*bi_end_io;

	void			*bi_private;
#ifdef CONFIG_BLK_CGROUP
	/*
	 * Optional ioc and css associated with this bio.  Put on bio
	 * release.  Read comment on top of bio_associate_current().
	 */
	struct io_context	*bi_ioc;
	struct cgroup_subsys_state *bi_css;
#endif
<<<<<<< HEAD
#if defined(CONFIG_BLK_DEV_INTEGRITY)
	struct bio_integrity_payload *bi_integrity;  /* data integrity */
#endif
=======
	union {
#if defined(CONFIG_BLK_DEV_INTEGRITY)
		struct bio_integrity_payload *bi_integrity; /* data integrity */
#endif
	};

	unsigned short		bi_vcnt;	/* how many bio_vec's */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Everything starting with bi_max_vecs will be preserved by bio_reset()
	 */

<<<<<<< HEAD
	unsigned int		bi_max_vecs;	/* max bvl_vecs we can hold */

	atomic_t		bi_cnt;		/* pin count */
=======
	unsigned short		bi_max_vecs;	/* max bvl_vecs we can hold */

	atomic_t		__bi_cnt;	/* pin count */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	struct bio_vec		*bi_io_vec;	/* the actual vec list */

	struct bio_set		*bi_pool;

	/*
	 * We can inline a number of vecs at the end of the bio, to avoid
	 * double allocations for a small number of bio_vecs. This member
	 * MUST obviously be kept at the very end of the bio.
	 */
	struct bio_vec		bi_inline_vecs[0];
};

<<<<<<< HEAD
=======
#define BIO_OP_SHIFT	(8 * FIELD_SIZEOF(struct bio, bi_opf) - REQ_OP_BITS)
#define bio_flags(bio)	((bio)->bi_opf & ((1 << BIO_OP_SHIFT) - 1))
#define bio_op(bio)	((bio)->bi_opf >> BIO_OP_SHIFT)

#define bio_set_op_attrs(bio, op, op_flags) do {			\
	if (__builtin_constant_p(op))					\
		BUILD_BUG_ON((op) + 0U >= (1U << REQ_OP_BITS));		\
	else								\
		WARN_ON_ONCE((op) + 0U >= (1U << REQ_OP_BITS));		\
	if (__builtin_constant_p(op_flags))				\
		BUILD_BUG_ON((op_flags) + 0U >= (1U << BIO_OP_SHIFT));	\
	else								\
		WARN_ON_ONCE((op_flags) + 0U >= (1U << BIO_OP_SHIFT));	\
	(bio)->bi_opf = bio_flags(bio);					\
	(bio)->bi_opf |= (((op) + 0U) << BIO_OP_SHIFT);			\
	(bio)->bi_opf |= (op_flags);					\
} while (0)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define BIO_RESET_BYTES		offsetof(struct bio, bi_max_vecs)

/*
 * bio flags
 */
<<<<<<< HEAD
#define BIO_UPTODATE	0	/* ok after I/O completion */
#define BIO_RW_BLOCK	1	/* RW_AHEAD set, and read/write would block */
#define BIO_EOF		2	/* out-out-bounds error */
#define BIO_SEG_VALID	3	/* bi_phys_segments valid */
#define BIO_CLONED	4	/* doesn't own data */
#define BIO_BOUNCED	5	/* bio is a bounce bio */
#define BIO_USER_MAPPED 6	/* contains user pages */
#define BIO_EOPNOTSUPP	7	/* not supported */
#define BIO_NULL_MAPPED 8	/* contains invalid user pages */
#define BIO_FS_INTEGRITY 9	/* fs owns integrity data, not block layer */
#define BIO_QUIET	10	/* Make BIO Quiet */
#define BIO_MAPPED_INTEGRITY 11/* integrity metadata has been remapped */
#define BIO_SNAP_STABLE	12	/* bio data must be snapshotted during write */

/*
 * Flags starting here get preserved by bio_reset() - this includes
 * BIO_POOL_IDX()
 */
#define BIO_RESET_BITS	13
#define BIO_OWNS_VEC	13	/* bio_free() should free bvec */

#define bio_flagged(bio, flag)	((bio)->bi_flags & (1 << (flag)))

/*
 * top 4 bits of bio flags indicate the pool this bio came from
 */
#define BIO_POOL_BITS		(4)
#define BIO_POOL_NONE		((1UL << BIO_POOL_BITS) - 1)
#define BIO_POOL_OFFSET		(BITS_PER_LONG - BIO_POOL_BITS)
#define BIO_POOL_MASK		(1UL << BIO_POOL_OFFSET)
#define BIO_POOL_IDX(bio)	((bio)->bi_flags >> BIO_POOL_OFFSET)
=======
#define BIO_SEG_VALID	1	/* bi_phys_segments valid */
#define BIO_CLONED	2	/* doesn't own data */
#define BIO_BOUNCED	3	/* bio is a bounce bio */
#define BIO_USER_MAPPED 4	/* contains user pages */
#define BIO_NULL_MAPPED 5	/* contains invalid user pages */
#define BIO_QUIET	6	/* Make BIO Quiet */
#define BIO_CHAIN	7	/* chained bio, ->bi_remaining in effect */
#define BIO_REFFED	8	/* bio has elevated ->bi_cnt */

/*
 * Flags starting here get preserved by bio_reset() - this includes
 * BVEC_POOL_IDX()
 */
#define BIO_RESET_BITS	10

/*
 * We support 6 different bvec pools, the last one is magic in that it
 * is backed by a mempool.
 */
#define BVEC_POOL_NR		6
#define BVEC_POOL_MAX		(BVEC_POOL_NR - 1)

/*
 * Top 4 bits of bio flags indicate the pool the bvecs came from.  We add
 * 1 to the actual index so that 0 indicates that there are no bvecs to be
 * freed.
 */
#define BVEC_POOL_BITS		(4)
#define BVEC_POOL_OFFSET	(16 - BVEC_POOL_BITS)
#define BVEC_POOL_IDX(bio)	((bio)->bi_flags >> BVEC_POOL_OFFSET)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* CONFIG_BLOCK */

/*
 * Request flags.  For use in the cmd_flags field of struct request, and in
<<<<<<< HEAD
 * bi_rw of struct bio.  Note that some flags are only valid in either one.
 */
enum rq_flag_bits {
	/* common flags */
	__REQ_WRITE,		/* not set, read. set, write */
=======
 * bi_opf of struct bio.  Note that some flags are only valid in either one.
 */
enum rq_flag_bits {
	/* common flags */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__REQ_FAILFAST_DEV,	/* no driver retries of device errors */
	__REQ_FAILFAST_TRANSPORT, /* no driver retries of transport errors */
	__REQ_FAILFAST_DRIVER,	/* no driver retries of driver errors */

	__REQ_SYNC,		/* request is sync (sync write or read) */
	__REQ_META,		/* metadata io request */
	__REQ_PRIO,		/* boost priority in cfq */
<<<<<<< HEAD
	__REQ_DISCARD,		/* request to discard sectors */
	__REQ_SECURE,		/* secure discard (used with __REQ_DISCARD) */
	__REQ_WRITE_SAME,	/* write same block many times */

	__REQ_NOIDLE,		/* don't anticipate more IO after this one */
	__REQ_FUA,		/* forced unit access */
	__REQ_FLUSH,		/* request for cache flush */
=======

	__REQ_NOIDLE,		/* don't anticipate more IO after this one */
	__REQ_INTEGRITY,	/* I/O includes block integrity payload */
	__REQ_FUA,		/* forced unit access */
	__REQ_PREFLUSH,		/* request for cache flush */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* bio only flags */
	__REQ_RAHEAD,		/* read ahead, can fail anytime */
	__REQ_THROTTLED,	/* This bio has already been subjected to
				 * throttling rules. Don't do it again. */

	/* request only flags */
	__REQ_SORTED,		/* elevator knows about this request */
	__REQ_SOFTBARRIER,	/* may not be passed by ioscheduler */
	__REQ_NOMERGE,		/* don't touch this for merging */
	__REQ_STARTED,		/* drive already may have started this one */
	__REQ_DONTPREP,		/* don't call prep for this one */
	__REQ_QUEUED,		/* uses queueing */
	__REQ_ELVPRIV,		/* elevator private data attached */
	__REQ_FAILED,		/* set if the request failed */
	__REQ_QUIET,		/* don't worry about errors */
<<<<<<< HEAD
	__REQ_PREEMPT,		/* set for "ide_preempt" requests */
=======
	__REQ_PREEMPT,		/* set for "ide_preempt" requests and also
				   for requests for which the SCSI "quiesce"
				   state must be ignored. */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__REQ_ALLOCED,		/* request came from our alloc pool */
	__REQ_COPY_USER,	/* contains copies of user pages */
	__REQ_FLUSH_SEQ,	/* request for flush sequence */
	__REQ_IO_STAT,		/* account I/O stat */
	__REQ_MIXED_MERGE,	/* merge of different types, fail separately */
<<<<<<< HEAD
	__REQ_KERNEL, 		/* direct IO to kernel pages */
	__REQ_PM,		/* runtime pm request */
	__REQ_NR_BITS,		/* stops here */
};

#define REQ_WRITE		(1 << __REQ_WRITE)
#define REQ_FAILFAST_DEV	(1 << __REQ_FAILFAST_DEV)
#define REQ_FAILFAST_TRANSPORT	(1 << __REQ_FAILFAST_TRANSPORT)
#define REQ_FAILFAST_DRIVER	(1 << __REQ_FAILFAST_DRIVER)
#define REQ_SYNC		(1 << __REQ_SYNC)
#define REQ_META		(1 << __REQ_META)
#define REQ_PRIO		(1 << __REQ_PRIO)
#define REQ_DISCARD		(1 << __REQ_DISCARD)
#define REQ_WRITE_SAME		(1 << __REQ_WRITE_SAME)
#define REQ_NOIDLE		(1 << __REQ_NOIDLE)
=======
	__REQ_PM,		/* runtime pm request */
	__REQ_HASHED,		/* on IO scheduler merge hash */
	__REQ_MQ_INFLIGHT,	/* track inflight for MQ */
	__REQ_NR_BITS,		/* stops here */
};

#define REQ_FAILFAST_DEV	(1ULL << __REQ_FAILFAST_DEV)
#define REQ_FAILFAST_TRANSPORT	(1ULL << __REQ_FAILFAST_TRANSPORT)
#define REQ_FAILFAST_DRIVER	(1ULL << __REQ_FAILFAST_DRIVER)
#define REQ_SYNC		(1ULL << __REQ_SYNC)
#define REQ_META		(1ULL << __REQ_META)
#define REQ_PRIO		(1ULL << __REQ_PRIO)
#define REQ_NOIDLE		(1ULL << __REQ_NOIDLE)
#define REQ_INTEGRITY		(1ULL << __REQ_INTEGRITY)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define REQ_FAILFAST_MASK \
	(REQ_FAILFAST_DEV | REQ_FAILFAST_TRANSPORT | REQ_FAILFAST_DRIVER)
#define REQ_COMMON_MASK \
<<<<<<< HEAD
	(REQ_WRITE | REQ_FAILFAST_MASK | REQ_SYNC | REQ_META | REQ_PRIO | \
	 REQ_DISCARD | REQ_WRITE_SAME | REQ_NOIDLE | REQ_FLUSH | REQ_FUA | \
	 REQ_SECURE)
#define REQ_CLONE_MASK		REQ_COMMON_MASK

#define BIO_NO_ADVANCE_ITER_MASK	(REQ_DISCARD|REQ_WRITE_SAME)

/* This mask is used for both bio and request merge checking */
#define REQ_NOMERGE_FLAGS \
	(REQ_NOMERGE | REQ_STARTED | REQ_SOFTBARRIER | REQ_FLUSH | REQ_FUA)

#define REQ_RAHEAD		(1 << __REQ_RAHEAD)
#define REQ_THROTTLED		(1 << __REQ_THROTTLED)

#define REQ_SORTED		(1 << __REQ_SORTED)
#define REQ_SOFTBARRIER		(1 << __REQ_SOFTBARRIER)
#define REQ_FUA			(1 << __REQ_FUA)
#define REQ_NOMERGE		(1 << __REQ_NOMERGE)
#define REQ_STARTED		(1 << __REQ_STARTED)
#define REQ_DONTPREP		(1 << __REQ_DONTPREP)
#define REQ_QUEUED		(1 << __REQ_QUEUED)
#define REQ_ELVPRIV		(1 << __REQ_ELVPRIV)
#define REQ_FAILED		(1 << __REQ_FAILED)
#define REQ_QUIET		(1 << __REQ_QUIET)
#define REQ_PREEMPT		(1 << __REQ_PREEMPT)
#define REQ_ALLOCED		(1 << __REQ_ALLOCED)
#define REQ_COPY_USER		(1 << __REQ_COPY_USER)
#define REQ_FLUSH		(1 << __REQ_FLUSH)
#define REQ_FLUSH_SEQ		(1 << __REQ_FLUSH_SEQ)
#define REQ_IO_STAT		(1 << __REQ_IO_STAT)
#define REQ_MIXED_MERGE		(1 << __REQ_MIXED_MERGE)
#define REQ_SECURE		(1 << __REQ_SECURE)
#define REQ_KERNEL		(1 << __REQ_KERNEL)
#define REQ_PM			(1 << __REQ_PM)

#if 1 //clb
#define REQ_PACKED			REQ_PM
#define REQ_PACKED_START	REQ_FAILFAST_DEV
#define REQ_PACKED_END		REQ_FAILFAST_TRANSPORT
#endif 
=======
	(REQ_FAILFAST_MASK | REQ_SYNC | REQ_META | REQ_PRIO | REQ_NOIDLE | \
	 REQ_PREFLUSH | REQ_FUA | REQ_INTEGRITY | REQ_NOMERGE)
#define REQ_CLONE_MASK		REQ_COMMON_MASK

/* This mask is used for both bio and request merge checking */
#define REQ_NOMERGE_FLAGS \
	(REQ_NOMERGE | REQ_STARTED | REQ_SOFTBARRIER | REQ_PREFLUSH | REQ_FUA | REQ_FLUSH_SEQ)

#define REQ_RAHEAD		(1ULL << __REQ_RAHEAD)
#define REQ_THROTTLED		(1ULL << __REQ_THROTTLED)

#define REQ_SORTED		(1ULL << __REQ_SORTED)
#define REQ_SOFTBARRIER		(1ULL << __REQ_SOFTBARRIER)
#define REQ_FUA			(1ULL << __REQ_FUA)
#define REQ_NOMERGE		(1ULL << __REQ_NOMERGE)
#define REQ_STARTED		(1ULL << __REQ_STARTED)
#define REQ_DONTPREP		(1ULL << __REQ_DONTPREP)
#define REQ_QUEUED		(1ULL << __REQ_QUEUED)
#define REQ_ELVPRIV		(1ULL << __REQ_ELVPRIV)
#define REQ_FAILED		(1ULL << __REQ_FAILED)
#define REQ_QUIET		(1ULL << __REQ_QUIET)
#define REQ_PREEMPT		(1ULL << __REQ_PREEMPT)
#define REQ_ALLOCED		(1ULL << __REQ_ALLOCED)
#define REQ_COPY_USER		(1ULL << __REQ_COPY_USER)
#define REQ_PREFLUSH		(1ULL << __REQ_PREFLUSH)
#define REQ_FLUSH_SEQ		(1ULL << __REQ_FLUSH_SEQ)
#define REQ_IO_STAT		(1ULL << __REQ_IO_STAT)
#define REQ_MIXED_MERGE		(1ULL << __REQ_MIXED_MERGE)
#define REQ_PM			(1ULL << __REQ_PM)
#define REQ_HASHED		(1ULL << __REQ_HASHED)
#define REQ_MQ_INFLIGHT		(1ULL << __REQ_MQ_INFLIGHT)

enum req_op {
	REQ_OP_READ,
	REQ_OP_WRITE,
	REQ_OP_DISCARD,		/* request to discard sectors */
	REQ_OP_SECURE_ERASE,	/* request to securely erase sectors */
	REQ_OP_WRITE_SAME,	/* write same block many times */
	REQ_OP_FLUSH,		/* request for cache flush */
};

#define REQ_OP_BITS 3

typedef unsigned int blk_qc_t;
#define BLK_QC_T_NONE	-1U
#define BLK_QC_T_SHIFT	16

static inline bool blk_qc_t_valid(blk_qc_t cookie)
{
	return cookie != BLK_QC_T_NONE;
}

static inline blk_qc_t blk_tag_to_qc_t(unsigned int tag, unsigned int queue_num)
{
	return tag | (queue_num << BLK_QC_T_SHIFT);
}

static inline unsigned int blk_qc_t_to_queue_num(blk_qc_t cookie)
{
	return cookie >> BLK_QC_T_SHIFT;
}

static inline unsigned int blk_qc_t_to_tag(blk_qc_t cookie)
{
	return cookie & ((1u << BLK_QC_T_SHIFT) - 1);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __LINUX_BLK_TYPES_H */
