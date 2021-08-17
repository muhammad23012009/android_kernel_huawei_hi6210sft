# Rules for making the NTFS driver.

obj-$(CONFIG_NTFS_FS) += ntfs.o

ntfs-y := aops.o attrib.o collate.o compress.o debug.o dir.o file.o \
	  index.o inode.o mft.o mst.o namei.o runlist.o super.o sysctl.o \
	  unistr.o upcase.o

ntfs-$(CONFIG_NTFS_RW) += bitmap.o lcnalloc.o logfile.o quota.o usnjrnl.o

<<<<<<< HEAD
ccflags-y := -DNTFS_VERSION=\"2.1.30\"
=======
ccflags-y := -DNTFS_VERSION=\"2.1.32\"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
ccflags-$(CONFIG_NTFS_DEBUG)	+= -DDEBUG
ccflags-$(CONFIG_NTFS_RW)	+= -DNTFS_RW

