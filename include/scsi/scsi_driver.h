#ifndef _SCSI_SCSI_DRIVER_H
#define _SCSI_SCSI_DRIVER_H

#include <linux/device.h>

struct module;
<<<<<<< HEAD
struct scsi_cmnd;
struct scsi_device;
struct request;
struct request_queue;


struct scsi_driver {
	struct module		*owner;
	struct device_driver	gendrv;

	void (*rescan)(struct device *);
	int (*done)(struct scsi_cmnd *);
	int (*eh_action)(struct scsi_cmnd *, unsigned char *, int, int);
=======
struct request;
struct scsi_cmnd;
struct scsi_device;

struct scsi_driver {
	struct device_driver	gendrv;

	void (*rescan)(struct device *);
	int (*init_command)(struct scsi_cmnd *);
	void (*uninit_command)(struct scsi_cmnd *);
	int (*done)(struct scsi_cmnd *);
	int (*eh_action)(struct scsi_cmnd *, int);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
#define to_scsi_driver(drv) \
	container_of((drv), struct scsi_driver, gendrv)

extern int scsi_register_driver(struct device_driver *);
#define scsi_unregister_driver(drv) \
	driver_unregister(drv);

extern int scsi_register_interface(struct class_interface *);
#define scsi_unregister_interface(intf) \
	class_interface_unregister(intf)

<<<<<<< HEAD
int scsi_setup_blk_pc_cmnd(struct scsi_device *sdev, struct request *req);
int scsi_setup_fs_cmnd(struct scsi_device *sdev, struct request *req);
int scsi_prep_state_check(struct scsi_device *sdev, struct request *req);
int scsi_prep_return(struct request_queue *q, struct request *req, int ret);
int scsi_prep_fn(struct request_queue *, struct request *);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _SCSI_SCSI_DRIVER_H */
