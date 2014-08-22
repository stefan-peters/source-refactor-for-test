option(BUILD_TESTS "Build tests" ON)
option(CMAKE_VERBOSE_MAKEFILE "Build verbose Makefiles" OFF)

set(VERSION "0.1.0")

# http://bloerg.net/2012/11/10/cmake-and-distutils.html
# https://bitbucket.org/cavallo71/example.cmake.python.module.spam/src

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules")

project(s2tr)
 
 if(APPLE)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
    # use the port libs instead of mixing python interpreter,libs and crash at the end
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/local/lib)
    cmake_policy(SET CMP0042 NEW) #todo is this correct?
else()
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)

link_directories(
    ${LLVM_LIBRARY_DIRS}
    ${CLANG_LIBRARY_DIRS}
)

include_directories(
	"lib/include"
)

if(BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(lib)
add_subdirectory(main)

