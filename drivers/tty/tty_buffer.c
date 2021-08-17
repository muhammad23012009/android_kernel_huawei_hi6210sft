/*
 * Tty buffer allocation management
 */

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/wait.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/ratelimit.h>

<<<<<<< HEAD
=======

#define MIN_TTYB_SIZE	256
#define TTYB_ALIGN_MASK	255

/*
 * Byte threshold to limit memory consumption for flip buffers.
 * The actual memory limit is > 2x this amount.
 */
#define TTYB_DEFAULT_MEM_LIMIT	(640 * 1024UL)

/*
 * We default to dicing tty buffer allocations to this many characters
 * in order to avoid multiple page allocations. We know the size of
 * tty_buffer itself but it must also be taken into account that the
 * the buffer is 256 byte aligned. See tty_buffer_find for the allocation
 * logic this must match
 */

#define TTY_BUFFER_PAGE	(((PAGE_SIZE - sizeof(struct tty_buffer)) / 2) & ~0xFF)

/**
 *	tty_buffer_lock_exclusive	-	gain exclusive access to buffer
 *	tty_buffer_unlock_exclusive	-	release exclusive access
 *
 *	@port - tty_port owning the flip buffer
 *
 *	Guarantees safe use of the line discipline's receive_buf() method by
 *	excluding the buffer work and any pending flush from using the flip
 *	buffer. Data can continue to be added concurrently to the flip buffer
 *	from the driver side.
 *
 *	On release, the buffer work is restarted if there is data in the
 *	flip buffer
 */

void tty_buffer_lock_exclusive(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;

	atomic_inc(&buf->priority);
	mutex_lock(&buf->lock);
}
EXPORT_SYMBOL_GPL(tty_buffer_lock_exclusive);

void tty_buffer_unlock_exclusive(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;
	int restart;

	restart = buf->head->commit != buf->head->read;

	atomic_dec(&buf->priority);
	mutex_unlock(&buf->lock);
	if (restart)
		queue_work(system_unbound_wq, &buf->work);
}
EXPORT_SYMBOL_GPL(tty_buffer_unlock_exclusive);

/**
 *	tty_buffer_space_avail	-	return unused buffer space
 *	@port - tty_port owning the flip buffer
 *
 *	Returns the # of bytes which can be written by the driver without
 *	reaching the buffer limit.
 *
 *	Note: this does not guarantee that memory is available to write
 *	the returned # of bytes (use tty_prepare_flip_string_xxx() to
 *	pre-allocate if memory guarantee is required).
 */

int tty_buffer_space_avail(struct tty_port *port)
{
	int space = port->buf.mem_limit - atomic_read(&port->buf.mem_used);
	return max(space, 0);
}
EXPORT_SYMBOL_GPL(tty_buffer_space_avail);

static void tty_buffer_reset(struct tty_buffer *p, size_t size)
{
	p->used = 0;
	p->size = size;
	p->next = NULL;
	p->commit = 0;
	p->read = 0;
	p->flags = 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 *	tty_buffer_free_all		-	free buffers used by a tty
 *	@tty: tty to free from
 *
 *	Remove all the buffers pending on a tty whether queued with data
 *	or in the free ring. Must be called when the tty is no longer in use
<<<<<<< HEAD
 *
 *	Locking: none
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

void tty_buffer_free_all(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;
<<<<<<< HEAD
	struct tty_buffer *thead;

	while ((thead = buf->head) != NULL) {
		buf->head = thead->next;
		kfree(thead);
	}
	while ((thead = buf->free) != NULL) {
		buf->free = thead->next;
		kfree(thead);
	}
	buf->tail = NULL;
	buf->memory_used = 0;
=======
	struct tty_buffer *p, *next;
	struct llist_node *llist;

	while ((p = buf->head) != NULL) {
		buf->head = p->next;
		if (p->size > 0)
			kfree(p);
	}
	llist = llist_del_all(&buf->free);
	llist_for_each_entry_safe(p, next, llist, free)
		kfree(p);

	tty_buffer_reset(&buf->sentinel, 0);
	buf->head = &buf->sentinel;
	buf->tail = &buf->sentinel;

	atomic_set(&buf->mem_used, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 *	tty_buffer_alloc	-	allocate a tty buffer
 *	@tty: tty device
 *	@size: desired size (characters)
 *
 *	Allocate a new tty buffer to hold the desired number of characters.
<<<<<<< HEAD
 *	Return NULL if out of memory or the allocation would exceed the
 *	per device queue
 *
 *	Locking: Caller must hold tty->buf.lock
=======
 *	We round our buffers off in 256 character chunks to get better
 *	allocation behaviour.
 *	Return NULL if out of memory or the allocation would exceed the
 *	per device queue
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

static struct tty_buffer *tty_buffer_alloc(struct tty_port *port, size_t size)
{
<<<<<<< HEAD
	struct tty_buffer *p;

	if (port->buf.memory_used + size > 65536)
=======
	struct llist_node *free;
	struct tty_buffer *p;

	/* Round the buffer size out */
	size = __ALIGN_MASK(size, TTYB_ALIGN_MASK);

	if (size <= MIN_TTYB_SIZE) {
		free = llist_del_first(&port->buf.free);
		if (free) {
			p = llist_entry(free, struct tty_buffer, free);
			goto found;
		}
	}

	/* Should possibly check if this fails for the largest buffer we
	   have queued and recycle that ? */
	if (atomic_read(&port->buf.mem_used) > port->buf.mem_limit)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return NULL;
	p = kmalloc(sizeof(struct tty_buffer) + 2 * size, GFP_ATOMIC);
	if (p == NULL)
		return NULL;
<<<<<<< HEAD
	p->used = 0;
	p->size = size;
	p->next = NULL;
	p->commit = 0;
	p->read = 0;
	p->char_buf_ptr = (char *)(p->data);
	p->flag_buf_ptr = (unsigned char *)p->char_buf_ptr + size;
	port->buf.memory_used += size;
=======

found:
	tty_buffer_reset(p, size);
	atomic_add(size, &port->buf.mem_used);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return p;
}

/**
 *	tty_buffer_free		-	free a tty buffer
 *	@tty: tty owning the buffer
 *	@b: the buffer to free
 *
 *	Free a tty buffer, or add it to the free list according to our
 *	internal strategy
<<<<<<< HEAD
 *
 *	Locking: Caller must hold tty->buf.lock
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

static void tty_buffer_free(struct tty_port *port, struct tty_buffer *b)
{
	struct tty_bufhead *buf = &port->buf;

	/* Dumb strategy for now - should keep some stats */
<<<<<<< HEAD
	buf->memory_used -= b->size;
	WARN_ON(buf->memory_used < 0);

	if (b->size >= 512)
		kfree(b);
	else {
		b->next = buf->free;
		buf->free = b;
	}
}

/**
 *	__tty_buffer_flush		-	flush full tty buffers
 *	@tty: tty to flush
 *
 *	flush all the buffers containing receive data. Caller must
 *	hold the buffer lock and must have ensured no parallel flush to
 *	ldisc is running.
 *
 *	Locking: Caller must hold tty->buf.lock
 */

static void __tty_buffer_flush(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;
	struct tty_buffer *thead;

	if (unlikely(buf->head == NULL))
		return;
	while ((thead = buf->head->next) != NULL) {
		tty_buffer_free(port, buf->head);
		buf->head = thead;
	}
	WARN_ON(buf->head != buf->tail);
	buf->head->read = buf->head->commit;
}

/**
 *	tty_buffer_flush		-	flush full tty buffers
 *	@tty: tty to flush
 *
 *	flush all the buffers containing receive data. If the buffer is
 *	being processed by flush_to_ldisc then we defer the processing
 *	to that function
 *
 *	Locking: none
 */

void tty_buffer_flush(struct tty_struct *tty)
{
	struct tty_port *port = tty->port;
	struct tty_bufhead *buf = &port->buf;
	unsigned long flags;

	spin_lock_irqsave(&buf->lock, flags);

	/* If the data is being pushed to the tty layer then we can't
	   process it here. Instead set a flag and the flush_to_ldisc
	   path will process the flush request before it exits */
	if (test_bit(TTYP_FLUSHING, &port->iflags)) {
		set_bit(TTYP_FLUSHPENDING, &port->iflags);
		spin_unlock_irqrestore(&buf->lock, flags);
		wait_event(tty->read_wait,
				test_bit(TTYP_FLUSHPENDING, &port->iflags) == 0);
		return;
	} else
		__tty_buffer_flush(port);
	spin_unlock_irqrestore(&buf->lock, flags);
}

/**
 *	tty_buffer_find		-	find a free tty buffer
 *	@tty: tty owning the buffer
 *	@size: characters wanted
 *
 *	Locate an existing suitable tty buffer or if we are lacking one then
 *	allocate a new one. We round our buffers off in 256 character chunks
 *	to get better allocation behaviour.
 *
 *	Locking: Caller must hold tty->buf.lock
 */

static struct tty_buffer *tty_buffer_find(struct tty_port *port, size_t size)
{
	struct tty_buffer **tbh = &port->buf.free;
	while ((*tbh) != NULL) {
		struct tty_buffer *t = *tbh;
		if (t->size >= size) {
			*tbh = t->next;
			t->next = NULL;
			t->used = 0;
			t->commit = 0;
			t->read = 0;
			port->buf.memory_used += t->size;
			return t;
		}
		tbh = &((*tbh)->next);
	}
	/* Round the buffer size out */
	size = (size + 0xFF) & ~0xFF;
	return tty_buffer_alloc(port, size);
	/* Should possibly check if this fails for the largest buffer we
	   have queued and recycle that ? */
}
/**
 *	tty_buffer_request_room		-	grow tty buffer if needed
 *	@tty: tty structure
 *	@size: size desired
=======
	WARN_ON(atomic_sub_return(b->size, &buf->mem_used) < 0);

	if (b->size > MIN_TTYB_SIZE)
		kfree(b);
	else if (b->size > 0)
		llist_add(&b->free, &buf->free);
}

/**
 *	tty_buffer_flush		-	flush full tty buffers
 *	@tty: tty to flush
 *	@ld:  optional ldisc ptr (must be referenced)
 *
 *	flush all the buffers containing receive data. If ld != NULL,
 *	flush the ldisc input buffer.
 *
 *	Locking: takes buffer lock to ensure single-threaded flip buffer
 *		 'consumer'
 */

void tty_buffer_flush(struct tty_struct *tty, struct tty_ldisc *ld)
{
	struct tty_port *port = tty->port;
	struct tty_bufhead *buf = &port->buf;
	struct tty_buffer *next;

	atomic_inc(&buf->priority);

	mutex_lock(&buf->lock);
	/* paired w/ release in __tty_buffer_request_room; ensures there are
	 * no pending memory accesses to the freed buffer
	 */
	while ((next = smp_load_acquire(&buf->head->next)) != NULL) {
		tty_buffer_free(port, buf->head);
		buf->head = next;
	}
	buf->head->read = buf->head->commit;

	if (ld && ld->ops->flush_buffer)
		ld->ops->flush_buffer(tty);

	atomic_dec(&buf->priority);
	mutex_unlock(&buf->lock);
}

/**
 *	tty_buffer_request_room		-	grow tty buffer if needed
 *	@tty: tty structure
 *	@size: size desired
 *	@flags: buffer flags if new buffer allocated (default = 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 *	Make at least size bytes of linear space available for the tty
 *	buffer. If we fail return the size we managed to find.
 *
<<<<<<< HEAD
 *	Locking: Takes port->buf.lock
 */
int tty_buffer_request_room(struct tty_port *port, size_t size)
{
	struct tty_bufhead *buf = &port->buf;
	struct tty_buffer *b, *n;
	int left;
	unsigned long flags;
	spin_lock_irqsave(&buf->lock, flags);
	/* OPTIMISATION: We could keep a per tty "zero" sized buffer to
	   remove this conditional if its worth it. This would be invisible
	   to the callers */
	b = buf->tail;
	if (b != NULL)
		left = b->size - b->used;
	else
		left = 0;

	if (left < size) {
		/* This is the slow path - looking for new buffers to use */
		if ((n = tty_buffer_find(port, size)) != NULL) {
			if (b != NULL) {
				b->next = n;
				b->commit = b->used;
			} else
				buf->head = n;
			buf->tail = n;
		} else
			size = left;
	}
	spin_unlock_irqrestore(&buf->lock, flags);
	return size;
}
=======
 *	Will change over to a new buffer if the current buffer is encoded as
 *	TTY_NORMAL (so has no flags buffer) and the new buffer requires
 *	a flags buffer.
 */
static int __tty_buffer_request_room(struct tty_port *port, size_t size,
				     int flags)
{
	struct tty_bufhead *buf = &port->buf;
	struct tty_buffer *b, *n;
	int left, change;

	b = buf->tail;
	if (b->flags & TTYB_NORMAL)
		left = 2 * b->size - b->used;
	else
		left = b->size - b->used;

	change = (b->flags & TTYB_NORMAL) && (~flags & TTYB_NORMAL);
	if (change || left < size) {
		/* This is the slow path - looking for new buffers to use */
		n = tty_buffer_alloc(port, size);
		if (n != NULL) {
			n->flags = flags;
			buf->tail = n;
			/* paired w/ acquire in flush_to_ldisc(); ensures
			 * flush_to_ldisc() sees buffer data.
			 */
			smp_store_release(&b->commit, b->used);
			/* paired w/ acquire in flush_to_ldisc(); ensures the
			 * latest commit value can be read before the head is
			 * advanced to the next buffer
			 */
			smp_store_release(&b->next, n);
		} else if (change)
			size = 0;
		else
			size = left;
	}
	return size;
}

int tty_buffer_request_room(struct tty_port *port, size_t size)
{
	return __tty_buffer_request_room(port, size, 0);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL_GPL(tty_buffer_request_room);

/**
 *	tty_insert_flip_string_fixed_flag - Add characters to the tty buffer
 *	@port: tty port
 *	@chars: characters
 *	@flag: flag value for each character
 *	@size: size
 *
 *	Queue a series of bytes to the tty buffering. All the characters
 *	passed are marked with the supplied flag. Returns the number added.
<<<<<<< HEAD
 *
 *	Locking: Called functions may take port->buf.lock
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

int tty_insert_flip_string_fixed_flag(struct tty_port *port,
		const unsigned char *chars, char flag, size_t size)
{
	int copied = 0;
	do {
		int goal = min_t(size_t, size - copied, TTY_BUFFER_PAGE);
<<<<<<< HEAD
		int space = tty_buffer_request_room(port, goal);
		struct tty_buffer *tb = port->buf.tail;
		/* If there is no space then tb may be NULL */
		if (unlikely(space == 0)) {
			break;
		}
		memcpy(tb->char_buf_ptr + tb->used, chars, space);
		memset(tb->flag_buf_ptr + tb->used, flag, space);
=======
		int flags = (flag == TTY_NORMAL) ? TTYB_NORMAL : 0;
		int space = __tty_buffer_request_room(port, goal, flags);
		struct tty_buffer *tb = port->buf.tail;
		if (unlikely(space == 0))
			break;
		memcpy(char_buf_ptr(tb, tb->used), chars, space);
		if (~tb->flags & TTYB_NORMAL)
			memset(flag_buf_ptr(tb, tb->used), flag, space);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		tb->used += space;
		copied += space;
		chars += space;
		/* There is a small chance that we need to split the data over
		   several buffers. If this is the case we must loop */
	} while (unlikely(size > copied));
	return copied;
}
EXPORT_SYMBOL(tty_insert_flip_string_fixed_flag);

/**
 *	tty_insert_flip_string_flags	-	Add characters to the tty buffer
 *	@port: tty port
 *	@chars: characters
 *	@flags: flag bytes
 *	@size: size
 *
 *	Queue a series of bytes to the tty buffering. For each character
 *	the flags array indicates the status of the character. Returns the
 *	number added.
<<<<<<< HEAD
 *
 *	Locking: Called functions may take port->buf.lock
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

int tty_insert_flip_string_flags(struct tty_port *port,
		const unsigned char *chars, const char *flags, size_t size)
{
	int copied = 0;
	do {
		int goal = min_t(size_t, size - copied, TTY_BUFFER_PAGE);
		int space = tty_buffer_request_room(port, goal);
		struct tty_buffer *tb = port->buf.tail;
<<<<<<< HEAD
		/* If there is no space then tb may be NULL */
		if (unlikely(space == 0)) {
			break;
		}
		memcpy(tb->char_buf_ptr + tb->used, chars, space);
		memcpy(tb->flag_buf_ptr + tb->used, flags, space);
=======
		if (unlikely(space == 0))
			break;
		memcpy(char_buf_ptr(tb, tb->used), chars, space);
		memcpy(flag_buf_ptr(tb, tb->used), flags, space);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		tb->used += space;
		copied += space;
		chars += space;
		flags += space;
		/* There is a small chance that we need to split the data over
		   several buffers. If this is the case we must loop */
	} while (unlikely(size > copied));
	return copied;
}
EXPORT_SYMBOL(tty_insert_flip_string_flags);

/**
<<<<<<< HEAD
=======
 *	__tty_insert_flip_char   -	Add one character to the tty buffer
 *	@port: tty port
 *	@ch: character
 *	@flag: flag byte
 *
 *	Queue a single byte to the tty buffering, with an optional flag.
 *	This is the slow path of tty_insert_flip_char.
 */
int __tty_insert_flip_char(struct tty_port *port, unsigned char ch, char flag)
{
	struct tty_buffer *tb;
	int flags = (flag == TTY_NORMAL) ? TTYB_NORMAL : 0;

	if (!__tty_buffer_request_room(port, 1, flags))
		return 0;

	tb = port->buf.tail;
	if (~tb->flags & TTYB_NORMAL)
		*flag_buf_ptr(tb, tb->used) = flag;
	*char_buf_ptr(tb, tb->used++) = ch;

	return 1;
}
EXPORT_SYMBOL(__tty_insert_flip_char);

/**
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *	tty_schedule_flip	-	push characters to ldisc
 *	@port: tty port to push from
 *
 *	Takes any pending buffers and transfers their ownership to the
 *	ldisc side of the queue. It then schedules those characters for
 *	processing by the line discipline.
<<<<<<< HEAD
 *	Note that this function can only be used when the low_latency flag
 *	is unset. Otherwise the workqueue won't be flushed.
 *
 *	Locking: Takes port->buf.lock
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

void tty_schedule_flip(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;
<<<<<<< HEAD
	unsigned long flags;
	WARN_ON(port->low_latency);

	spin_lock_irqsave(&buf->lock, flags);
	if (buf->tail != NULL)
		buf->tail->commit = buf->tail->used;
	spin_unlock_irqrestore(&buf->lock, flags);
	schedule_work(&buf->work);
=======

	/* paired w/ acquire in flush_to_ldisc(); ensures
	 * flush_to_ldisc() sees buffer data.
	 */
	smp_store_release(&buf->tail->commit, buf->tail->used);
	queue_work(system_unbound_wq, &buf->work);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL(tty_schedule_flip);

/**
 *	tty_prepare_flip_string		-	make room for characters
 *	@port: tty port
 *	@chars: return pointer for character write area
 *	@size: desired size
 *
 *	Prepare a block of space in the buffer for data. Returns the length
 *	available and buffer pointer to the space which is now allocated and
 *	accounted for as ready for normal characters. This is used for drivers
 *	that need their own block copy routines into the buffer. There is no
 *	guarantee the buffer is a DMA target!
<<<<<<< HEAD
 *
 *	Locking: May call functions taking port->buf.lock
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

int tty_prepare_flip_string(struct tty_port *port, unsigned char **chars,
		size_t size)
{
<<<<<<< HEAD
	int space = tty_buffer_request_room(port, size);
	if (likely(space)) {
		struct tty_buffer *tb = port->buf.tail;
		*chars = tb->char_buf_ptr + tb->used;
		memset(tb->flag_buf_ptr + tb->used, TTY_NORMAL, space);
=======
	int space = __tty_buffer_request_room(port, size, TTYB_NORMAL);
	if (likely(space)) {
		struct tty_buffer *tb = port->buf.tail;
		*chars = char_buf_ptr(tb, tb->used);
		if (~tb->flags & TTYB_NORMAL)
			memset(flag_buf_ptr(tb, tb->used), TTY_NORMAL, space);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		tb->used += space;
	}
	return space;
}
EXPORT_SYMBOL_GPL(tty_prepare_flip_string);

/**
<<<<<<< HEAD
 *	tty_prepare_flip_string_flags	-	make room for characters
 *	@port: tty port
 *	@chars: return pointer for character write area
 *	@flags: return pointer for status flag write area
 *	@size: desired size
 *
 *	Prepare a block of space in the buffer for data. Returns the length
 *	available and buffer pointer to the space which is now allocated and
 *	accounted for as ready for characters. This is used for drivers
 *	that need their own block copy routines into the buffer. There is no
 *	guarantee the buffer is a DMA target!
 *
 *	Locking: May call functions taking port->buf.lock
 */

int tty_prepare_flip_string_flags(struct tty_port *port,
			unsigned char **chars, char **flags, size_t size)
{
	int space = tty_buffer_request_room(port, size);
	if (likely(space)) {
		struct tty_buffer *tb = port->buf.tail;
		*chars = tb->char_buf_ptr + tb->used;
		*flags = tb->flag_buf_ptr + tb->used;
		tb->used += space;
	}
	return space;
}
EXPORT_SYMBOL_GPL(tty_prepare_flip_string_flags);


=======
 *	tty_ldisc_receive_buf		-	forward data to line discipline
 *	@ld:	line discipline to process input
 *	@p:	char buffer
 *	@f:	TTY_* flags buffer
 *	@count:	number of bytes to process
 *
 *	Callers other than flush_to_ldisc() need to exclude the kworker
 *	from concurrent use of the line discipline, see paste_selection().
 *
 *	Returns the number of bytes processed
 */
int tty_ldisc_receive_buf(struct tty_ldisc *ld, unsigned char *p,
			  char *f, int count)
{
	if (ld->ops->receive_buf2)
		count = ld->ops->receive_buf2(ld->tty, p, f, count);
	else {
		count = min_t(int, count, ld->tty->receive_room);
		if (count && ld->ops->receive_buf)
			ld->ops->receive_buf(ld->tty, p, f, count);
	}
	if (count > 0)
		memset(p, 0, count);
	return count;
}
EXPORT_SYMBOL_GPL(tty_ldisc_receive_buf);

static int
receive_buf(struct tty_ldisc *ld, struct tty_buffer *head, int count)
{
	unsigned char *p = char_buf_ptr(head, head->read);
	char	      *f = NULL;

	if (~head->flags & TTYB_NORMAL)
		f = flag_buf_ptr(head, head->read);

	return tty_ldisc_receive_buf(ld, p, f, count);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 *	flush_to_ldisc
 *	@work: tty structure passed from work queue.
 *
 *	This routine is called out of the software interrupt to flush data
 *	from the buffer chain to the line discipline.
 *
<<<<<<< HEAD
 *	Locking: holds tty->buf.lock to guard buffer list. Drops the lock
 *	while invoking the line discipline receive_buf method. The
 *	receive_buf method is single threaded for each tty instance.
=======
 *	The receive_buf method is single threaded for each tty instance.
 *
 *	Locking: takes buffer lock to ensure single-threaded flip buffer
 *		 'consumer'
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

static void flush_to_ldisc(struct work_struct *work)
{
	struct tty_port *port = container_of(work, struct tty_port, buf.work);
	struct tty_bufhead *buf = &port->buf;
	struct tty_struct *tty;
<<<<<<< HEAD
	unsigned long 	flags;
	struct tty_ldisc *disc;

	tty = port->itty;
=======
	struct tty_ldisc *disc;

	tty = READ_ONCE(port->itty);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (tty == NULL)
		return;

	disc = tty_ldisc_ref(tty);
<<<<<<< HEAD
	if (disc == NULL)	/*  !TTY_LDISC */
		return;

	spin_lock_irqsave(&buf->lock, flags);

	if (!test_and_set_bit(TTYP_FLUSHING, &port->iflags)) {
		struct tty_buffer *head;
		while ((head = buf->head) != NULL) {
			int count;
			char *char_buf;
			unsigned char *flag_buf;

			count = head->commit - head->read;
			if (!count) {
				if (head->next == NULL)
					break;
				buf->head = head->next;
				tty_buffer_free(port, head);
				continue;
			}
			if (!tty->receive_room)
				break;
			if (count > tty->receive_room)
				count = tty->receive_room;
			char_buf = head->char_buf_ptr + head->read;
			flag_buf = head->flag_buf_ptr + head->read;
			head->read += count;
			spin_unlock_irqrestore(&buf->lock, flags);
			disc->ops->receive_buf(tty, char_buf,
							flag_buf, count);
			spin_lock_irqsave(&buf->lock, flags);
			/* Ldisc or user is trying to flush the buffers.
			   We may have a deferred request to flush the
			   input buffer, if so pull the chain under the lock
			   and empty the queue */
			if (test_bit(TTYP_FLUSHPENDING, &port->iflags)) {
				__tty_buffer_flush(port);
				clear_bit(TTYP_FLUSHPENDING, &port->iflags);
				wake_up(&tty->read_wait);
				break;
			}
		}
		clear_bit(TTYP_FLUSHING, &port->iflags);
	}

	spin_unlock_irqrestore(&buf->lock, flags);
=======
	if (disc == NULL)
		return;

	mutex_lock(&buf->lock);

	while (1) {
		struct tty_buffer *head = buf->head;
		struct tty_buffer *next;
		int count;

		/* Ldisc or user is trying to gain exclusive access */
		if (atomic_read(&buf->priority))
			break;

		/* paired w/ release in __tty_buffer_request_room();
		 * ensures commit value read is not stale if the head
		 * is advancing to the next buffer
		 */
		next = smp_load_acquire(&head->next);
		/* paired w/ release in __tty_buffer_request_room() or in
		 * tty_buffer_flush(); ensures we see the committed buffer data
		 */
		count = smp_load_acquire(&head->commit) - head->read;
		if (!count) {
			if (next == NULL)
				break;
			buf->head = next;
			tty_buffer_free(port, head);
			continue;
		}

		count = receive_buf(disc, head, count);
		if (!count)
			break;
		head->read += count;
	}

	mutex_unlock(&buf->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	tty_ldisc_deref(disc);
}

/**
<<<<<<< HEAD
 *	tty_flush_to_ldisc
 *	@tty: tty to push
 *
 *	Push the terminal flip buffers to the line discipline.
 *
 *	Must not be called from IRQ context.
 */
void tty_flush_to_ldisc(struct tty_struct *tty)
{
	if (!tty->port->low_latency)
		flush_work(&tty->port->buf.work);
}

/**
 *	tty_flip_buffer_push	-	terminal
 *	@port: tty port to push
 *
 *	Queue a push of the terminal flip buffers to the line discipline. This
 *	function must not be called from IRQ context if port->low_latency is
 *	set.
 *
 *	In the event of the queue being busy for flipping the work will be
 *	held off and retried later.
 *
 *	Locking: tty buffer lock. Driver locks in low latency mode.
=======
 *	tty_flip_buffer_push	-	terminal
 *	@port: tty port to push
 *
 *	Queue a push of the terminal flip buffers to the line discipline.
 *	Can be called from IRQ/atomic context.
 *
 *	In the event of the queue being busy for flipping the work will be
 *	held off and retried later.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

void tty_flip_buffer_push(struct tty_port *port)
{
<<<<<<< HEAD
	struct tty_bufhead *buf = &port->buf;
	unsigned long flags;

	spin_lock_irqsave(&buf->lock, flags);
	if (buf->tail != NULL)
		buf->tail->commit = buf->tail->used;
	spin_unlock_irqrestore(&buf->lock, flags);

	if (port->low_latency)
		flush_to_ldisc(&buf->work);
	else
		schedule_work(&buf->work);
=======
	tty_schedule_flip(port);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL(tty_flip_buffer_push);

/**
 *	tty_buffer_init		-	prepare a tty buffer structure
 *	@tty: tty to initialise
 *
 *	Set up the initial state of the buffer management for a tty device.
 *	Must be called before the other tty buffer functions are used.
<<<<<<< HEAD
 *
 *	Locking: none
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

void tty_buffer_init(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;

<<<<<<< HEAD
	spin_lock_init(&buf->lock);
	buf->head = NULL;
	buf->tail = NULL;
	buf->free = NULL;
	buf->memory_used = 0;
	INIT_WORK(&buf->work, flush_to_ldisc);
}

=======
	mutex_init(&buf->lock);
	tty_buffer_reset(&buf->sentinel, 0);
	buf->head = &buf->sentinel;
	buf->tail = &buf->sentinel;
	init_llist_head(&buf->free);
	atomic_set(&buf->mem_used, 0);
	atomic_set(&buf->priority, 0);
	INIT_WORK(&buf->work, flush_to_ldisc);
	buf->mem_limit = TTYB_DEFAULT_MEM_LIMIT;
}

/**
 *	tty_buffer_set_limit	-	change the tty buffer memory limit
 *	@port: tty port to change
 *
 *	Change the tty buffer memory limit.
 *	Must be called before the other tty buffer functions are used.
 */

int tty_buffer_set_limit(struct tty_port *port, int limit)
{
	if (limit < MIN_TTYB_SIZE)
		return -EINVAL;
	port->buf.mem_limit = limit;
	return 0;
}
EXPORT_SYMBOL_GPL(tty_buffer_set_limit);

/* slave ptys can claim nested buffer lock when handling BRK and INTR */
void tty_buffer_set_lock_subclass(struct tty_port *port)
{
	lockdep_set_subclass(&port->buf.lock, TTY_LOCK_SLAVE);
}

bool tty_buffer_restart_work(struct tty_port *port)
{
	return queue_work(system_unbound_wq, &port->buf.work);
}

bool tty_buffer_cancel_work(struct tty_port *port)
{
	return cancel_work_sync(&port->buf.work);
}

void tty_buffer_flush_work(struct tty_port *port)
{
	flush_work(&port->buf.work);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
