#ifndef MMC_QUEUE_H
#define MMC_QUEUE_H

<<<<<<< HEAD
#define MMC_REQ_SPECIAL_MASK	(REQ_DISCARD | REQ_FLUSH)
=======
static inline bool mmc_req_is_special(struct request *req)
{
	return req &&
		(req_op(req) == REQ_OP_FLUSH ||
		 req_op(req) == REQ_OP_DISCARD ||
		 req_op(req) == REQ_OP_SECURE_ERASE);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct request;
struct task_struct;

struct mmc_blk_request {
	struct mmc_request	mrq;
	struct mmc_command	sbc;
	struct mmc_command	cmd;
	struct mmc_command	stop;
	struct mmc_data		data;
<<<<<<< HEAD
=======
	int			retune_retry_done;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

enum mmc_packed_type {
	MMC_PACKED_NONE = 0,
	MMC_PACKED_WRITE,
};

#define mmc_packed_cmd(type)	((type) != MMC_PACKED_NONE)
#define mmc_packed_wr(type)	((type) == MMC_PACKED_WRITE)

struct mmc_packed {
	struct list_head	list;
<<<<<<< HEAD
	u32			cmd_hdr[1024];
=======
	__le32			cmd_hdr[1024];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int		blocks;
	u8			nr_entries;
	u8			retries;
	s16			idx_failure;
};

struct mmc_queue_req {
	struct request		*req;
	struct mmc_blk_request	brq;
	struct scatterlist	*sg;
	char			*bounce_buf;
	struct scatterlist	*bounce_sg;
	unsigned int		bounce_sg_len;
	struct mmc_async_req	mmc_active;
	enum mmc_packed_type	cmd_type;
	struct mmc_packed	*packed;
};

struct mmc_queue {
	struct mmc_card		*card;
	struct task_struct	*thread;
	struct semaphore	thread_sem;
<<<<<<< HEAD
	volatile unsigned long	flags;
#define MMC_QUEUE_SUSPENDED	(1 << 0)
#define MMC_QUEUE_NEW_REQUEST	(1 << 1)
#define MMC_QUEUE_SUSPENDED_BIT		(1 << 0)
#define MMC_QUEUE_NEW_REQUEST_BIT	(1 << 1)

	int			(*issue_fn)(struct mmc_queue *, struct request *);
=======
	unsigned int		flags;
#define MMC_QUEUE_SUSPENDED	(1 << 0)
#define MMC_QUEUE_NEW_REQUEST	(1 << 1)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	void			*data;
	struct request_queue	*queue;
	struct mmc_queue_req	mqrq[2];
	struct mmc_queue_req	*mqrq_cur;
	struct mmc_queue_req	*mqrq_prev;
<<<<<<< HEAD
	int tmp_get_card_flag;
=======
#ifdef CONFIG_MMC_SIMULATE_MAX_SPEED
	atomic_t max_write_speed;
	atomic_t max_read_speed;
	atomic_t cache_size;
	/* i/o tracking */
	atomic_long_t cache_used;
	unsigned long cache_jiffies;
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

extern int mmc_init_queue(struct mmc_queue *, struct mmc_card *, spinlock_t *,
			  const char *);
extern void mmc_cleanup_queue(struct mmc_queue *);
<<<<<<< HEAD
extern int mmc_queue_suspend(struct mmc_queue *, int wait);
=======
extern void mmc_queue_suspend(struct mmc_queue *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern void mmc_queue_resume(struct mmc_queue *);

extern unsigned int mmc_queue_map_sg(struct mmc_queue *,
				     struct mmc_queue_req *);
extern void mmc_queue_bounce_pre(struct mmc_queue_req *);
extern void mmc_queue_bounce_post(struct mmc_queue_req *);

extern int mmc_packed_init(struct mmc_queue *, struct mmc_card *);
extern void mmc_packed_clean(struct mmc_queue *);

<<<<<<< HEAD
=======
extern int mmc_access_rpmb(struct mmc_queue *);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
