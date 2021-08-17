#ifndef _LINUX_RESET_CONTROLLER_H_
#define _LINUX_RESET_CONTROLLER_H_

#include <linux/list.h>

struct reset_controller_dev;

/**
<<<<<<< HEAD
 * struct reset_control_ops
=======
 * struct reset_control_ops - reset controller driver callbacks
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * @reset: for self-deasserting resets, does all necessary
 *         things to reset the device
 * @assert: manually assert the reset line, if supported
 * @deassert: manually deassert the reset line, if supported
<<<<<<< HEAD
=======
 * @status: return the status of the reset line, if supported
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
struct reset_control_ops {
	int (*reset)(struct reset_controller_dev *rcdev, unsigned long id);
	int (*assert)(struct reset_controller_dev *rcdev, unsigned long id);
	int (*deassert)(struct reset_controller_dev *rcdev, unsigned long id);
<<<<<<< HEAD
=======
	int (*status)(struct reset_controller_dev *rcdev, unsigned long id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct module;
struct device_node;
<<<<<<< HEAD
=======
struct of_phandle_args;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * struct reset_controller_dev - reset controller entity that might
 *                               provide multiple reset controls
 * @ops: a pointer to device specific struct reset_control_ops
 * @owner: kernel module of the reset controller driver
 * @list: internal list of reset controller devices
<<<<<<< HEAD
=======
 * @reset_control_head: head of internal list of requested reset controls
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @of_node: corresponding device tree node as phandle target
 * @of_reset_n_cells: number of cells in reset line specifiers
 * @of_xlate: translation function to translate from specifier as found in the
 *            device tree to id as given to the reset control ops
 * @nr_resets: number of reset controls in this reset controller device
 */
struct reset_controller_dev {
<<<<<<< HEAD
	struct reset_control_ops *ops;
	struct module *owner;
	struct list_head list;
=======
	const struct reset_control_ops *ops;
	struct module *owner;
	struct list_head list;
	struct list_head reset_control_head;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct device_node *of_node;
	int of_reset_n_cells;
	int (*of_xlate)(struct reset_controller_dev *rcdev,
			const struct of_phandle_args *reset_spec);
	unsigned int nr_resets;
};

int reset_controller_register(struct reset_controller_dev *rcdev);
void reset_controller_unregister(struct reset_controller_dev *rcdev);

<<<<<<< HEAD
=======
struct device;
int devm_reset_controller_register(struct device *dev,
				   struct reset_controller_dev *rcdev);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
