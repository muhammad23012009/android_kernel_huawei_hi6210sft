/*
 *	IPV6 GSO/GRO offload support
 *	Linux INET6 implementation
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#ifndef __ip6_offload_h
#define __ip6_offload_h

int ipv6_exthdrs_offload_init(void);
<<<<<<< HEAD
int udp_offload_init(void);
=======
int udpv6_offload_init(void);
int udpv6_offload_exit(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int tcpv6_offload_init(void);

#endif
