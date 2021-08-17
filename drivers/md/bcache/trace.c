#include "bcache.h"
#include "btree.h"
<<<<<<< HEAD
#include "request.h"

=======

#include <linux/blktrace_api.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/module.h>

#define CREATE_TRACE_POINTS
#include <trace/events/bcache.h>

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_request_start);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_request_end);
<<<<<<< HEAD
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_passthrough);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_cache_hit);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_cache_miss);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_read_retry);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_writethrough);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_writeback);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_write_skip);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_read);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_write);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_write_dirty);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_read_dirty);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_journal_write);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_cache_insert);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_gc_start);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_gc_end);
=======

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_bypass_sequential);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_bypass_congested);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_read);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_write);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_read_retry);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_cache_insert);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_journal_replay_key);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_journal_write);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_journal_full);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_journal_entry_full);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_cache_cannibalize);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_read);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_write);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_node_alloc);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_node_alloc_fail);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_node_free);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_gc_coalesce);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_gc_start);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_gc_end);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_gc_copy);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_gc_copy_collision);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_insert_key);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_node_split);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_node_compact);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_btree_set_root);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_invalidate);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_alloc_fail);

EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_writeback);
EXPORT_TRACEPOINT_SYMBOL_GPL(bcache_writeback_collision);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
