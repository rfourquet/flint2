/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2008, 2009 William Hart
    Copyright (C) 2010, 2012 Sebastian Pancratz
    Copyright (C) 2013 Mike Hansen

******************************************************************************/


#ifdef T

#include "templates.h"

void
_TEMPLATE(T, poly_mullow_KS)(TEMPLATE(T, struct) * rop,
                   const TEMPLATE(T, struct) * op1, long len1,
                   const TEMPLATE(T, struct) * op2, long len2, long n,
                   const TEMPLATE(T, ctx_t) ctx)
{
    const long d = TEMPLATE(T, ctx_degree)(ctx);
    long bits, i, m;
    fmpz *f, *g, *h;

    FQ_VEC_NORM(op1, len1, ctx);
    FQ_VEC_NORM(op2, len2, ctx);

    if (!len1 | !len2)
    {
        _TEMPLATE(T, poly_zero)(rop, n, ctx);
        return;
    }

    bits = 2 * fmpz_bits(TEMPLATE(T, ctx_prime)(ctx))
        + FLINT_BIT_COUNT(d) + FLINT_BIT_COUNT(FLINT_MIN(len1, len2));

    f = _fmpz_vec_init(n + len1 + len2);
    g = f + n;
    h = g + len1;

    for (i = 0; i < len1; i++)
    {
        fmpz_poly_bit_pack(g + i, op1 + i, bits);
    }
    for (i = 0; i < len2; i++)
    {
        fmpz_poly_bit_pack(h + i, op2 + i, bits);
    }

    m = FLINT_MIN(n, len1 + len2 - 1);

    if (len1 >= len2)
        _fmpz_poly_mullow(f, g, len1, h, len2, m);
    else
        _fmpz_poly_mullow(f, h, len2, g, len1, m);

    for (i = 0; i < m; i++)
    {
        fmpz_poly_bit_unpack_unsigned(rop + i, f + i, bits);
        TEMPLATE(T, reduce)(rop + i, ctx);
    }
    for (; i < n; i++)
    {
        TEMPLATE(T, zero)(rop + i, ctx);
    }

    _fmpz_vec_clear(f, n + len1 + len2);
}

void
TEMPLATE(T, poly_mullow_KS)(TEMPLATE(T, poly_t) rop,
                  const TEMPLATE(T, poly_t) op1, const TEMPLATE(T, poly_t) op2, long n,
                  const TEMPLATE(T, ctx_t) ctx)
{
    const long len1 = op1->length;
    const long len2 = op2->length;

    if (len1 == 0 || len2 == 0 || n == 0)
    {
        TEMPLATE(T, poly_zero)(rop, ctx);
    }
    else
    {
        const long lenr = op1->length + op2->length - 1;

        if (n > lenr)
            n = lenr;

        TEMPLATE(T, poly_fit_length)(rop, n, ctx);
        _TEMPLATE(T, poly_mullow_KS)(rop->coeffs, op1->coeffs, len1,
                           op2->coeffs, len2, n, ctx);
        _TEMPLATE(T, poly_set_length)(rop, n, ctx);
        _TEMPLATE(T, poly_normalise)(rop, ctx);
    }
}


#endif
