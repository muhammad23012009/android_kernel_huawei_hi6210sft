
<<<<<<< HEAD
extern void zorro_name_device(struct zorro_dev *z);
=======
#ifdef CONFIG_ZORRO_NAMES
extern void zorro_name_device(struct zorro_dev *z);
#else
static inline void zorro_name_device(struct zorro_dev *dev) { }
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int zorro_create_sysfs_dev_files(struct zorro_dev *z);

