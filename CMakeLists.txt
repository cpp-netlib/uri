# Copyright (c) Glyn Matthews 2012.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)


cmake_minimum_required(VERSION 2.8)
project(Uri)

find_package(Threads REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.52 REQUIRED)

include_directories(${Uri_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

set(CMAKE_VERBOSE_MAKEFILE true)

if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  INCLUDE(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)

  if (HAVE_STD11)
    set(CMAKE_CXX_FLAGS -std=c++11)
  else()
    message(FATAL_ERROR "No C++ 11 support (Compiler does not define -std=c++11).")
  endif()
  message("C++ Flags: ${CMAKE_CXX_FLAGS} link flags: ${CMAKE_CXX_LINK_FLAGS}")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
  set(CMAKE_CXX_LINK_FLAGS "-std=c++11 -stdlib=libc++")
  message("C++ Flags: ${CMAKE_CXX_FLAGS} link flags: ${CMAKE_CXX_LINK_FLAGS}")
endif()


if (MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_VARIADIC_MAX=10)
endif(MSVC)
if (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif(WIN32)

set(Uri_SRCS src/uri.cpp src/uri_io.cpp src/uri_builder.cpp src/detail/uri_parse.cpp)
add_library(cppnetlib-uri ${Uri_SRCS})

if (CMAKE_PROJECT_NAME STREQUAL "Uri" OR CPP-NETLIB_BUILD_TESTS)
  enable_testing()
  if (NOT DEFINED GTEST_FOUND)
    find_package(GTest REQUIRED)
  endif()
  
  include_directories(${GTEST_INCLUDE_DIRS})

  set(
    TESTS
    uri_test
    uri_encoding_test
    uri_normalization_test
    uri_comparison_test
    uri_relativize_test
    uri_resolve_test
    uri_builder_test
    )
  foreach (test ${TESTS})
    add_executable(${test} test/${test}.cpp)
    target_link_libraries(${test} ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-uri)
    if (OPENSSL_FOUND)
      target_link_libraries(${test} ${OPENSSL_LIBRARIES})
    endif()
    set_target_properties(${test}
      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${Uri_BINARY_DIR}/tests)
    add_test(${test}
        ${Uri_BINARY_DIR}/tests/${test})
  endforeach (test)

endif()
