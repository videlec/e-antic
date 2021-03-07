/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016-2018 Vincent Delecroix
    Copyright (C) 2019-2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_CPPYY_H
#define E_ANTIC_CPPYY_H

#include "renfxx.h"

namespace eantic {
namespace cppyy {

// cppyy does not see the operators provided by boost::operators so we provide
// something to make them explicit here:
template <typename S, typename T>
auto add(const S& lhs, const T& rhs) { return lhs + rhs; }
template <typename S, typename T>
auto sub(const S& lhs, const T& rhs) { return lhs - rhs; }
template <typename S, typename T>
auto mul(const S& lhs, const T& rhs) { return lhs * rhs; }
template <typename S, typename T>
auto truediv(const S& lhs, const T& rhs) { return lhs / rhs; }
template <typename T>
auto neg(const T& value) { return -value; }

inline mpq_class rational(const renf_elem_class& x) {
    return static_cast<mpq_class>(x);
}

}  // namespace cppyy
}  // namespace eantic

#endif
