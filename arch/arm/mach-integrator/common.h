<<<<<<< HEAD
=======
#include <linux/reboot.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/amba/serial.h>
extern struct amba_pl010_data ap_uart_data;
void integrator_init_early(void);
int integrator_init(bool is_cp);
void integrator_reserve(void);
<<<<<<< HEAD
void integrator_restart(char, const char *);
void integrator_init_sysfs(struct device *parent, u32 id);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
