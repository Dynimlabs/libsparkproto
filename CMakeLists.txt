cmake_minimum_required(VERSION 3.10.2)

project(sparkproto)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# try using c++14, if c++14 does not find, a previous standard will be used
set(CMAKE_CXX_STANDARD 14)

# set version of library
set(MAJOR 1)
set(MINOR 0)
set(PATCH 0)
set(VERSION_SUFFIX "a1")

configure_file(configure.h.in ${CMAKE_CURRENT_BINARY_DIR}/libsparkproto/configure.h)

if(ENABLE_LOGGING)
        add_definitions(-DSPARK_LOGGING_ENABLE)
endif(ENABLE_LOGGING)

find_package(OpenCV)

find_package(Protobuf REQUIRED)

file(GLOB SRC "src/*.cc" "generated/libsparkproto/*.cc" "libsparkproto/*.cc")

add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC})

target_include_directories(
    ${CMAKE_PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROTOBUF_INCLUDE_DIR}>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/generated>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PUBLIC $<INSTALL_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:${CMAKE_SOURCE_DIR}/generated>
)

target_link_libraries(${CMAKE_PROJECT_NAME} ${PROTOBUF_LIBRARIES})

install(TARGETS ${CMAKE_PROJECT_NAME}
        LIBRARY DESTINATION lib COMPONENT Runtime
        ARCHIVE DESTINATION lib COMPONENT Development
        PUBLIC_HEADER DESTINATION include COMPONENT Development
        )

install(DIRECTORY
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/generated/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        )

install(DIRECTORY
        ${CMAKE_SOURCE_DIR}/libsparkproto
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        )


# add unittest
if(ENABLE_TESTS)
        enable_testing()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")
        add_subdirectory(test)
endif(ENABLE_TESTS)

# add examples
if(ENABLE_EXAMPLES)
        add_subdirectory(examples)
endif(ENABLE_EXAMPLES)
