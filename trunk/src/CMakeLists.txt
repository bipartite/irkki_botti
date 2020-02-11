FILE(GLOB SOURCE_FILES "*.cc")
FILE(GLOB HEADER_FILES "*.hh")

set(SOURCES ${SOURCE_FILES} ${HEADER_FILES})
cmake_minimum_required(VERSION 2.6)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GCC detected, enabling pedantic mode and warnings")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Generate config.h
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Libraries

# An example of how to use a library. You'll need FindExampleLibrary.cmake for this to work
# Put that file in trunk/cmake/Modules/ and also look inside the file for further instructions.
#find_package(ExampleLibrary REQUIRED)
#include_directories(${ExampleLibrary_INCLUDE_DIRS})
#set(LIBS ${LIBS} ${ExampleLibrary_LIBRARIES})

find_package(Boost REQUIRED COMPONENTS filesystem )
include_directories(${Boost_INCLUDE_DIRS})
set(LIBS ${LIBS} ${Boost_LIBRARIES})




# Assumes the project generates only one executable. If you need more, you'll need to alter
# the script and replace ${PROJECT_SHORT_NAME} by executable name.
add_executable("${PROJECT_SHORT_NAME}" ${SOURCES})
target_link_libraries("${PROJECT_SHORT_NAME}" ${LIBS})
install(TARGETS "${PROJECT_SHORT_NAME}" DESTINATION bin)

