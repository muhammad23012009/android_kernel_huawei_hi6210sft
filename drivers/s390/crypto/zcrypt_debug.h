/*
 *  Copyright IBM Corp. 2012
 *  Author(s): Holger Dengler (hd@linux.vnet.ibm.com)
 */
#ifndef ZCRYPT_DEBUG_H
#define ZCRYPT_DEBUG_H

#include <asm/debug.h>
#include "zcrypt_api.h"

/* that gives us 15 characters in the text event views */
#define ZCRYPT_DBF_LEN	16

<<<<<<< HEAD
/* sort out low debug levels early to avoid wasted sprints */
static inline int zcrypt_dbf_passes(debug_info_t *dbf_grp, int level)
{
	return (level <= dbf_grp->level);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define DBF_ERR		3	/* error conditions	*/
#define DBF_WARN	4	/* warning conditions	*/
#define DBF_INFO	6	/* informational	*/

#define RC2WARN(rc) ((rc) ? DBF_WARN : DBF_INFO)

#define ZCRYPT_DBF_COMMON(level, text...) \
	do { \
<<<<<<< HEAD
		if (zcrypt_dbf_passes(zcrypt_dbf_common, level)) { \
=======
		if (debug_level_enabled(zcrypt_dbf_common, level)) { \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			char debug_buffer[ZCRYPT_DBF_LEN]; \
			snprintf(debug_buffer, ZCRYPT_DBF_LEN, text); \
			debug_text_event(zcrypt_dbf_common, level, \
					 debug_buffer); \
		} \
	} while (0)

#define ZCRYPT_DBF_DEVICES(level, text...) \
	do { \
<<<<<<< HEAD
		if (zcrypt_dbf_passes(zcrypt_dbf_devices, level)) { \
=======
		if (debug_level_enabled(zcrypt_dbf_devices, level)) { \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			char debug_buffer[ZCRYPT_DBF_LEN]; \
			snprintf(debug_buffer, ZCRYPT_DBF_LEN, text); \
			debug_text_event(zcrypt_dbf_devices, level, \
					 debug_buffer); \
		} \
	} while (0)

#define ZCRYPT_DBF_DEV(level, device, text...) \
	do { \
<<<<<<< HEAD
		if (zcrypt_dbf_passes(device->dbf_area, level)) { \
=======
		if (debug_level_enabled(device->dbf_area, level)) { \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			char debug_buffer[ZCRYPT_DBF_LEN]; \
			snprintf(debug_buffer, ZCRYPT_DBF_LEN, text); \
			debug_text_event(device->dbf_area, level, \
					 debug_buffer); \
		} \
	} while (0)

int zcrypt_debug_init(void);
void zcrypt_debug_exit(void);

#endif /* ZCRYPT_DEBUG_H */
