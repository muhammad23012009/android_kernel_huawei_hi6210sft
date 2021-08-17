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

int ieee754dp_cmp(ieee754dp x, ieee754dp y, int cmp, int sig)
{
=======
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include "ieee754dp.h"

int ieee754dp_cmp(union ieee754dp x, union ieee754dp y, int cmp, int sig)
{
	s64 vx;
	s64 vy;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	COMPXDP;
	COMPYDP;

	EXPLODEXDP;
	EXPLODEYDP;
	FLUSHXDP;
	FLUSHYDP;
<<<<<<< HEAD
	CLEARCX;	/* Even clear inexact flag here */

	if (ieee754dp_isnan(x) || ieee754dp_isnan(y)) {
		if (sig || xc == IEEE754_CLASS_SNAN || yc == IEEE754_CLASS_SNAN)
			SETCX(IEEE754_INVALID_OPERATION);
		if (cmp & IEEE754_CUN)
			return 1;
		if (cmp & (IEEE754_CLT | IEEE754_CGT)) {
			if (sig && SETANDTESTCX(IEEE754_INVALID_OPERATION))
				return ieee754si_xcpt(0, "fcmpf", x);
		}
		return 0;
	} else {
		s64 vx = x.bits;
		s64 vy = y.bits;
=======
	ieee754_clearcx();	/* Even clear inexact flag here */

	if (ieee754_class_nan(xc) || ieee754_class_nan(yc)) {
		if (sig ||
		    xc == IEEE754_CLASS_SNAN || yc == IEEE754_CLASS_SNAN)
			ieee754_setcx(IEEE754_INVALID_OPERATION);
		return (cmp & IEEE754_CUN) != 0;
	} else {
		vx = x.bits;
		vy = y.bits;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (vx < 0)
			vx = -vx ^ DP_SIGN_BIT;
		if (vy < 0)
			vy = -vy ^ DP_SIGN_BIT;

		if (vx < vy)
			return (cmp & IEEE754_CLT) != 0;
		else if (vx == vy)
			return (cmp & IEEE754_CEQ) != 0;
		else
			return (cmp & IEEE754_CGT) != 0;
	}
}
