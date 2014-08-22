file(GLOB LIB_SOURCES "src/*.cpp")

add_definitions(
    ${LLVM_CFLAGS}
    -fno-rtti
    -fno-exceptions
)

include_directories (
	"src"
    ${LLVM_INCLUDE_DIRS}
    ${CLANG_INCLUDE_DIRS}
)

set(LIB_NAME ${CMAKE_PROJECT_NAME}_support)

add_library(${LIB_NAME} SHARED ${LIB_SOURCES}) 

target_link_libraries(${LIB_NAME}
	${LLVM_LDFLAGS}
	${CLANG_LIBS}
	${LLVM_MODULE_LIBS}
	${LLVM_SYSTEM_LIBS}
	${PYTHON_LIBRARY}
)


set_target_properties(${LIB_NAME} PROPERTIES 
	SOVERSION ${VERSION}
    VERSION ${VERSION}
)

#
# TESTS
#

if(BUILD_TESTS)
	
	include(UseGoogleMock)

	include_directories(${GMOCK_INCLUDE_DIRS})
	link_directories(${GMOCK_LIBRARY_DIRS})

	macro(add_gmock_test source)

	  	GET_FILENAME_COMPONENT(test_name ${source} NAME)
	  	add_executable(${test_name} ${source} "tests/main.cpp")
		add_dependencies(${test_name} GMockExternal ${LIB_NAME})

		target_link_libraries(${test_name} ${GMOCK_LIBS} ${LIB_NAME})
	  	add_test(NAME ${test_name} COMMAND ${test_name})
	endmacro()


	file(GLOB TEST_SOURCES "tests/test_*.cpp")

	foreach(source ${TEST_SOURCES})
		add_gmock_test(${source})
	endforeach()
endif()
