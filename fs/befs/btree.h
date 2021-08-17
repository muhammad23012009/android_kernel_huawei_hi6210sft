/*
 * btree.h
 * 
 */


<<<<<<< HEAD
int befs_btree_find(struct super_block *sb, befs_data_stream * ds,
		    const char *key, befs_off_t * value);

int befs_btree_read(struct super_block *sb, befs_data_stream * ds,
=======
int befs_btree_find(struct super_block *sb, const befs_data_stream *ds,
		    const char *key, befs_off_t * value);

int befs_btree_read(struct super_block *sb, const befs_data_stream *ds,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		    loff_t key_no, size_t bufsize, char *keybuf,
		    size_t * keysize, befs_off_t * value);

