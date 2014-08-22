find_package(Threads REQUIRED)
include(ExternalProject)

set(GMOCK_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/gmock")

ExternalProject_Add(GMockExternal
    SVN_REPOSITORY http://googlemock.googlecode.com/svn/trunk
    SVN_REVISION -r HEAD
    TIMEOUT 10
    PREFIX "${GMOCK_PREFIX}"
    CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
    UPDATE_COMMAND ""
    INSTALL_COMMAND "")

set(LIBPREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
set(LIBSUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")

set(GMOCK_SRC_LOCATION "${GMOCK_PREFIX}/src/GMockExternal")
set(GMOCK_LOCATION "${GMOCK_PREFIX}/src/GMockExternal-build")

set(GMOCK_LIBRARY  "${GMOCK_LOCATION}/${LIBPREFIX}gtest${LIBSUFFIX}")
set(GMOCK_MAINLIB  "${GMOCK_LOCATION}/${LIBPREFIX}gtest_main${LIBSUFFIX}")

set(GMOCK_INCLUDE_DIRS "${GMOCK_SRC_LOCATION}/gtest/include" "${GMOCK_SRC_LOCATION}/include")
set(GMOCK_LIBRARY_DIRS "${GMOCK_LOCATION}" "${GMOCK_LOCATION}/gtest")
set(GMOCK_LIBS gtest gtest_main gmock gmock_main)