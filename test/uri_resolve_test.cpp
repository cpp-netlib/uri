// Copyright (c) Glyn Matthews 2012, 2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <gtest/gtest.h>
#include <network/uri.hpp>

const network::uri base_uri("http://a/b/c/d;p?q");

// normal examples
// http://tools.ietf.org/html/rfc3986#section-5.4.1

TEST(uri_resolve_test, DISABLED_normal_example_1) {
//     "g"             =  "http://a/b/c/g"
  network::uri_builder builder;
  builder
    .path("g")
    ;
  ASSERT_EQ(network::uri("http://a/b/c/g"),
	    base_uri.resolve(network::uri(builder), network::uri_comparison_level::string_comparison));
}

//      "./g"           =  "http://a/b/c/g"
//      "g/"            =  "http://a/b/c/g/"
//      "/g"            =  "http://a/g"
//      "//g"           =  "http://g"
//      "?y"            =  "http://a/b/c/d;p?y"
//      "g?y"           =  "http://a/b/c/g?y"
//      "#s"            =  "http://a/b/c/d;p?q#s"
//      "g#s"           =  "http://a/b/c/g#s"
//      "g?y#s"         =  "http://a/b/c/g?y#s"
//      ";x"            =  "http://a/b/c/;x"
//      "g;x"           =  "http://a/b/c/g;x"
//      "g;x?y#s"       =  "http://a/b/c/g;x?y#s"
//      ""              =  "http://a/b/c/d;p?q"
//      "."             =  "http://a/b/c/"
//      "./"            =  "http://a/b/c/"
//      ".."            =  "http://a/b/"
//      "../"           =  "http://a/b/"
//      "../g"          =  "http://a/b/g"
//      "../.."         =  "http://a/"
//      "../../"        =  "http://a/"
//      "../../g"       =  "http://a/g"

// abnormal examples
// http://tools.ietf.org/html/rfc3986#section-5.4.2

//      "../../../g"    =  "http://a/g"
//      "../../../../g" =  "http://a/g"
//      "/./g"          =  "http://a/g"
//      "/../g"         =  "http://a/g"
//      "g."            =  "http://a/b/c/g."
//      ".g"            =  "http://a/b/c/.g"
//      "g.."           =  "http://a/b/c/g.."
//      "..g"           =  "http://a/b/c/..g"
//      "./../g"        =  "http://a/b/g"
//      "./g/."         =  "http://a/b/c/g/"
//      "g/./h"         =  "http://a/b/c/g/h"
//      "g/../h"        =  "http://a/b/c/h"
//      "g;x=1/./y"     =  "http://a/b/c/g;x=1/y"
//      "g;x=1/../y"    =  "http://a/b/c/y"
//      "g?y/./x"       =  "http://a/b/c/g?y/./x"
//      "g?y/../x"      =  "http://a/b/c/g?y/../x"
//      "g#s/./x"       =  "http://a/b/c/g#s/./x"
//      "g#s/../x"      =  "http://a/b/c/g#s/../x"
