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

ieee754dp ieee754dp_mul(ieee754dp x, ieee754dp y)
{
=======
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include "ieee754dp.h"

union ieee754dp ieee754dp_mul(union ieee754dp x, union ieee754dp y)
{
	int re;
	int rs;
	u64 rm;
	unsigned lxm;
	unsigned hxm;
	unsigned lym;
	unsigned hym;
	u64 lrm;
	u64 hrm;
	u64 t;
	u64 at;

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
		return ieee754dp_nanxcpt(ieee754dp_indef(), "mul", x, y);
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
		/* Infinity handling */

	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_INF):
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754dp_xcpt(ieee754dp_indef(), "mul", x, y);
=======
	/*
	 * Infinity handling
	 */
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_INF):
		ieee754_setcx(IEEE754_INVALID_OPERATION);
		return ieee754dp_indef();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_INF):
		return ieee754dp_inf(xs ^ ys);

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_ZERO):
		return ieee754dp_zero(xs ^ ys);


	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_DNORM):
		DPDNORMX;

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_DNORM):
		DPDNORMY;
		break;

	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_NORM):
		DPDNORMX;
		break;

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_NORM):
		break;
	}
	/* rm = xm * ym, re = xe+ye basically */
	assert(xm & DP_HIDDEN_BIT);
	assert(ym & DP_HIDDEN_BIT);
<<<<<<< HEAD
	{
		int re = xe + ye;
		int rs = xs ^ ys;
		u64 rm;

		/* shunt to top of word */
		xm <<= 64 - (DP_MBITS + 1);
		ym <<= 64 - (DP_MBITS + 1);

		/* multiply 32bits xm,ym to give high 32bits rm with stickness
		 */

		/* 32 * 32 => 64 */
#define DPXMULT(x, y)	((u64)(x) * (u64)y)

		{
			unsigned lxm = xm;
			unsigned hxm = xm >> 32;
			unsigned lym = ym;
			unsigned hym = ym >> 32;
			u64 lrm;
			u64 hrm;

			lrm = DPXMULT(lxm, lym);
			hrm = DPXMULT(hxm, hym);

			{
				u64 t = DPXMULT(lxm, hym);
				{
					u64 at =
					    lrm + (t << 32);
					hrm += at < lrm;
					lrm = at;
				}
				hrm = hrm + (t >> 32);
			}

			{
				u64 t = DPXMULT(hxm, lym);
				{
					u64 at =
					    lrm + (t << 32);
					hrm += at < lrm;
					lrm = at;
				}
				hrm = hrm + (t >> 32);
			}
			rm = hrm | (lrm != 0);
		}

		/*
		 * sticky shift down to normal rounding precision
		 */
		if ((s64) rm < 0) {
			rm =
			    (rm >> (64 - (DP_MBITS + 1 + 3))) |
			    ((rm << (DP_MBITS + 1 + 3)) != 0);
			re++;
		} else {
			rm =
			    (rm >> (64 - (DP_MBITS + 1 + 3 + 1))) |
			    ((rm << (DP_MBITS + 1 + 3 + 1)) != 0);
		}
		assert(rm & (DP_HIDDEN_BIT << 3));
		DPNORMRET2(rs, re, rm, "mul", x, y);
	}
=======

	re = xe + ye;
	rs = xs ^ ys;

	/* shunt to top of word */
	xm <<= 64 - (DP_FBITS + 1);
	ym <<= 64 - (DP_FBITS + 1);

	/*
	 * Multiply 64 bits xm, ym to give high 64 bits rm with stickness.
	 */

	/* 32 * 32 => 64 */
#define DPXMULT(x, y)	((u64)(x) * (u64)y)

	lxm = xm;
	hxm = xm >> 32;
	lym = ym;
	hym = ym >> 32;

	lrm = DPXMULT(lxm, lym);
	hrm = DPXMULT(hxm, hym);

	t = DPXMULT(lxm, hym);

	at = lrm + (t << 32);
	hrm += at < lrm;
	lrm = at;

	hrm = hrm + (t >> 32);

	t = DPXMULT(hxm, lym);

	at = lrm + (t << 32);
	hrm += at < lrm;
	lrm = at;

	hrm = hrm + (t >> 32);

	rm = hrm | (lrm != 0);

	/*
	 * Sticky shift down to normal rounding precision.
	 */
	if ((s64) rm < 0) {
		rm = (rm >> (64 - (DP_FBITS + 1 + 3))) |
		     ((rm << (DP_FBITS + 1 + 3)) != 0);
		re++;
	} else {
		rm = (rm >> (64 - (DP_FBITS + 1 + 3 + 1))) |
		     ((rm << (DP_FBITS + 1 + 3 + 1)) != 0);
	}
	assert(rm & (DP_HIDDEN_BIT << 3));

	return ieee754dp_format(rs, re, rm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
