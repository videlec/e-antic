/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renf_elem.h>
#include <stdio.h>

void main(void)
{
    fmpq_t k;
    renf_t nf;
    renf_elem_t a,b;
    fmpq_poly_t pol;
    char * s;
    size_t i;
    int flags[5] = {
        EANTIC_STR_ALG,
        EANTIC_STR_D,
        EANTIC_STR_ARB,
        EANTIC_STR_ALG | EANTIC_STR_D,
        EANTIC_STR_ALG | EANTIC_STR_ARB};

    FLINT_TEST_INIT(state);

    fmpq_init(k);
    fmpq_set_si(k, 5, 1);
    renf_init_nth_root_fmpq(nf, k, 2, 20 + n_randint(state, 100));
    renf_elem_init(a, nf);

    fmpq_poly_init(pol);
    fmpq_set_si(k, 1, 2);
    fmpq_poly_set_coeff_fmpq(pol, 0, k);
    fmpq_set_si(k, -2, 3);
    fmpq_poly_set_coeff_fmpq(pol, 1, k);

    renf_elem_set_fmpq_poly(a, pol, nf);

    /* flags are EANTIC_STR_ALG, EANTIC_STR_D and EANTIC_STR_ARB */
    for (i = 0; i < 5; i++)
    {
        s = renf_elem_get_str_pretty(a, "x", nf, 10, flags[i]);
        printf("%d: ", flags[i]);
        printf(s);
        printf("\n");
        fflush(stdout);
        flint_free(s);
    }

    FLINT_TEST_CLEANUP(state);
}

