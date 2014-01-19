#include "haarwavelet.h"
#include <cassert>



//======================================== AbstractHaarWavelet ========================================



unsigned int AbstractHaarWavelet::dimensions() const
{
    return rects.size();
}

std::vector<cv::Rect>::const_iterator AbstractHaarWavelet::rects_begin() const
{
    return rects.begin();
}

const std::vector<cv::Rect>::const_iterator AbstractHaarWavelet::rects_end() const
{
    return rects.end();
}



//======================================== HaarWavelet ========================================



HaarWavelet::HaarWavelet() {}

HaarWavelet::HaarWavelet(std::vector<cv::Rect> rects_,
                         std::vector<float> weights_)
{
    assert(rects.size() == weights.size()); //TODO convert into exception
    rects = rects_;
    weights = weights_;
}

bool HaarWavelet::read(std::istream &input)
{
    int rectangles;
    input >> rectangles;

    for (int i = 0; i < rectangles; i++)
    {
        float weight_;
        cv::Rect rect_;

        input >> rect_.x
              >> rect_.y
              >> rect_.width
              >> rect_.height
              >> weight_;

        rects.push_back(rect_);
        weights.push_back(weight_);
    }

    return true;
}

/**
 * See also constructor that takes a std::istream.
 */
bool HaarWavelet::write(std::ostream &output) const
{
    if (dimensions() == 0) //won't store a meaningless wavelet
    {
        return false;
    }

    output << dimensions() << ' ';

    bool first = true;
    for (unsigned int i = 0; i < dimensions(); i++)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            output << ' ';
        }
        output << rects[i].x << ' '
               << rects[i].y << ' '
               << rects[i].width << ' '
               << rects[i].height << ' '
               << weights[i];
    }

    return true;
}

const cv::Rect HaarWavelet::rect(const int index) const
{
    return rects[index];
}

std::vector<float>::const_iterator HaarWavelet::weights_begin() const
{
    return weights.begin();
}

const std::vector<float>::const_iterator HaarWavelet::weights_end() const
{
    return weights.end();
}

float HaarWavelet::weight(const int index) const
{
    return weights[index];
}

void HaarWavelet::weight(const int index, const float new_value)
{
    weights[index] = new_value;
}



//======================================== MyHaarWavelet ========================================



MyHaarWavelet::MyHaarWavelet() : HaarWavelet() {}

MyHaarWavelet::MyHaarWavelet(std::vector<cv::Rect> rects_, std::vector<float> weights_) : HaarWavelet(rects_, weights_),
                                                                                          means(rects_.size()) {}

MyHaarWavelet::MyHaarWavelet(std::vector<cv::Rect> rects_,
                             std::vector<float> weights_,
                             std::vector<float> means_) : HaarWavelet(rects_, weights_),
                                                          means(means_) {}

bool MyHaarWavelet::read(std::istream &input)
{
    if ( !HaarWavelet::read(input) )
    {
        return false;
    }

    means.resize(dimensions());
    for (unsigned int i = 0; i < dimensions(); i++)
    {
        input >> means[i];
    }

    return true;
}

bool MyHaarWavelet::write(std::ostream &output) const
{
    if ( !HaarWavelet::write(output) )
    {
        return false;
    }

    for (unsigned int i = 0; i < dimensions(); i++)
    {
        output << ' ' << means[i];
    }

    return true;
}

std::vector<float>::const_iterator MyHaarWavelet::means_begin() const
{
    return means.begin();
}

const std::vector<float>::const_iterator MyHaarWavelet::means_end() const
{
    return means.end();
}



//======================================== DualWeightHaarWavelet ========================================



DualWeightHaarWavelet::DualWeightHaarWavelet() {}

DualWeightHaarWavelet::DualWeightHaarWavelet(const DualWeightHaarWavelet &w)
{
    rects = w.rects;
    weightsPositive = w.weightsPositive;
    weightsNegative = w.weightsNegative;
}

std::vector<float>::const_iterator DualWeightHaarWavelet::weightsPositive_begin() const
{
    return weightsPositive.begin();
}

const std::vector<float>::const_iterator DualWeightHaarWavelet::weightsPositive_end() const
{
    return weightsPositive.end();
}

std::vector<float>::const_iterator DualWeightHaarWavelet::weightsNegative_begin() const
{
    return weightsNegative.begin();
}

const std::vector<float>::const_iterator DualWeightHaarWavelet::weightsNegative_end() const
{
    return weightsNegative.end();
}

bool DualWeightHaarWavelet::read(std::istream &input)
{
    int rectangles;
    input >> rectangles;

    for (int i = 0; i < rectangles; i++)
    {
        float weight_p, weight_n;
        cv::Rect rect_;

        input >> rect_.x
              >> rect_.y
              >> rect_.width
              >> rect_.height
              >> weight_p
              >> weight_n;

        rects.push_back(rect_);
        weightsPositive.push_back(weight_p);
        weightsNegative.push_back(weight_n);
    }

    return true;
}

bool DualWeightHaarWavelet::write(std::ostream &output) const
{
    if (dimensions() == 0) //won't store a meaningless wavelet
    {
        return false;
    }

    output << dimensions() << ' ';

    bool first = true;
    for (unsigned int i = 0; i < dimensions(); i++)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            output << ' ';
        }
        output << rects[i].x << ' '
               << rects[i].y << ' '
               << rects[i].width << ' '
               << rects[i].height << ' '
               << weightsPositive[i] << ' '
               << weightsNegative[i];
    }

    return true;
}
