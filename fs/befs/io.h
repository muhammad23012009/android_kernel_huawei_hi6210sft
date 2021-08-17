/*
 * io.h
 */

struct buffer_head *befs_bread_iaddr(struct super_block *sb,
				     befs_inode_addr iaddr);

<<<<<<< HEAD
struct buffer_head *befs_bread(struct super_block *sb, befs_blocknr_t block);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
