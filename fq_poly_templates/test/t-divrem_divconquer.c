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

    Copyright (C) 2013 Mike Hansen

******************************************************************************/


#ifdef T

#include "templates.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    printf("divrem_divconquer....");
    fflush(stdout);

    flint_randinit(state);

    /* Check q*b + r = a, no aliasing */
    for (i = 0; i < 5000; i++)
    {
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) a, b, q, r, t;

        TEMPLATE(T, ctx_randtest)(ctx, state);

        TEMPLATE(T, poly_init)(a, ctx);
        TEMPLATE(T, poly_init)(b, ctx);
        TEMPLATE(T, poly_init)(q, ctx);
        TEMPLATE(T, poly_init)(r, ctx);
        TEMPLATE(T, poly_init)(t, ctx);
        TEMPLATE(T, poly_randtest)(a, state, n_randint(state, 100), ctx);
        TEMPLATE(T, poly_randtest_not_zero)(b, state, n_randint(state, 100) + 1, ctx);

        TEMPLATE(T, poly_divrem_divconquer)(q, r, a, b, ctx);

        TEMPLATE(T, poly_mul)(t, q, b, ctx);
        TEMPLATE(T, poly_add)(t, t, r, ctx);

        result = (TEMPLATE(T, poly_equal)(a, t, ctx));
        if (!result)
        {
            printf("FAIL #1:\n");
            printf("a = "), TEMPLATE(T, poly_print)(a, ctx), printf("\n\n");
            printf("b = "), TEMPLATE(T, poly_print)(b, ctx), printf("\n\n");
            printf("q = "), TEMPLATE(T, poly_print)(q, ctx), printf("\n\n");
            printf("r = "), TEMPLATE(T, poly_print)(r, ctx), printf("\n\n");
            printf("t = "), TEMPLATE(T, poly_print)(t, ctx), printf("\n\n");
            abort();
        }

        TEMPLATE(T, poly_clear)(a, ctx);
        TEMPLATE(T, poly_clear)(b, ctx);
        TEMPLATE(T, poly_clear)(q, ctx);
        TEMPLATE(T, poly_clear)(r, ctx);
        TEMPLATE(T, poly_clear)(t, ctx);
        TEMPLATE(T, ctx_clear)(ctx);
    }

    /* Alias a and q, b and r */
    for (i = 0; i < 500; i++)
    {
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) a, b, q, r;

        TEMPLATE(T, ctx_randtest)(ctx, state);

        TEMPLATE(T, poly_init)(a, ctx);
        TEMPLATE(T, poly_init)(b, ctx);
        TEMPLATE(T, poly_init)(q, ctx);
        TEMPLATE(T, poly_init)(r, ctx);
        TEMPLATE(T, poly_randtest)(a, state, n_randint(state, 100), ctx);
        TEMPLATE(T, poly_randtest_not_zero)(b, state, n_randint(state, 100) + 1, ctx);

        TEMPLATE(T, poly_divrem_divconquer)(q, r, a, b, ctx);
        TEMPLATE(T, poly_divrem_divconquer)(a, b, a, b, ctx);

        result = (TEMPLATE(T, poly_equal)(q, a, ctx) && TEMPLATE(T, poly_equal)(r, b, ctx));
        if (!result)
        {
            printf("FAIL #2:\n");
            printf("a = "), TEMPLATE(T, poly_print)(a, ctx), printf("\n\n");
            printf("b = "), TEMPLATE(T, poly_print)(b, ctx), printf("\n\n");
            printf("q = "), TEMPLATE(T, poly_print)(q, ctx), printf("\n\n");
            printf("r = "), TEMPLATE(T, poly_print)(r, ctx), printf("\n\n");
            abort();
        }

        TEMPLATE(T, poly_clear)(a, ctx);
        TEMPLATE(T, poly_clear)(b, ctx);
        TEMPLATE(T, poly_clear)(q, ctx);
        TEMPLATE(T, poly_clear)(r, ctx);
        TEMPLATE(T, ctx_clear)(ctx);
    }

    /* Alias b and q, a and r */
    for (i = 0; i < 500; i++)
    {
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) a, b, q, r;

        TEMPLATE(T, ctx_randtest)(ctx, state);

        TEMPLATE(T, poly_init)(a, ctx);
        TEMPLATE(T, poly_init)(b, ctx);
        TEMPLATE(T, poly_init)(q, ctx);
        TEMPLATE(T, poly_init)(r, ctx);
        TEMPLATE(T, poly_randtest)(a, state, n_randint(state, 100), ctx);
        TEMPLATE(T, poly_randtest_not_zero)(b, state, n_randint(state, 100) + 1, ctx);

        TEMPLATE(T, poly_divrem_divconquer)(q, r, a, b, ctx);
        TEMPLATE(T, poly_divrem_divconquer)(b, a, a, b, ctx);

        result = (TEMPLATE(T, poly_equal)(q, b, ctx) && TEMPLATE(T, poly_equal)(r, a, ctx));
        if (!result)
        {
            printf("FAIL #3:\n");
            printf("a = "), TEMPLATE(T, poly_print)(a, ctx), printf("\n\n");
            printf("b = "), TEMPLATE(T, poly_print)(b, ctx), printf("\n\n");
            printf("q = "), TEMPLATE(T, poly_print)(q, ctx), printf("\n\n");
            printf("r = "), TEMPLATE(T, poly_print)(r, ctx), printf("\n\n");
            abort();
        }

        TEMPLATE(T, poly_clear)(a, ctx);
        TEMPLATE(T, poly_clear)(b, ctx);
        TEMPLATE(T, poly_clear)(q, ctx);
        TEMPLATE(T, poly_clear)(r, ctx);
        TEMPLATE(T, ctx_clear)(ctx);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return 0;
}


#endif
