project(haarcommon)
cmake_minimum_required(VERSION 2.8)

# Debug and release configurations
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -D_DEBUG -Wextra -Wall -pedantic")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package( OpenCV REQUIRED COMPONENTS core imgproc highgui)

# Depends on OpenImageIO
# include_directories(/home/ramiro/workspace/oiio/dist/linux64.debug/include)
# link_directories(/home/ramiro/workspace/oiio/dist/linux64.debug/lib)

set(source_files haarwavelet.h haarwavelet.cpp haarwaveletutilities.h haarwaveletutilities.cpp detector.h detector.cpp)
add_library( haarcommon STATIC ${source_files} )
target_link_libraries( haarcommon ${OpenCV_LIBS} )
