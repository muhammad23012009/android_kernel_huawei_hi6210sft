<<<<<<< HEAD
=======
#include <linux/reboot.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define ARRAY_AND_SIZE(x)	(x), ARRAY_SIZE(x)

extern void timer_init(int irq);

<<<<<<< HEAD
extern void __init icu_init_irq(void);
extern void __init mmp_map_io(void);
extern void mmp_restart(char, const char *);
extern void __init pxa168_clk_init(void);
extern void __init pxa910_clk_init(void);
extern void __init mmp2_clk_init(void);
=======
extern void __init mmp_map_io(void);
extern void mmp_restart(enum reboot_mode, const char *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
