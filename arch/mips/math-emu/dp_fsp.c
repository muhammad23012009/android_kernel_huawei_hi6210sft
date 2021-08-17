/* IEEE754 floating point arithmetic
 * double precision: common utilities
 */
/*
 * MIPS floating point support
 * Copyright (C) 1994-2000 Algorithmics Ltd.
 *
<<<<<<< HEAD
 * ########################################################################
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
<<<<<<< HEAD
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 */


#include "ieee754dp.h"

ieee754dp ieee754dp_fsp(ieee754sp x)
=======
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include "ieee754sp.h"
#include "ieee754dp.h"

static inline union ieee754dp ieee754dp_nan_fsp(int xs, u64 xm)
{
	return builddp(xs, DP_EMAX + 1 + DP_EBIAS,
		       xm << (DP_FBITS - SP_FBITS));
}

union ieee754dp ieee754dp_fsp(union ieee754sp x)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	COMPXSP;

	EXPLODEXSP;

<<<<<<< HEAD
	CLEARCX;
=======
	ieee754_clearcx();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	FLUSHXSP;

	switch (xc) {
	case IEEE754_CLASS_SNAN:
<<<<<<< HEAD
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754dp_nanxcpt(ieee754dp_indef(), "fsp");
	case IEEE754_CLASS_QNAN:
		return ieee754dp_nanxcpt(builddp(xs,
						 DP_EMAX + 1 + DP_EBIAS,
						 ((u64) xm
						  << (DP_MBITS -
						      SP_MBITS))), "fsp",
					 x);
	case IEEE754_CLASS_INF:
		return ieee754dp_inf(xs);
	case IEEE754_CLASS_ZERO:
		return ieee754dp_zero(xs);
	case IEEE754_CLASS_DNORM:
		/* normalize */
		while ((xm >> SP_MBITS) == 0) {
=======
		return ieee754dp_nanxcpt(ieee754dp_nan_fsp(xs, xm));

	case IEEE754_CLASS_QNAN:
		return ieee754dp_nan_fsp(xs, xm);

	case IEEE754_CLASS_INF:
		return ieee754dp_inf(xs);

	case IEEE754_CLASS_ZERO:
		return ieee754dp_zero(xs);

	case IEEE754_CLASS_DNORM:
		/* normalize */
		while ((xm >> SP_FBITS) == 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			xm <<= 1;
			xe--;
		}
		break;
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case IEEE754_CLASS_NORM:
		break;
	}

<<<<<<< HEAD
	/* CAN'T possibly overflow,underflow, or need rounding
=======
	/*
	 * Can't possibly overflow,underflow, or need rounding
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 */

	/* drop the hidden bit */
	xm &= ~SP_HIDDEN_BIT;

	return builddp(xs, xe + DP_EBIAS,
<<<<<<< HEAD
		       (u64) xm << (DP_MBITS - SP_MBITS));
=======
		       (u64) xm << (DP_FBITS - SP_FBITS));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
