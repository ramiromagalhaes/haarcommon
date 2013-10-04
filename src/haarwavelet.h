#ifndef HAARWAVELET_H
#define HAARWAVELET_H

#include <vector>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>



/**
 * @brief The HaarWavelet class represents a single Haar wavelet.
 */
class HaarWavelet
{
public:

    /**
     * Constructs an "empty" instance of this object.
     */
    HaarWavelet();

    /**
     * "Raw" constructor for a Haar wavelet.
     */
    HaarWavelet(std::vector<cv::Rect> rects_, std::vector<float> weights_);

    /**
     * Amount of rectangles this Haar wavelet has
     */
    unsigned int dimensions() const;

    /**
     * Returns a constant iterator to the start of a collection of rectangles that compose this wavelet.
     */
    std::vector<cv::Rect>::const_iterator rects_begin() const;

    /**
     * Returns a constant iterator to the end of the collection of rectangles that compose this wavelet.
     */
    const std::vector<cv::Rect>::const_iterator rects_end() const;

    /**
     * Returns the cv::Rect at position 'index'.
     */
    const cv::Rect rect(const int index) const;

    /**
     * Returns a constant iterator...
     */
    std::vector<float>::const_iterator weights_begin() const;

    /**
     * Returns a constant iterator...
     */
    const std::vector<float>::const_iterator weights_end() const;

    /**
     * Returns the weight applied to the index'th rectangle.
     */
    float weight(const int index) const;

    /**
     * Changes the value of the weight applied to the index'th rectangle.
     */
    void weight(const int index, const float new_value);

    /**
     * Reads som data and sets this HaarWavelet with it.
     */
    virtual bool read(std::istream &input);

    /**
     * Writes this Haar wavelet into the given std::ostream.
     */
    virtual bool write(std::ostream &output) const;

protected:

    /**
     * Each rectangle and its associated weight of this Haar wavelet
     */
    std::vector<cv::Rect> rects;
    std::vector<float> weights;
};



class MyHaarWavelet : public HaarWavelet
{
public:

    /**
     * Constructs an "empty" instance of this object.
     */
    MyHaarWavelet();

    /**
     * "Raw" constructor for a Haar wavelet.
     */
    MyHaarWavelet(std::vector<cv::Rect> rects_, std::vector<float> weights_);

    /**
     * "Raw" constructor for a Haar wavelet.
     */
    MyHaarWavelet(std::vector<cv::Rect> rects_, std::vector<float> weights_, std::vector<float> means_);

    /**
     * Reads som data and sets this HaarWavelet with it.
     */
    bool read(std::istream &input);

    /**
     * Writes this Haar wavelet into the given std::ostream.
     */
    bool write(std::ostream &output) const;

    /**
     * Returns a constant iterator...
     */
    std::vector<float>::const_iterator means_begin() const;

    /**
     * Returns a constant iterator...
     */
    const std::vector<float>::const_iterator means_end() const;

protected:
    std::vector<float> means;
};

#endif // HAARWAVELET_H
