/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 * Copyright 2012 Linaro Ltd.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifdef CONFIG_CPU_IDLE
extern int imx5_cpuidle_init(void);
extern int imx6q_cpuidle_init(void);
<<<<<<< HEAD
=======
extern int imx6sl_cpuidle_init(void);
extern int imx6sx_cpuidle_init(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
static inline int imx5_cpuidle_init(void)
{
	return 0;
}
static inline int imx6q_cpuidle_init(void)
{
	return 0;
}
<<<<<<< HEAD
=======
static inline int imx6sl_cpuidle_init(void)
{
	return 0;
}
static inline int imx6sx_cpuidle_init(void)
{
	return 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
