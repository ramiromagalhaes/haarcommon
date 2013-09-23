#include "haarwavelet.h"
#include <cassert>
#include <limits>



HaarWavelet::HaarWavelet() {}

HaarWavelet::HaarWavelet(std::vector<cv::Rect> rects_,
                         std::vector<float> weights_)
{
    assert(rects.size() == weights.size()); //TODO convert into exception
    rects = rects_;
    weights = weights_;
}



unsigned int HaarWavelet::dimensions() const
{
    return (int)rects.size();
}



//TODO Write documentation about how one should extract rois from the image
float HaarWavelet::value(const cv::Mat & sum, const cv::Mat & squareSum, const float scale) const
{
    assert(sum.data && squareSum.data); //TODO convert into exception

    float returnValue = 0;

    const int dim = dimensions();
    for (int i = 0; i < dim; ++i)
    {
        cv::Rect r = rects[i];
        r.x *= scale;
        r.y *= scale;
        r.height *= scale;
        r.width  *= scale;

        const float meanRectValue = singleRectangleValue(r, sum) / r.area();
        returnValue += (weights[i] * meanRectValue);
    }

    return returnValue / std::numeric_limits<unsigned char>::max();
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



std::vector<cv::Rect>::const_iterator HaarWavelet::rects_begin() const
{
    return rects.begin();
}

const std::vector<cv::Rect>::const_iterator HaarWavelet::rects_end() const
{
    return rects.end();
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



float HaarWavelet::singleRectangleValue(const cv::Rect &r, const cv::Mat & s) const
{
    if (s.type() != cv::DataType<double>::type)
    {
        throw 31;
    }

    float rectVal = 0;

    //As per Lienhart, Maydt, 2002, section 2.2
    const int x_w = r.x + r.width;
    const int y_h = r.y + r.height;

    //TODO is there a faster implementation that avoids invoking s.at() functions?
    rectVal = s.at<double>(r.y, r.x)     // (x,     y)
                - s.at<double>(r.y, x_w) // (x + w, y)
                - s.at<double>(y_h, r.x) // (x,     y + h)
                + s.at<double>(y_h, x_w);// (x + w, y + h)

    return rectVal;
}



//======================================== ViolaJonesHaarWavelet ========================================



float ViolaJonesHaarWavelet::value(const cv::Mat &sum, const cv::Mat &squareSum, const float scale) const
{
    assert(sum.data && squareSum.data); //TODO convert into exception

    const cv::Rect all(0, 0, sum.cols, sum.rows);
    const float area = (sum.cols - 1) * (sum.rows - 1); //Area of the image. We use minus 1 because the original
                                                        //image is 1 height and width smaller than the integrals.

    const float mean = singleRectangleValue( all, sum ) / area;
    const float variance = mean * mean - singleRectangleValue(all, squareSum) / area;

    float returnValue = 0;
    const int dim = dimensions();
    for (int i = 0; i < dim; ++i)
    {
        cv::Rect r = rects[i];
        r.x *= scale;
        r.y *= scale;
        r.height *= scale;
        r.width  *= scale;

        returnValue += weights[i] * singleRectangleValue(r, sum);
    }

    return returnValue / variance;
}
