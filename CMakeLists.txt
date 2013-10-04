project(haarcommon)
cmake_minimum_required(VERSION 2.8)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME HaarWaveletTest COMMAND haarwavelettest)
