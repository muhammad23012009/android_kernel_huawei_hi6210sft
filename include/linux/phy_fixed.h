#ifndef __PHY_FIXED_H
#define __PHY_FIXED_H

struct fixed_phy_status {
	int link;
	int speed;
	int duplex;
	int pause;
	int asym_pause;
};

<<<<<<< HEAD
#ifdef CONFIG_FIXED_PHY
extern int fixed_phy_add(unsigned int irq, int phy_id,
			 struct fixed_phy_status *status);
#else
static inline int fixed_phy_add(unsigned int irq, int phy_id,
				struct fixed_phy_status *status)
{
	return -ENODEV;
}
#endif /* CONFIG_FIXED_PHY */

/*
 * This function issued only by fixed_phy-aware drivers, no need
 * protect it with #ifdef
 */
extern int fixed_phy_set_link_update(struct phy_device *phydev,
			int (*link_update)(struct net_device *,
					   struct fixed_phy_status *));
=======
struct device_node;

#if IS_ENABLED(CONFIG_FIXED_PHY)
extern int fixed_phy_add(unsigned int irq, int phy_id,
			 struct fixed_phy_status *status,
			 int link_gpio);
extern struct phy_device *fixed_phy_register(unsigned int irq,
					     struct fixed_phy_status *status,
					     int link_gpio,
					     struct device_node *np);
extern void fixed_phy_unregister(struct phy_device *phydev);
extern int fixed_phy_set_link_update(struct phy_device *phydev,
			int (*link_update)(struct net_device *,
					   struct fixed_phy_status *));
extern int fixed_phy_update_state(struct phy_device *phydev,
			   const struct fixed_phy_status *status,
			   const struct fixed_phy_status *changed);
#else
static inline int fixed_phy_add(unsigned int irq, int phy_id,
				struct fixed_phy_status *status,
				int link_gpio)
{
	return -ENODEV;
}
static inline struct phy_device *fixed_phy_register(unsigned int irq,
						struct fixed_phy_status *status,
						int gpio_link,
						struct device_node *np)
{
	return ERR_PTR(-ENODEV);
}
static inline void fixed_phy_unregister(struct phy_device *phydev)
{
}
static inline int fixed_phy_set_link_update(struct phy_device *phydev,
			int (*link_update)(struct net_device *,
					   struct fixed_phy_status *))
{
	return -ENODEV;
}
static inline int fixed_phy_update_state(struct phy_device *phydev,
			   const struct fixed_phy_status *status,
			   const struct fixed_phy_status *changed)
{
	return -ENODEV;
}
#endif /* CONFIG_FIXED_PHY */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __PHY_FIXED_H */
