cmake_minimum_required(VERSION 3.10)
project(person_tracker)

set(CMAKE_CXX_STANDARD 17)
# Set compiler flags, Wall for extra warnings, pedantic for more warnings regarding extensions,
# pthread for being able to do threading in c++.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -pthread")
set(CMAKE_C_COMPILER "gcc-7")
set(CMAKE_CXX_COMPILER "g++-7")

# Include package fmt (for formatting text. This needs to be installed separately)
find_package(fmt REQUIRED)

# Include Boost components
find_package(Boost 1.65 COMPONENTS system thread program_options filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Include OpenCV
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/lib)
include_directories(/opt/include)
find_package(OpenCV REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB person_tracker_SRC
     "*.h"
     "*.cpp"
     "utils/*.cpp"
)

add_executable(person_tracker ${person_tracker_SRC}) #Add app's main starting file.

target_link_libraries(person_tracker ${Boost_LIBRARIES} ${OpenCV_LIBS} fmt::fmt stdc++fs)