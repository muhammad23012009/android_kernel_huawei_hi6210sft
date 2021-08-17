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

ieee754sp ieee754sp_mul(ieee754sp x, ieee754sp y)
{
=======
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include "ieee754sp.h"

union ieee754sp ieee754sp_mul(union ieee754sp x, union ieee754sp y)
{
	int re;
	int rs;
	unsigned rm;
	unsigned short lxm;
	unsigned short hxm;
	unsigned short lym;
	unsigned short hym;
	unsigned lrm;
	unsigned hrm;
	unsigned t;
	unsigned at;

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
		return ieee754sp_nanxcpt(ieee754sp_indef(), "mul", x, y);
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
		/* Infinity handling */

	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_INF):
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754sp_xcpt(ieee754sp_indef(), "mul", x, y);
=======
	/*
	 * Infinity handling
	 */
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_INF):
		ieee754_setcx(IEEE754_INVALID_OPERATION);
		return ieee754sp_indef();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_INF):
		return ieee754sp_inf(xs ^ ys);

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_ZERO):
		return ieee754sp_zero(xs ^ ys);


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
	/* rm = xm * ym, re = xe+ye basically */
	assert(xm & SP_HIDDEN_BIT);
	assert(ym & SP_HIDDEN_BIT);

<<<<<<< HEAD
	{
		int re = xe + ye;
		int rs = xs ^ ys;
		unsigned rm;

		/* shunt to top of word */
		xm <<= 32 - (SP_MBITS + 1);
		ym <<= 32 - (SP_MBITS + 1);

		/* multiply 32bits xm,ym to give high 32bits rm with stickness
		 */
		{
			unsigned short lxm = xm & 0xffff;
			unsigned short hxm = xm >> 16;
			unsigned short lym = ym & 0xffff;
			unsigned short hym = ym >> 16;
			unsigned lrm;
			unsigned hrm;

			lrm = lxm * lym;	/* 16 * 16 => 32 */
			hrm = hxm * hym;	/* 16 * 16 => 32 */

			{
				unsigned t = lxm * hym; /* 16 * 16 => 32 */
				{
					unsigned at = lrm + (t << 16);
					hrm += at < lrm;
					lrm = at;
				}
				hrm = hrm + (t >> 16);
			}

			{
				unsigned t = hxm * lym; /* 16 * 16 => 32 */
				{
					unsigned at = lrm + (t << 16);
					hrm += at < lrm;
					lrm = at;
				}
				hrm = hrm + (t >> 16);
			}
			rm = hrm | (lrm != 0);
		}

		/*
		 * sticky shift down to normal rounding precision
		 */
		if ((int) rm < 0) {
			rm = (rm >> (32 - (SP_MBITS + 1 + 3))) |
			    ((rm << (SP_MBITS + 1 + 3)) != 0);
			re++;
		} else {
			rm = (rm >> (32 - (SP_MBITS + 1 + 3 + 1))) |
			    ((rm << (SP_MBITS + 1 + 3 + 1)) != 0);
		}
		assert(rm & (SP_HIDDEN_BIT << 3));

		SPNORMRET2(rs, re, rm, "mul", x, y);
	}
=======
	re = xe + ye;
	rs = xs ^ ys;

	/* shunt to top of word */
	xm <<= 32 - (SP_FBITS + 1);
	ym <<= 32 - (SP_FBITS + 1);

	/*
	 * Multiply 32 bits xm, ym to give high 32 bits rm with stickness.
	 */
	lxm = xm & 0xffff;
	hxm = xm >> 16;
	lym = ym & 0xffff;
	hym = ym >> 16;

	lrm = lxm * lym;	/* 16 * 16 => 32 */
	hrm = hxm * hym;	/* 16 * 16 => 32 */

	t = lxm * hym; /* 16 * 16 => 32 */
	at = lrm + (t << 16);
	hrm += at < lrm;
	lrm = at;
	hrm = hrm + (t >> 16);

	t = hxm * lym; /* 16 * 16 => 32 */
	at = lrm + (t << 16);
	hrm += at < lrm;
	lrm = at;
	hrm = hrm + (t >> 16);

	rm = hrm | (lrm != 0);

	/*
	 * Sticky shift down to normal rounding precision.
	 */
	if ((int) rm < 0) {
		rm = (rm >> (32 - (SP_FBITS + 1 + 3))) |
		    ((rm << (SP_FBITS + 1 + 3)) != 0);
		re++;
	} else {
		rm = (rm >> (32 - (SP_FBITS + 1 + 3 + 1))) |
		     ((rm << (SP_FBITS + 1 + 3 + 1)) != 0);
	}
	assert(rm & (SP_HIDDEN_BIT << 3));

	return ieee754sp_format(rs, re, rm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
