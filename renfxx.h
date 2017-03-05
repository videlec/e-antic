/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef NF_EMB_ELEMXX_H
#define NF_EMB_ELEMXX_H

#include "gmpxx.h"

#include "renf.h"
#include "renf_elem.h"

class renf_elem_class
{
private:
    mutable renf * nf;
    mutable renf_elem_t a;
    mutable fmpq_t b;
public:
    // construction as zero in a given number field
    renf_elem_class(renf_t nf);

    // construction as integers or rationals
    renf_elem_class(int);
    renf_elem_class(long);
    renf_elem_class(const mpz_class);
    renf_elem_class(const mpq_class);
    renf_elem_class(const fmpq_t);

    ~renf_elem_class();

    // assignment
    renf_elem_class& operator=(slong n); 
    renf_elem_class& operator=(fmpz_t z);
    renf_elem_class& operator=(fmpq_t q);
    renf_elem_class& operator=(fmpq_poly_t p);
    renf_elem_class& operator=(renf_elem_class a);

    // binary operations
    renf_elem_class operator+(const renf_elem_class &) const;
    renf_elem_class operator-(const renf_elem_class &) const;
    renf_elem_class operator*(const renf_elem_class &) const;
    renf_elem_class operator/(const renf_elem_class &) const;

    // inplace binary operations
    renf_elem_class& operator+=(const renf_elem_class &);
    renf_elem_class& operator-=(const renf_elem_class &);
    renf_elem_class& operator*=(const renf_elem_class &);
    renf_elem_class& operator/=(const renf_elem_class &);

    // unary operations
    renf_elem_class operator-();
    renf_elem_class operator+();

    // comparisons
    bool operator>(const renf_elem_class &);
    bool operator>=(const renf_elem_class &);
    bool operator==(const renf_elem_class &);
    bool operator<=(const renf_elem_class &);
    bool operator<(const renf_elem_class &);
    bool operator!=(const renf_elem_class &);

    bool operator>(slong);
    bool operator>=(slong);
    bool operator==(slong);
    bool operator<=(slong);
    bool operator<(slong);
    bool operator!=(slong);

    bool is_zero() { return renf_elem_is_zero(this->a, this->nf); };
    bool is_one() { return renf_elem_is_one(this->a, this->nf); };

    // input, output
    friend std::ostream& operator << (std::ostream &, const renf_elem_class&);
    void print();
};

renf_elem_class::renf_elem_class(renf_t nf)
{
    this->nf = nf;
    renf_elem_init(this->a, nf);
    renf_elem_zero(this->a, nf);
}

renf_elem_class::renf_elem_class(const mpq_class x)
{
    this->nf = NULL;
    fmpq_init(this->b);
    fmpq_set_mpq(this->b, x.__get_mp());
}

renf_elem_class::renf_elem_class(const fmpq_t x)
{
    this->nf = NULL;
    fmpq_init(this->b);
    fmpq_set(this->b, x);
}


renf_elem_class::renf_elem_class(const mpz_class x)
{
    this->nf = NULL;
    fmpq_init(this->b);
    fmpz_set_mpz(fmpq_numref(this->b), x.__get_mp());
    fmpz_set_si(fmpq_denref(this->b), 1);
}

renf_elem_class::renf_elem_class(int x)
{
    this->nf = NULL;
    fmpq_init(this->b);
    fmpq_set_si(this->b, x, 1);
}

renf_elem_class::renf_elem_class(long x)
{
    this->nf = NULL;
    fmpq_init(this->b);
    fmpq_set_si(this->b, x, 1);
}


#define __renf_elem_set(TYP, FUN) \
    renf_elem_class& renf_elem_class::operator=(TYP n) { FUN(this->a, n, this->nf); }
__renf_elem_set(slong, renf_elem_set_si);
__renf_elem_set(fmpz_t, renf_elem_set_fmpz);
__renf_elem_set(fmpq_t, renf_elem_set_fmpq);
__renf_elem_set(fmpq_poly_t, renf_elem_set_fmpq_poly);
#undef __renf_elem_set

renf_elem_class& renf_elem_class::operator=(renf_elem_class a)
{
    if (a.nf == NULL)
    {
        if (this->nf != NULL)
        {
            renf_elem_clear(this->a, this->nf);
            fmpq_init(this->b);
            this->nf = NULL;
        }
        fmpq_set(this->b, a.b);
    }
    else
    {
        if (this->nf == NULL)
        {
            this->nf = a.nf;
            fmpq_clear(this->b);
            renf_elem_init(this->a, this->nf);
        }
        else if (this->nf != a.nf)
        {
            renf_elem_clear(this->a, this->nf);
            this->nf = a.nf;
            renf_elem_init(this->a, this->nf);
        }
        renf_elem_set(this->a, a.a, this->nf);
    }
}

renf_elem_class::~renf_elem_class(void)
{
    if (nf == NULL) fmpq_clear(this->b);
    else renf_elem_clear(this->a, this->nf);
}

std::ostream& operator<<(std::ostream & os, const renf_elem_class& a)
{
    char * res;
    if (a.nf == NULL)
        res = fmpq_get_str(NULL, 10, a.b);
    else
        res = renf_elem_get_str_pretty(a.a, "x", a.nf, 5);
    os << res;
    flint_free(res);
    return os;
}

#define __renf_elem_op(OP, INOP, FUN1, FUN2, FUN3) \
renf_elem_class renf_elem_class::OP(const renf_elem_class & other) const \
{                                                     \
    if (this->nf != NULL)                             \
    {                                                 \
        renf_elem_class ans(this->nf);                \
        if (this->nf == other.nf)                     \
            FUN1(ans.a, this->a, other.a, this->nf);  \
        else if (other.nf == NULL)                    \
            FUN2(ans.a, this->a, other.b, this->nf);  \
        else                                          \
            flint_abort();                            \
        return ans;                                   \
    }                                                 \
    else if (other.nf == NULL)                        \
    {                                                 \
        renf_elem_class ans(this->b);                 \
        FUN3(ans.b, this->b, other.b);                \
        return ans;                                   \
    }                                                 \
    else                                              \
    {                                                 \
        renf_elem_class ans(other.nf);                \
        FUN2(ans.a, other.a, this->b, other.nf);      \
        return ans;                                   \
    }                                                 \
}
// TODO: implement inplace operators
__renf_elem_op(operator+, operator+=, renf_elem_add, renf_elem_add_fmpq, fmpq_add);
__renf_elem_op(operator*, operator*=, renf_elem_mul, renf_elem_mul_fmpq, fmpq_mul);
__renf_elem_op(operator-, operator-=, renf_elem_sub, renf_elem_sub_fmpq, fmpq_sub);
__renf_elem_op(operator/, operator/=, renf_elem_div, renf_elem_div_fmpq, fmpq_div);
#undef __renf_elem_op


bool renf_elem_class::operator==(const renf_elem_class& other)
{
    if (this->nf != NULL)
    {
        if (this->nf == other.nf)
            return renf_elem_equal(this->a, other.a, this->nf);
        else if (other.nf == NULL)
            return renf_elem_cmp_fmpq(this->a, other.b, this->nf) == 0;
        else
            flint_abort();
    }
    else if (other.nf == NULL)
    {
        return fmpq_equal(this->b, other.b);
    }
    else
    {
        return renf_elem_cmp_fmpq(other.a, this->b, other.nf) == 0;
    }
}
bool renf_elem_class::operator==(slong n)
{
    if (this->nf != NULL)
    {
        renf_elem_class other(this->nf);
        renf_elem_set_si(other.a, n, this->nf);
        return renf_elem_equal(this->a, other.a, this->nf);
    }
    else
    {
        fmpq_t other;
        int ans;
        fmpq_init(other);
        fmpq_set_si(other, n, 1);
        ans = fmpq_cmp(this->b, other) == 0;
        flint_free(other);
        return ans;
    }
}

bool renf_elem_class::operator>(const renf_elem_class & other)
{
    if (this->nf != NULL)
    {
        if (this->nf == other.nf)
            return renf_elem_cmp(this->a, other.a, this->nf) > 0;
        else if (other.nf == NULL)
            return renf_elem_cmp_fmpq(this->a, other.b, this->nf) > 0;
        else
            flint_abort();
    }
    else if (other.nf == NULL)
    {
        return fmpq_cmp(this->b, other.b) > 0;
    }
    else
    {
        return renf_elem_cmp_fmpq(other.a, this->b, other.nf) < 0;
    }
}
bool renf_elem_class::operator>(slong n)
{
    if (this->nf != NULL)
    {
        renf_elem_class other(this->nf);
        renf_elem_set_si(other.a, n, this->nf);
        return renf_elem_cmp(this->a, other.a, this->nf) > 0;
    }
    else
    {
        fmpq_t other;
        int ans;
        fmpq_init(other);
        fmpq_set_si(other, n, 1);
        ans = fmpq_cmp(this->b, other) > 0;
        flint_free(other);
        return ans;
    }
}

bool renf_elem_class::operator!=(const renf_elem_class& other)
{
    return not (*this == other);
}
bool renf_elem_class::operator!=(slong n)
{
    return not (*this == n);
}

bool renf_elem_class::operator>=(const renf_elem_class & other)
{
    return *this == other || *this > other;
}
bool renf_elem_class::operator>=(slong n)
{
    return *this == n || *this > n;
}

bool renf_elem_class::operator<=(const renf_elem_class& other)
{
    return not (*this > other);
}
bool renf_elem_class::operator<=(slong n)
{
    return not (*this > n);
}

bool renf_elem_class::operator<(const renf_elem_class& other)
{
    return not (*this >= other);
}
bool renf_elem_class::operator<(slong n)
{
    return not (*this >= 0);
}

#endif
