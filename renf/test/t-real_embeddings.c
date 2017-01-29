/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "renf.h"
#include "poly_extra.h"

#define MAX_DEGREE 20

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    printf("real_embeddings....");
    fflush(stdout);

    for (iter = 0; iter < 100; iter++)
    {
        int i, n;
        fmpz_poly_t p;
        renf nfemb[MAX_DEGREE];

        fmpz_poly_init(p);

        do{
            fmpz_poly_randtest_irreducible(p, state, MAX_DEGREE + 1, 20);
        }while(fmpz_poly_degree(p) < 1);

        n  = renf_set_embeddings_fmpz_poly(nfemb, p, MAX_DEGREE, 64);

        fmpz_poly_clear(p);
        for (i = 0; i < n; i++) renf_clear(nfemb + i);
    }

    FLINT_TEST_CLEANUP(state);

    printf("PASS\n");
    return 0;
}