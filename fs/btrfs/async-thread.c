/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
<<<<<<< HEAD
=======
 * Copyright (C) 2014 Fujitsu.  All rights reserved.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
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
 */

#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/freezer.h>
#include "async-thread.h"
<<<<<<< HEAD

#define WORK_QUEUED_BIT 0
#define WORK_DONE_BIT 1
#define WORK_ORDER_DONE_BIT 2
#define WORK_HIGH_PRIO_BIT 3

/*
 * container for the kthread task pointer and the list of pending work
 * One of these is allocated per thread.
 */
struct btrfs_worker_thread {
	/* pool we belong to */
	struct btrfs_workers *workers;

	/* list of struct btrfs_work that are waiting for service */
	struct list_head pending;
	struct list_head prio_pending;

	/* list of worker threads from struct btrfs_workers */
	struct list_head worker_list;

	/* kthread */
	struct task_struct *task;

	/* number of things on the pending list */
	atomic_t num_pending;

	/* reference counter for this struct */
	atomic_t refs;

	unsigned long sequence;

	/* protects the pending list. */
	spinlock_t lock;

	/* set to non-zero when this thread is already awake and kicking */
	int working;

	/* are we currently idle */
	int idle;
};

static int __btrfs_start_workers(struct btrfs_workers *workers);

/*
 * btrfs_start_workers uses kthread_run, which can block waiting for memory
 * for a very long time.  It will actually throttle on page writeback,
 * and so it may not make progress until after our btrfs worker threads
 * process all of the pending work structs in their queue
 *
 * This means we can't use btrfs_start_workers from inside a btrfs worker
 * thread that is used as part of cleaning dirty memory, which pretty much
 * involves all of the worker threads.
 *
 * Instead we have a helper queue who never has more than one thread
 * where we scheduler thread start operations.  This worker_start struct
 * is used to contain the work and hold a pointer to the queue that needs
 * another worker.
 */
struct worker_start {
	struct btrfs_work work;
	struct btrfs_workers *queue;
};

static void start_new_worker_func(struct btrfs_work *work)
{
	struct worker_start *start;
	start = container_of(work, struct worker_start, work);
	__btrfs_start_workers(start->queue);
	kfree(start);
}

/*
 * helper function to move a thread onto the idle list after it
 * has finished some requests.
 */
static void check_idle_worker(struct btrfs_worker_thread *worker)
{
	if (!worker->idle && atomic_read(&worker->num_pending) <
	    worker->workers->idle_thresh / 2) {
		unsigned long flags;
		spin_lock_irqsave(&worker->workers->lock, flags);
		worker->idle = 1;

		/* the list may be empty if the worker is just starting */
		if (!list_empty(&worker->worker_list)) {
			list_move(&worker->worker_list,
				 &worker->workers->idle_list);
		}
		spin_unlock_irqrestore(&worker->workers->lock, flags);
	}
}

/*
 * helper function to move a thread off the idle list after new
 * pending work is added.
 */
static void check_busy_worker(struct btrfs_worker_thread *worker)
{
	if (worker->idle && atomic_read(&worker->num_pending) >=
	    worker->workers->idle_thresh) {
		unsigned long flags;
		spin_lock_irqsave(&worker->workers->lock, flags);
		worker->idle = 0;

		if (!list_empty(&worker->worker_list)) {
			list_move_tail(&worker->worker_list,
				      &worker->workers->worker_list);
		}
		spin_unlock_irqrestore(&worker->workers->lock, flags);
	}
}

static void check_pending_worker_creates(struct btrfs_worker_thread *worker)
{
	struct btrfs_workers *workers = worker->workers;
	struct worker_start *start;
	unsigned long flags;

	rmb();
	if (!workers->atomic_start_pending)
		return;

	start = kzalloc(sizeof(*start), GFP_NOFS);
	if (!start)
		return;

	start->work.func = start_new_worker_func;
	start->queue = workers;

	spin_lock_irqsave(&workers->lock, flags);
	if (!workers->atomic_start_pending)
		goto out;

	workers->atomic_start_pending = 0;
	if (workers->num_workers + workers->num_workers_starting >=
	    workers->max_workers)
		goto out;

	workers->num_workers_starting += 1;
	spin_unlock_irqrestore(&workers->lock, flags);
	btrfs_queue_worker(workers->atomic_worker_start, &start->work);
	return;

out:
	kfree(start);
	spin_unlock_irqrestore(&workers->lock, flags);
}

static noinline void run_ordered_completions(struct btrfs_workers *workers,
					    struct btrfs_work *work)
{
	if (!workers->ordered)
		return;

	set_bit(WORK_DONE_BIT, &work->flags);

	spin_lock(&workers->order_lock);

	while (1) {
		if (!list_empty(&workers->prio_order_list)) {
			work = list_entry(workers->prio_order_list.next,
					  struct btrfs_work, order_list);
		} else if (!list_empty(&workers->order_list)) {
			work = list_entry(workers->order_list.next,
					  struct btrfs_work, order_list);
		} else {
			break;
		}
		if (!test_bit(WORK_DONE_BIT, &work->flags))
			break;

		/* we are going to call the ordered done function, but
		 * we leave the work item on the list as a barrier so
		 * that later work items that are done don't have their
		 * functions called before this one returns
		 */
		if (test_and_set_bit(WORK_ORDER_DONE_BIT, &work->flags))
			break;

		spin_unlock(&workers->order_lock);

		work->ordered_func(work);

		/* now take the lock again and drop our item from the list */
		spin_lock(&workers->order_lock);
		list_del(&work->order_list);
		spin_unlock(&workers->order_lock);

		/*
		 * we don't want to call the ordered free functions
		 * with the lock held though
		 */
		work->ordered_free(work);
		spin_lock(&workers->order_lock);
	}

	spin_unlock(&workers->order_lock);
}

static void put_worker(struct btrfs_worker_thread *worker)
{
	if (atomic_dec_and_test(&worker->refs))
		kfree(worker);
}

static int try_worker_shutdown(struct btrfs_worker_thread *worker)
{
	int freeit = 0;

	spin_lock_irq(&worker->lock);
	spin_lock(&worker->workers->lock);
	if (worker->workers->num_workers > 1 &&
	    worker->idle &&
	    !worker->working &&
	    !list_empty(&worker->worker_list) &&
	    list_empty(&worker->prio_pending) &&
	    list_empty(&worker->pending) &&
	    atomic_read(&worker->num_pending) == 0) {
		freeit = 1;
		list_del_init(&worker->worker_list);
		worker->workers->num_workers--;
	}
	spin_unlock(&worker->workers->lock);
	spin_unlock_irq(&worker->lock);

	if (freeit)
		put_worker(worker);
	return freeit;
}

static struct btrfs_work *get_next_work(struct btrfs_worker_thread *worker,
					struct list_head *prio_head,
					struct list_head *head)
{
	struct btrfs_work *work = NULL;
	struct list_head *cur = NULL;

	if(!list_empty(prio_head))
		cur = prio_head->next;

	smp_mb();
	if (!list_empty(&worker->prio_pending))
		goto refill;

	if (!list_empty(head))
		cur = head->next;

	if (cur)
		goto out;

refill:
	spin_lock_irq(&worker->lock);
	list_splice_tail_init(&worker->prio_pending, prio_head);
	list_splice_tail_init(&worker->pending, head);

	if (!list_empty(prio_head))
		cur = prio_head->next;
	else if (!list_empty(head))
		cur = head->next;
	spin_unlock_irq(&worker->lock);

	if (!cur)
		goto out_fail;

out:
	work = list_entry(cur, struct btrfs_work, list);

out_fail:
	return work;
}

/*
 * main loop for servicing work items
 */
static int worker_loop(void *arg)
{
	struct btrfs_worker_thread *worker = arg;
	struct list_head head;
	struct list_head prio_head;
	struct btrfs_work *work;

	INIT_LIST_HEAD(&head);
	INIT_LIST_HEAD(&prio_head);

	do {
again:
		while (1) {


			work = get_next_work(worker, &prio_head, &head);
			if (!work)
				break;

			list_del(&work->list);
			clear_bit(WORK_QUEUED_BIT, &work->flags);

			work->worker = worker;

			work->func(work);

			atomic_dec(&worker->num_pending);
			/*
			 * unless this is an ordered work queue,
			 * 'work' was probably freed by func above.
			 */
			run_ordered_completions(worker->workers, work);

			check_pending_worker_creates(worker);
			cond_resched();
		}

		spin_lock_irq(&worker->lock);
		check_idle_worker(worker);

		if (freezing(current)) {
			worker->working = 0;
			spin_unlock_irq(&worker->lock);
			try_to_freeze();
		} else {
			spin_unlock_irq(&worker->lock);
			if (!kthread_should_stop()) {
				cpu_relax();
				/*
				 * we've dropped the lock, did someone else
				 * jump_in?
				 */
				smp_mb();
				if (!list_empty(&worker->pending) ||
				    !list_empty(&worker->prio_pending))
					continue;

				/*
				 * this short schedule allows more work to
				 * come in without the queue functions
				 * needing to go through wake_up_process()
				 *
				 * worker->working is still 1, so nobody
				 * is going to try and wake us up
				 */
				schedule_timeout(1);
				smp_mb();
				if (!list_empty(&worker->pending) ||
				    !list_empty(&worker->prio_pending))
					continue;

				if (kthread_should_stop())
					break;

				/* still no more work?, sleep for real */
				spin_lock_irq(&worker->lock);
				set_current_state(TASK_INTERRUPTIBLE);
				if (!list_empty(&worker->pending) ||
				    !list_empty(&worker->prio_pending)) {
					spin_unlock_irq(&worker->lock);
					set_current_state(TASK_RUNNING);
					goto again;
				}

				/*
				 * this makes sure we get a wakeup when someone
				 * adds something new to the queue
				 */
				worker->working = 0;
				spin_unlock_irq(&worker->lock);

				if (!kthread_should_stop()) {
					schedule_timeout(HZ * 120);
					if (!worker->working &&
					    try_worker_shutdown(worker)) {
						return 0;
					}
				}
			}
			__set_current_state(TASK_RUNNING);
		}
	} while (!kthread_should_stop());
	return 0;
}

/*
 * this will wait for all the worker threads to shutdown
 */
void btrfs_stop_workers(struct btrfs_workers *workers)
{
	struct list_head *cur;
	struct btrfs_worker_thread *worker;
	int can_stop;

	spin_lock_irq(&workers->lock);
	list_splice_init(&workers->idle_list, &workers->worker_list);
	while (!list_empty(&workers->worker_list)) {
		cur = workers->worker_list.next;
		worker = list_entry(cur, struct btrfs_worker_thread,
				    worker_list);

		atomic_inc(&worker->refs);
		workers->num_workers -= 1;
		if (!list_empty(&worker->worker_list)) {
			list_del_init(&worker->worker_list);
			put_worker(worker);
			can_stop = 1;
		} else
			can_stop = 0;
		spin_unlock_irq(&workers->lock);
		if (can_stop)
			kthread_stop(worker->task);
		spin_lock_irq(&workers->lock);
		put_worker(worker);
	}
	spin_unlock_irq(&workers->lock);
}

/*
 * simple init on struct btrfs_workers
 */
void btrfs_init_workers(struct btrfs_workers *workers, char *name, int max,
			struct btrfs_workers *async_helper)
{
	workers->num_workers = 0;
	workers->num_workers_starting = 0;
	INIT_LIST_HEAD(&workers->worker_list);
	INIT_LIST_HEAD(&workers->idle_list);
	INIT_LIST_HEAD(&workers->order_list);
	INIT_LIST_HEAD(&workers->prio_order_list);
	spin_lock_init(&workers->lock);
	spin_lock_init(&workers->order_lock);
	workers->max_workers = max;
	workers->idle_thresh = 32;
	workers->name = name;
	workers->ordered = 0;
	workers->atomic_start_pending = 0;
	workers->atomic_worker_start = async_helper;
}

/*
 * starts new worker threads.  This does not enforce the max worker
 * count in case you need to temporarily go past it.
 */
static int __btrfs_start_workers(struct btrfs_workers *workers)
{
	struct btrfs_worker_thread *worker;
	int ret = 0;

	worker = kzalloc(sizeof(*worker), GFP_NOFS);
	if (!worker) {
		ret = -ENOMEM;
		goto fail;
	}

	INIT_LIST_HEAD(&worker->pending);
	INIT_LIST_HEAD(&worker->prio_pending);
	INIT_LIST_HEAD(&worker->worker_list);
	spin_lock_init(&worker->lock);

	atomic_set(&worker->num_pending, 0);
	atomic_set(&worker->refs, 1);
	worker->workers = workers;
	worker->task = kthread_run(worker_loop, worker,
				   "btrfs-%s-%d", workers->name,
				   workers->num_workers + 1);
	if (IS_ERR(worker->task)) {
		ret = PTR_ERR(worker->task);
		kfree(worker);
		goto fail;
	}
	spin_lock_irq(&workers->lock);
	list_add_tail(&worker->worker_list, &workers->idle_list);
	worker->idle = 1;
	workers->num_workers++;
	workers->num_workers_starting--;
	WARN_ON(workers->num_workers_starting < 0);
	spin_unlock_irq(&workers->lock);

	return 0;
fail:
	spin_lock_irq(&workers->lock);
	workers->num_workers_starting--;
	spin_unlock_irq(&workers->lock);
	return ret;
}

int btrfs_start_workers(struct btrfs_workers *workers)
{
	spin_lock_irq(&workers->lock);
	workers->num_workers_starting++;
	spin_unlock_irq(&workers->lock);
	return __btrfs_start_workers(workers);
}

/*
 * run through the list and find a worker thread that doesn't have a lot
 * to do right now.  This can return null if we aren't yet at the thread
 * count limit and all of the threads are busy.
 */
static struct btrfs_worker_thread *next_worker(struct btrfs_workers *workers)
{
	struct btrfs_worker_thread *worker;
	struct list_head *next;
	int enforce_min;

	enforce_min = (workers->num_workers + workers->num_workers_starting) <
		workers->max_workers;

	/*
	 * if we find an idle thread, don't move it to the end of the
	 * idle list.  This improves the chance that the next submission
	 * will reuse the same thread, and maybe catch it while it is still
	 * working
	 */
	if (!list_empty(&workers->idle_list)) {
		next = workers->idle_list.next;
		worker = list_entry(next, struct btrfs_worker_thread,
				    worker_list);
		return worker;
	}
	if (enforce_min || list_empty(&workers->worker_list))
		return NULL;

	/*
	 * if we pick a busy task, move the task to the end of the list.
	 * hopefully this will keep things somewhat evenly balanced.
	 * Do the move in batches based on the sequence number.  This groups
	 * requests submitted at roughly the same time onto the same worker.
	 */
	next = workers->worker_list.next;
	worker = list_entry(next, struct btrfs_worker_thread, worker_list);
	worker->sequence++;

	if (worker->sequence % workers->idle_thresh == 0)
		list_move_tail(next, &workers->worker_list);
	return worker;
}

/*
 * selects a worker thread to take the next job.  This will either find
 * an idle worker, start a new worker up to the max count, or just return
 * one of the existing busy workers.
 */
static struct btrfs_worker_thread *find_worker(struct btrfs_workers *workers)
{
	struct btrfs_worker_thread *worker;
	unsigned long flags;
	struct list_head *fallback;
	int ret;

	spin_lock_irqsave(&workers->lock, flags);
again:
	worker = next_worker(workers);

	if (!worker) {
		if (workers->num_workers + workers->num_workers_starting >=
		    workers->max_workers) {
			goto fallback;
		} else if (workers->atomic_worker_start) {
			workers->atomic_start_pending = 1;
			goto fallback;
		} else {
			workers->num_workers_starting++;
			spin_unlock_irqrestore(&workers->lock, flags);
			/* we're below the limit, start another worker */
			ret = __btrfs_start_workers(workers);
			spin_lock_irqsave(&workers->lock, flags);
			if (ret)
				goto fallback;
			goto again;
		}
	}
	goto found;

fallback:
	fallback = NULL;
	/*
	 * we have failed to find any workers, just
	 * return the first one we can find.
	 */
	if (!list_empty(&workers->worker_list))
		fallback = workers->worker_list.next;
	if (!list_empty(&workers->idle_list))
		fallback = workers->idle_list.next;
	BUG_ON(!fallback);
	worker = list_entry(fallback,
		  struct btrfs_worker_thread, worker_list);
found:
	/*
	 * this makes sure the worker doesn't exit before it is placed
	 * onto a busy/idle list
	 */
	atomic_inc(&worker->num_pending);
	spin_unlock_irqrestore(&workers->lock, flags);
	return worker;
}

/*
 * btrfs_requeue_work just puts the work item back on the tail of the list
 * it was taken from.  It is intended for use with long running work functions
 * that make some progress and want to give the cpu up for others.
 */
void btrfs_requeue_work(struct btrfs_work *work)
{
	struct btrfs_worker_thread *worker = work->worker;
	unsigned long flags;
	int wake = 0;

	if (test_and_set_bit(WORK_QUEUED_BIT, &work->flags))
		return;

	spin_lock_irqsave(&worker->lock, flags);
	if (test_bit(WORK_HIGH_PRIO_BIT, &work->flags))
		list_add_tail(&work->list, &worker->prio_pending);
	else
		list_add_tail(&work->list, &worker->pending);
	atomic_inc(&worker->num_pending);

	/* by definition we're busy, take ourselves off the idle
	 * list
	 */
	if (worker->idle) {
		spin_lock(&worker->workers->lock);
		worker->idle = 0;
		list_move_tail(&worker->worker_list,
			      &worker->workers->worker_list);
		spin_unlock(&worker->workers->lock);
	}
	if (!worker->working) {
		wake = 1;
		worker->working = 1;
	}

	if (wake)
		wake_up_process(worker->task);
	spin_unlock_irqrestore(&worker->lock, flags);
}

void btrfs_set_work_high_prio(struct btrfs_work *work)
=======
#include "ctree.h"

#define WORK_DONE_BIT 0
#define WORK_ORDER_DONE_BIT 1
#define WORK_HIGH_PRIO_BIT 2

#define NO_THRESHOLD (-1)
#define DFT_THRESHOLD (32)

struct __btrfs_workqueue {
	struct workqueue_struct *normal_wq;

	/* File system this workqueue services */
	struct btrfs_fs_info *fs_info;

	/* List head pointing to ordered work list */
	struct list_head ordered_list;

	/* Spinlock for ordered_list */
	spinlock_t list_lock;

	/* Thresholding related variants */
	atomic_t pending;

	/* Up limit of concurrency workers */
	int limit_active;

	/* Current number of concurrency workers */
	int current_active;

	/* Threshold to change current_active */
	int thresh;
	unsigned int count;
	spinlock_t thres_lock;
};

struct btrfs_workqueue {
	struct __btrfs_workqueue *normal;
	struct __btrfs_workqueue *high;
};

static void normal_work_helper(struct btrfs_work *work);

#define BTRFS_WORK_HELPER(name)					\
void btrfs_##name(struct work_struct *arg)				\
{									\
	struct btrfs_work *work = container_of(arg, struct btrfs_work,	\
					       normal_work);		\
	normal_work_helper(work);					\
}

struct btrfs_fs_info *
btrfs_workqueue_owner(struct __btrfs_workqueue *wq)
{
	return wq->fs_info;
}

struct btrfs_fs_info *
btrfs_work_owner(struct btrfs_work *work)
{
	return work->wq->fs_info;
}

bool btrfs_workqueue_normal_congested(struct btrfs_workqueue *wq)
{
	/*
	 * We could compare wq->normal->pending with num_online_cpus()
	 * to support "thresh == NO_THRESHOLD" case, but it requires
	 * moving up atomic_inc/dec in thresh_queue/exec_hook. Let's
	 * postpone it until someone needs the support of that case.
	 */
	if (wq->normal->thresh == NO_THRESHOLD)
		return false;

	return atomic_read(&wq->normal->pending) > wq->normal->thresh * 2;
}

BTRFS_WORK_HELPER(worker_helper);
BTRFS_WORK_HELPER(delalloc_helper);
BTRFS_WORK_HELPER(flush_delalloc_helper);
BTRFS_WORK_HELPER(cache_helper);
BTRFS_WORK_HELPER(submit_helper);
BTRFS_WORK_HELPER(fixup_helper);
BTRFS_WORK_HELPER(endio_helper);
BTRFS_WORK_HELPER(endio_meta_helper);
BTRFS_WORK_HELPER(endio_meta_write_helper);
BTRFS_WORK_HELPER(endio_raid56_helper);
BTRFS_WORK_HELPER(endio_repair_helper);
BTRFS_WORK_HELPER(rmw_helper);
BTRFS_WORK_HELPER(endio_write_helper);
BTRFS_WORK_HELPER(freespace_write_helper);
BTRFS_WORK_HELPER(delayed_meta_helper);
BTRFS_WORK_HELPER(readahead_helper);
BTRFS_WORK_HELPER(qgroup_rescan_helper);
BTRFS_WORK_HELPER(extent_refs_helper);
BTRFS_WORK_HELPER(scrub_helper);
BTRFS_WORK_HELPER(scrubwrc_helper);
BTRFS_WORK_HELPER(scrubnc_helper);
BTRFS_WORK_HELPER(scrubparity_helper);

static struct __btrfs_workqueue *
__btrfs_alloc_workqueue(struct btrfs_fs_info *fs_info, const char *name,
			unsigned int flags, int limit_active, int thresh)
{
	struct __btrfs_workqueue *ret = kzalloc(sizeof(*ret), GFP_KERNEL);

	if (!ret)
		return NULL;

	ret->fs_info = fs_info;
	ret->limit_active = limit_active;
	atomic_set(&ret->pending, 0);
	if (thresh == 0)
		thresh = DFT_THRESHOLD;
	/* For low threshold, disabling threshold is a better choice */
	if (thresh < DFT_THRESHOLD) {
		ret->current_active = limit_active;
		ret->thresh = NO_THRESHOLD;
	} else {
		/*
		 * For threshold-able wq, let its concurrency grow on demand.
		 * Use minimal max_active at alloc time to reduce resource
		 * usage.
		 */
		ret->current_active = 1;
		ret->thresh = thresh;
	}

	if (flags & WQ_HIGHPRI)
		ret->normal_wq = alloc_workqueue("%s-%s-high", flags,
						 ret->current_active, "btrfs",
						 name);
	else
		ret->normal_wq = alloc_workqueue("%s-%s", flags,
						 ret->current_active, "btrfs",
						 name);
	if (!ret->normal_wq) {
		kfree(ret);
		return NULL;
	}

	INIT_LIST_HEAD(&ret->ordered_list);
	spin_lock_init(&ret->list_lock);
	spin_lock_init(&ret->thres_lock);
	trace_btrfs_workqueue_alloc(ret, name, flags & WQ_HIGHPRI);
	return ret;
}

static inline void
__btrfs_destroy_workqueue(struct __btrfs_workqueue *wq);

struct btrfs_workqueue *btrfs_alloc_workqueue(struct btrfs_fs_info *fs_info,
					      const char *name,
					      unsigned int flags,
					      int limit_active,
					      int thresh)
{
	struct btrfs_workqueue *ret = kzalloc(sizeof(*ret), GFP_KERNEL);

	if (!ret)
		return NULL;

	ret->normal = __btrfs_alloc_workqueue(fs_info, name,
					      flags & ~WQ_HIGHPRI,
					      limit_active, thresh);
	if (!ret->normal) {
		kfree(ret);
		return NULL;
	}

	if (flags & WQ_HIGHPRI) {
		ret->high = __btrfs_alloc_workqueue(fs_info, name, flags,
						    limit_active, thresh);
		if (!ret->high) {
			__btrfs_destroy_workqueue(ret->normal);
			kfree(ret);
			return NULL;
		}
	}
	return ret;
}

/*
 * Hook for threshold which will be called in btrfs_queue_work.
 * This hook WILL be called in IRQ handler context,
 * so workqueue_set_max_active MUST NOT be called in this hook
 */
static inline void thresh_queue_hook(struct __btrfs_workqueue *wq)
{
	if (wq->thresh == NO_THRESHOLD)
		return;
	atomic_inc(&wq->pending);
}

/*
 * Hook for threshold which will be called before executing the work,
 * This hook is called in kthread content.
 * So workqueue_set_max_active is called here.
 */
static inline void thresh_exec_hook(struct __btrfs_workqueue *wq)
{
	int new_current_active;
	long pending;
	int need_change = 0;

	if (wq->thresh == NO_THRESHOLD)
		return;

	atomic_dec(&wq->pending);
	spin_lock(&wq->thres_lock);
	/*
	 * Use wq->count to limit the calling frequency of
	 * workqueue_set_max_active.
	 */
	wq->count++;
	wq->count %= (wq->thresh / 4);
	if (!wq->count)
		goto  out;
	new_current_active = wq->current_active;

	/*
	 * pending may be changed later, but it's OK since we really
	 * don't need it so accurate to calculate new_max_active.
	 */
	pending = atomic_read(&wq->pending);
	if (pending > wq->thresh)
		new_current_active++;
	if (pending < wq->thresh / 2)
		new_current_active--;
	new_current_active = clamp_val(new_current_active, 1, wq->limit_active);
	if (new_current_active != wq->current_active)  {
		need_change = 1;
		wq->current_active = new_current_active;
	}
out:
	spin_unlock(&wq->thres_lock);

	if (need_change) {
		workqueue_set_max_active(wq->normal_wq, wq->current_active);
	}
}

static void run_ordered_work(struct __btrfs_workqueue *wq,
			     struct btrfs_work *self)
{
	struct list_head *list = &wq->ordered_list;
	struct btrfs_work *work;
	spinlock_t *lock = &wq->list_lock;
	unsigned long flags;
	void *wtag;
	bool free_self = false;

	while (1) {
		spin_lock_irqsave(lock, flags);
		if (list_empty(list))
			break;
		work = list_entry(list->next, struct btrfs_work,
				  ordered_list);
		if (!test_bit(WORK_DONE_BIT, &work->flags))
			break;

		/*
		 * we are going to call the ordered done function, but
		 * we leave the work item on the list as a barrier so
		 * that later work items that are done don't have their
		 * functions called before this one returns
		 */
		if (test_and_set_bit(WORK_ORDER_DONE_BIT, &work->flags))
			break;
		trace_btrfs_ordered_sched(work);
		spin_unlock_irqrestore(lock, flags);
		work->ordered_func(work);

		/* now take the lock again and drop our item from the list */
		spin_lock_irqsave(lock, flags);
		list_del(&work->ordered_list);
		spin_unlock_irqrestore(lock, flags);

		if (work == self) {
			/*
			 * This is the work item that the worker is currently
			 * executing.
			 *
			 * The kernel workqueue code guarantees non-reentrancy
			 * of work items. I.e., if a work item with the same
			 * address and work function is queued twice, the second
			 * execution is blocked until the first one finishes. A
			 * work item may be freed and recycled with the same
			 * work function; the workqueue code assumes that the
			 * original work item cannot depend on the recycled work
			 * item in that case (see find_worker_executing_work()).
			 *
			 * Note that the work of one Btrfs filesystem may depend
			 * on the work of another Btrfs filesystem via, e.g., a
			 * loop device. Therefore, we must not allow the current
			 * work item to be recycled until we are really done,
			 * otherwise we break the above assumption and can
			 * deadlock.
			 */
			free_self = true;
		} else {
			/*
			 * We don't want to call the ordered free functions with
			 * the lock held though. Save the work as tag for the
			 * trace event, because the callback could free the
			 * structure.
			 */
			wtag = work;
			work->ordered_free(work);
			trace_btrfs_all_work_done(wq->fs_info, wtag);
		}
	}
	spin_unlock_irqrestore(lock, flags);

	if (free_self) {
		wtag = self;
		self->ordered_free(self);
		trace_btrfs_all_work_done(wq->fs_info, wtag);
	}
}

static void normal_work_helper(struct btrfs_work *work)
{
	struct __btrfs_workqueue *wq;
	void *wtag;
	int need_order = 0;

	/*
	 * We should not touch things inside work in the following cases:
	 * 1) after work->func() if it has no ordered_free
	 *    Since the struct is freed in work->func().
	 * 2) after setting WORK_DONE_BIT
	 *    The work may be freed in other threads almost instantly.
	 * So we save the needed things here.
	 */
	if (work->ordered_func)
		need_order = 1;
	wq = work->wq;
	/* Safe for tracepoints in case work gets freed by the callback */
	wtag = work;

	trace_btrfs_work_sched(work);
	thresh_exec_hook(wq);
	work->func(work);
	if (need_order) {
		set_bit(WORK_DONE_BIT, &work->flags);
		run_ordered_work(wq, work);
	}
	if (!need_order)
		trace_btrfs_all_work_done(wq->fs_info, wtag);
}

void btrfs_init_work(struct btrfs_work *work, btrfs_work_func_t uniq_func,
		     btrfs_func_t func,
		     btrfs_func_t ordered_func,
		     btrfs_func_t ordered_free)
{
	work->func = func;
	work->ordered_func = ordered_func;
	work->ordered_free = ordered_free;
	INIT_WORK(&work->normal_work, uniq_func);
	INIT_LIST_HEAD(&work->ordered_list);
	work->flags = 0;
}

static inline void __btrfs_queue_work(struct __btrfs_workqueue *wq,
				      struct btrfs_work *work)
{
	unsigned long flags;

	work->wq = wq;
	thresh_queue_hook(wq);
	if (work->ordered_func) {
		spin_lock_irqsave(&wq->list_lock, flags);
		list_add_tail(&work->ordered_list, &wq->ordered_list);
		spin_unlock_irqrestore(&wq->list_lock, flags);
	}
	trace_btrfs_work_queued(work);
	queue_work(wq->normal_wq, &work->normal_work);
}

void btrfs_queue_work(struct btrfs_workqueue *wq,
		      struct btrfs_work *work)
{
	struct __btrfs_workqueue *dest_wq;

	if (test_bit(WORK_HIGH_PRIO_BIT, &work->flags) && wq->high)
		dest_wq = wq->high;
	else
		dest_wq = wq->normal;
	__btrfs_queue_work(dest_wq, work);
}

static inline void
__btrfs_destroy_workqueue(struct __btrfs_workqueue *wq)
{
	destroy_workqueue(wq->normal_wq);
	trace_btrfs_workqueue_destroy(wq);
	kfree(wq);
}

void btrfs_destroy_workqueue(struct btrfs_workqueue *wq)
{
	if (!wq)
		return;
	if (wq->high)
		__btrfs_destroy_workqueue(wq->high);
	__btrfs_destroy_workqueue(wq->normal);
	kfree(wq);
}

void btrfs_workqueue_set_max(struct btrfs_workqueue *wq, int limit_active)
{
	if (!wq)
		return;
	wq->normal->limit_active = limit_active;
	if (wq->high)
		wq->high->limit_active = limit_active;
}

void btrfs_set_work_high_priority(struct btrfs_work *work)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	set_bit(WORK_HIGH_PRIO_BIT, &work->flags);
}

<<<<<<< HEAD
/*
 * places a struct btrfs_work into the pending queue of one of the kthreads
 */
void btrfs_queue_worker(struct btrfs_workers *workers, struct btrfs_work *work)
{
	struct btrfs_worker_thread *worker;
	unsigned long flags;
	int wake = 0;

	/* don't requeue something already on a list */
	if (test_and_set_bit(WORK_QUEUED_BIT, &work->flags))
		return;

	worker = find_worker(workers);
	if (workers->ordered) {
		/*
		 * you're not allowed to do ordered queues from an
		 * interrupt handler
		 */
		spin_lock(&workers->order_lock);
		if (test_bit(WORK_HIGH_PRIO_BIT, &work->flags)) {
			list_add_tail(&work->order_list,
				      &workers->prio_order_list);
		} else {
			list_add_tail(&work->order_list, &workers->order_list);
		}
		spin_unlock(&workers->order_lock);
	} else {
		INIT_LIST_HEAD(&work->order_list);
	}

	spin_lock_irqsave(&worker->lock, flags);

	if (test_bit(WORK_HIGH_PRIO_BIT, &work->flags))
		list_add_tail(&work->list, &worker->prio_pending);
	else
		list_add_tail(&work->list, &worker->pending);
	check_busy_worker(worker);

	/*
	 * avoid calling into wake_up_process if this thread has already
	 * been kicked
	 */
	if (!worker->working)
		wake = 1;
	worker->working = 1;

	if (wake)
		wake_up_process(worker->task);
	spin_unlock_irqrestore(&worker->lock, flags);
=======
void btrfs_flush_workqueue(struct btrfs_workqueue *wq)
{
	if (wq->high)
		flush_workqueue(wq->high->normal_wq);

	flush_workqueue(wq->normal->normal_wq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
