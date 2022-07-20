cmake_minimum_required(VERSION 3.1)

project(centicom
        VERSION 0.1
        LANGUAGES C CXX)

set(EXECUTABLE_NAME "centicom")

# C++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -O2")
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Setting Clang-specific compiler flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                         -Weverything \
                         -Wno-unknown-warning-option \
                         -Wno-c++98-compat \
                         -Wno-reserved-identifier \
                         -Wno-reserved-macro-identifier \
                         -Wno-padded")
endif()

# Options
option(DEBUG "Enable debugging symbols" OFF)

# Required libraries
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
find_package(Boost 1.65.0 COMPONENTS program_options)

if(!BOOST_FOUND)
    message(FATAL_ERROR "Could not find the required Boost libraries!")
endif()

if(DEBUG)
    message(STATUS "Enabling debug symbols")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

# Enable pthread support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

#configure_file(${CMAKE_CURRENT_LIST_DIR}/inc/config.h.in
#               ${CMAKE_CURRENT_BINARY_DIR}/inc/config.h)

add_executable(${EXECUTABLE_NAME})

target_sources(${EXECUTABLE_NAME} PRIVATE
               ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
               ${CMAKE_CURRENT_LIST_DIR}/src/Centicom.cpp
               ${CMAKE_CURRENT_LIST_DIR}/src/Curses.cpp
               ${CMAKE_CURRENT_LIST_DIR}/src/Connection.cpp)

include_directories(${EXECUTABLE_NAME}
                    ${CMAKE_CURRENT_LIST_DIR}/inc/
                    ${CMAKE_CURRENT_BINARY_DIR}/inc/
                    ${Boost_INCLUDE_DIRS})

target_link_libraries(${EXECUTABLE_NAME}
                      ${CURSES_LIBRARIES}
                      ${Boost_LIBRARIES})
