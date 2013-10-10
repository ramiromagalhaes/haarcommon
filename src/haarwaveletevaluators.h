#ifndef HAARWAVELETEVALUATORS_H
#define HAARWAVELETEVALUATORS_H

#include "haarwavelet.h"
#include <numeric>
#include <limits>



struct WaveletEvaluator
{
    /**
     * Calculates the sum of pixels inside a rectangular area of the image.
     */
    //TODO Write documentation about how one should extract rois from the image
    float singleRectangleValue(const cv::Rect &r, const cv::Mat & s) const
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
};



struct IntensityNormalizedWaveletEvaluator : public WaveletEvaluator
{
    /**
     * Returns the value of this Haar wavelet when applied to an image in a certain position.
     * If scale > 1, the Haar wavelet streaches right and down.
     */
    virtual float operator()(const HaarWavelet & w,
                             const cv::Mat & sum,
                             const cv::Mat &, //Not used here
                             const float scale = 1.0f) const
    {
        std::vector<float> s(w.dimensions());
        srfs(w, sum, s, scale);

        return std::inner_product(w.weights_begin(), w.weights_end(),
                                  s.begin(), 0.0f);
    }

    virtual float operator()(const MyHaarWavelet & w,
                             const cv::Mat & sum,
                             const cv::Mat &, //Not used here
                             const float scale = 1.0f) const
    {
        std::vector<float> s(w.dimensions());
        srfs(w, sum, s, scale);

        std::transform(s.begin(), s.end(),
                       w.means_begin(),
                       s.begin(),
                       std::minus<float>());

        return std::inner_product(w.weights_begin(), w.weights_end(),
                                  s.begin(), 0.0f); //TODO NEED AN EVALUATOR WITH ABSOLUTE VALUE.
    }

    /**
     * Sets the values of the single rectangle feature space.
     * If scale > 1, the Haar wavelet streaches right and down.
     */
    template <typename floating_point_type>
    void srfs(const HaarWavelet & w, const cv::Mat & sum, std::vector<floating_point_type> &srfsVector, const float scale = 1.0f) const
    {
        const int dim = w.dimensions();
        for (int i = 0; i < dim; ++i)
        {
            cv::Rect r = w.rect(i);
            r.x *= scale;
            r.y *= scale;
            r.height *= scale;
            r.width  *= scale;
            srfsVector[i] = singleRectangleValue(r, sum);

            //SRFS works with normalized means (Pavani et al., 2010, section 2.3).
            //AFAIK, Pavani's classifier only normalized things by the maximum numeric value of each pixel.
            //Viola and Jones perform a variance normalization.
            srfsVector[i] /= w.rect(i).area() * std::numeric_limits<unsigned char>::max();
        }
    }
};



/**
 * Returns the value of this Haar wavelet when applied to an image in a certain position.
 * If scale > 1, the Haar wavelet streaches right and down.
 *
 * The only normalization mentioned in Pavani et al. is the "intensity normalization" (section 2.3).
 * Viola and Jones perform a variance normalization that might be more resilient to lightning conditions.
 * This function implements what Viola and Jones did.
 */
struct VarianceNormalizedWaveletEvaluator : public WaveletEvaluator
{
    virtual float operator()(const HaarWavelet & w,
                             const cv::Mat & sum,
                             const cv::Mat & squareSum/*, const cv::Mat & tilted*/,
                             const float scale = 1.0f) const
    {
        std::vector<float> s(w.dimensions());
        srfs(w, sum, squareSum, s, scale);

        float inner_product = std::inner_product(w.weights_begin(), w.weights_end(),
                                  s.begin(), 0.0f);
        return inner_product;
    }

    virtual float operator()(const MyHaarWavelet & w,
                             const cv::Mat & sum,
                             const cv::Mat & squareSum/*, const cv::Mat & tilted*/,
                             const float scale = 1.0f) const
    {
        std::vector<float> s(w.dimensions());
        srfs(w, sum, squareSum, s, scale);

        std::transform(s.begin(), s.end(),
                       w.means_begin(),
                       s.begin(),
                       std::minus<float>());

        float inner_product = std::inner_product(w.weights_begin(), w.weights_end(),
                                  s.begin(), 0.0f); //TODO NEED AN EVALUATOR WITH ABSOLUTE VALUE.
        return inner_product;
    }

    template <typename floating_point_type>
    void srfs(const HaarWavelet & w, const cv::Mat & sum, const cv::Mat & squareSum, std::vector<floating_point_type> &srfsVector, const float scale = 1.0f) const
    {
        assert(sum.data); //TODO convert into exception?

        const cv::Rect all(0, 0, sum.cols - 1, sum.rows - 1);
        const float area = all.area();
        const float mean = singleRectangleValue( all, sum ) / area;
        const float stdDev = std::sqrt( (singleRectangleValue(all, squareSum) / area) - mean * mean );

        const int dim = w.dimensions();
        for (int i = 0; i < dim; ++i)
        {
            cv::Rect r = w.rect(i);
            r.x *= scale;
            r.y *= scale;
            r.height *= scale;
            r.width  *= scale;

            srfsVector[i] = (singleRectangleValue(r, sum) - mean * r.area()) / (2.0f * stdDev * r.area());
        }
    }
};



#endif // HAARWAVELETEVALUATORS_H