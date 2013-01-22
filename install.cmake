# Copyright (c) Wojciech Kordalski 2013.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

set(CPPNETLIB_URI_TARGETS cppnetlib-uri)

install(TARGETS ${CPPNETLIB_URI_TARGETS}
    EXPORT cppnetlib-depends
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev
)

install(DIRECTORY include/network DESTINATION "${INSTALL_INC_DIR}/boost" COMPONENT dev)
configure_file(CppNetlib-uri-Configure.cmake.in "${PROJECT_BINARY_DIR}/CppNetlib-uri-Configure.cmake" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/CppNetlib-uri-Configure.cmake" DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)