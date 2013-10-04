# Debug and release configurations
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -D_DEBUG -Wextra -Wall -pedantic")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package( OpenCV REQUIRED COMPONENTS core imgproc )

set(source_files haarwavelet.h
                 haarwavelet.cpp
                 haarwaveletevaluators.h
                 haarwaveletutilities.h)
add_library( haarcommon SHARED ${source_files} )
target_link_libraries( haarcommon ${OpenCV_LIBS} )
