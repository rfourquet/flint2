/*
    Copyright (C) 2018 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "nmod_mpoly.h"
#include "fq_nmod_mpoly.h"


void fq_nmod_mpolyun_init(
    fq_nmod_mpolyun_t A,
    flint_bitcnt_t bits,
    const fq_nmod_mpoly_ctx_t ctx)
{
    A->coeffs = NULL;
    A->exps = NULL;
    A->alloc = 0;
    A->length = 0;
    A->bits = bits;
}


void fq_nmod_mpolyun_clear(
    fq_nmod_mpolyun_t A,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i;
    for (i = 0; i < A->alloc; i++)
        fq_nmod_mpolyn_clear(A->coeffs + i, ctx);
    flint_free(A->coeffs);
    flint_free(A->exps);
}


int fq_nmod_mpolyun_is_canonical(
    const fq_nmod_mpolyun_t A,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i;

    if (A->length > A->alloc)
    {
        return 0;
    }

    for (i = 0; i < A->length; i++)
    {
        if (!fq_nmod_mpolyn_is_canonical(A->coeffs + i, ctx))
        {
            return 0;
        }

        if (i > 0 && A->exps[i - 1] <= A->exps[i])
        {
            return 0;
        }
    }

    return 1;
}


void fq_nmod_mpolyun_print_pretty(
    const fq_nmod_mpolyun_t poly,
    const char ** x,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i;
    if (poly->length == 0)
        flint_printf("0");
    for (i = 0; i < poly->length; i++)
    {
        if (i != 0)
            flint_printf(" + ");
        flint_printf("(");
        fq_nmod_mpolyn_print_pretty(poly->coeffs + i,x,ctx);
        flint_printf(")*X^%wd",poly->exps[i]);
    }
}


void fq_nmod_mpolyun_swap(
    fq_nmod_mpolyun_t A,
    fq_nmod_mpolyun_t B)
{
   fq_nmod_mpolyun_struct t = *A;
   *A = *B;
   *B = t;
}


void fq_nmod_mpolyun_zero(
    fq_nmod_mpolyun_t A,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i;
    for (i = 0; i < A->alloc; i++) {
        fq_nmod_mpolyn_clear(A->coeffs + i, ctx);
        fq_nmod_mpolyn_init(A->coeffs + i, A->bits, ctx);
    }
    A->length = 0;
}


void fq_nmod_mpolyun_fit_length(
    fq_nmod_mpolyun_t A,
    slong length,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i;
    slong old_alloc = A->alloc;
    slong new_alloc = FLINT_MAX(length, 2*A->alloc);

    if (length > old_alloc)
    {
        if (old_alloc == 0)
        {
            A->exps = (ulong *) flint_malloc(new_alloc*sizeof(ulong));
            A->coeffs = (fq_nmod_mpolyn_struct *) flint_malloc(
                                      new_alloc*sizeof(fq_nmod_mpolyn_struct));
        }
    else
        {
            A->exps = (ulong *) flint_realloc(A->exps,
                                                      new_alloc*sizeof(ulong));
            A->coeffs = (fq_nmod_mpolyn_struct *) flint_realloc(A->coeffs,
                                      new_alloc*sizeof(fq_nmod_mpolyn_struct));
        }

        for (i = old_alloc; i < new_alloc; i++)
        {
            fq_nmod_mpolyn_init(A->coeffs + i, A->bits, ctx);
        }
        A->alloc = new_alloc;
    }
}


void fq_nmod_mpolyun_one(
    fq_nmod_mpolyun_t A,
    const fq_nmod_mpoly_ctx_t ctx)
{
    fq_nmod_mpolyun_fit_length(A, 1, ctx);
    fq_nmod_mpolyn_one(A->coeffs + 0, ctx);
    A->exps[0] = 0;
    A->length = 1;
}


int fq_nmod_mpolyn_is_nonzero_fq_nmod(
    const fq_nmod_mpolyn_t A,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong N;

    if (A->length != WORD(1))
    {
        return 0;
    }

    if (fq_nmod_poly_degree(A->coeffs + 0, ctx->fqctx) != 0)
    {
        return 0;
    }

    N = mpoly_words_per_exp(A->bits, ctx->minfo);
    return mpoly_monomial_is_zero(A->exps + N*0, N);
}


int fq_nmod_mpolyun_is_nonzero_fq_nmod(
    const fq_nmod_mpolyun_t A,
    const fq_nmod_mpoly_ctx_t ctx)
{
    if (A->length != 1 || A->exps[0] != 0)
    {
        return 0;
    }

    return fq_nmod_mpolyn_is_nonzero_fq_nmod(A->coeffs + 0, ctx);
}


void fq_nmod_mpolyun_scalar_mul_fq_nmod(
    fq_nmod_mpolyun_t A,
    const fq_nmod_t c,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i, j;
    FLINT_ASSERT(!fq_nmod_is_zero(c, ctx->fqctx));
    for (i = 0; i < A->length; i++)
    {
        fq_nmod_mpolyn_struct * Ai = A->coeffs + i;
        for (j = 0; j < Ai->length; j++)
        {
            fq_nmod_poly_scalar_mul_fq_nmod(Ai->coeffs + j,
                                            Ai->coeffs + j, c, ctx->fqctx);
        }
    }
}


void fq_nmod_mpolyun_shift_right(
    fq_nmod_mpolyun_t A,
    ulong s)
{
    slong i;
    for (i = 0; i < A->length; i++)
    {
        FLINT_ASSERT(A->exps[i] >= s);
        A->exps[i] -= s;
    }
}


void fq_nmod_mpolyun_shift_left(
    fq_nmod_mpolyun_t A,
    ulong s)
{
    slong i;
    for (i = 0; i < A->length; i++)
    {
        A->exps[i] += s;
        FLINT_ASSERT(A->exps[i] >= s);
    }
}


void fq_nmod_mpolyn_mul_poly(
    fq_nmod_mpolyn_t A,
    const fq_nmod_mpolyn_t B,
    const fq_nmod_poly_t c,
    fq_nmod_poly_t t, /* temp */
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i;
    fq_nmod_poly_struct * Acoeff, * Bcoeff;
    ulong * Aexp, * Bexp;
    slong Blen = B->length;
    slong N;

    FLINT_ASSERT(A->bits == B->bits);
    FLINT_ASSERT(A->bits <= FLINT_BITS);
    FLINT_ASSERT(!fq_nmod_poly_is_zero(c, ctx->fqctx));

    if (A == B)
    {
        Acoeff = A->coeffs;
        for (i = 0; i < Blen; i++)
        {
            fq_nmod_poly_mul(t, Acoeff + i, c, ctx->fqctx);
            fq_nmod_poly_swap(t, Acoeff + i, ctx->fqctx);
        }
    }
    else
    {
        fq_nmod_mpolyn_fit_length(A, Blen, ctx);
        Acoeff = A->coeffs;
        Bcoeff = B->coeffs;
        Aexp = A->exps;
        Bexp = B->exps;

        N = mpoly_words_per_exp_sp(B->bits, ctx->minfo);

        for (i = 0; i < Blen; i++)
        {
            fq_nmod_poly_mul(Acoeff + i, Bcoeff + i, c, ctx->fqctx);
            mpoly_monomial_set(Aexp + N*i, Bexp + N*i, N);
        }
        A->length = Blen;
    }
}

void fq_nmod_mpolyun_mul_poly(
    fq_nmod_mpolyun_t A,
    const fq_nmod_mpolyun_t B,
    const fq_nmod_poly_t c,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i, Blen;
    fq_nmod_mpolyn_struct * Acoeff, * Bcoeff;
    ulong * Aexp, * Bexp;
    fq_nmod_poly_t t;

    FLINT_ASSERT(A->bits == B->bits);
    FLINT_ASSERT(!fq_nmod_poly_is_zero(c, ctx->fqctx));

    fq_nmod_poly_init(t, ctx->fqctx);

    Blen = B->length;
    fq_nmod_mpolyun_fit_length(A, Blen, ctx);
    Acoeff = A->coeffs;
    Bcoeff = B->coeffs;
    Aexp = A->exps;
    Bexp = B->exps;

    for (i = 0; i < Blen; i++)
    {
        fq_nmod_mpolyn_mul_poly(Acoeff + i, Bcoeff + i, c, t, ctx);
        Aexp[i] = Bexp[i];
    }
    A->length = Blen;

    fq_nmod_poly_clear(t, ctx->fqctx);
}


void fq_nmod_mpolyn_divexact_poly(
    fq_nmod_mpolyn_t A,
    const fq_nmod_mpolyn_t B,
    const fq_nmod_poly_t c,
    fq_nmod_poly_t q, /* temp */
    fq_nmod_poly_t r, /* temp */
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i;
    fq_nmod_poly_struct * Acoeff, * Bcoeff;
    ulong * Aexp, * Bexp;
    slong Blen = B->length;
    slong N;

    FLINT_ASSERT(A->bits == B->bits);
    FLINT_ASSERT(A->bits <= FLINT_BITS);
    FLINT_ASSERT(!fq_nmod_poly_is_zero(c, ctx->fqctx));

    if (A == B)
    {
        Acoeff = A->coeffs;
        for (i = 0; i < Blen; i++)
        {
            fq_nmod_poly_divrem(q, r, Acoeff + i, c, ctx->fqctx);
            FLINT_ASSERT(fq_nmod_poly_is_zero(r, ctx->fqctx));
            fq_nmod_poly_swap(q, Acoeff + i, ctx->fqctx);
        }
    }
    else
    {
        fq_nmod_mpolyn_fit_length(A, Blen, ctx);
        Acoeff = A->coeffs;
        Bcoeff = B->coeffs;
        Aexp = A->exps;
        Bexp = B->exps;

        N = mpoly_words_per_exp_sp(B->bits, ctx->minfo);

        for (i = 0; i < Blen; i++)
        {
            fq_nmod_poly_divrem(Acoeff + i, r, Bcoeff + i, c, ctx->fqctx);
            FLINT_ASSERT(fq_nmod_poly_is_zero(r, ctx->fqctx));
            mpoly_monomial_set(Aexp + N*i, Bexp + N*i, N);
        }
        A->length = Blen;
    }
}

void fq_nmod_mpolyun_divexact_poly(
    fq_nmod_mpolyun_t A,
    const fq_nmod_mpolyun_t B,
    const fq_nmod_poly_t c,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i, Blen;
    fq_nmod_mpolyn_struct * Acoeff, * Bcoeff;
    ulong * Aexp, * Bexp;
    fq_nmod_poly_t q, r;

    FLINT_ASSERT(A->bits == B->bits);
    FLINT_ASSERT(!fq_nmod_poly_is_zero(c, ctx->fqctx));

    fq_nmod_poly_init(q, ctx->fqctx);
    fq_nmod_poly_init(r, ctx->fqctx);

    Blen = B->length;
    fq_nmod_mpolyun_fit_length(A, Blen, ctx);
    Acoeff = A->coeffs;
    Bcoeff = B->coeffs;
    Aexp = A->exps;
    Bexp = B->exps;

    for (i = 0; i < Blen; i++)
    {
        fq_nmod_mpolyn_divexact_poly(Acoeff + i, Bcoeff + i, c, q, r, ctx);
        Aexp[i] = Bexp[i];
    }
    A->length = Blen;

    fq_nmod_poly_clear(q, ctx->fqctx);
    fq_nmod_poly_clear(r, ctx->fqctx);
}


void fq_nmod_mpolyun_content_poly(
    fq_nmod_poly_t g,
    fq_nmod_mpolyun_t B,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i, j;
    fq_nmod_poly_t t;

    fq_nmod_poly_zero(g, ctx->fqctx);
    fq_nmod_poly_init(t, ctx->fqctx);

    for (i = 0; i < B->length; i++)
    {
        for (j = 0; j < (B->coeffs + i)->length; j++)
        {
            fq_nmod_poly_gcd(t, g, (B->coeffs + i)->coeffs + j, ctx->fqctx);
            fq_nmod_poly_swap(t, g, ctx->fqctx);
            if (fq_nmod_poly_degree(g, ctx->fqctx) == 0)
                break;
        }
    }

    fq_nmod_poly_clear(t, ctx->fqctx);
}

void _fq_nmod_mpoly_to_mpolyun_perm_deflate(
                    fq_nmod_mpolyun_t A, const fq_nmod_mpoly_ctx_t uctx,
                    const fq_nmod_mpoly_t B, const fq_nmod_mpoly_ctx_t ctx,
                 const slong * perm, const ulong * shift, const ulong * stride)
{
    slong m = uctx->minfo->nvars;
    fq_nmod_mpolyu_t Au;
    fq_nmod_mpolyu_init(Au, A->bits, uctx);
    _fq_nmod_mpoly_to_mpolyu_perm_deflate(Au, uctx, B, ctx, perm, shift, stride);
    fq_nmod_mpolyu_cvtto_mpolyun(A, Au, m - 1, uctx);
    fq_nmod_mpolyu_clear(Au, uctx);
    return;
}

void _fq_nmod_mpoly_from_mpolyun_perm_inflate(
    fq_nmod_mpoly_t A, flint_bitcnt_t Abits, const fq_nmod_mpoly_ctx_t ctx,
                        fq_nmod_mpolyun_t B, const fq_nmod_mpoly_ctx_t uctx,
                 const slong * perm, const ulong * shift, const ulong * stride)
{
    slong m = uctx->minfo->nvars;
    fq_nmod_mpolyu_t Au;
    fq_nmod_mpolyu_init(Au, B->bits, uctx);
    fq_nmod_mpolyu_cvtfrom_mpolyun(Au, B, m - 1, uctx);
    _fq_nmod_mpoly_from_mpolyu_perm_inflate(A, Abits, ctx, Au, uctx, perm, shift, stride);
    fq_nmod_mpolyu_clear(Au, uctx);
    return;
}


slong fq_nmod_mpolyun_lastdeg(
    fq_nmod_mpolyun_t A,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i, j;
    slong deg = -WORD(1);

    for (i = 0; i < A->length; i++)
    {
        fq_nmod_mpolyn_struct * Ai = A->coeffs + i;
        for (j = 0; j < Ai->length; j++)
        {
            deg = FLINT_MAX(deg, fq_nmod_poly_degree(Ai->coeffs + j, ctx->fqctx));
        }
    }
    FLINT_ASSERT(deg >= 0);
    return deg;
}

void fq_nmod_mpolyun_set(
    fq_nmod_mpolyun_t A,
    const fq_nmod_mpolyun_t B,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i, Blen;
    fq_nmod_mpolyn_struct * Acoeff, * Bcoeff;
    ulong * Aexp, * Bexp;

    Blen = B->length;
    fq_nmod_mpolyun_fit_length(A, Blen, ctx);
    Acoeff = A->coeffs;
    Bcoeff = B->coeffs;
    Aexp = A->exps;
    Bexp = B->exps;

    for (i = 0; i < Blen; i++)
    {
        fq_nmod_mpolyn_set(Acoeff + i, Bcoeff + i, ctx);
        FLINT_ASSERT((Acoeff + i)->bits == B->bits);
        Aexp[i] = Bexp[i];
    }
    A->length = Blen;
}

/* take the last variable of B out */
void fq_nmod_mpoly_cvtto_mpolyn(
    fq_nmod_mpolyn_t A,
    fq_nmod_mpoly_t B,
    slong var,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i;
    slong k;
    ulong * oneexp;
    slong offset;
    slong shift;
    ulong mask;
    slong N;
    TMP_INIT;

    FLINT_ASSERT(B->bits <= FLINT_BITS);
    FLINT_ASSERT(ctx->minfo->ord == ORD_LEX);

    TMP_START;

    N = mpoly_words_per_exp_sp(B->bits, ctx->minfo);
    oneexp = TMP_ALLOC(N*sizeof(ulong));
    mask = (-UWORD(1)) >> (FLINT_BITS - B->bits);
    mpoly_gen_monomial_offset_shift_sp(oneexp, &offset, &shift, var,
                                                          B->bits, ctx->minfo);

    fq_nmod_mpolyn_fit_bits(A, B->bits, ctx);
    A->bits = B->bits;

    k = 0;
    fq_nmod_mpolyn_fit_length(A, k + 1, ctx);
    for (i = 0; i < B->length; i++)
    {
        ulong c = (B->exps[N*i + offset] >> shift) & mask;
        mpoly_monomial_msub(A->exps + N*k, B->exps + N*i, c, oneexp, N);

        if (k > 0 && mpoly_monomial_equal(A->exps + N*k, A->exps + N*(k - 1), N))
        {
            fq_nmod_poly_set_coeff(A->coeffs + k - 1, c, B->coeffs + i,
                                                                   ctx->fqctx);
        } else
        {
            fq_nmod_poly_zero(A->coeffs + k, ctx->fqctx);
            fq_nmod_poly_set_coeff(A->coeffs + k, c, B->coeffs + i, ctx->fqctx);
            k++;
            fq_nmod_mpolyn_fit_length(A, k + 1, ctx);
        }
    }

    fq_nmod_mpolyn_set_length(A, k, ctx);
    TMP_END;
}

void fq_nmod_mpolyu_cvtto_mpolyun(
    fq_nmod_mpolyun_t A,
    fq_nmod_mpolyu_t B,
    slong k,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i, Blen;
    fq_nmod_mpolyn_struct * Acoeff;
    fq_nmod_mpoly_struct * Bcoeff;
    ulong * Aexp, * Bexp;

    Blen = B->length;
    fq_nmod_mpolyun_fit_length(A, Blen, ctx);
    Acoeff = A->coeffs;
    Bcoeff = B->coeffs;
    Aexp = A->exps;
    Bexp = B->exps;

    for (i = 0; i < Blen; i++)
    {
        fq_nmod_mpoly_cvtto_mpolyn(Acoeff + i, Bcoeff + i, k, ctx);
        Aexp[i] = Bexp[i];
    }

    /* demote remaining coefficients */
    for (i = Blen; i < A->length; i++)
    {
        fq_nmod_mpolyn_clear(Acoeff + i, ctx);
        fq_nmod_mpolyn_init(Acoeff + i, A->bits, ctx);
    }
    A->length = Blen;  
}


/* put the last variable of B back into A */
void fq_nmod_mpoly_cvtfrom_mpolyn(
    fq_nmod_mpoly_t A,
    fq_nmod_mpolyn_t B,
    slong var,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i, j;
    slong k;
    slong N;
    ulong * oneexp;
    TMP_INIT;

    FLINT_ASSERT(B->bits == A->bits);
    FLINT_ASSERT(B->bits <= FLINT_BITS);
    FLINT_ASSERT(ctx->minfo->ord == ORD_LEX);

    TMP_START;

    N = mpoly_words_per_exp_sp(B->bits, ctx->minfo);
    oneexp = TMP_ALLOC(N*sizeof(ulong));
    mpoly_gen_monomial_sp(oneexp, var, B->bits, ctx->minfo);

    fq_nmod_mpoly_fit_length(A, B->length, ctx);

    k = 0;
    for (i = 0; i < B->length; i++)
    {
        for (j = (B->coeffs + i)->length - 1; j >= 0; j--)
        {
            if (!fq_nmod_is_zero((B->coeffs + i)->coeffs + j, ctx->fqctx))
            {
                fq_nmod_mpoly_fit_length(A, k + 1, ctx);
                fq_nmod_set(A->coeffs + k, (B->coeffs + i)->coeffs + j,
                                                                   ctx->fqctx);
                mpoly_monomial_madd(A->exps + N*k, B->exps + N*i, j, oneexp, N);                
                k++;
            }
        }
    }

    A->length = k;
    TMP_END;
}

void fq_nmod_mpolyu_cvtfrom_mpolyun(
    fq_nmod_mpolyu_t A,
    fq_nmod_mpolyun_t B,
    slong var,
    const fq_nmod_mpoly_ctx_t ctx)
{
    slong i;

    fq_nmod_mpolyu_fit_length(A, B->length, ctx);

    for (i = 0; i < B->length; i++)
    {
        fq_nmod_mpoly_cvtfrom_mpolyn(A->coeffs + i, B->coeffs + i, var, ctx);
        A->exps[i] = B->exps[i];
    }
    A->length = B->length;
}
