#ifndef __USB3503_H__
#define __USB3503_H__

#define USB3503_I2C_NAME	"usb3503"

<<<<<<< HEAD
=======
#define USB3503_OFF_PORT1	(1 << 1)
#define USB3503_OFF_PORT2	(1 << 2)
#define USB3503_OFF_PORT3	(1 << 3)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
enum usb3503_mode {
	USB3503_MODE_UNKNOWN,
	USB3503_MODE_HUB,
	USB3503_MODE_STANDBY,
};

struct usb3503_platform_data {
	enum usb3503_mode	initial_mode;
<<<<<<< HEAD
=======
	u8	port_off_mask;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int	gpio_intn;
	int	gpio_connect;
	int	gpio_reset;
};

#endif
