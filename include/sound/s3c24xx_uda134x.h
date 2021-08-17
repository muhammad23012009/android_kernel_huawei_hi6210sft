#ifndef _S3C24XX_UDA134X_H_
#define _S3C24XX_UDA134X_H_ 1

#include <sound/uda134x.h>

struct s3c24xx_uda134x_platform_data {
	int l3_clk;
	int l3_mode;
	int l3_data;
<<<<<<< HEAD
	void (*power) (int);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int model;
};

#endif
