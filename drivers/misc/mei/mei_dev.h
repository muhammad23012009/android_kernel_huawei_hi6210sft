/*
 *
 * Intel Management Engine Interface (Intel MEI) Linux driver
 * Copyright (c) 2003-2012, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#ifndef _MEI_DEV_H_
#define _MEI_DEV_H_

#include <linux/types.h>
<<<<<<< HEAD
#include <linux/watchdog.h>
=======
#include <linux/cdev.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/poll.h>
#include <linux/mei.h>
#include <linux/mei_cl_bus.h>

#include "hw.h"
<<<<<<< HEAD
#include "hw-me-regs.h"
#include "hbm.h"

/*
 * watch dog definition
 */
#define MEI_WD_HDR_SIZE       4
#define MEI_WD_STOP_MSG_SIZE  MEI_WD_HDR_SIZE
#define MEI_WD_START_MSG_SIZE (MEI_WD_HDR_SIZE + 16)

#define MEI_WD_DEFAULT_TIMEOUT   120  /* seconds */
#define MEI_WD_MIN_TIMEOUT       120  /* seconds */
#define MEI_WD_MAX_TIMEOUT     65535  /* seconds */

#define MEI_WD_STOP_TIMEOUT      10 /* msecs */

#define MEI_WD_STATE_INDEPENDENCE_MSG_SENT       (1 << 0)

#define MEI_RD_MSG_BUF_SIZE           (128 * sizeof(u32))

=======
#include "hbm.h"

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * AMTHI Client UUID
 */
extern const uuid_le mei_amthif_guid;

<<<<<<< HEAD
/*
 * Watchdog Client UUID
 */
extern const uuid_le mei_wd_guid;

/*
 * Watchdog independence state message
 */
extern const u8 mei_wd_state_independence_msg[3][4];

/*
 * Number of Maximum MEI Clients
 */
#define MEI_CLIENTS_MAX 256
=======
#define MEI_RD_MSG_BUF_SIZE           (128 * sizeof(u32))

/*
 * Number of Maximum MEI Clients
 */
#define MEI_CLIENTS_MAX 256

/*
 * maximum number of consecutive resets
 */
#define MEI_MAX_CONSEC_RESET  3
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Number of File descriptors/handles
 * that can be opened to the driver.
 *
 * Limit to 255: 256 Total Clients
<<<<<<< HEAD
 * minus internal client for MEI Bus Messags
 */
#define  MEI_MAX_OPEN_HANDLE_COUNT (MEI_CLIENTS_MAX - 1)

/*
 * Internal Clients Number
 */
#define MEI_HOST_CLIENT_ID_ANY        (-1)
#define MEI_HBM_HOST_CLIENT_ID         0 /* not used, just for documentation */
#define MEI_WD_HOST_CLIENT_ID          1
#define MEI_IAMTHIF_HOST_CLIENT_ID     2


=======
 * minus internal client for MEI Bus Messages
 */
#define  MEI_MAX_OPEN_HANDLE_COUNT (MEI_CLIENTS_MAX - 1)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* File state */
enum file_state {
	MEI_FILE_INITIALIZING = 0,
	MEI_FILE_CONNECTING,
	MEI_FILE_CONNECTED,
	MEI_FILE_DISCONNECTING,
<<<<<<< HEAD
	MEI_FILE_DISCONNECTED
=======
	MEI_FILE_DISCONNECT_REPLY,
	MEI_FILE_DISCONNECT_REQUIRED,
	MEI_FILE_DISCONNECTED,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* MEI device states */
enum mei_dev_state {
	MEI_DEV_INITIALIZING = 0,
	MEI_DEV_INIT_CLIENTS,
	MEI_DEV_ENABLED,
	MEI_DEV_RESETTING,
	MEI_DEV_DISABLED,
	MEI_DEV_POWER_DOWN,
	MEI_DEV_POWER_UP
};

const char *mei_dev_state_str(int state);

enum iamthif_states {
	MEI_IAMTHIF_IDLE,
	MEI_IAMTHIF_WRITING,
<<<<<<< HEAD
	MEI_IAMTHIF_FLOW_CONTROL,
	MEI_IAMTHIF_READING,
	MEI_IAMTHIF_READ_COMPLETE
=======
	MEI_IAMTHIF_READING,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

enum mei_file_transaction_states {
	MEI_IDLE,
	MEI_WRITING,
	MEI_WRITE_COMPLETE,
<<<<<<< HEAD
	MEI_FLOW_CONTROL,
	MEI_READING,
	MEI_READ_COMPLETE
};

enum mei_wd_states {
	MEI_WD_IDLE,
	MEI_WD_RUNNING,
	MEI_WD_STOPPING,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/**
 * enum mei_cb_file_ops  - file operation associated with the callback
<<<<<<< HEAD
 * @MEI_FOP_READ   - read
 * @MEI_FOP_WRITE  - write
 * @MEI_FOP_IOCTL  - ioctl
 * @MEI_FOP_OPEN   - open
 * @MEI_FOP_CLOSE  - close
=======
 * @MEI_FOP_READ:       read
 * @MEI_FOP_WRITE:      write
 * @MEI_FOP_CONNECT:    connect
 * @MEI_FOP_DISCONNECT: disconnect
 * @MEI_FOP_DISCONNECT_RSP: disconnect response
 * @MEI_FOP_NOTIFY_START:   start notification
 * @MEI_FOP_NOTIFY_STOP:    stop notification
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
enum mei_cb_file_ops {
	MEI_FOP_READ = 0,
	MEI_FOP_WRITE,
<<<<<<< HEAD
	MEI_FOP_IOCTL,
	MEI_FOP_OPEN,
	MEI_FOP_CLOSE
=======
	MEI_FOP_CONNECT,
	MEI_FOP_DISCONNECT,
	MEI_FOP_DISCONNECT_RSP,
	MEI_FOP_NOTIFY_START,
	MEI_FOP_NOTIFY_STOP,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/*
 * Intel MEI message data struct
 */
struct mei_msg_data {
<<<<<<< HEAD
	u32 size;
	unsigned char *data;
};

/**
 * struct mei_me_client - representation of me (fw) client
 *
 * @props  - client properties
 * @client_id - me client id
 * @mei_flow_ctrl_creds - flow control credits
 */
struct mei_me_client {
	struct mei_client_properties props;
	u8 client_id;
	u8 mei_flow_ctrl_creds;
=======
	size_t size;
	unsigned char *data;
};

/* Maximum number of processed FW status registers */
#define MEI_FW_STATUS_MAX 6
/* Minimal  buffer for FW status string (8 bytes in dw + space or '\0') */
#define MEI_FW_STATUS_STR_SZ (MEI_FW_STATUS_MAX * (8 + 1))


/*
 * struct mei_fw_status - storage of FW status data
 *
 * @count: number of actually available elements in array
 * @status: FW status registers
 */
struct mei_fw_status {
	int count;
	u32 status[MEI_FW_STATUS_MAX];
};

/**
 * struct mei_me_client - representation of me (fw) client
 *
 * @list: link in me client list
 * @refcnt: struct reference count
 * @props: client properties
 * @client_id: me client id
 * @tx_flow_ctrl_creds: flow control credits
 * @connect_count: number connections to this client
 * @bus_added: added to bus
 */
struct mei_me_client {
	struct list_head list;
	struct kref refcnt;
	struct mei_client_properties props;
	u8 client_id;
	u8 tx_flow_ctrl_creds;
	u8 connect_count;
	u8 bus_added;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};


struct mei_cl;

/**
 * struct mei_cl_cb - file operation callback structure
 *
<<<<<<< HEAD
 * @cl - file client who is running this operation
 * @fop_type - file operation type
=======
 * @list: link in callback queue
 * @cl: file client who is running this operation
 * @fop_type: file operation type
 * @buf: buffer for data associated with the callback
 * @buf_idx: last read index
 * @fp: pointer to file structure
 * @status: io status of the cb
 * @internal: communication between driver and FW flag
 * @completed: the transfer or reception has completed
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
struct mei_cl_cb {
	struct list_head list;
	struct mei_cl *cl;
	enum mei_cb_file_ops fop_type;
<<<<<<< HEAD
	struct mei_msg_data request_buffer;
	struct mei_msg_data response_buffer;
	unsigned long buf_idx;
	unsigned long read_time;
	struct file *file_object;
};

/* MEI client instance carried as file->pirvate_data*/
=======
	struct mei_msg_data buf;
	size_t buf_idx;
	const struct file *fp;
	int status;
	u32 internal:1;
	u32 completed:1;
};

/**
 * struct mei_cl - me client host representation
 *    carried in file->private_data
 *
 * @link: link in the clients list
 * @dev: mei parent device
 * @state: file operation state
 * @tx_wait: wait queue for tx completion
 * @rx_wait: wait queue for rx completion
 * @wait:  wait queue for management operation
 * @ev_wait: notification wait queue
 * @ev_async: event async notification
 * @status: connection status
 * @me_cl: fw client connected
 * @fp: file associated with client
 * @host_client_id: host id
 * @tx_flow_ctrl_creds: transmit flow credentials
 * @rx_flow_ctrl_creds: receive flow credentials
 * @timer_count:  watchdog timer for operation completion
 * @notify_en: notification - enabled/disabled
 * @notify_ev: pending notification event
 * @writing_state: state of the tx
 * @rd_pending: pending read credits
 * @rd_completed: completed read
 *
 * @cldev: device on the mei client bus
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct mei_cl {
	struct list_head link;
	struct mei_device *dev;
	enum file_state state;
	wait_queue_head_t tx_wait;
	wait_queue_head_t rx_wait;
	wait_queue_head_t wait;
<<<<<<< HEAD
	int status;
	/* ID of client connected */
	u8 host_client_id;
	u8 me_client_id;
	u8 mei_flow_ctrl_creds;
	u8 timer_count;
	enum mei_file_transaction_states reading_state;
	enum mei_file_transaction_states writing_state;
	int sm_state;
	struct mei_cl_cb *read_cb;

	/* MEI CL bus data */
	struct mei_cl_device *device;
	struct list_head device_link;
	uuid_le device_uuid;
};

/** struct mei_hw_ops
 *
 * @host_is_ready    - query for host readiness

 * @hw_is_ready      - query if hw is ready
 * @hw_reset         - reset hw
 * @hw_start         - start hw after reset
 * @hw_config        - configure hw

 * @intr_clear       - clear pending interrupts
 * @intr_enable      - enable interrupts
 * @intr_disable     - disable interrupts

 * @hbuf_free_slots  - query for write buffer empty slots
 * @hbuf_is_ready    - query if write buffer is empty
 * @hbuf_max_len     - query for write buffer max len

 * @write            - write a message to FW

 * @rdbuf_full_slots - query how many slots are filled

 * @read_hdr         - get first 4 bytes (header)
 * @read             - read a buffer from the FW
 */
struct mei_hw_ops {

	bool (*host_is_ready) (struct mei_device *dev);

	bool (*hw_is_ready) (struct mei_device *dev);
	void (*hw_reset) (struct mei_device *dev, bool enable);
	int  (*hw_start) (struct mei_device *dev);
	void (*hw_config) (struct mei_device *dev);

	void (*intr_clear) (struct mei_device *dev);
	void (*intr_enable) (struct mei_device *dev);
	void (*intr_disable) (struct mei_device *dev);

	int (*hbuf_free_slots) (struct mei_device *dev);
	bool (*hbuf_is_ready) (struct mei_device *dev);
	size_t (*hbuf_max_len) (const struct mei_device *dev);
=======
	wait_queue_head_t ev_wait;
	struct fasync_struct *ev_async;
	int status;
	struct mei_me_client *me_cl;
	const struct file *fp;
	u8 host_client_id;
	u8 tx_flow_ctrl_creds;
	u8 rx_flow_ctrl_creds;
	u8 timer_count;
	u8 notify_en;
	u8 notify_ev;
	enum mei_file_transaction_states writing_state;
	struct list_head rd_pending;
	struct list_head rd_completed;

	struct mei_cl_device *cldev;
};

/**
 * struct mei_hw_ops - hw specific ops
 *
 * @host_is_ready    : query for host readiness
 *
 * @hw_is_ready      : query if hw is ready
 * @hw_reset         : reset hw
 * @hw_start         : start hw after reset
 * @hw_config        : configure hw
 *
 * @fw_status        : get fw status registers
 * @pg_state         : power gating state of the device
 * @pg_in_transition : is device now in pg transition
 * @pg_is_enabled    : is power gating enabled
 *
 * @intr_clear       : clear pending interrupts
 * @intr_enable      : enable interrupts
 * @intr_disable     : disable interrupts
 *
 * @hbuf_free_slots  : query for write buffer empty slots
 * @hbuf_is_ready    : query if write buffer is empty
 * @hbuf_max_len     : query for write buffer max len
 *
 * @write            : write a message to FW
 *
 * @rdbuf_full_slots : query how many slots are filled
 *
 * @read_hdr         : get first 4 bytes (header)
 * @read             : read a buffer from the FW
 */
struct mei_hw_ops {

	bool (*host_is_ready)(struct mei_device *dev);

	bool (*hw_is_ready)(struct mei_device *dev);
	int (*hw_reset)(struct mei_device *dev, bool enable);
	int (*hw_start)(struct mei_device *dev);
	void (*hw_config)(struct mei_device *dev);


	int (*fw_status)(struct mei_device *dev, struct mei_fw_status *fw_sts);
	enum mei_pg_state (*pg_state)(struct mei_device *dev);
	bool (*pg_in_transition)(struct mei_device *dev);
	bool (*pg_is_enabled)(struct mei_device *dev);

	void (*intr_clear)(struct mei_device *dev);
	void (*intr_enable)(struct mei_device *dev);
	void (*intr_disable)(struct mei_device *dev);

	int (*hbuf_free_slots)(struct mei_device *dev);
	bool (*hbuf_is_ready)(struct mei_device *dev);
	size_t (*hbuf_max_len)(const struct mei_device *dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	int (*write)(struct mei_device *dev,
		     struct mei_msg_hdr *hdr,
		     unsigned char *buf);

	int (*rdbuf_full_slots)(struct mei_device *dev);

	u32 (*read_hdr)(const struct mei_device *dev);
<<<<<<< HEAD
	int (*read) (struct mei_device *dev,
=======
	int (*read)(struct mei_device *dev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		     unsigned char *buf, unsigned long len);
};

/* MEI bus API*/
<<<<<<< HEAD

/**
 * struct mei_cl_ops - MEI CL device ops
 * This structure allows ME host clients to implement technology
 * specific operations.
 *
 * @enable: Enable an MEI CL device. Some devices require specific
 *	HECI commands to initialize completely.
 * @disable: Disable an MEI CL device.
 * @send: Tx hook for the device. This allows ME host clients to trap
 *	the device driver buffers before actually physically
 *	pushing it to the ME.
 * @recv: Rx hook for the device. This allows ME host clients to trap the
 *	ME buffers before forwarding them to the device driver.
 */
struct mei_cl_ops {
	int (*enable)(struct mei_cl_device *device);
	int (*disable)(struct mei_cl_device *device);
	int (*send)(struct mei_cl_device *device, u8 *buf, size_t length);
	int (*recv)(struct mei_cl_device *device, u8 *buf, size_t length);
};

struct mei_cl_device *mei_cl_add_device(struct mei_device *dev,
					uuid_le uuid, char *name,
					struct mei_cl_ops *ops);
void mei_cl_remove_device(struct mei_cl_device *device);

int __mei_cl_async_send(struct mei_cl *cl, u8 *buf, size_t length);
int __mei_cl_send(struct mei_cl *cl, u8 *buf, size_t length);
int __mei_cl_recv(struct mei_cl *cl, u8 *buf, size_t length);
void mei_cl_bus_rx_event(struct mei_cl *cl);
int mei_cl_bus_init(void);
void mei_cl_bus_exit(void);


/**
 * struct mei_cl_device - MEI device handle
 * An mei_cl_device pointer is returned from mei_add_device()
 * and links MEI bus clients to their actual ME host client pointer.
 * Drivers for MEI devices will get an mei_cl_device pointer
 * when being probed and shall use it for doing ME bus I/O.
 *
 * @dev: linux driver model device pointer
 * @uuid: me client uuid
 * @cl: mei client
 * @ops: ME transport ops
 * @event_cb: Drivers register this callback to get asynchronous ME
 *	events (e.g. Rx buffer pending) notifications.
 * @events: Events bitmask sent to the driver.
 * @priv_data: client private data
 */
struct mei_cl_device {
	struct device dev;

	struct mei_cl *cl;

	const struct mei_cl_ops *ops;

	struct work_struct event_work;
	mei_cl_event_cb_t event_cb;
	void *event_context;
	unsigned long events;

	void *priv_data;
};

/**
 * struct mei_device -  MEI private device struct

 * @hbm_state - state of host bus message protocol
 * @mem_addr - mem mapped base register address

 * @hbuf_depth - depth of hardware host/write buffer is slots
 * @hbuf_is_ready - query if the host host/write buffer is ready
 * @wr_msg - the buffer for hbm control messages
 * @wr_ext_msg - the buffer for hbm control responses (set in read cycle)
 */
struct mei_device {
	struct pci_dev *pdev;	/* pointer to pci device struct */
	/*
	 * lists of queues
	 */
	/* array of pointers to aio lists */
	struct mei_cl_cb read_list;		/* driver read queue */
	struct mei_cl_cb write_list;		/* driver write queue */
	struct mei_cl_cb write_waiting_list;	/* write waiting queue */
	struct mei_cl_cb ctrl_wr_list;		/* managed write IOCTL list */
	struct mei_cl_cb ctrl_rd_list;		/* managed read IOCTL list */

	/*
	 * list of files
	 */
	struct list_head file_list;
	long open_handle_count;

	/*
	 * lock for the device
	 */
	struct mutex device_lock; /* device lock */
	struct delayed_work timer_work;	/* MEI timer delayed work (timeouts) */
=======
void mei_cl_bus_rescan(struct mei_device *bus);
void mei_cl_bus_rescan_work(struct work_struct *work);
void mei_cl_bus_dev_fixup(struct mei_cl_device *dev);
ssize_t __mei_cl_send(struct mei_cl *cl, u8 *buf, size_t length,
			bool blocking);
ssize_t __mei_cl_recv(struct mei_cl *cl, u8 *buf, size_t length);
bool mei_cl_bus_rx_event(struct mei_cl *cl);
bool mei_cl_bus_notify_event(struct mei_cl *cl);
void mei_cl_bus_remove_devices(struct mei_device *bus);
int mei_cl_bus_init(void);
void mei_cl_bus_exit(void);

/**
 * enum mei_pg_event - power gating transition events
 *
 * @MEI_PG_EVENT_IDLE: the driver is not in power gating transition
 * @MEI_PG_EVENT_WAIT: the driver is waiting for a pg event to complete
 * @MEI_PG_EVENT_RECEIVED: the driver received pg event
 * @MEI_PG_EVENT_INTR_WAIT: the driver is waiting for a pg event interrupt
 * @MEI_PG_EVENT_INTR_RECEIVED: the driver received pg event interrupt
 */
enum mei_pg_event {
	MEI_PG_EVENT_IDLE,
	MEI_PG_EVENT_WAIT,
	MEI_PG_EVENT_RECEIVED,
	MEI_PG_EVENT_INTR_WAIT,
	MEI_PG_EVENT_INTR_RECEIVED,
};

/**
 * enum mei_pg_state - device internal power gating state
 *
 * @MEI_PG_OFF: device is not power gated - it is active
 * @MEI_PG_ON:  device is power gated - it is in lower power state
 */
enum mei_pg_state {
	MEI_PG_OFF = 0,
	MEI_PG_ON =  1,
};

const char *mei_pg_state_str(enum mei_pg_state state);

/**
 * struct mei_device -  MEI private device struct
 *
 * @dev         : device on a bus
 * @cdev        : character device
 * @minor       : minor number allocated for device
 *
 * @write_list  : write pending list
 * @write_waiting_list : write completion list
 * @ctrl_wr_list : pending control write list
 * @ctrl_rd_list : pending control read list
 *
 * @file_list   : list of opened handles
 * @open_handle_count: number of opened handles
 *
 * @device_lock : big device lock
 * @timer_work  : MEI timer delayed work (timeouts)
 *
 * @recvd_hw_ready : hw ready message received flag
 *
 * @wait_hw_ready : wait queue for receive HW ready message form FW
 * @wait_pg     : wait queue for receive PG message from FW
 * @wait_hbm_start : wait queue for receive HBM start message from FW
 *
 * @reset_count : number of consecutive resets
 * @dev_state   : device state
 * @hbm_state   : state of host bus message protocol
 * @init_clients_timer : HBM init handshake timeout
 *
 * @pg_event    : power gating event
 * @pg_domain   : runtime PM domain
 *
 * @rd_msg_buf  : control messages buffer
 * @rd_msg_hdr  : read message header storage
 *
 * @hbuf_depth  : depth of hardware host/write buffer is slots
 * @hbuf_is_ready : query if the host host/write buffer is ready
 *
 * @version     : HBM protocol version in use
 * @hbm_f_pg_supported  : hbm feature pgi protocol
 * @hbm_f_dc_supported  : hbm feature dynamic clients
 * @hbm_f_dot_supported : hbm feature disconnect on timeout
 * @hbm_f_ev_supported  : hbm feature event notification
 * @hbm_f_fa_supported  : hbm feature fixed address client
 * @hbm_f_ie_supported  : hbm feature immediate reply to enum request
 *
 * @me_clients_rwsem: rw lock over me_clients list
 * @me_clients  : list of FW clients
 * @me_clients_map : FW clients bit map
 * @host_clients_map : host clients id pool
 *
 * @allow_fixed_address: allow user space to connect a fixed client
 * @override_fixed_address: force allow fixed address behavior
 *
 * @amthif_cmd_list : amthif list for cmd waiting
 * @iamthif_cl  : amthif host client
 * @iamthif_open_count : number of opened amthif connections
 * @iamthif_stall_timer : timer to detect amthif hang
 * @iamthif_state : amthif processor state
 * @iamthif_canceled : current amthif command is canceled
 *
 * @reset_work  : work item for the device reset
 * @bus_rescan_work : work item for the bus rescan
 *
 * @device_list : mei client bus list
 * @cl_bus_lock : client bus list lock
 *
 * @dbgfs_dir   : debugfs mei root directory
 *
 * @ops:        : hw specific operations
 * @hw          : hw specific data
 */
struct mei_device {
	struct device *dev;
	struct cdev cdev;
	int minor;

	struct mei_cl_cb write_list;
	struct mei_cl_cb write_waiting_list;
	struct mei_cl_cb ctrl_wr_list;
	struct mei_cl_cb ctrl_rd_list;

	struct list_head file_list;
	long open_handle_count;

	struct mutex device_lock;
	struct delayed_work timer_work;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	bool recvd_hw_ready;
	/*
	 * waiting queue for receive message from FW
	 */
	wait_queue_head_t wait_hw_ready;
<<<<<<< HEAD
	wait_queue_head_t wait_recvd_msg;
	wait_queue_head_t wait_stop_wd;
=======
	wait_queue_head_t wait_pg;
	wait_queue_head_t wait_hbm_start;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * mei device  states
	 */
<<<<<<< HEAD
=======
	unsigned long reset_count;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	enum mei_dev_state dev_state;
	enum mei_hbm_state hbm_state;
	u16 init_clients_timer;

<<<<<<< HEAD
	unsigned char rd_msg_buf[MEI_RD_MSG_BUF_SIZE];	/* control messages */
=======
	/*
	 * Power Gating support
	 */
	enum mei_pg_event pg_event;
#ifdef CONFIG_PM
	struct dev_pm_domain pg_domain;
#endif /* CONFIG_PM */

	unsigned char rd_msg_buf[MEI_RD_MSG_BUF_SIZE];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32 rd_msg_hdr;

	/* write buffer */
	u8 hbuf_depth;
	bool hbuf_is_ready;

<<<<<<< HEAD
	/* used for control messages */
	struct {
		struct mei_msg_hdr hdr;
		unsigned char data[128];
	} wr_msg;

	struct {
		struct mei_msg_hdr hdr;
		unsigned char data[4];	/* All HBM messages are 4 bytes */
	} wr_ext_msg;		/* for control responses */

	struct hbm_version version;

	struct mei_me_client *me_clients; /* Note: memory has to be allocated */
	DECLARE_BITMAP(me_clients_map, MEI_CLIENTS_MAX);
	DECLARE_BITMAP(host_clients_map, MEI_CLIENTS_MAX);
	unsigned long me_clients_num;
	unsigned long me_client_presentation_num;
	unsigned long me_client_index;

	struct mei_cl wd_cl;
	enum mei_wd_states wd_state;
	bool wd_pending;
	u16 wd_timeout;
	unsigned char wd_data[MEI_WD_START_MSG_SIZE];


	/* amthif list for cmd waiting */
	struct mei_cl_cb amthif_cmd_list;
	/* driver managed amthif list for reading completed amthif cmd data */
	struct mei_cl_cb amthif_rd_complete_list;
	struct file *iamthif_file_object;
	struct mei_cl iamthif_cl;
	struct mei_cl_cb *iamthif_current_cb;
	int iamthif_mtu;
	unsigned long iamthif_timer;
	u32 iamthif_stall_timer;
	unsigned char *iamthif_msg_buf; /* Note: memory has to be allocated */
	u32 iamthif_msg_buf_size;
	u32 iamthif_msg_buf_index;
	enum iamthif_states iamthif_state;
	bool iamthif_flow_control_pending;
	bool iamthif_ioctl;
	bool iamthif_canceled;

	struct work_struct init_work;

	/* List of bus devices */
	struct list_head device_list;
=======
	struct hbm_version version;
	unsigned int hbm_f_pg_supported:1;
	unsigned int hbm_f_dc_supported:1;
	unsigned int hbm_f_dot_supported:1;
	unsigned int hbm_f_ev_supported:1;
	unsigned int hbm_f_fa_supported:1;
	unsigned int hbm_f_ie_supported:1;

	struct rw_semaphore me_clients_rwsem;
	struct list_head me_clients;
	DECLARE_BITMAP(me_clients_map, MEI_CLIENTS_MAX);
	DECLARE_BITMAP(host_clients_map, MEI_CLIENTS_MAX);

	bool allow_fixed_address;
	bool override_fixed_address;

	/* amthif list for cmd waiting */
	struct mei_cl_cb amthif_cmd_list;
	struct mei_cl iamthif_cl;
	long iamthif_open_count;
	u32 iamthif_stall_timer;
	enum iamthif_states iamthif_state;
	bool iamthif_canceled;

	struct work_struct reset_work;
	struct work_struct bus_rescan_work;

	/* List of bus devices */
	struct list_head device_list;
	struct mutex cl_bus_lock;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#if IS_ENABLED(CONFIG_DEBUG_FS)
	struct dentry *dbgfs_dir;
#endif /* CONFIG_DEBUG_FS */


	const struct mei_hw_ops *ops;
	char hw[0] __aligned(sizeof(void *));
};

static inline unsigned long mei_secs_to_jiffies(unsigned long sec)
{
	return msecs_to_jiffies(sec * MSEC_PER_SEC);
}

/**
 * mei_data2slots - get slots - number of (dwords) from a message length
 *	+ size of the mei header
<<<<<<< HEAD
 * @length - size of the messages in bytes
 * returns  - number of slots
=======
 *
 * @length: size of the messages in bytes
 *
 * Return: number of slots
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static inline u32 mei_data2slots(size_t length)
{
	return DIV_ROUND_UP(sizeof(struct mei_msg_hdr) + length, 4);
}

<<<<<<< HEAD
/*
 * mei init function prototypes
 */
void mei_device_init(struct mei_device *dev);
void mei_reset(struct mei_device *dev, int interrupts);
int mei_start(struct mei_device *dev);
void mei_stop(struct mei_device *dev);
=======
/**
 * mei_slots2data - get data in slots - bytes from slots
 *
 * @slots: number of available slots
 *
 * Return: number of bytes in slots
 */
static inline u32 mei_slots2data(int slots)
{
	return slots * 4;
}

/*
 * mei init function prototypes
 */
void mei_device_init(struct mei_device *dev,
		     struct device *device,
		     const struct mei_hw_ops *hw_ops);
int mei_reset(struct mei_device *dev);
int mei_start(struct mei_device *dev);
int mei_restart(struct mei_device *dev);
void mei_stop(struct mei_device *dev);
void mei_cancel_work(struct mei_device *dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 *  MEI interrupt functions prototype
 */

void mei_timer(struct work_struct *work);
<<<<<<< HEAD
=======
void mei_schedule_stall_timer(struct mei_device *dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int mei_irq_read_handler(struct mei_device *dev,
		struct mei_cl_cb *cmpl_list, s32 *slots);

int mei_irq_write_handler(struct mei_device *dev, struct mei_cl_cb *cmpl_list);
void mei_irq_compl_handler(struct mei_device *dev, struct mei_cl_cb *cmpl_list);

/*
 * AMTHIF - AMT Host Interface Functions
 */
void mei_amthif_reset_params(struct mei_device *dev);

<<<<<<< HEAD
int mei_amthif_host_init(struct mei_device *dev);

int mei_amthif_write(struct mei_device *dev, struct mei_cl_cb *priv_cb);

int mei_amthif_read(struct mei_device *dev, struct file *file,
		char __user *ubuf, size_t length, loff_t *offset);

unsigned int mei_amthif_poll(struct mei_device *dev,
		struct file *file, poll_table *wait);

int mei_amthif_release(struct mei_device *dev, struct file *file);

struct mei_cl_cb *mei_amthif_find_read_list_entry(struct mei_device *dev,
						struct file *file);

void mei_amthif_run_next_cmd(struct mei_device *dev);

int mei_amthif_irq_write_complete(struct mei_device *dev, s32 *slots,
			struct mei_cl_cb *cb, struct mei_cl_cb *cmpl_list);

void mei_amthif_complete(struct mei_device *dev, struct mei_cl_cb *cb);
int mei_amthif_irq_read_msg(struct mei_device *dev,
=======
int mei_amthif_host_init(struct mei_device *dev, struct mei_me_client *me_cl);

unsigned int mei_amthif_poll(struct file *file, poll_table *wait);

int mei_amthif_release(struct mei_device *dev, struct file *file);

int mei_amthif_write(struct mei_cl *cl, struct mei_cl_cb *cb);
int mei_amthif_run_next_cmd(struct mei_device *dev);
int mei_amthif_irq_write(struct mei_cl *cl, struct mei_cl_cb *cb,
			struct mei_cl_cb *cmpl_list);

void mei_amthif_complete(struct mei_cl *cl, struct mei_cl_cb *cb);
int mei_amthif_irq_read_msg(struct mei_cl *cl,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			    struct mei_msg_hdr *mei_hdr,
			    struct mei_cl_cb *complete_list);
int mei_amthif_irq_read(struct mei_device *dev, s32 *slots);

/*
<<<<<<< HEAD
 * NFC functions
 */
int mei_nfc_host_init(struct mei_device *dev);
void mei_nfc_host_exit(void);

/*
 * NFC Client UUID
 */
extern const uuid_le mei_nfc_guid;

int mei_amthif_irq_write_complete(struct mei_device *dev, s32 *slots,
			struct mei_cl_cb *cb, struct mei_cl_cb *cmpl_list);

void mei_amthif_complete(struct mei_device *dev, struct mei_cl_cb *cb);
int mei_amthif_irq_read_message(struct mei_cl_cb *complete_list,
		struct mei_device *dev, struct mei_msg_hdr *mei_hdr);
int mei_amthif_irq_read(struct mei_device *dev, s32 *slots);


int mei_wd_send(struct mei_device *dev);
int mei_wd_stop(struct mei_device *dev);
int mei_wd_host_init(struct mei_device *dev);
/*
 * mei_watchdog_register  - Registering watchdog interface
 *   once we got connection to the WD Client
 * @dev - mei device
 */
void mei_watchdog_register(struct mei_device *dev);
/*
 * mei_watchdog_unregister  - Unregistering watchdog interface
 * @dev - mei device
 */
void mei_watchdog_unregister(struct mei_device *dev);

/*
 * Register Access Function
 */

static inline void mei_hw_config(struct mei_device *dev)
{
	dev->ops->hw_config(dev);
}
static inline void mei_hw_reset(struct mei_device *dev, bool enable)
{
	dev->ops->hw_reset(dev, enable);
}

static inline void mei_hw_start(struct mei_device *dev)
{
	dev->ops->hw_start(dev);
=======
 * Register Access Function
 */


static inline void mei_hw_config(struct mei_device *dev)
{
	dev->ops->hw_config(dev);
}

static inline enum mei_pg_state mei_pg_state(struct mei_device *dev)
{
	return dev->ops->pg_state(dev);
}

static inline bool mei_pg_in_transition(struct mei_device *dev)
{
	return dev->ops->pg_in_transition(dev);
}

static inline bool mei_pg_is_enabled(struct mei_device *dev)
{
	return dev->ops->pg_is_enabled(dev);
}

static inline int mei_hw_reset(struct mei_device *dev, bool enable)
{
	return dev->ops->hw_reset(dev, enable);
}

static inline int mei_hw_start(struct mei_device *dev)
{
	return dev->ops->hw_start(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void mei_clear_interrupts(struct mei_device *dev)
{
	dev->ops->intr_clear(dev);
}

static inline void mei_enable_interrupts(struct mei_device *dev)
{
	dev->ops->intr_enable(dev);
}

static inline void mei_disable_interrupts(struct mei_device *dev)
{
	dev->ops->intr_disable(dev);
}

static inline bool mei_host_is_ready(struct mei_device *dev)
{
	return dev->ops->host_is_ready(dev);
}
static inline bool mei_hw_is_ready(struct mei_device *dev)
{
	return dev->ops->hw_is_ready(dev);
}

static inline bool mei_hbuf_is_ready(struct mei_device *dev)
{
	return dev->ops->hbuf_is_ready(dev);
}

static inline int mei_hbuf_empty_slots(struct mei_device *dev)
{
	return dev->ops->hbuf_free_slots(dev);
}

static inline size_t mei_hbuf_max_len(const struct mei_device *dev)
{
	return dev->ops->hbuf_max_len(dev);
}

static inline int mei_write_message(struct mei_device *dev,
<<<<<<< HEAD
			struct mei_msg_hdr *hdr,
			unsigned char *buf)
=======
			struct mei_msg_hdr *hdr, void *buf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return dev->ops->write(dev, hdr, buf);
}

static inline u32 mei_read_hdr(const struct mei_device *dev)
{
	return dev->ops->read_hdr(dev);
}

static inline void mei_read_slots(struct mei_device *dev,
		     unsigned char *buf, unsigned long len)
{
	dev->ops->read(dev, buf, len);
}

static inline int mei_count_full_read_slots(struct mei_device *dev)
{
	return dev->ops->rdbuf_full_slots(dev);
}

<<<<<<< HEAD
=======
static inline int mei_fw_status(struct mei_device *dev,
				struct mei_fw_status *fw_status)
{
	return dev->ops->fw_status(dev, fw_status);
}

bool mei_hbuf_acquire(struct mei_device *dev);

bool mei_write_is_idle(struct mei_device *dev);

void mei_irq_discard_msg(struct mei_device *dev, struct mei_msg_hdr *hdr);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#if IS_ENABLED(CONFIG_DEBUG_FS)
int mei_dbgfs_register(struct mei_device *dev, const char *name);
void mei_dbgfs_deregister(struct mei_device *dev);
#else
static inline int mei_dbgfs_register(struct mei_device *dev, const char *name)
{
	return 0;
}
static inline void mei_dbgfs_deregister(struct mei_device *dev) {}
#endif /* CONFIG_DEBUG_FS */

<<<<<<< HEAD
int mei_register(struct mei_device *dev);
void mei_deregister(struct mei_device *dev);

#define MEI_HDR_FMT "hdr:host=%02d me=%02d len=%d comp=%1d"
#define MEI_HDR_PRM(hdr)                  \
	(hdr)->host_addr, (hdr)->me_addr, \
	(hdr)->length, (hdr)->msg_complete
=======
int mei_register(struct mei_device *dev, struct device *parent);
void mei_deregister(struct mei_device *dev);

#define MEI_HDR_FMT "hdr:host=%02d me=%02d len=%d internal=%1d comp=%1d"
#define MEI_HDR_PRM(hdr)                  \
	(hdr)->host_addr, (hdr)->me_addr, \
	(hdr)->length, (hdr)->internal, (hdr)->msg_complete

ssize_t mei_fw_status2str(struct mei_fw_status *fw_sts, char *buf, size_t len);
/**
 * mei_fw_status_str - fetch and convert fw status registers to printable string
 *
 * @dev: the device structure
 * @buf: string buffer at minimal size MEI_FW_STATUS_STR_SZ
 * @len: buffer len must be >= MEI_FW_STATUS_STR_SZ
 *
 * Return: number of bytes written or < 0 on failure
 */
static inline ssize_t mei_fw_status_str(struct mei_device *dev,
					char *buf, size_t len)
{
	struct mei_fw_status fw_status;
	int ret;

	buf[0] = '\0';

	ret = mei_fw_status(dev, &fw_status);
	if (ret)
		return ret;

	ret = mei_fw_status2str(&fw_status, buf, MEI_FW_STATUS_STR_SZ);

	return ret;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
