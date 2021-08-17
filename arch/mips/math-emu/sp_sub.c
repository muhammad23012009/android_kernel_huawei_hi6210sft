/* IEEE754 floating point arithmetic
 * single precision
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


#include "ieee754sp.h"

ieee754sp ieee754sp_sub(ieee754sp x, ieee754sp y)
{
=======
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include "ieee754sp.h"

union ieee754sp ieee754sp_sub(union ieee754sp x, union ieee754sp y)
{
	int s;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	COMPXSP;
	COMPYSP;

	EXPLODEXSP;
	EXPLODEYSP;

<<<<<<< HEAD
	CLEARCX;
=======
	ieee754_clearcx();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	FLUSHXSP;
	FLUSHYSP;

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
		return ieee754sp_nanxcpt(y);

	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_QNAN):
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_INF):
<<<<<<< HEAD
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754sp_nanxcpt(ieee754sp_indef(), "sub", x, y);
=======
		return ieee754sp_nanxcpt(x);
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
		return ieee754sp_xcpt(ieee754sp_indef(), "sub", x, y);
=======
	/*
	 * Infinity handling
	 */
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_INF):
		if (xs != ys)
			return x;
		ieee754_setcx(IEEE754_INVALID_OPERATION);
		return ieee754sp_indef();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_INF):
		return ieee754sp_inf(ys ^ 1);

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
			return ieee754sp_zero(ieee754_csr.rm ==
					      IEEE754_RD);
=======
			return ieee754sp_zero(ieee754_csr.rm == FPU_CSR_RD);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_ZERO):
		return x;

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_DNORM):
		/* quick fix up */
<<<<<<< HEAD
		DPSIGN(y) ^= 1;
=======
		SPSIGN(y) ^= 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return y;

	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_DNORM):
		SPDNORMX;

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_DNORM):
		SPDNORMY;
		break;

	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_NORM):
		SPDNORMX;
		break;

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_NORM):
		break;
	}
	/* flip sign of y and handle as add */
	ys ^= 1;

	assert(xm & SP_HIDDEN_BIT);
	assert(ym & SP_HIDDEN_BIT);


	/* provide guard,round and stick bit space */
	xm <<= 3;
	ym <<= 3;

	if (xe > ye) {
<<<<<<< HEAD
		/* have to shift y fraction right to align
		 */
		int s = xe - ye;
		SPXSRSYn(s);
	} else if (ye > xe) {
		/* have to shift x fraction right to align
		 */
		int s = ye - xe;
		SPXSRSXn(s);
=======
		/*
		 * have to shift y fraction right to align
		 */
		s = xe - ye;
		ym = XSPSRS(ym, s);
		ye += s;
	} else if (ye > xe) {
		/*
		 * have to shift x fraction right to align
		 */
		s = ye - xe;
		xm = XSPSRS(xm, s);
		xe += s;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	assert(xe == ye);
	assert(xe <= SP_EMAX);

	if (xs == ys) {
		/* generate 28 bit result of adding two 27 bit numbers
		 */
		xm = xm + ym;
<<<<<<< HEAD
		xe = xe;
		xs = xs;

		if (xm >> (SP_MBITS + 1 + 3)) { /* carry out */
=======

		if (xm >> (SP_FBITS + 1 + 3)) { /* carry out */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			SPXSRSX1();	/* shift preserving sticky */
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
				return ieee754sp_zero(1);	/* round negative inf. => sign = -1 */
			else
				return ieee754sp_zero(0);	/* other round modes   => sign = 1 */
		}
		/* normalize to rounding precision
		 */
<<<<<<< HEAD
		while ((xm >> (SP_MBITS + 3)) == 0) {
=======
		while ((xm >> (SP_FBITS + 3)) == 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			xm <<= 1;
			xe--;
		}
	}
<<<<<<< HEAD
	SPNORMRET2(xs, xe, xm, "sub", x, y);
=======

	return ieee754sp_format(xs, xe, xm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
