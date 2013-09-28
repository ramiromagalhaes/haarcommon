#ifndef HAARWAVELETUTILITIES_H
#define HAARWAVELETUTILITIES_H

#include <string>
#include <iostream>
#include <fstream>

#include "haarwavelet.h"



template <typename HaarWaveletType>
bool loadHaarWavelets(const std::string &filename, std::vector<HaarWaveletType> & wavelets)
{
    std::ifstream ifs;
    ifs.open(filename.c_str(), std::ifstream::in);

    if ( !ifs.is_open() )
    {
        return false;
    }

    do
    {
        HaarWaveletType wavelet;
        wavelet.read(ifs);
        if ( !ifs.eof() )
        {
            wavelets.push_back(wavelet);
        }
        else
        {
            break;
        }
    } while (true);

    ifs.close();

    return true;
}



template <typename HaarWaveletType>
bool writeHaarWavelets(const std::string &filename, const std::vector<HaarWaveletType> &wavelets)
{
    std::ofstream ofs;
    ofs.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);

    if (!ofs.is_open())
    {
        return false;
    }

    typename std::vector<HaarWaveletType>::const_iterator it = wavelets.begin();
    const typename std::vector<HaarWaveletType>::const_iterator end = wavelets.end();
    for(; it != end; ++it)
    {
        it->write(ofs);
        ofs << '\n';
    }
    ofs.close();

    return true;
}



#endif // HAARWAVELETUTILITIES_H
