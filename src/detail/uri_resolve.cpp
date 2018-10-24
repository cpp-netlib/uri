// Copyright 2013-2016 Glyn Matthews.
// Copyright 2013 Hannes Kamecke.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "uri_resolve.hpp"
#include <algorithm>

#ifdef NETWORK_URI_EXTERNAL_BOOST
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>
namespace network_boost = boost;
#else   // NETWORK_URI_EXTERNAL_BOOST
#include "../boost/algorithm/string/find.hpp"
#include "../boost/algorithm/string/erase.hpp"
#include "../boost/algorithm/string/replace.hpp"
#include "../boost/algorithm/string/predicate.hpp"
#endif  // NETWORK_URI_EXTERNAL_BOOST

namespace network {
namespace detail {
// remove_dot_segments
inline
void remove_last_segment(std::string& path) {
  while (!path.empty()) {
    if (path.back() == '/') {
      path.pop_back();
      break;
    }
    path.pop_back();
  }
}

// implementation of http://tools.ietf.org/html/rfc3986#section-5.2.4
static
std::string remove_dot_segments(std::string input) {
  std::string result;

  while (!input.empty()) {
    if (network_boost::starts_with(input, "../")) {
      network_boost::erase_head(input, 3);
    } else if (network_boost::starts_with(input, "./")) {
      network_boost::erase_head(input, 2);
    } else if (network_boost::starts_with(input, "/./")) {
      network_boost::replace_head(input, 3, "/");
    } else if (input == "/.") {
      network_boost::replace_head(input, 2, "/");
    } else if (network_boost::starts_with(input, "/../")) {
      network_boost::erase_head(input, 3);
      remove_last_segment(result);
    } else if (network_boost::starts_with(input, "/..")) {
      network_boost::replace_head(input, 3, "/");
      remove_last_segment(result);
    } else if (network_boost::algorithm::all(input, [](char ch) { return ch == '.'; })) {
      input.clear();
    }
    else {
      int n = (input.front() == '/')? 1 : 0;
      auto slash = network_boost::find_nth(input, "/", n);
      result.append(std::begin(input), std::begin(slash));
      input.erase(std::begin(input), std::begin(slash));
    }
  }
  return result;
}

std::string remove_dot_segments(string_view path) {
  return remove_dot_segments(path.to_string());
}

// implementation of http://tools.ietf.org/html/rfc3986#section-5.2.3
std::string merge_paths(const uri& base, const uri& reference) {
  std::string result;

  if (!base.has_path() || base.path().empty()) {
    result = "/";
  } else {
    const auto& base_path = base.path();
    auto last_slash = network_boost::find_last(base_path, "/");
    result.append(std::begin(base_path), std::end(last_slash));
  }
  if (reference.has_path()) {
    result.append(reference.path().to_string());
  }
  return remove_dot_segments(string_view(result));
}
}  // namespace detail
}  // namespace network
