/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_auxiliary.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/support_argument.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include "test.hpp"

int
main()
{
    using spirit_test::test;
    using namespace boost::spirit;
    using namespace boost::spirit::qi;
    using namespace boost::phoenix;
    using namespace boost::spirit::arg_names;

    {
        BOOST_TEST(test("123", lazy(val(int_))));
    }

    {
        int result;
        BOOST_TEST(test("123", lazy(val(int_))[ref(result) = _1]));
        BOOST_TEST((result == 123));
    }

    {
        rule<char const*, std::string()> r;

        r =
                '<' >> *(char_ - '>')[_val += _1] >> '>'
            >>  "</" >> lazy(_val) >> '>'
        ;

        BOOST_TEST(test("<tag></tag>", r));
        BOOST_TEST(!test("<foo></bar>", r));
    }

    return boost::report_errors();
}
