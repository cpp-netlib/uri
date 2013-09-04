// Copyright (c) Glyn Matthews 2012, 2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <gtest/gtest.h>
#include <network/uri.hpp>

// Compare the underlying strings because ``normalize`` is used in the
// ``uri`` equality operator.

TEST(uri_normalization_test, string_comparison) {
  network::uri instance("http://www.example.com/");
  ASSERT_EQ("http://www.example.com/",
	    instance.normalize(network::uri_comparison_level::string_comparison));
}

TEST(uri_normalization_test, string_comparison_with_case) {
  network::uri instance("HTTP://www.example.com/");
  ASSERT_EQ("HTTP://www.example.com/",
            instance.normalize(network::uri_comparison_level::string_comparison));
}

TEST(uri_normalization_test, normalize_case) {
  network::uri instance("HTTP://www.example.com/");
  ASSERT_EQ("http://www.example.com/",
            instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, normalize_percent_encoding) {
  network::uri instance("http://www%2Eexample%2Ecom/%7E%66%6F%6F%62%61%72%5F%36%39/");
  ASSERT_EQ("http://www.example.com/~foobar_69/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, normalize_with_lower_case) {
  network::uri instance("http://www.example.com/%2a");
  ASSERT_EQ("http://www.example.com/%2A",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, normalize_percent_encoding_with_lower_case_elements) {
  network::uri instance("http://www%2eexample%2ecom/%7e%66%6f%6f%62%61%72%5f%36%39/");
  ASSERT_EQ("http://www.example.com/~foobar_69/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, normalize_percent_encoding_is_upper_case) {
  network::uri instance("HTTP://www%2Eexample%2Ecom/%7E%66%6F%6F%62%61%72%5F%36%39/");
  ASSERT_EQ("http://www.example.com/~foobar_69/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_add_trailing_slash) {
  network::uri instance("http://www.example.com");
  ASSERT_EQ("http://www.example.com/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_dont_add_trailing_slash_if_path_is_not_empty) {
  network::uri instance("http://www.example.com/path");
  ASSERT_EQ("http://www.example.com/path",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_remove_dot_segments) {
  network::uri instance("http://www.example.com/a/./b/");
  ASSERT_EQ("http://www.example.com/a/b/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_remove_double_dot_segments) {
  network::uri instance("http://www.example.com/a/../b/");
  ASSERT_EQ("http://www.example.com/b/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_remove_dot_segments_with_percent_encoded_dot) {
  network::uri instance("http://www.example.com/a/%2E/b/");
  ASSERT_EQ("http://www.example.com/a/b/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_remove_double_dot_segments_with_percent_encoded_dots) {
  network::uri instance("http://www.example.com/a/%2E%2E/b/");
  ASSERT_EQ("http://www.example.com/b/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_remove_double_dot_segments_with_query) {
  network::uri instance("http://www.example.com/a/../b/?key=value");
  ASSERT_EQ("http://www.example.com/b/?key=value",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_remove_double_dot_segments_with_empty_query) {
  network::uri instance("http://www.example.com/a/../b/?");
  ASSERT_EQ("http://www.example.com/b/?",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_remove_double_dot_segments_with_fragment) {
  network::uri instance("http://www.example.com/a/../b/#fragment");
  ASSERT_EQ("http://www.example.com/b/#fragment",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_remove_double_dot_segments_with_empty_fragment) {
  network::uri instance("http://www.example.com/a/../b/#");
  ASSERT_EQ("http://www.example.com/b/#",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_segmented_remove_double_dot_segments_with_query_and_fragment) {
  network::uri instance("http://www.example.com/a/../b/?key=value#fragment");
  ASSERT_EQ("http://www.example.com/b/?key=value#fragment",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_double_dash) {
  network::uri instance("http://www.example.com//");
  ASSERT_EQ("http://www.example.com/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_triple_dash) {
  network::uri instance("http://www.example.com///");
  ASSERT_EQ("http://www.example.com/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_depth_below_root) {
  network::uri instance("http://www.example.com/..");
  ASSERT_THROW(instance.normalize(network::uri_comparison_level::syntax_based), std::system_error);
}

TEST(uri_normalization_test, path_depth_below_root_2) {
  network::uri instance("http://www.example.com/a/../..");
  ASSERT_THROW(instance.normalize(network::uri_comparison_level::syntax_based), std::system_error);
}

TEST(uri_normalization_test, path_dash_dot_dash) {
  network::uri instance("http://www.example.com/./");
  ASSERT_EQ("http://www.example.com/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}

TEST(uri_normalization_test, path_dash_dot_dash_dot) {
  network::uri instance("http://www.example.com/./.");
  ASSERT_EQ("http://www.example.com/",
	    instance.normalize(network::uri_comparison_level::syntax_based));
}
