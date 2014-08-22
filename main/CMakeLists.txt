file(GLOB SOURCES "*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES}) 

target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_support)
