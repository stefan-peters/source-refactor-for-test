# Find LLVM
#
# It defines the following variables
#  LLVM_FOUND        - True if llvm found.
#  LLVM_INCLUDE_DIRS - where to find llvm include files
#  LLVM_LIBRARY_DIRS - where to find llvm libs
#  LLVM_CFLAGS       - llvm compiler flags
#  LLVM_LDFLAGS      - llvm linker flags
#  LLVM_MODULE_LIBS  - list of llvm libs for working with modules.
#  LLVM_SYSTEM_LIBS  - list of system libs needed by llvm

find_program(LLVM_CONFIG_EXECUTABLE llvm-config DOC "llvm-config executable")

execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --version
  OUTPUT_VARIABLE LLVM_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1" LLVM_VERSION_MAJOR
	     "${LLVM_VERSION}")

string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\2" LLVM_VERSION_MINOR
	     "${LLVM_VERSION}")

execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --includedir
  OUTPUT_VARIABLE LLVM_INCLUDE_DIRS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --libdir
  OUTPUT_VARIABLE LLVM_LIBRARY_DIRS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --cppflags
  OUTPUT_VARIABLE LLVM_CFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (LLVM_CFLAGS MATCHES "\\-DNDEBUG")
    set(LLVM_WITH_NDEBUG TRUE)
else (LLVM_CFLAGS MATCHES "\\-DNDEBUG")
    set(LLVM_WITH_NDEBUG FALSE)
endif (LLVM_CFLAGS MATCHES "\\-DNDEBUG")


find_library(LLVM_MODULE_LIBS LLVM-${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR} ${LLVM_LIBRARY_DIRS})

if (NOT LLVM_MODULE_LIBS)
  execute_process(
    COMMAND ${LLVM_CONFIG_EXECUTABLE} --libs
    OUTPUT_VARIABLE LLVM_MODULE_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()

execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --ldflags
  OUTPUT_VARIABLE LLVM_LDFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)


execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --system-libs
  OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LLVM DEFAULT_MSG LLVM_INCLUDE_DIRS LLVM_MODULE_LIBS)
