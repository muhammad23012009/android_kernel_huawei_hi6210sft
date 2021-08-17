#ifndef _ASMPARISC_SHMPARAM_H
#define _ASMPARISC_SHMPARAM_H

<<<<<<< HEAD
#define __ARCH_FORCE_SHMLBA 	1

#define SHMLBA 0x00400000   /* attach addr needs to be 4 Mb aligned */
=======
#define SHMLBA	   PAGE_SIZE	/* attach addr a multiple of this */
#define SHM_COLOUR 0x00400000	/* shared mappings colouring */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _ASMPARISC_SHMPARAM_H */
