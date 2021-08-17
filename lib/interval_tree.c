<<<<<<< HEAD
#include <linux/init.h>
#include <linux/interval_tree.h>
#include <linux/interval_tree_generic.h>
=======
#include <linux/interval_tree.h>
#include <linux/interval_tree_generic.h>
#include <linux/compiler.h>
#include <linux/export.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define START(node) ((node)->start)
#define LAST(node)  ((node)->last)

INTERVAL_TREE_DEFINE(struct interval_tree_node, rb,
		     unsigned long, __subtree_last,
		     START, LAST,, interval_tree)
<<<<<<< HEAD
=======

EXPORT_SYMBOL_GPL(interval_tree_insert);
EXPORT_SYMBOL_GPL(interval_tree_remove);
EXPORT_SYMBOL_GPL(interval_tree_iter_first);
EXPORT_SYMBOL_GPL(interval_tree_iter_next);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
