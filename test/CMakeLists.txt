project(haarcommon)
cmake_minimum_required(VERSION 2.8)

# Debug and release configurations
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -D_DEBUG -Wextra -Wall -pedantic")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package( OpenCV REQUIRED COMPONENTS core imgproc highgui)
find_package( Boost  REQUIRED COMPONENTS system filesystem unit_test_framework )

add_executable( test haarwavelettest.cpp )
target_link_libraries( test haarcommon ${OpenCV_LIBS} ${Boost_LIBRARIES} )
