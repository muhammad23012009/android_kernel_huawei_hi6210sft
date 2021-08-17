#include <linux/ceph/ceph_debug.h>

#include <linux/exportfs.h>
#include <linux/slab.h>
#include <asm/unaligned.h>

#include "super.h"
#include "mds_client.h"

/*
<<<<<<< HEAD
 * NFS export support
 *
 * NFS re-export of a ceph mount is, at present, only semireliable.
 * The basic issue is that the Ceph architectures doesn't lend itself
 * well to generating filehandles that will remain valid forever.
 *
 * So, we do our best.  If you're lucky, your inode will be in the
 * client's cache.  If it's not, and you have a connectable fh, then
 * the MDS server may be able to find it for you.  Otherwise, you get
 * ESTALE.
 *
 * There are ways to this more reliable, but in the non-connectable fh
 * case, we won't every work perfectly, and in the connectable case,
 * some changes are needed on the MDS side to work better.
 */

/*
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Basic fh
 */
struct ceph_nfs_fh {
	u64 ino;
} __attribute__ ((packed));

/*
<<<<<<< HEAD
 * Larger 'connectable' fh that includes parent ino and name hash.
 * Use this whenever possible, as it works more reliably.
 */
struct ceph_nfs_confh {
	u64 ino, parent_ino;
	u32 parent_name_hash;
} __attribute__ ((packed));

/*
 * The presence of @parent_inode here tells us whether NFS wants a
 * connectable file handle.  However, we want to make a connectionable
 * file handle unconditionally so that the MDS gets as much of a hint
 * as possible.  That means we only use @parent_dentry to indicate
 * whether nfsd wants a connectable fh, and whether we should indicate
 * failure from a too-small @max_len.
 */
=======
 * Larger fh that includes parent ino.
 */
struct ceph_nfs_confh {
	u64 ino, parent_ino;
} __attribute__ ((packed));

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int ceph_encode_fh(struct inode *inode, u32 *rawfh, int *max_len,
			  struct inode *parent_inode)
{
	int type;
	struct ceph_nfs_fh *fh = (void *)rawfh;
	struct ceph_nfs_confh *cfh = (void *)rawfh;
	int connected_handle_length = sizeof(*cfh)/4;
	int handle_length = sizeof(*fh)/4;
<<<<<<< HEAD
	struct dentry *dentry;
	struct dentry *parent;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* don't re-export snaps */
	if (ceph_snap(inode) != CEPH_NOSNAP)
		return -EINVAL;

<<<<<<< HEAD
	dentry = d_find_alias(inode);

	/* if we found an alias, generate a connectable fh */
	if (*max_len >= connected_handle_length && dentry) {
		dout("encode_fh %p connectable\n", dentry);
		spin_lock(&dentry->d_lock);
		parent = dentry->d_parent;
		cfh->ino = ceph_ino(inode);
		cfh->parent_ino = ceph_ino(parent->d_inode);
		cfh->parent_name_hash = ceph_dentry_hash(parent->d_inode,
							 dentry);
		*max_len = connected_handle_length;
		type = 2;
		spin_unlock(&dentry->d_lock);
	} else if (*max_len >= handle_length) {
		if (parent_inode) {
			/* nfsd wants connectable */
			*max_len = connected_handle_length;
			type = FILEID_INVALID;
		} else {
			dout("encode_fh %p\n", dentry);
			fh->ino = ceph_ino(inode);
			*max_len = handle_length;
			type = 1;
		}
	} else {
		*max_len = handle_length;
		type = FILEID_INVALID;
	}
	if (dentry)
		dput(dentry);
	return type;
}

/*
 * convert regular fh to dentry
 *
 * FIXME: we should try harder by querying the mds for the ino.
 */
static struct dentry *__fh_to_dentry(struct super_block *sb,
				     struct ceph_nfs_fh *fh, int fh_len)
=======
	if (parent_inode && (*max_len < connected_handle_length)) {
		*max_len = connected_handle_length;
		return FILEID_INVALID;
	} else if (*max_len < handle_length) {
		*max_len = handle_length;
		return FILEID_INVALID;
	}

	if (parent_inode) {
		dout("encode_fh %llx with parent %llx\n",
		     ceph_ino(inode), ceph_ino(parent_inode));
		cfh->ino = ceph_ino(inode);
		cfh->parent_ino = ceph_ino(parent_inode);
		*max_len = connected_handle_length;
		type = FILEID_INO32_GEN_PARENT;
	} else {
		dout("encode_fh %llx\n", ceph_ino(inode));
		fh->ino = ceph_ino(inode);
		*max_len = handle_length;
		type = FILEID_INO32_GEN;
	}
	return type;
}

static struct dentry *__fh_to_dentry(struct super_block *sb, u64 ino)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct ceph_mds_client *mdsc = ceph_sb_to_client(sb)->mdsc;
	struct inode *inode;
	struct dentry *dentry;
	struct ceph_vino vino;
	int err;

<<<<<<< HEAD
	if (fh_len < sizeof(*fh) / 4)
		return ERR_PTR(-ESTALE);

	dout("__fh_to_dentry %llx\n", fh->ino);
	vino.ino = fh->ino;
=======
	vino.ino = ino;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	vino.snap = CEPH_NOSNAP;
	inode = ceph_find_inode(sb, vino);
	if (!inode) {
		struct ceph_mds_request *req;
<<<<<<< HEAD
=======
		int mask;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		req = ceph_mdsc_create_request(mdsc, CEPH_MDS_OP_LOOKUPINO,
					       USE_ANY_MDS);
		if (IS_ERR(req))
			return ERR_CAST(req);

<<<<<<< HEAD
=======
		mask = CEPH_STAT_CAP_INODE;
		if (ceph_security_xattr_wanted(d_inode(sb->s_root)))
			mask |= CEPH_CAP_XATTR_SHARED;
		req->r_args.getattr.mask = cpu_to_le32(mask);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		req->r_ino1 = vino;
		req->r_num_caps = 1;
		err = ceph_mdsc_do_request(mdsc, NULL, req);
		inode = req->r_target_inode;
		if (inode)
			ihold(inode);
		ceph_mdsc_put_request(req);
		if (!inode)
			return ERR_PTR(-ESTALE);
	}

	dentry = d_obtain_alias(inode);
<<<<<<< HEAD
	if (IS_ERR(dentry)) {
		pr_err("fh_to_dentry %llx -- inode %p but ENOMEM\n",
		       fh->ino, inode);
		iput(inode);
		return dentry;
	}
	err = ceph_init_dentry(dentry);
	if (err < 0) {
		iput(inode);
		return ERR_PTR(err);
	}
	dout("__fh_to_dentry %llx %p dentry %p\n", fh->ino, inode, dentry);
=======
	if (IS_ERR(dentry))
		return dentry;
	err = ceph_init_dentry(dentry);
	if (err < 0) {
		dput(dentry);
		return ERR_PTR(err);
	}
	dout("__fh_to_dentry %llx %p dentry %p\n", ino, inode, dentry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return dentry;
}

/*
<<<<<<< HEAD
 * convert connectable fh to dentry
 */
static struct dentry *__cfh_to_dentry(struct super_block *sb,
				      struct ceph_nfs_confh *cfh, int fh_len)
{
	struct ceph_mds_client *mdsc = ceph_sb_to_client(sb)->mdsc;
	struct inode *inode;
	struct dentry *dentry;
	struct ceph_vino vino;
	int err;

	if (fh_len < sizeof(*cfh) / 4)
		return ERR_PTR(-ESTALE);

	dout("__cfh_to_dentry %llx (%llx/%x)\n",
	     cfh->ino, cfh->parent_ino, cfh->parent_name_hash);

	vino.ino = cfh->ino;
	vino.snap = CEPH_NOSNAP;
	inode = ceph_find_inode(sb, vino);
	if (!inode) {
		struct ceph_mds_request *req;

		req = ceph_mdsc_create_request(mdsc, CEPH_MDS_OP_LOOKUPHASH,
					       USE_ANY_MDS);
		if (IS_ERR(req))
			return ERR_CAST(req);

		req->r_ino1 = vino;
		req->r_ino2.ino = cfh->parent_ino;
		req->r_ino2.snap = CEPH_NOSNAP;
		req->r_path2 = kmalloc(16, GFP_NOFS);
		snprintf(req->r_path2, 16, "%d", cfh->parent_name_hash);
		req->r_num_caps = 1;
		err = ceph_mdsc_do_request(mdsc, NULL, req);
		inode = req->r_target_inode;
		if (inode)
			ihold(inode);
		ceph_mdsc_put_request(req);
		if (!inode)
			return ERR_PTR(err ? err : -ESTALE);
	}

	dentry = d_obtain_alias(inode);
	if (IS_ERR(dentry)) {
		pr_err("cfh_to_dentry %llx -- inode %p but ENOMEM\n",
		       cfh->ino, inode);
		iput(inode);
		return dentry;
	}
	err = ceph_init_dentry(dentry);
	if (err < 0) {
		iput(inode);
		return ERR_PTR(err);
	}
	dout("__cfh_to_dentry %llx %p dentry %p\n", cfh->ino, inode, dentry);
	return dentry;
}

static struct dentry *ceph_fh_to_dentry(struct super_block *sb, struct fid *fid,
					int fh_len, int fh_type)
{
	if (fh_type == 1)
		return __fh_to_dentry(sb, (struct ceph_nfs_fh *)fid->raw,
								fh_len);
	else
		return __cfh_to_dentry(sb, (struct ceph_nfs_confh *)fid->raw,
								fh_len);
}

/*
 * get parent, if possible.
 *
 * FIXME: we could do better by querying the mds to discover the
 * parent.
 */
static struct dentry *ceph_fh_to_parent(struct super_block *sb,
					 struct fid *fid,
					int fh_len, int fh_type)
{
	struct ceph_nfs_confh *cfh = (void *)fid->raw;
	struct ceph_vino vino;
	struct inode *inode;
	struct dentry *dentry;
	int err;

	if (fh_type == 1)
		return ERR_PTR(-ESTALE);
	if (fh_len < sizeof(*cfh) / 4)
		return ERR_PTR(-ESTALE);

	pr_debug("fh_to_parent %llx/%d\n", cfh->parent_ino,
		 cfh->parent_name_hash);

	vino.ino = cfh->ino;
	vino.snap = CEPH_NOSNAP;
	inode = ceph_find_inode(sb, vino);
	if (!inode)
		return ERR_PTR(-ESTALE);

	dentry = d_obtain_alias(inode);
	if (IS_ERR(dentry)) {
		pr_err("fh_to_parent %llx -- inode %p but ENOMEM\n",
		       cfh->ino, inode);
		iput(inode);
		return dentry;
	}
	err = ceph_init_dentry(dentry);
	if (err < 0) {
		iput(inode);
		return ERR_PTR(err);
	}
	dout("fh_to_parent %llx %p dentry %p\n", cfh->ino, inode, dentry);
	return dentry;
=======
 * convert regular fh to dentry
 */
static struct dentry *ceph_fh_to_dentry(struct super_block *sb,
					struct fid *fid,
					int fh_len, int fh_type)
{
	struct ceph_nfs_fh *fh = (void *)fid->raw;

	if (fh_type != FILEID_INO32_GEN  &&
	    fh_type != FILEID_INO32_GEN_PARENT)
		return NULL;
	if (fh_len < sizeof(*fh) / 4)
		return NULL;

	dout("fh_to_dentry %llx\n", fh->ino);
	return __fh_to_dentry(sb, fh->ino);
}

static struct dentry *__get_parent(struct super_block *sb,
				   struct dentry *child, u64 ino)
{
	struct ceph_mds_client *mdsc = ceph_sb_to_client(sb)->mdsc;
	struct ceph_mds_request *req;
	struct inode *inode;
	struct dentry *dentry;
	int mask;
	int err;

	req = ceph_mdsc_create_request(mdsc, CEPH_MDS_OP_LOOKUPPARENT,
				       USE_ANY_MDS);
	if (IS_ERR(req))
		return ERR_CAST(req);

	if (child) {
		req->r_inode = d_inode(child);
		ihold(d_inode(child));
	} else {
		req->r_ino1 = (struct ceph_vino) {
			.ino = ino,
			.snap = CEPH_NOSNAP,
		};
	}

	mask = CEPH_STAT_CAP_INODE;
	if (ceph_security_xattr_wanted(d_inode(sb->s_root)))
		mask |= CEPH_CAP_XATTR_SHARED;
	req->r_args.getattr.mask = cpu_to_le32(mask);

	req->r_num_caps = 1;
	err = ceph_mdsc_do_request(mdsc, NULL, req);
	if (err) {
		ceph_mdsc_put_request(req);
		return ERR_PTR(err);
	}

	inode = req->r_target_inode;
	if (inode)
		ihold(inode);
	ceph_mdsc_put_request(req);
	if (!inode)
		return ERR_PTR(-ENOENT);

	dentry = d_obtain_alias(inode);
	if (IS_ERR(dentry))
		return dentry;
	err = ceph_init_dentry(dentry);
	if (err < 0) {
		dput(dentry);
		return ERR_PTR(err);
	}
	dout("__get_parent ino %llx parent %p ino %llx.%llx\n",
	     child ? ceph_ino(d_inode(child)) : ino,
	     dentry, ceph_vinop(inode));
	return dentry;
}

static struct dentry *ceph_get_parent(struct dentry *child)
{
	/* don't re-export snaps */
	if (ceph_snap(d_inode(child)) != CEPH_NOSNAP)
		return ERR_PTR(-EINVAL);

	dout("get_parent %p ino %llx.%llx\n",
	     child, ceph_vinop(d_inode(child)));
	return __get_parent(child->d_sb, child, 0);
}

/*
 * convert regular fh to parent
 */
static struct dentry *ceph_fh_to_parent(struct super_block *sb,
					struct fid *fid,
					int fh_len, int fh_type)
{
	struct ceph_nfs_confh *cfh = (void *)fid->raw;
	struct dentry *dentry;

	if (fh_type != FILEID_INO32_GEN_PARENT)
		return NULL;
	if (fh_len < sizeof(*cfh) / 4)
		return NULL;

	dout("fh_to_parent %llx\n", cfh->parent_ino);
	dentry = __get_parent(sb, NULL, cfh->ino);
	if (unlikely(dentry == ERR_PTR(-ENOENT)))
		dentry = __fh_to_dentry(sb, cfh->parent_ino);
	return dentry;
}

static int ceph_get_name(struct dentry *parent, char *name,
			 struct dentry *child)
{
	struct ceph_mds_client *mdsc;
	struct ceph_mds_request *req;
	int err;

	mdsc = ceph_inode_to_client(d_inode(child))->mdsc;
	req = ceph_mdsc_create_request(mdsc, CEPH_MDS_OP_LOOKUPNAME,
				       USE_ANY_MDS);
	if (IS_ERR(req))
		return PTR_ERR(req);

	inode_lock(d_inode(parent));

	req->r_inode = d_inode(child);
	ihold(d_inode(child));
	req->r_ino2 = ceph_vino(d_inode(parent));
	req->r_locked_dir = d_inode(parent);
	req->r_num_caps = 2;
	err = ceph_mdsc_do_request(mdsc, NULL, req);

	inode_unlock(d_inode(parent));

	if (!err) {
		struct ceph_mds_reply_info_parsed *rinfo = &req->r_reply_info;
		memcpy(name, rinfo->dname, rinfo->dname_len);
		name[rinfo->dname_len] = 0;
		dout("get_name %p ino %llx.%llx name %s\n",
		     child, ceph_vinop(d_inode(child)), name);
	} else {
		dout("get_name %p ino %llx.%llx err %d\n",
		     child, ceph_vinop(d_inode(child)), err);
	}

	ceph_mdsc_put_request(req);
	return err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

const struct export_operations ceph_export_ops = {
	.encode_fh = ceph_encode_fh,
	.fh_to_dentry = ceph_fh_to_dentry,
	.fh_to_parent = ceph_fh_to_parent,
<<<<<<< HEAD
=======
	.get_parent = ceph_get_parent,
	.get_name = ceph_get_name,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
