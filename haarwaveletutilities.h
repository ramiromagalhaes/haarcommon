#ifndef HAARWAVELETUTILITIES_H
#define HAARWAVELETUTILITIES_H

#include <string>
#include "haarwavelet.h"

bool loadHaarWavelets(cv::Size * const sampleSize, const std::string &filename, std::vector<HaarWavelet *> & wavelets);

bool writeHaarWavelets(const char * filename, const std::vector<HaarWavelet * > &wavelets);

#endif // HAARWAVELETUTILITIES_H
