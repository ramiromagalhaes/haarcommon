find_package( Boost  REQUIRED COMPONENTS unit_test_framework )

include_directories( ${CMAKE_SOURCE_DIR}/src )

add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable( haarwavelettest haarwavelettest.cpp )
target_link_libraries( haarwavelettest haarcommon ${OpenCV_LIBS} ${Boost_LIBRARIES} )
