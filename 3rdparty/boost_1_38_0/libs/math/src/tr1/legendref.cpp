//  Copyright John Maddock 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_MATH_TR1_SOURCE
#include <boost/math/tr1.hpp>
#include <boost/math/special_functions/legendre.hpp>
#include "c_policy.hpp"

extern "C" float BOOST_MATH_TR1_DECL legendref BOOST_PREVENT_MACRO_SUBSTITUTION(unsigned n, float x)
{
   return c_policies::legendre_p BOOST_PREVENT_MACRO_SUBSTITUTION(n, x);
}
