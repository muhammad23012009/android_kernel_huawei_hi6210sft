/*
 * DMA Engine test module
 *
 * Copyright (C) 2007 Atmel Corporation
 * Copyright (C) 2013 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/freezer.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/wait.h>
<<<<<<< HEAD
#include <linux/ctype.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>

static unsigned int test_buf_size = 16384;
module_param(test_buf_size, uint, S_IRUGO);
MODULE_PARM_DESC(test_buf_size, "Size of the memcpy test buffer");

static char test_channel[20];
module_param_string(channel, test_channel, sizeof(test_channel), S_IRUGO);
MODULE_PARM_DESC(channel, "Bus ID of the channel to test (default: any)");

static char test_device[20];
module_param_string(device, test_device, sizeof(test_device), S_IRUGO);
MODULE_PARM_DESC(device, "Bus ID of the DMA Engine to test (default: any)");

static unsigned int threads_per_chan = 1;
module_param(threads_per_chan, uint, S_IRUGO);
=======

static unsigned int test_buf_size = 16384;
module_param(test_buf_size, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(test_buf_size, "Size of the memcpy test buffer");

static char test_channel[20];
module_param_string(channel, test_channel, sizeof(test_channel),
		S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(channel, "Bus ID of the channel to test (default: any)");

static char test_device[32];
module_param_string(device, test_device, sizeof(test_device),
		S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(device, "Bus ID of the DMA Engine to test (default: any)");

static unsigned int threads_per_chan = 1;
module_param(threads_per_chan, uint, S_IRUGO | S_IWUSR);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_PARM_DESC(threads_per_chan,
		"Number of threads to start per channel (default: 1)");

static unsigned int max_channels;
<<<<<<< HEAD
module_param(max_channels, uint, S_IRUGO);
=======
module_param(max_channels, uint, S_IRUGO | S_IWUSR);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_PARM_DESC(max_channels,
		"Maximum number of channels to use (default: all)");

static unsigned int iterations;
<<<<<<< HEAD
module_param(iterations, uint, S_IRUGO);
MODULE_PARM_DESC(iterations,
		"Iterations before stopping test (default: infinite)");

static unsigned int xor_sources = 3;
module_param(xor_sources, uint, S_IRUGO);
=======
module_param(iterations, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(iterations,
		"Iterations before stopping test (default: infinite)");

static unsigned int sg_buffers = 1;
module_param(sg_buffers, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(sg_buffers,
		"Number of scatter gather buffers (default: 1)");

static unsigned int dmatest;
module_param(dmatest, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(dmatest,
		"dmatest 0-memcpy 1-slave_sg (default: 0)");

static unsigned int xor_sources = 3;
module_param(xor_sources, uint, S_IRUGO | S_IWUSR);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_PARM_DESC(xor_sources,
		"Number of xor source buffers (default: 3)");

static unsigned int pq_sources = 3;
<<<<<<< HEAD
module_param(pq_sources, uint, S_IRUGO);
=======
module_param(pq_sources, uint, S_IRUGO | S_IWUSR);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_PARM_DESC(pq_sources,
		"Number of p+q source buffers (default: 3)");

static int timeout = 3000;
<<<<<<< HEAD
module_param(timeout, uint, S_IRUGO);
MODULE_PARM_DESC(timeout, "Transfer Timeout in msec (default: 3000), "
		 "Pass -1 for infinite timeout");

=======
module_param(timeout, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(timeout, "Transfer Timeout in msec (default: 3000), "
		 "Pass -1 for infinite timeout");

static bool noverify;
module_param(noverify, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(noverify, "Disable random data setup and verification");

static bool verbose;
module_param(verbose, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(verbose, "Enable \"success\" result messages (default: off)");

/**
 * struct dmatest_params - test parameters.
 * @buf_size:		size of the memcpy test buffer
 * @channel:		bus ID of the channel to test
 * @device:		bus ID of the DMA Engine to test
 * @threads_per_chan:	number of threads to start per channel
 * @max_channels:	maximum number of channels to use
 * @iterations:		iterations before stopping test
 * @xor_sources:	number of xor source buffers
 * @pq_sources:		number of p+q source buffers
 * @timeout:		transfer timeout in msec, -1 for infinite timeout
 */
struct dmatest_params {
	unsigned int	buf_size;
	char		channel[20];
	char		device[32];
	unsigned int	threads_per_chan;
	unsigned int	max_channels;
	unsigned int	iterations;
	unsigned int	xor_sources;
	unsigned int	pq_sources;
	int		timeout;
	bool		noverify;
};

/**
 * struct dmatest_info - test information.
 * @params:		test parameters
 * @lock:		access protection to the fields of this structure
 */
static struct dmatest_info {
	/* Test parameters */
	struct dmatest_params	params;

	/* Internal state */
	struct list_head	channels;
	unsigned int		nr_channels;
	struct mutex		lock;
	bool			did_init;
} test_info = {
	.channels = LIST_HEAD_INIT(test_info.channels),
	.lock = __MUTEX_INITIALIZER(test_info.lock),
};

static int dmatest_run_set(const char *val, const struct kernel_param *kp);
static int dmatest_run_get(char *val, const struct kernel_param *kp);
static const struct kernel_param_ops run_ops = {
	.set = dmatest_run_set,
	.get = dmatest_run_get,
};
static bool dmatest_run;
module_param_cb(run, &run_ops, &dmatest_run, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(run, "Run the test (default: false)");

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Maximum amount of mismatched bytes in buffer to print */
#define MAX_ERROR_COUNT		32

/*
 * Initialization patterns. All bytes in the source buffer has bit 7
 * set, all bytes in the destination buffer has bit 7 cleared.
 *
 * Bit 6 is set for all bytes which are to be copied by the DMA
 * engine. Bit 5 is set for all bytes which are to be overwritten by
 * the DMA engine.
 *
 * The remaining bits are the inverse of a counter which increments by
 * one for each byte address.
 */
#define PATTERN_SRC		0x80
#define PATTERN_DST		0x00
#define PATTERN_COPY		0x40
#define PATTERN_OVERWRITE	0x20
#define PATTERN_COUNT_MASK	0x1f

<<<<<<< HEAD
enum dmatest_error_type {
	DMATEST_ET_OK,
	DMATEST_ET_MAP_SRC,
	DMATEST_ET_MAP_DST,
	DMATEST_ET_PREP,
	DMATEST_ET_SUBMIT,
	DMATEST_ET_TIMEOUT,
	DMATEST_ET_DMA_ERROR,
	DMATEST_ET_DMA_IN_PROGRESS,
	DMATEST_ET_VERIFY,
	DMATEST_ET_VERIFY_BUF,
};

struct dmatest_verify_buffer {
	unsigned int	index;
	u8		expected;
	u8		actual;
};

struct dmatest_verify_result {
	unsigned int			error_count;
	struct dmatest_verify_buffer	data[MAX_ERROR_COUNT];
	u8				pattern;
	bool				is_srcbuf;
};

struct dmatest_thread_result {
	struct list_head	node;
	unsigned int		n;
	unsigned int		src_off;
	unsigned int		dst_off;
	unsigned int		len;
	enum dmatest_error_type	type;
	union {
		unsigned long			data;
		dma_cookie_t			cookie;
		enum dma_status			status;
		int				error;
		struct dmatest_verify_result	*vr;
	};
};

struct dmatest_result {
	struct list_head	node;
	char			*name;
	struct list_head	results;
};

struct dmatest_info;

=======
/* poor man's completion - we want to use wait_event_freezable() on it */
struct dmatest_done {
	bool			done;
	wait_queue_head_t	*wait;
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct dmatest_thread {
	struct list_head	node;
	struct dmatest_info	*info;
	struct task_struct	*task;
	struct dma_chan		*chan;
	u8			**srcs;
	u8			**dsts;
	enum dma_transaction_type type;
<<<<<<< HEAD
=======
	wait_queue_head_t done_wait;
	struct dmatest_done test_done;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bool			done;
};

struct dmatest_chan {
	struct list_head	node;
	struct dma_chan		*chan;
	struct list_head	threads;
};

<<<<<<< HEAD
/**
 * struct dmatest_params - test parameters.
 * @buf_size:		size of the memcpy test buffer
 * @channel:		bus ID of the channel to test
 * @device:		bus ID of the DMA Engine to test
 * @threads_per_chan:	number of threads to start per channel
 * @max_channels:	maximum number of channels to use
 * @iterations:		iterations before stopping test
 * @xor_sources:	number of xor source buffers
 * @pq_sources:		number of p+q source buffers
 * @timeout:		transfer timeout in msec, -1 for infinite timeout
 */
struct dmatest_params {
	unsigned int	buf_size;
	char		channel[20];
	char		device[20];
	unsigned int	threads_per_chan;
	unsigned int	max_channels;
	unsigned int	iterations;
	unsigned int	xor_sources;
	unsigned int	pq_sources;
	int		timeout;
};

/**
 * struct dmatest_info - test information.
 * @params:		test parameters
 * @lock:		access protection to the fields of this structure
 */
struct dmatest_info {
	/* Test parameters */
	struct dmatest_params	params;

	/* Internal state */
	struct list_head	channels;
	unsigned int		nr_channels;
	struct mutex		lock;

	/* debugfs related stuff */
	struct dentry		*root;
	struct dmatest_params	dbgfs_params;

	/* Test results */
	struct list_head	results;
	struct mutex		results_lock;
};

static struct dmatest_info test_info;
=======
static DECLARE_WAIT_QUEUE_HEAD(thread_wait);
static bool wait;

static bool is_threaded_test_run(struct dmatest_info *info)
{
	struct dmatest_chan *dtc;

	list_for_each_entry(dtc, &info->channels, node) {
		struct dmatest_thread *thread;

		list_for_each_entry(thread, &dtc->threads, node) {
			if (!thread->done)
				return true;
		}
	}

	return false;
}

static int dmatest_wait_get(char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;
	struct dmatest_params *params = &info->params;

	if (params->iterations)
		wait_event(thread_wait, !is_threaded_test_run(info));
	wait = true;
	return param_get_bool(val, kp);
}

static const struct kernel_param_ops wait_ops = {
	.get = dmatest_wait_get,
	.set = param_set_bool,
};
module_param_cb(wait, &wait_ops, &wait, S_IRUGO);
MODULE_PARM_DESC(wait, "Wait for tests to complete (default: false)");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static bool dmatest_match_channel(struct dmatest_params *params,
		struct dma_chan *chan)
{
	if (params->channel[0] == '\0')
		return true;
	return strcmp(dma_chan_name(chan), params->channel) == 0;
}

static bool dmatest_match_device(struct dmatest_params *params,
		struct dma_device *device)
{
	if (params->device[0] == '\0')
		return true;
	return strcmp(dev_name(device->dev), params->device) == 0;
}

static unsigned long dmatest_random(void)
{
	unsigned long buf;

<<<<<<< HEAD
	get_random_bytes(&buf, sizeof(buf));
=======
	prandom_bytes(&buf, sizeof(buf));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return buf;
}

static void dmatest_init_srcs(u8 **bufs, unsigned int start, unsigned int len,
		unsigned int buf_size)
{
	unsigned int i;
	u8 *buf;

	for (; (buf = *bufs); bufs++) {
		for (i = 0; i < start; i++)
			buf[i] = PATTERN_SRC | (~i & PATTERN_COUNT_MASK);
		for ( ; i < start + len; i++)
			buf[i] = PATTERN_SRC | PATTERN_COPY
				| (~i & PATTERN_COUNT_MASK);
		for ( ; i < buf_size; i++)
			buf[i] = PATTERN_SRC | (~i & PATTERN_COUNT_MASK);
		buf++;
	}
}

static void dmatest_init_dsts(u8 **bufs, unsigned int start, unsigned int len,
		unsigned int buf_size)
{
	unsigned int i;
	u8 *buf;

	for (; (buf = *bufs); bufs++) {
		for (i = 0; i < start; i++)
			buf[i] = PATTERN_DST | (~i & PATTERN_COUNT_MASK);
		for ( ; i < start + len; i++)
			buf[i] = PATTERN_DST | PATTERN_OVERWRITE
				| (~i & PATTERN_COUNT_MASK);
		for ( ; i < buf_size; i++)
			buf[i] = PATTERN_DST | (~i & PATTERN_COUNT_MASK);
	}
}

<<<<<<< HEAD
static unsigned int dmatest_verify(struct dmatest_verify_result *vr, u8 **bufs,
		unsigned int start, unsigned int end, unsigned int counter,
		u8 pattern, bool is_srcbuf)
=======
static void dmatest_mismatch(u8 actual, u8 pattern, unsigned int index,
		unsigned int counter, bool is_srcbuf)
{
	u8		diff = actual ^ pattern;
	u8		expected = pattern | (~counter & PATTERN_COUNT_MASK);
	const char	*thread_name = current->comm;

	if (is_srcbuf)
		pr_warn("%s: srcbuf[0x%x] overwritten! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
	else if ((pattern & PATTERN_COPY)
			&& (diff & (PATTERN_COPY | PATTERN_OVERWRITE)))
		pr_warn("%s: dstbuf[0x%x] not copied! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
	else if (diff & PATTERN_SRC)
		pr_warn("%s: dstbuf[0x%x] was copied! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
	else
		pr_warn("%s: dstbuf[0x%x] mismatch! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
}

static unsigned int dmatest_verify(u8 **bufs, unsigned int start,
		unsigned int end, unsigned int counter, u8 pattern,
		bool is_srcbuf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned int i;
	unsigned int error_count = 0;
	u8 actual;
	u8 expected;
	u8 *buf;
	unsigned int counter_orig = counter;
<<<<<<< HEAD
	struct dmatest_verify_buffer *vb;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (; (buf = *bufs); bufs++) {
		counter = counter_orig;
		for (i = start; i < end; i++) {
			actual = buf[i];
			expected = pattern | (~counter & PATTERN_COUNT_MASK);
			if (actual != expected) {
<<<<<<< HEAD
				if (error_count < MAX_ERROR_COUNT && vr) {
					vb = &vr->data[error_count];
					vb->index = i;
					vb->expected = expected;
					vb->actual = actual;
				}
=======
				if (error_count < MAX_ERROR_COUNT)
					dmatest_mismatch(actual, pattern, i,
							 counter, is_srcbuf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				error_count++;
			}
			counter++;
		}
	}

	if (error_count > MAX_ERROR_COUNT)
<<<<<<< HEAD
		pr_warning("%s: %u errors suppressed\n",
=======
		pr_warn("%s: %u errors suppressed\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			current->comm, error_count - MAX_ERROR_COUNT);

	return error_count;
}

<<<<<<< HEAD
/* poor man's completion - we want to use wait_event_freezable() on it */
struct dmatest_done {
	bool			done;
	wait_queue_head_t	*wait;
};
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void dmatest_callback(void *arg)
{
	struct dmatest_done *done = arg;
<<<<<<< HEAD

	done->done = true;
	wake_up_all(done->wait);
}

static inline void unmap_src(struct device *dev, dma_addr_t *addr, size_t len,
			     unsigned int count)
{
	while (count--)
		dma_unmap_single(dev, addr[count], len, DMA_TO_DEVICE);
}

static inline void unmap_dst(struct device *dev, dma_addr_t *addr, size_t len,
			     unsigned int count)
{
	while (count--)
		dma_unmap_single(dev, addr[count], len, DMA_BIDIRECTIONAL);
=======
	struct dmatest_thread *thread =
		container_of(done, struct dmatest_thread, test_done);
	if (!thread->done) {
		done->done = true;
		wake_up_all(done->wait);
	} else {
		/*
		 * If thread->done, it means that this callback occurred
		 * after the parent thread has cleaned up. This can
		 * happen in the case that driver doesn't implement
		 * the terminate_all() functionality and a dma operation
		 * did not occur within the timeout period
		 */
		WARN(1, "dmatest: Kernel memory may be corrupted!!\n");
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static unsigned int min_odd(unsigned int x, unsigned int y)
{
	unsigned int val = min(x, y);

	return val % 2 ? val : val - 1;
}

<<<<<<< HEAD
static char *verify_result_get_one(struct dmatest_verify_result *vr,
		unsigned int i)
{
	struct dmatest_verify_buffer *vb = &vr->data[i];
	u8 diff = vb->actual ^ vr->pattern;
	static char buf[512];
	char *msg;

	if (vr->is_srcbuf)
		msg = "srcbuf overwritten!";
	else if ((vr->pattern & PATTERN_COPY)
			&& (diff & (PATTERN_COPY | PATTERN_OVERWRITE)))
		msg = "dstbuf not copied!";
	else if (diff & PATTERN_SRC)
		msg = "dstbuf was copied!";
	else
		msg = "dstbuf mismatch!";

	snprintf(buf, sizeof(buf) - 1, "%s [0x%x] Expected %02x, got %02x", msg,
		 vb->index, vb->expected, vb->actual);

	return buf;
}

static char *thread_result_get(const char *name,
		struct dmatest_thread_result *tr)
{
	static const char * const messages[] = {
		[DMATEST_ET_OK]			= "No errors",
		[DMATEST_ET_MAP_SRC]		= "src mapping error",
		[DMATEST_ET_MAP_DST]		= "dst mapping error",
		[DMATEST_ET_PREP]		= "prep error",
		[DMATEST_ET_SUBMIT]		= "submit error",
		[DMATEST_ET_TIMEOUT]		= "test timed out",
		[DMATEST_ET_DMA_ERROR]		=
			"got completion callback (DMA_ERROR)",
		[DMATEST_ET_DMA_IN_PROGRESS]	=
			"got completion callback (DMA_IN_PROGRESS)",
		[DMATEST_ET_VERIFY]		= "errors",
		[DMATEST_ET_VERIFY_BUF]		= "verify errors",
	};
	static char buf[512];

	snprintf(buf, sizeof(buf) - 1,
		 "%s: #%u: %s with src_off=0x%x ""dst_off=0x%x len=0x%x (%lu)",
		 name, tr->n, messages[tr->type], tr->src_off, tr->dst_off,
		 tr->len, tr->data);

	return buf;
}

static int thread_result_add(struct dmatest_info *info,
		struct dmatest_result *r, enum dmatest_error_type type,
		unsigned int n, unsigned int src_off, unsigned int dst_off,
		unsigned int len, unsigned long data)
{
	struct dmatest_thread_result *tr;

	tr = kzalloc(sizeof(*tr), GFP_KERNEL);
	if (!tr)
		return -ENOMEM;

	tr->type = type;
	tr->n = n;
	tr->src_off = src_off;
	tr->dst_off = dst_off;
	tr->len = len;
	tr->data = data;

	mutex_lock(&info->results_lock);
	list_add_tail(&tr->node, &r->results);
	mutex_unlock(&info->results_lock);

	pr_warn("%s\n", thread_result_get(r->name, tr));
	return 0;
}

static unsigned int verify_result_add(struct dmatest_info *info,
		struct dmatest_result *r, unsigned int n,
		unsigned int src_off, unsigned int dst_off, unsigned int len,
		u8 **bufs, int whence, unsigned int counter, u8 pattern,
		bool is_srcbuf)
{
	struct dmatest_verify_result *vr;
	unsigned int error_count;
	unsigned int buf_off = is_srcbuf ? src_off : dst_off;
	unsigned int start, end;

	if (whence < 0) {
		start = 0;
		end = buf_off;
	} else if (whence > 0) {
		start = buf_off + len;
		end = info->params.buf_size;
	} else {
		start = buf_off;
		end = buf_off + len;
	}

	vr = kmalloc(sizeof(*vr), GFP_KERNEL);
	if (!vr) {
		pr_warn("dmatest: No memory to store verify result\n");
		return dmatest_verify(NULL, bufs, start, end, counter, pattern,
				      is_srcbuf);
	}

	vr->pattern = pattern;
	vr->is_srcbuf = is_srcbuf;

	error_count = dmatest_verify(vr, bufs, start, end, counter, pattern,
				     is_srcbuf);
	if (error_count) {
		vr->error_count = error_count;
		thread_result_add(info, r, DMATEST_ET_VERIFY_BUF, n, src_off,
				  dst_off, len, (unsigned long)vr);
		return error_count;
	}

	kfree(vr);
	return 0;
}

static void result_free(struct dmatest_info *info, const char *name)
{
	struct dmatest_result *r, *_r;

	mutex_lock(&info->results_lock);
	list_for_each_entry_safe(r, _r, &info->results, node) {
		struct dmatest_thread_result *tr, *_tr;

		if (name && strcmp(r->name, name))
			continue;

		list_for_each_entry_safe(tr, _tr, &r->results, node) {
			if (tr->type == DMATEST_ET_VERIFY_BUF)
				kfree(tr->vr);
			list_del(&tr->node);
			kfree(tr);
		}

		kfree(r->name);
		list_del(&r->node);
		kfree(r);
	}

	mutex_unlock(&info->results_lock);
}

static struct dmatest_result *result_init(struct dmatest_info *info,
		const char *name)
{
	struct dmatest_result *r;

	r = kzalloc(sizeof(*r), GFP_KERNEL);
	if (r) {
		r->name = kstrdup(name, GFP_KERNEL);
		INIT_LIST_HEAD(&r->results);
		mutex_lock(&info->results_lock);
		list_add_tail(&r->node, &info->results);
		mutex_unlock(&info->results_lock);
	}
	return r;
=======
static void result(const char *err, unsigned int n, unsigned int src_off,
		   unsigned int dst_off, unsigned int len, unsigned long data)
{
	pr_info("%s: result #%u: '%s' with src_off=0x%x dst_off=0x%x len=0x%x (%lu)\n",
		current->comm, n, err, src_off, dst_off, len, data);
}

static void dbg_result(const char *err, unsigned int n, unsigned int src_off,
		       unsigned int dst_off, unsigned int len,
		       unsigned long data)
{
	pr_debug("%s: result #%u: '%s' with src_off=0x%x dst_off=0x%x len=0x%x (%lu)\n",
		 current->comm, n, err, src_off, dst_off, len, data);
}

#define verbose_result(err, n, src_off, dst_off, len, data) ({	\
	if (verbose)						\
		result(err, n, src_off, dst_off, len, data);	\
	else							\
		dbg_result(err, n, src_off, dst_off, len, data);\
})

static unsigned long long dmatest_persec(s64 runtime, unsigned int val)
{
	unsigned long long per_sec = 1000000;

	if (runtime <= 0)
		return 0;

	/* drop precision until runtime is 32-bits */
	while (runtime > UINT_MAX) {
		runtime >>= 1;
		per_sec <<= 1;
	}

	per_sec *= val;
	do_div(per_sec, runtime);
	return per_sec;
}

static unsigned long long dmatest_KBs(s64 runtime, unsigned long long len)
{
	return dmatest_persec(runtime, len >> 10);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * This function repeatedly tests DMA transfers of various lengths and
 * offsets for a given operation type until it is told to exit by
 * kthread_stop(). There may be multiple threads running this function
 * in parallel for a single channel, and there may be multiple channels
 * being tested in parallel.
 *
 * Before each test, the source and destination buffer is initialized
 * with a known pattern. This pattern is different depending on
 * whether it's in an area which is supposed to be copied or
 * overwritten, and different in the source and destination buffers.
 * So if the DMA engine doesn't copy exactly what we tell it to copy,
 * we'll notice.
 */
static int dmatest_func(void *data)
{
<<<<<<< HEAD
	DECLARE_WAIT_QUEUE_HEAD_ONSTACK(done_wait);
	struct dmatest_thread	*thread = data;
	struct dmatest_done	done = { .wait = &done_wait };
=======
	struct dmatest_thread	*thread = data;
	struct dmatest_done	*done = &thread->test_done;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct dmatest_info	*info;
	struct dmatest_params	*params;
	struct dma_chan		*chan;
	struct dma_device	*dev;
<<<<<<< HEAD
	const char		*thread_name;
	unsigned int		src_off, dst_off, len;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int		error_count;
	unsigned int		failed_tests = 0;
	unsigned int		total_tests = 0;
	dma_cookie_t		cookie;
	enum dma_status		status;
	enum dma_ctrl_flags 	flags;
	u8			*pq_coefs = NULL;
	int			ret;
	int			src_cnt;
	int			dst_cnt;
	int			i;
<<<<<<< HEAD
	struct dmatest_result	*result;

	thread_name = current->comm;
=======
	ktime_t			ktime, start, diff;
	ktime_t			filltime = ktime_set(0, 0);
	ktime_t			comparetime = ktime_set(0, 0);
	s64			runtime = 0;
	unsigned long long	total_len = 0;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	set_freezable();

	ret = -ENOMEM;

	smp_rmb();
	info = thread->info;
	params = &info->params;
	chan = thread->chan;
	dev = chan->device;
	if (thread->type == DMA_MEMCPY)
		src_cnt = dst_cnt = 1;
<<<<<<< HEAD
=======
	else if (thread->type == DMA_SG)
		src_cnt = dst_cnt = sg_buffers;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	else if (thread->type == DMA_XOR) {
		/* force odd to ensure dst = src */
		src_cnt = min_odd(params->xor_sources | 1, dev->max_xor);
		dst_cnt = 1;
	} else if (thread->type == DMA_PQ) {
		/* force odd to ensure dst = src */
		src_cnt = min_odd(params->pq_sources | 1, dma_maxpq(dev, 0));
		dst_cnt = 2;

		pq_coefs = kmalloc(params->pq_sources+1, GFP_KERNEL);
		if (!pq_coefs)
			goto err_thread_type;

		for (i = 0; i < src_cnt; i++)
			pq_coefs[i] = 1;
	} else
		goto err_thread_type;

<<<<<<< HEAD
	result = result_init(info, thread_name);
	if (!result)
		goto err_srcs;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	thread->srcs = kcalloc(src_cnt+1, sizeof(u8 *), GFP_KERNEL);
	if (!thread->srcs)
		goto err_srcs;
	for (i = 0; i < src_cnt; i++) {
		thread->srcs[i] = kmalloc(params->buf_size, GFP_KERNEL);
		if (!thread->srcs[i])
			goto err_srcbuf;
	}
	thread->srcs[i] = NULL;

	thread->dsts = kcalloc(dst_cnt+1, sizeof(u8 *), GFP_KERNEL);
	if (!thread->dsts)
		goto err_dsts;
	for (i = 0; i < dst_cnt; i++) {
		thread->dsts[i] = kmalloc(params->buf_size, GFP_KERNEL);
		if (!thread->dsts[i])
			goto err_dstbuf;
	}
	thread->dsts[i] = NULL;

	set_user_nice(current, 10);

	/*
<<<<<<< HEAD
	 * src buffers are freed by the DMAEngine code with dma_unmap_single()
	 * dst buffers are freed by ourselves below
	 */
	flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT
	      | DMA_COMPL_SKIP_DEST_UNMAP | DMA_COMPL_SRC_UNMAP_SINGLE;

	while (!kthread_should_stop()
	       && !(params->iterations && total_tests >= params->iterations)) {
		struct dma_async_tx_descriptor *tx = NULL;
		dma_addr_t dma_srcs[src_cnt];
		dma_addr_t dma_dsts[dst_cnt];
		u8 align = 0;
=======
	 * src and dst buffers are freed by ourselves below
	 */
	flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT;

	ktime = ktime_get();
	while (!(kthread_should_stop() ||
	       (params->iterations && total_tests >= params->iterations))) {
		struct dma_async_tx_descriptor *tx = NULL;
		struct dmaengine_unmap_data *um;
		dma_addr_t srcs[src_cnt];
		dma_addr_t *dsts;
		unsigned int src_off, dst_off, len;
		u8 align = 0;
		struct scatterlist tx_sg[src_cnt];
		struct scatterlist rx_sg[src_cnt];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		total_tests++;

		/* honor alignment restrictions */
<<<<<<< HEAD
		if (thread->type == DMA_MEMCPY)
=======
		if (thread->type == DMA_MEMCPY || thread->type == DMA_SG)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			align = dev->copy_align;
		else if (thread->type == DMA_XOR)
			align = dev->xor_align;
		else if (thread->type == DMA_PQ)
			align = dev->pq_align;

		if (1 << align > params->buf_size) {
			pr_err("%u-byte buffer too small for %d-byte alignment\n",
			       params->buf_size, 1 << align);
			break;
		}

<<<<<<< HEAD
		len = dmatest_random() % params->buf_size + 1;
		len = (len >> align) << align;
		if (!len)
			len = 1 << align;
		src_off = dmatest_random() % (params->buf_size - len + 1);
		dst_off = dmatest_random() % (params->buf_size - len + 1);

		src_off = (src_off >> align) << align;
		dst_off = (dst_off >> align) << align;

		dmatest_init_srcs(thread->srcs, src_off, len, params->buf_size);
		dmatest_init_dsts(thread->dsts, dst_off, len, params->buf_size);

		for (i = 0; i < src_cnt; i++) {
			u8 *buf = thread->srcs[i] + src_off;

			dma_srcs[i] = dma_map_single(dev->dev, buf, len,
						     DMA_TO_DEVICE);
			ret = dma_mapping_error(dev->dev, dma_srcs[i]);
			if (ret) {
				unmap_src(dev->dev, dma_srcs, len, i);
				thread_result_add(info, result,
						  DMATEST_ET_MAP_SRC,
						  total_tests, src_off, dst_off,
						  len, ret);
				failed_tests++;
				continue;
			}
		}
		/* map with DMA_BIDIRECTIONAL to force writeback/invalidate */
		for (i = 0; i < dst_cnt; i++) {
			dma_dsts[i] = dma_map_single(dev->dev, thread->dsts[i],
						     params->buf_size,
						     DMA_BIDIRECTIONAL);
			ret = dma_mapping_error(dev->dev, dma_dsts[i]);
			if (ret) {
				unmap_src(dev->dev, dma_srcs, len, src_cnt);
				unmap_dst(dev->dev, dma_dsts, params->buf_size,
					  i);
				thread_result_add(info, result,
						  DMATEST_ET_MAP_DST,
						  total_tests, src_off, dst_off,
						  len, ret);
				failed_tests++;
				continue;
			}
=======
		if (params->noverify)
			len = params->buf_size;
		else
			len = dmatest_random() % params->buf_size + 1;

		len = (len >> align) << align;
		if (!len)
			len = 1 << align;

		total_len += len;

		if (params->noverify) {
			src_off = 0;
			dst_off = 0;
		} else {
			start = ktime_get();
			src_off = dmatest_random() % (params->buf_size - len + 1);
			dst_off = dmatest_random() % (params->buf_size - len + 1);

			src_off = (src_off >> align) << align;
			dst_off = (dst_off >> align) << align;

			dmatest_init_srcs(thread->srcs, src_off, len,
					  params->buf_size);
			dmatest_init_dsts(thread->dsts, dst_off, len,
					  params->buf_size);

			diff = ktime_sub(ktime_get(), start);
			filltime = ktime_add(filltime, diff);
		}

		um = dmaengine_get_unmap_data(dev->dev, src_cnt+dst_cnt,
					      GFP_KERNEL);
		if (!um) {
			failed_tests++;
			result("unmap data NULL", total_tests,
			       src_off, dst_off, len, ret);
			continue;
		}

		um->len = params->buf_size;
		for (i = 0; i < src_cnt; i++) {
			void *buf = thread->srcs[i];
			struct page *pg = virt_to_page(buf);
			unsigned pg_off = (unsigned long) buf & ~PAGE_MASK;

			um->addr[i] = dma_map_page(dev->dev, pg, pg_off,
						   um->len, DMA_TO_DEVICE);
			srcs[i] = um->addr[i] + src_off;
			ret = dma_mapping_error(dev->dev, um->addr[i]);
			if (ret) {
				result("src mapping error", total_tests,
				       src_off, dst_off, len, ret);
				goto error_unmap_continue;
			}
			um->to_cnt++;
		}
		/* map with DMA_BIDIRECTIONAL to force writeback/invalidate */
		dsts = &um->addr[src_cnt];
		for (i = 0; i < dst_cnt; i++) {
			void *buf = thread->dsts[i];
			struct page *pg = virt_to_page(buf);
			unsigned pg_off = (unsigned long) buf & ~PAGE_MASK;

			dsts[i] = dma_map_page(dev->dev, pg, pg_off, um->len,
					       DMA_BIDIRECTIONAL);
			ret = dma_mapping_error(dev->dev, dsts[i]);
			if (ret) {
				result("dst mapping error", total_tests,
				       src_off, dst_off, len, ret);
				goto error_unmap_continue;
			}
			um->bidi_cnt++;
		}

		sg_init_table(tx_sg, src_cnt);
		sg_init_table(rx_sg, src_cnt);
		for (i = 0; i < src_cnt; i++) {
			sg_dma_address(&rx_sg[i]) = srcs[i];
			sg_dma_address(&tx_sg[i]) = dsts[i] + dst_off;
			sg_dma_len(&tx_sg[i]) = len;
			sg_dma_len(&rx_sg[i]) = len;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		if (thread->type == DMA_MEMCPY)
			tx = dev->device_prep_dma_memcpy(chan,
<<<<<<< HEAD
							 dma_dsts[0] + dst_off,
							 dma_srcs[0], len,
							 flags);
		else if (thread->type == DMA_XOR)
			tx = dev->device_prep_dma_xor(chan,
						      dma_dsts[0] + dst_off,
						      dma_srcs, src_cnt,
=======
							 dsts[0] + dst_off,
							 srcs[0], len, flags);
		else if (thread->type == DMA_SG)
			tx = dev->device_prep_dma_sg(chan, tx_sg, src_cnt,
						     rx_sg, src_cnt, flags);
		else if (thread->type == DMA_XOR)
			tx = dev->device_prep_dma_xor(chan,
						      dsts[0] + dst_off,
						      srcs, src_cnt,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						      len, flags);
		else if (thread->type == DMA_PQ) {
			dma_addr_t dma_pq[dst_cnt];

			for (i = 0; i < dst_cnt; i++)
<<<<<<< HEAD
				dma_pq[i] = dma_dsts[i] + dst_off;
			tx = dev->device_prep_dma_pq(chan, dma_pq, dma_srcs,
=======
				dma_pq[i] = dsts[i] + dst_off;
			tx = dev->device_prep_dma_pq(chan, dma_pq, srcs,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						     src_cnt, pq_coefs,
						     len, flags);
		}

		if (!tx) {
<<<<<<< HEAD
			unmap_src(dev->dev, dma_srcs, len, src_cnt);
			unmap_dst(dev->dev, dma_dsts, params->buf_size,
				  dst_cnt);
			thread_result_add(info, result, DMATEST_ET_PREP,
					  total_tests, src_off, dst_off,
					  len, 0);
			msleep(100);
			failed_tests++;
			continue;
		}

		done.done = false;
		tx->callback = dmatest_callback;
		tx->callback_param = &done;
		cookie = tx->tx_submit(tx);

		if (dma_submit_error(cookie)) {
			thread_result_add(info, result, DMATEST_ET_SUBMIT,
					  total_tests, src_off, dst_off,
					  len, cookie);
			msleep(100);
			failed_tests++;
			continue;
		}
		dma_async_issue_pending(chan);

		wait_event_freezable_timeout(done_wait, done.done,
=======
			result("prep error", total_tests, src_off,
			       dst_off, len, ret);
			msleep(100);
			goto error_unmap_continue;
		}

		done->done = false;
		tx->callback = dmatest_callback;
		tx->callback_param = done;
		cookie = tx->tx_submit(tx);

		if (dma_submit_error(cookie)) {
			result("submit error", total_tests, src_off,
			       dst_off, len, ret);
			msleep(100);
			goto error_unmap_continue;
		}
		dma_async_issue_pending(chan);

		wait_event_freezable_timeout(thread->done_wait, done->done,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					     msecs_to_jiffies(params->timeout));

		status = dma_async_is_tx_complete(chan, cookie, NULL, NULL);

<<<<<<< HEAD
		if (!done.done) {
			/*
			 * We're leaving the timed out dma operation with
			 * dangling pointer to done_wait.  To make this
			 * correct, we'll need to allocate wait_done for
			 * each test iteration and perform "who's gonna
			 * free it this time?" dancing.  For now, just
			 * leave it dangling.
			 */
			thread_result_add(info, result, DMATEST_ET_TIMEOUT,
					  total_tests, src_off, dst_off,
					  len, 0);
			failed_tests++;
			continue;
		} else if (status != DMA_SUCCESS) {
			enum dmatest_error_type type = (status == DMA_ERROR) ?
				DMATEST_ET_DMA_ERROR : DMATEST_ET_DMA_IN_PROGRESS;
			thread_result_add(info, result, type,
					  total_tests, src_off, dst_off,
					  len, status);
			failed_tests++;
			continue;
		}

		/* Unmap by myself (see DMA_COMPL_SKIP_DEST_UNMAP above) */
		unmap_dst(dev->dev, dma_dsts, params->buf_size, dst_cnt);

		error_count = 0;

		pr_debug("%s: verifying source buffer...\n", thread_name);
		error_count += verify_result_add(info, result, total_tests,
				src_off, dst_off, len, thread->srcs, -1,
				0, PATTERN_SRC, true);
		error_count += verify_result_add(info, result, total_tests,
				src_off, dst_off, len, thread->srcs, 0,
				src_off, PATTERN_SRC | PATTERN_COPY, true);
		error_count += verify_result_add(info, result, total_tests,
				src_off, dst_off, len, thread->srcs, 1,
				src_off + len, PATTERN_SRC, true);

		pr_debug("%s: verifying dest buffer...\n", thread_name);
		error_count += verify_result_add(info, result, total_tests,
				src_off, dst_off, len, thread->dsts, -1,
				0, PATTERN_DST, false);
		error_count += verify_result_add(info, result, total_tests,
				src_off, dst_off, len, thread->dsts, 0,
				src_off, PATTERN_SRC | PATTERN_COPY, false);
		error_count += verify_result_add(info, result, total_tests,
				src_off, dst_off, len, thread->dsts, 1,
				dst_off + len, PATTERN_DST, false);

		if (error_count) {
			thread_result_add(info, result, DMATEST_ET_VERIFY,
					  total_tests, src_off, dst_off,
					  len, error_count);
			failed_tests++;
		} else {
			thread_result_add(info, result, DMATEST_ET_OK,
					  total_tests, src_off, dst_off,
					  len, 0);
		}
	}

	ret = 0;
	for (i = 0; thread->dsts[i]; i++)
		kfree(thread->dsts[i]);
err_dstbuf:
	kfree(thread->dsts);
err_dsts:
	for (i = 0; thread->srcs[i]; i++)
		kfree(thread->srcs[i]);
err_srcbuf:
=======
		if (!done->done) {
			dmaengine_unmap_put(um);
			result("test timed out", total_tests, src_off, dst_off,
			       len, 0);
			goto error_unmap_continue;
		} else if (status != DMA_COMPLETE) {
			dmaengine_unmap_put(um);
			result(status == DMA_ERROR ?
			       "completion error status" :
			       "completion busy status", total_tests, src_off,
			       dst_off, len, ret);
			goto error_unmap_continue;
		}

		dmaengine_unmap_put(um);

		if (params->noverify) {
			verbose_result("test passed", total_tests, src_off,
				       dst_off, len, 0);
			continue;
		}

		start = ktime_get();
		pr_debug("%s: verifying source buffer...\n", current->comm);
		error_count = dmatest_verify(thread->srcs, 0, src_off,
				0, PATTERN_SRC, true);
		error_count += dmatest_verify(thread->srcs, src_off,
				src_off + len, src_off,
				PATTERN_SRC | PATTERN_COPY, true);
		error_count += dmatest_verify(thread->srcs, src_off + len,
				params->buf_size, src_off + len,
				PATTERN_SRC, true);

		pr_debug("%s: verifying dest buffer...\n", current->comm);
		error_count += dmatest_verify(thread->dsts, 0, dst_off,
				0, PATTERN_DST, false);
		error_count += dmatest_verify(thread->dsts, dst_off,
				dst_off + len, src_off,
				PATTERN_SRC | PATTERN_COPY, false);
		error_count += dmatest_verify(thread->dsts, dst_off + len,
				params->buf_size, dst_off + len,
				PATTERN_DST, false);

		diff = ktime_sub(ktime_get(), start);
		comparetime = ktime_add(comparetime, diff);

		if (error_count) {
			result("data error", total_tests, src_off, dst_off,
			       len, error_count);
			failed_tests++;
		} else {
			verbose_result("test passed", total_tests, src_off,
				       dst_off, len, 0);
		}

		continue;

error_unmap_continue:
		dmaengine_unmap_put(um);
		failed_tests++;
	}
	ktime = ktime_sub(ktime_get(), ktime);
	ktime = ktime_sub(ktime, comparetime);
	ktime = ktime_sub(ktime, filltime);
	runtime = ktime_to_us(ktime);

	ret = 0;
err_dstbuf:
	for (i = 0; thread->dsts[i]; i++)
		kfree(thread->dsts[i]);
	kfree(thread->dsts);
err_dsts:
err_srcbuf:
	for (i = 0; thread->srcs[i]; i++)
		kfree(thread->srcs[i]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(thread->srcs);
err_srcs:
	kfree(pq_coefs);
err_thread_type:
<<<<<<< HEAD
	pr_notice("%s: terminating after %u tests, %u failures (status %d)\n",
			thread_name, total_tests, failed_tests, ret);

	/* terminate all transfers on specified channels */
	if (ret)
		dmaengine_terminate_all(chan);

	thread->done = true;

	if (params->iterations > 0)
		while (!kthread_should_stop()) {
			DECLARE_WAIT_QUEUE_HEAD_ONSTACK(wait_dmatest_exit);
			interruptible_sleep_on(&wait_dmatest_exit);
		}
=======
	pr_info("%s: summary %u tests, %u failures %llu iops %llu KB/s (%d)\n",
		current->comm, total_tests, failed_tests,
		dmatest_persec(runtime, total_tests),
		dmatest_KBs(runtime, total_len), ret);

	/* terminate all transfers on specified channels */
	if (ret || failed_tests)
		dmaengine_terminate_all(chan);

	thread->done = true;
	wake_up(&thread_wait);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ret;
}

static void dmatest_cleanup_channel(struct dmatest_chan *dtc)
{
	struct dmatest_thread	*thread;
	struct dmatest_thread	*_thread;
	int			ret;

	list_for_each_entry_safe(thread, _thread, &dtc->threads, node) {
		ret = kthread_stop(thread->task);
<<<<<<< HEAD
		pr_debug("dmatest: thread %s exited with status %d\n",
				thread->task->comm, ret);
		list_del(&thread->node);
=======
		pr_debug("thread %s exited with status %d\n",
			 thread->task->comm, ret);
		list_del(&thread->node);
		put_task_struct(thread->task);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		kfree(thread);
	}

	/* terminate all transfers on specified channels */
	dmaengine_terminate_all(dtc->chan);

	kfree(dtc);
}

static int dmatest_add_threads(struct dmatest_info *info,
		struct dmatest_chan *dtc, enum dma_transaction_type type)
{
	struct dmatest_params *params = &info->params;
	struct dmatest_thread *thread;
	struct dma_chan *chan = dtc->chan;
	char *op;
	unsigned int i;

	if (type == DMA_MEMCPY)
		op = "copy";
<<<<<<< HEAD
=======
	else if (type == DMA_SG)
		op = "sg";
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	else if (type == DMA_XOR)
		op = "xor";
	else if (type == DMA_PQ)
		op = "pq";
	else
		return -EINVAL;

	for (i = 0; i < params->threads_per_chan; i++) {
		thread = kzalloc(sizeof(struct dmatest_thread), GFP_KERNEL);
		if (!thread) {
<<<<<<< HEAD
			pr_warning("dmatest: No memory for %s-%s%u\n",
				   dma_chan_name(chan), op, i);

=======
			pr_warn("No memory for %s-%s%u\n",
				dma_chan_name(chan), op, i);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		}
		thread->info = info;
		thread->chan = dtc->chan;
		thread->type = type;
<<<<<<< HEAD
		smp_wmb();
		thread->task = kthread_run(dmatest_func, thread, "%s-%s%u",
				dma_chan_name(chan), op, i);
		if (IS_ERR(thread->task)) {
			pr_warning("dmatest: Failed to run thread %s-%s%u\n",
					dma_chan_name(chan), op, i);
=======
		thread->test_done.wait = &thread->done_wait;
		init_waitqueue_head(&thread->done_wait);
		smp_wmb();
		thread->task = kthread_create(dmatest_func, thread, "%s-%s%u",
				dma_chan_name(chan), op, i);
		if (IS_ERR(thread->task)) {
			pr_warn("Failed to create thread %s-%s%u\n",
				dma_chan_name(chan), op, i);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			kfree(thread);
			break;
		}

		/* srcbuf and dstbuf are allocated by the thread itself */
<<<<<<< HEAD

		list_add_tail(&thread->node, &dtc->threads);
=======
		get_task_struct(thread->task);
		list_add_tail(&thread->node, &dtc->threads);
		wake_up_process(thread->task);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return i;
}

static int dmatest_add_channel(struct dmatest_info *info,
		struct dma_chan *chan)
{
	struct dmatest_chan	*dtc;
	struct dma_device	*dma_dev = chan->device;
	unsigned int		thread_count = 0;
	int cnt;

	dtc = kmalloc(sizeof(struct dmatest_chan), GFP_KERNEL);
	if (!dtc) {
<<<<<<< HEAD
		pr_warning("dmatest: No memory for %s\n", dma_chan_name(chan));
=======
		pr_warn("No memory for %s\n", dma_chan_name(chan));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -ENOMEM;
	}

	dtc->chan = chan;
	INIT_LIST_HEAD(&dtc->threads);

	if (dma_has_cap(DMA_MEMCPY, dma_dev->cap_mask)) {
<<<<<<< HEAD
		cnt = dmatest_add_threads(info, dtc, DMA_MEMCPY);
		thread_count += cnt > 0 ? cnt : 0;
	}
=======
		if (dmatest == 0) {
			cnt = dmatest_add_threads(info, dtc, DMA_MEMCPY);
			thread_count += cnt > 0 ? cnt : 0;
		}
	}

	if (dma_has_cap(DMA_SG, dma_dev->cap_mask)) {
		if (dmatest == 1) {
			cnt = dmatest_add_threads(info, dtc, DMA_SG);
			thread_count += cnt > 0 ? cnt : 0;
		}
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (dma_has_cap(DMA_XOR, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(info, dtc, DMA_XOR);
		thread_count += cnt > 0 ? cnt : 0;
	}
	if (dma_has_cap(DMA_PQ, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(info, dtc, DMA_PQ);
		thread_count += cnt > 0 ? cnt : 0;
	}

<<<<<<< HEAD
	pr_info("dmatest: Started %u threads using %s\n",
=======
	pr_info("Started %u threads using %s\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		thread_count, dma_chan_name(chan));

	list_add_tail(&dtc->node, &info->channels);
	info->nr_channels++;

	return 0;
}

static bool filter(struct dma_chan *chan, void *param)
{
	struct dmatest_params *params = param;

	if (!dmatest_match_channel(params, chan) ||
	    !dmatest_match_device(params, chan->device))
		return false;
	else
		return true;
}

<<<<<<< HEAD
static int __run_threaded_test(struct dmatest_info *info)
{
	dma_cap_mask_t mask;
	struct dma_chan *chan;
	struct dmatest_params *params = &info->params;
	int err = 0;

	dma_cap_zero(mask);
	dma_cap_set(DMA_MEMCPY, mask);
	for (;;) {
		chan = dma_request_channel(mask, filter, params);
		if (chan) {
			err = dmatest_add_channel(info, chan);
			if (err) {
=======
static void request_channels(struct dmatest_info *info,
			     enum dma_transaction_type type)
{
	dma_cap_mask_t mask;

	dma_cap_zero(mask);
	dma_cap_set(type, mask);
	for (;;) {
		struct dmatest_params *params = &info->params;
		struct dma_chan *chan;

		chan = dma_request_channel(mask, filter, params);
		if (chan) {
			if (dmatest_add_channel(info, chan)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				dma_release_channel(chan);
				break; /* add_channel failed, punt */
			}
		} else
			break; /* no more channels available */
		if (params->max_channels &&
		    info->nr_channels >= params->max_channels)
			break; /* we have all we need */
	}
<<<<<<< HEAD
	return err;
}

#ifndef MODULE
static int run_threaded_test(struct dmatest_info *info)
{
	int ret;

	mutex_lock(&info->lock);
	ret = __run_threaded_test(info);
	mutex_unlock(&info->lock);
	return ret;
}
#endif

static void __stop_threaded_test(struct dmatest_info *info)
=======
}

static void run_threaded_test(struct dmatest_info *info)
{
	struct dmatest_params *params = &info->params;

	/* Copy test parameters */
	params->buf_size = test_buf_size;
	strlcpy(params->channel, strim(test_channel), sizeof(params->channel));
	strlcpy(params->device, strim(test_device), sizeof(params->device));
	params->threads_per_chan = threads_per_chan;
	params->max_channels = max_channels;
	params->iterations = iterations;
	params->xor_sources = xor_sources;
	params->pq_sources = pq_sources;
	params->timeout = timeout;
	params->noverify = noverify;

	request_channels(info, DMA_MEMCPY);
	request_channels(info, DMA_XOR);
	request_channels(info, DMA_SG);
	request_channels(info, DMA_PQ);
}

static void stop_threaded_test(struct dmatest_info *info)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct dmatest_chan *dtc, *_dtc;
	struct dma_chan *chan;

	list_for_each_entry_safe(dtc, _dtc, &info->channels, node) {
		list_del(&dtc->node);
		chan = dtc->chan;
		dmatest_cleanup_channel(dtc);
<<<<<<< HEAD
		pr_debug("dmatest: dropped channel %s\n", dma_chan_name(chan));
=======
		pr_debug("dropped channel %s\n", dma_chan_name(chan));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dma_release_channel(chan);
	}

	info->nr_channels = 0;
}

<<<<<<< HEAD
static void stop_threaded_test(struct dmatest_info *info)
{
	mutex_lock(&info->lock);
	__stop_threaded_test(info);
	mutex_unlock(&info->lock);
}

static int __restart_threaded_test(struct dmatest_info *info, bool run)
{
	struct dmatest_params *params = &info->params;

	/* Stop any running test first */
	__stop_threaded_test(info);

	if (run == false)
		return 0;

	/* Clear results from previous run */
	result_free(info, NULL);

	/* Copy test parameters */
	memcpy(params, &info->dbgfs_params, sizeof(*params));

	/* Run test with new parameters */
	return __run_threaded_test(info);
}

static bool __is_threaded_test_run(struct dmatest_info *info)
{
	struct dmatest_chan *dtc;

	list_for_each_entry(dtc, &info->channels, node) {
		struct dmatest_thread *thread;

		list_for_each_entry(thread, &dtc->threads, node) {
			if (!thread->done)
				return true;
		}
	}

	return false;
}

static ssize_t dtf_write_string(void *to, size_t available, loff_t *ppos,
		const void __user *from, size_t count)
{
	char tmp[20];
	ssize_t len;

	len = simple_write_to_buffer(tmp, sizeof(tmp) - 1, ppos, from, count);
	if (len >= 0) {
		tmp[len] = '\0';
		strlcpy(to, strim(tmp), available);
	}

	return len;
}

static ssize_t dtf_read_channel(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	struct dmatest_info *info = file->private_data;
	return simple_read_from_buffer(buf, count, ppos,
			info->dbgfs_params.channel,
			strlen(info->dbgfs_params.channel));
}

static ssize_t dtf_write_channel(struct file *file, const char __user *buf,
		size_t size, loff_t *ppos)
{
	struct dmatest_info *info = file->private_data;
	return dtf_write_string(info->dbgfs_params.channel,
				sizeof(info->dbgfs_params.channel),
				ppos, buf, size);
}

static const struct file_operations dtf_channel_fops = {
	.read	= dtf_read_channel,
	.write	= dtf_write_channel,
	.open	= simple_open,
	.llseek	= default_llseek,
};

static ssize_t dtf_read_device(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	struct dmatest_info *info = file->private_data;
	return simple_read_from_buffer(buf, count, ppos,
			info->dbgfs_params.device,
			strlen(info->dbgfs_params.device));
}

static ssize_t dtf_write_device(struct file *file, const char __user *buf,
		size_t size, loff_t *ppos)
{
	struct dmatest_info *info = file->private_data;
	return dtf_write_string(info->dbgfs_params.device,
				sizeof(info->dbgfs_params.device),
				ppos, buf, size);
}

static const struct file_operations dtf_device_fops = {
	.read	= dtf_read_device,
	.write	= dtf_write_device,
	.open	= simple_open,
	.llseek	= default_llseek,
};

static ssize_t dtf_read_run(struct file *file, char __user *user_buf,
		size_t count, loff_t *ppos)
{
	struct dmatest_info *info = file->private_data;
	char buf[3];

	mutex_lock(&info->lock);

	if (__is_threaded_test_run(info)) {
		buf[0] = 'Y';
	} else {
		__stop_threaded_test(info);
		buf[0] = 'N';
	}

	mutex_unlock(&info->lock);
	buf[1] = '\n';
	buf[2] = 0x00;
	return simple_read_from_buffer(user_buf, count, ppos, buf, 2);
}

static ssize_t dtf_write_run(struct file *file, const char __user *user_buf,
		size_t count, loff_t *ppos)
{
	struct dmatest_info *info = file->private_data;
	char buf[16];
	bool bv;
	int ret = 0;

	if (copy_from_user(buf, user_buf, min(count, (sizeof(buf) - 1))))
		return -EFAULT;

	if (strtobool(buf, &bv) == 0) {
		mutex_lock(&info->lock);

		if (__is_threaded_test_run(info))
			ret = -EBUSY;
		else
			ret = __restart_threaded_test(info, bv);

		mutex_unlock(&info->lock);
	}

	return ret ? ret : count;
}

static const struct file_operations dtf_run_fops = {
	.read	= dtf_read_run,
	.write	= dtf_write_run,
	.open	= simple_open,
	.llseek	= default_llseek,
};

static int dtf_results_show(struct seq_file *sf, void *data)
{
	struct dmatest_info *info = sf->private;
	struct dmatest_result *result;
	struct dmatest_thread_result *tr;
	unsigned int i;

	mutex_lock(&info->results_lock);
	list_for_each_entry(result, &info->results, node) {
		list_for_each_entry(tr, &result->results, node) {
			seq_printf(sf, "%s\n",
				thread_result_get(result->name, tr));
			if (tr->type == DMATEST_ET_VERIFY_BUF) {
				for (i = 0; i < tr->vr->error_count; i++) {
					seq_printf(sf, "\t%s\n",
						verify_result_get_one(tr->vr, i));
				}
			}
		}
	}

	mutex_unlock(&info->results_lock);
	return 0;
}

static int dtf_results_open(struct inode *inode, struct file *file)
{
	return single_open(file, dtf_results_show, inode->i_private);
}

static const struct file_operations dtf_results_fops = {
	.open		= dtf_results_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int dmatest_register_dbgfs(struct dmatest_info *info)
{
	struct dentry *d;
	struct dmatest_params *params = &info->dbgfs_params;
	int ret = -ENOMEM;

	d = debugfs_create_dir("dmatest", NULL);
	if (IS_ERR(d))
		return PTR_ERR(d);
	if (!d)
		goto err_root;

	info->root = d;

	/* Copy initial values */
	memcpy(params, &info->params, sizeof(*params));

	/* Test parameters */

	d = debugfs_create_u32("test_buf_size", S_IWUSR | S_IRUGO, info->root,
			       (u32 *)&params->buf_size);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_file("channel", S_IRUGO | S_IWUSR, info->root,
				info, &dtf_channel_fops);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_file("device", S_IRUGO | S_IWUSR, info->root,
				info, &dtf_device_fops);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_u32("threads_per_chan", S_IWUSR | S_IRUGO, info->root,
			       (u32 *)&params->threads_per_chan);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_u32("max_channels", S_IWUSR | S_IRUGO, info->root,
			       (u32 *)&params->max_channels);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_u32("iterations", S_IWUSR | S_IRUGO, info->root,
			       (u32 *)&params->iterations);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_u32("xor_sources", S_IWUSR | S_IRUGO, info->root,
			       (u32 *)&params->xor_sources);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_u32("pq_sources", S_IWUSR | S_IRUGO, info->root,
			       (u32 *)&params->pq_sources);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_u32("timeout", S_IWUSR | S_IRUGO, info->root,
			       (u32 *)&params->timeout);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	/* Run or stop threaded test */
	d = debugfs_create_file("run", S_IWUSR | S_IRUGO, info->root,
				info, &dtf_run_fops);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	/* Results of test in progress */
	d = debugfs_create_file("results", S_IRUGO, info->root, info,
				&dtf_results_fops);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	return 0;

err_node:
	debugfs_remove_recursive(info->root);
err_root:
	pr_err("dmatest: Failed to initialize debugfs\n");
=======
static void restart_threaded_test(struct dmatest_info *info, bool run)
{
	/* we might be called early to set run=, defer running until all
	 * parameters have been evaluated
	 */
	if (!info->did_init)
		return;

	/* Stop any running test first */
	stop_threaded_test(info);

	/* Run test with new parameters */
	run_threaded_test(info);
}

static int dmatest_run_get(char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;

	mutex_lock(&info->lock);
	if (is_threaded_test_run(info)) {
		dmatest_run = true;
	} else {
		stop_threaded_test(info);
		dmatest_run = false;
	}
	mutex_unlock(&info->lock);

	return param_get_bool(val, kp);
}

static int dmatest_run_set(const char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;
	int ret;

	mutex_lock(&info->lock);
	ret = param_set_bool(val, kp);
	if (ret) {
		mutex_unlock(&info->lock);
		return ret;
	}

	if (is_threaded_test_run(info))
		ret = -EBUSY;
	else if (dmatest_run)
		restart_threaded_test(info, dmatest_run);

	mutex_unlock(&info->lock);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static int __init dmatest_init(void)
{
	struct dmatest_info *info = &test_info;
	struct dmatest_params *params = &info->params;
<<<<<<< HEAD
	int ret;

	memset(info, 0, sizeof(*info));

	mutex_init(&info->lock);
	INIT_LIST_HEAD(&info->channels);

	mutex_init(&info->results_lock);
	INIT_LIST_HEAD(&info->results);

	/* Set default parameters */
	params->buf_size = test_buf_size;
	strlcpy(params->channel, test_channel, sizeof(params->channel));
	strlcpy(params->device, test_device, sizeof(params->device));
	params->threads_per_chan = threads_per_chan;
	params->max_channels = max_channels;
	params->iterations = iterations;
	params->xor_sources = xor_sources;
	params->pq_sources = pq_sources;
	params->timeout = timeout;

	ret = dmatest_register_dbgfs(info);
	if (ret)
		return ret;

#ifdef MODULE
	return 0;
#else
	return run_threaded_test(info);
#endif
=======

	if (dmatest_run) {
		mutex_lock(&info->lock);
		run_threaded_test(info);
		mutex_unlock(&info->lock);
	}

	if (params->iterations && wait)
		wait_event(thread_wait, !is_threaded_test_run(info));

	/* module parameters are stable, inittime tests are started,
	 * let userspace take over 'run' control
	 */
	info->did_init = true;

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
/* when compiled-in wait for drivers to load first */
late_initcall(dmatest_init);

static void __exit dmatest_exit(void)
{
	struct dmatest_info *info = &test_info;

<<<<<<< HEAD
	debugfs_remove_recursive(info->root);
	stop_threaded_test(info);
	result_free(info, NULL);
=======
	mutex_lock(&info->lock);
	stop_threaded_test(info);
	mutex_unlock(&info->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
module_exit(dmatest_exit);

MODULE_AUTHOR("Haavard Skinnemoen (Atmel)");
MODULE_LICENSE("GPL v2");
