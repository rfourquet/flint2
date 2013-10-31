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

    Copyright (C) 2011 Sebastian Pancratz
    Copyright (C) 2009 William Hart
    Copyright (C) 2013 Mike Hansen

******************************************************************************/


#ifdef T

#include "templates.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    flint_printf("div_basecase....");
    fflush(stdout);

    flint_randinit(state);

    /* Compare to divrem_basecase */
    for (i = 0; i < 500; i++)
    {
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) a, b, q, q2, r2;

        TEMPLATE(T, ctx_randtest)(ctx, state);
        
        TEMPLATE(T, poly_init)(a, ctx);
        TEMPLATE(T, poly_init)(b, ctx);
        TEMPLATE(T, poly_init)(q, ctx);
        TEMPLATE(T, poly_init)(q2, ctx);
        TEMPLATE(T, poly_init)(r2, ctx);

        TEMPLATE(T, poly_randtest)(a, state, n_randint(state, 100), ctx);
        TEMPLATE(T, poly_randtest_not_zero)(b, state, n_randint(state, 100) + 1, ctx);

        TEMPLATE(T, poly_div_basecase)(q, a, b, ctx);
        TEMPLATE(T, poly_divrem_basecase)(q2, r2, a, b, ctx);

        result = (TEMPLATE(T, poly_equal)(q, q2, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("ctx = "), TEMPLATE(T, ctx_print)(ctx), flint_printf("\n\n");
            flint_printf("a = "), TEMPLATE(T, poly_print)(a, ctx), flint_printf("\n\n");
            flint_printf("b = "), TEMPLATE(T, poly_print)(b, ctx), flint_printf("\n\n");
            flint_printf("q = "), TEMPLATE(T, poly_print)(q, ctx), flint_printf("\n\n");
            flint_printf("q2 = "), TEMPLATE(T, poly_print)(q2, ctx), flint_printf("\n\n");
            flint_printf("r2 = "), TEMPLATE(T, poly_print)(r2, ctx), flint_printf("\n\n");
            abort();
        }

        TEMPLATE(T, poly_clear)(a, ctx);
        TEMPLATE(T, poly_clear)(b, ctx);
        TEMPLATE(T, poly_clear)(q, ctx);
        TEMPLATE(T, poly_clear)(q2, ctx);
        TEMPLATE(T, poly_clear)(r2, ctx);
        TEMPLATE(T, ctx_clear)(ctx);
    }

    /* Alias a and q */
    for (i = 0; i < 500; i++)
    {
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) a, b, q;

        TEMPLATE(T, ctx_randtest)(ctx, state);

        TEMPLATE(T, poly_init)(a, ctx);
        TEMPLATE(T, poly_init)(b, ctx);
        TEMPLATE(T, poly_init)(q, ctx);
        TEMPLATE(T, poly_randtest)(a, state, n_randint(state, 100), ctx);
        TEMPLATE(T, poly_randtest_not_zero)(b, state, n_randint(state, 100) + 1, ctx);

        TEMPLATE(T, poly_div_basecase)(q, a, b, ctx);
        TEMPLATE(T, poly_div_basecase)(a, a, b, ctx);

        result = (TEMPLATE(T, poly_equal)(q, a, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("ctx = "), TEMPLATE(T, ctx_print)(ctx), flint_printf("\n\n");
            flint_printf("a = "), TEMPLATE(T, poly_print)(a, ctx), flint_printf("\n\n");
            flint_printf("b = "), TEMPLATE(T, poly_print)(b, ctx), flint_printf("\n\n");
            flint_printf("q = "), TEMPLATE(T, poly_print)(q, ctx), flint_printf("\n\n");
            abort();
        }

        TEMPLATE(T, poly_clear)(a, ctx);
        TEMPLATE(T, poly_clear)(b, ctx);
        TEMPLATE(T, poly_clear)(q, ctx);
        TEMPLATE(T, ctx_clear)(ctx);
    }

    /* Alias b and q */
    for (i = 0; i < 500; i++)
    {
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) a, b, q;

        TEMPLATE(T, ctx_randtest)(ctx, state);

        TEMPLATE(T, poly_init)(a, ctx);
        TEMPLATE(T, poly_init)(b, ctx);
        TEMPLATE(T, poly_init)(q, ctx);
        TEMPLATE(T, poly_randtest)(a, state, n_randint(state, 100), ctx);
        TEMPLATE(T, poly_randtest_not_zero)(b, state, n_randint(state, 100) + 1, ctx);

        TEMPLATE(T, poly_div_basecase)(q, a, b, ctx);
        TEMPLATE(T, poly_div_basecase)(b, a, b, ctx);

        result = (TEMPLATE(T, poly_equal)(q, b, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("ctx = "), TEMPLATE(T, ctx_print)(ctx), flint_printf("\n\n");
            flint_printf("a = "), TEMPLATE(T, poly_print)(a, ctx), flint_printf("\n\n");
            flint_printf("b = "), TEMPLATE(T, poly_print)(b, ctx), flint_printf("\n\n");
            flint_printf("q = "), TEMPLATE(T, poly_print)(q, ctx), flint_printf("\n\n");
            abort();
        }

        TEMPLATE(T, poly_clear)(a, ctx);
        TEMPLATE(T, poly_clear)(b, ctx);
        TEMPLATE(T, poly_clear)(q, ctx);
        TEMPLATE(T, ctx_clear)(ctx);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return 0;
}


#endif
