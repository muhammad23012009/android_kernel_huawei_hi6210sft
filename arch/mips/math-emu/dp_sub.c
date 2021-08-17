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

ieee754dp ieee754dp_sub(ieee754dp x, ieee754dp y)
{
=======
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include "ieee754dp.h"

union ieee754dp ieee754dp_sub(union ieee754dp x, union ieee754dp y)
{
	int s;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	COMPXDP;
	COMPYDP;

	EXPLODEXDP;
	EXPLODEYDP;

<<<<<<< HEAD
	CLEARCX;
=======
	ieee754_clearcx();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	FLUSHXDP;
	FLUSHYDP;

	switch (CLPAIR(xc, yc)) {
<<<<<<< HEAD
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_SNAN):
=======
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_SNAN):
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_SNAN):
<<<<<<< HEAD
=======
		return ieee754dp_nanxcpt(y);

	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_QNAN):
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_INF):
<<<<<<< HEAD
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754dp_nanxcpt(ieee754dp_indef(), "sub", x, y);
=======
		return ieee754dp_nanxcpt(x);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_QNAN):
		return y;

	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_INF):
		return x;


<<<<<<< HEAD
		/* Infinity handling
		 */

	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_INF):
		if (xs != ys)
			return x;
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754dp_xcpt(ieee754dp_indef(), "sub", x, y);
=======
	/*
	 * Infinity handling
	 */
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_INF):
		if (xs != ys)
			return x;
		ieee754_setcx(IEEE754_INVALID_OPERATION);
		return ieee754dp_indef();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_INF):
		return ieee754dp_inf(ys ^ 1);

	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_DNORM):
		return x;

<<<<<<< HEAD
		/* Zero handling
		 */

=======
	/*
	 * Zero handling
	 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_ZERO):
		if (xs != ys)
			return x;
		else
<<<<<<< HEAD
			return ieee754dp_zero(ieee754_csr.rm ==
					      IEEE754_RD);
=======
			return ieee754dp_zero(ieee754_csr.rm == FPU_CSR_RD);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_ZERO):
		return x;

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_DNORM):
		/* quick fix up */
		DPSIGN(y) ^= 1;
		return y;

	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_DNORM):
		DPDNORMX;
		/* FALL THROUGH */

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_DNORM):
		/* normalize ym,ye */
		DPDNORMY;
		break;

	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_NORM):
		/* normalize xm,xe */
		DPDNORMX;
		break;

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_NORM):
		break;
	}
	/* flip sign of y and handle as add */
	ys ^= 1;

	assert(xm & DP_HIDDEN_BIT);
	assert(ym & DP_HIDDEN_BIT);


	/* provide guard,round and stick bit dpace */
	xm <<= 3;
	ym <<= 3;

	if (xe > ye) {
<<<<<<< HEAD
		/* have to shift y fraction right to align
		 */
		int s = xe - ye;
		ym = XDPSRS(ym, s);
		ye += s;
	} else if (ye > xe) {
		/* have to shift x fraction right to align
		 */
		int s = ye - xe;
=======
		/*
		 * Have to shift y fraction right to align
		 */
		s = xe - ye;
		ym = XDPSRS(ym, s);
		ye += s;
	} else if (ye > xe) {
		/*
		 * Have to shift x fraction right to align
		 */
		s = ye - xe;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		xm = XDPSRS(xm, s);
		xe += s;
	}
	assert(xe == ye);
	assert(xe <= DP_EMAX);

	if (xs == ys) {
		/* generate 28 bit result of adding two 27 bit numbers
		 */
		xm = xm + ym;
<<<<<<< HEAD
		xe = xe;
		xs = xs;

		if (xm >> (DP_MBITS + 1 + 3)) { /* carry out */
=======

		if (xm >> (DP_FBITS + 1 + 3)) { /* carry out */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			xm = XDPSRS1(xm);	/* shift preserving sticky */
			xe++;
		}
	} else {
		if (xm >= ym) {
			xm = xm - ym;
<<<<<<< HEAD
			xe = xe;
			xs = xs;
		} else {
			xm = ym - xm;
			xe = xe;
			xs = ys;
		}
		if (xm == 0) {
			if (ieee754_csr.rm == IEEE754_RD)
=======
		} else {
			xm = ym - xm;
			xs = ys;
		}
		if (xm == 0) {
			if (ieee754_csr.rm == FPU_CSR_RD)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				return ieee754dp_zero(1);	/* round negative inf. => sign = -1 */
			else
				return ieee754dp_zero(0);	/* other round modes   => sign = 1 */
		}

		/* normalize to rounding precision
		 */
<<<<<<< HEAD
		while ((xm >> (DP_MBITS + 3)) == 0) {
=======
		while ((xm >> (DP_FBITS + 3)) == 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			xm <<= 1;
			xe--;
		}
	}
<<<<<<< HEAD
	DPNORMRET2(xs, xe, xm, "sub", x, y);
=======

	return ieee754dp_format(xs, xe, xm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
