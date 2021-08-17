/*
 * Module: eeprom_93xx46
 * platform description for 93xx46 EEPROMs.
 */

<<<<<<< HEAD
=======
struct gpio_desc;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct eeprom_93xx46_platform_data {
	unsigned char	flags;
#define EE_ADDR8	0x01		/*  8 bit addr. cfg */
#define EE_ADDR16	0x02		/* 16 bit addr. cfg */
#define EE_READONLY	0x08		/* forbid writing */

<<<<<<< HEAD
=======
	unsigned int	quirks;
/* Single word read transfers only; no sequential read. */
#define EEPROM_93XX46_QUIRK_SINGLE_WORD_READ		(1 << 0)
/* Instructions such as EWEN are (addrlen + 2) in length. */
#define EEPROM_93XX46_QUIRK_INSTRUCTION_LENGTH		(1 << 1)
/* Add extra cycle after address during a read */
#define EEPROM_93XX46_QUIRK_EXTRA_READ_CYCLE		BIT(2)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * optional hooks to control additional logic
	 * before and after spi transfer.
	 */
	void (*prepare)(void *);
	void (*finish)(void *);
<<<<<<< HEAD
=======
	struct gpio_desc *select;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
