<<<<<<< HEAD
/* Copyright (C) 2007-2013 B.A.T.M.A.N. contributors:
=======
/* Copyright (C) 2007-2016  B.A.T.M.A.N. contributors:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Marek Lindner
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
<<<<<<< HEAD
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
=======
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#ifndef _NET_BATMAN_ADV_ICMP_SOCKET_H_
#define _NET_BATMAN_ADV_ICMP_SOCKET_H_

<<<<<<< HEAD
#define BATADV_ICMP_SOCKET "socket"

void batadv_socket_init(void);
int batadv_socket_setup(struct batadv_priv *bat_priv);
void batadv_socket_receive_packet(struct batadv_icmp_packet_rr *icmp_packet,
				  size_t icmp_len);

=======
#include "main.h"

#include <linux/types.h>

struct batadv_icmp_header;

#define BATADV_ICMP_SOCKET "socket"

int batadv_socket_setup(struct batadv_priv *bat_priv);

#ifdef CONFIG_BATMAN_ADV_DEBUGFS

void batadv_socket_init(void);
void batadv_socket_receive_packet(struct batadv_icmp_header *icmph,
				  size_t icmp_len);

#else

static inline void batadv_socket_init(void)
{
}

static inline void
batadv_socket_receive_packet(struct batadv_icmp_header *icmph, size_t icmp_len)
{
}

#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _NET_BATMAN_ADV_ICMP_SOCKET_H_ */
