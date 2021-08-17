/*
 * OpenFirmware regulator support routines
 *
 */

#ifndef __LINUX_OF_REG_H
#define __LINUX_OF_REG_H

<<<<<<< HEAD
=======
struct regulator_desc;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct of_regulator_match {
	const char *name;
	void *driver_data;
	struct regulator_init_data *init_data;
	struct device_node *of_node;
<<<<<<< HEAD
=======
	const struct regulator_desc *desc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#if defined(CONFIG_OF)
extern struct regulator_init_data
	*of_get_regulator_init_data(struct device *dev,
<<<<<<< HEAD
				    struct device_node *node);
=======
				    struct device_node *node,
				    const struct regulator_desc *desc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int of_regulator_match(struct device *dev, struct device_node *node,
			      struct of_regulator_match *matches,
			      unsigned int num_matches);
#else
static inline struct regulator_init_data
	*of_get_regulator_init_data(struct device *dev,
<<<<<<< HEAD
				    struct device_node *node)
=======
				    struct device_node *node,
				    const struct regulator_desc *desc)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return NULL;
}

static inline int of_regulator_match(struct device *dev,
				     struct device_node *node,
				     struct of_regulator_match *matches,
				     unsigned int num_matches)
{
	return 0;
}
#endif /* CONFIG_OF */

#endif /* __LINUX_OF_REG_H */
