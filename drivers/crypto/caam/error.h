/*
 * CAAM Error Reporting code header
 *
 * Copyright 2009-2011 Freescale Semiconductor, Inc.
 */

#ifndef CAAM_ERROR_H
#define CAAM_ERROR_H
#define CAAM_ERROR_STR_MAX 302
<<<<<<< HEAD
extern char *caam_jr_strstatus(char *outstr, u32 status);
=======
void caam_jr_strstatus(struct device *jrdev, u32 status);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* CAAM_ERROR_H */
