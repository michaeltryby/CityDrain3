
// Copyright Aleksey Gurtovoy 2000-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: remove.cpp 49268 2008-10-11 06:26:17Z agurtovoy $
// $Date: 2008-10-11 02:26:17 -0400 (Sat, 11 Oct 2008) $
// $Revision: 49268 $

#include <boost/mpl/remove.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/equal.hpp>

#include <boost/mpl/aux_/test.hpp>


MPL_TEST_CASE()
{
    typedef vector6<int,float,char,float,float,double> types;
    typedef mpl::remove< types,float >::type result;
    typedef vector3<int,char,double> answer;
    MPL_ASSERT(( equal< result,answer > ));
}
