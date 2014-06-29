#ifndef HAARWAVELETEVALUATORS_H
#define HAARWAVELETEVALUATORS_H

#include "haarwavelet.h"
#include <cmath>
#include <numeric>
#include <limits>



//TODO It seems possible to better factor those methods with a combination of templates and virtual methods.
//Consider traits or policies. http://stackoverflow.com/questions/14718055/what-is-the-difference-between-a-trait-and-a-policy



struct WaveletEvaluator
{
    /**
     * @brief singleRectangleValue Calculates the sum of pixels in a rectangular
     * region of the image which integral image is s.
     * @param r the rectangulare region on the original image.
     * @param s the integral image of the original image. It may be the simple sum or the squared sum.
     * @return sum of pixels found inside the rectangular region r of the original image.
     */
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
                             const float scale = 1.0) const
    {
        std::vector<float> s(w.dimensions());
        srfs(w, sum, s, scale);

        return std::inner_product(w.weights_begin(), w.weights_end(),
                                  s.begin(), 0.0f);
    }

    virtual float operator()(const MyHaarWavelet & w,
                             const cv::Mat & sum,
                             const cv::Mat &, //Not used here
                             const float scale = 1.0) const
    {
        std::vector<float> s(w.dimensions());
        srfs(w, sum, s, scale);

        std::transform(s.begin(), s.end(),
                       w.means_begin(),
                       s.begin(),
                       std::minus<float>());

        return std::abs(std::inner_product(w.weights_begin(), w.weights_end(),
                                           s.begin(), 0.0f));
    }

    virtual std::pair<float,float> operator()(const DualWeightHaarWavelet & w,
                                              const cv::Mat & sum,
                                              const cv::Mat &, //Not used here
                                              const float scale = 1.0) const
    {
        std::vector<float> s(w.dimensions());
        srfs(w, sum, s, scale);

        std::pair<float, float> featureValues;
        featureValues.first  = std::inner_product(w.weightsPositive_begin(),
                                                  w.weightsPositive_end(),
                                                  s.begin(), 0.0f);
        featureValues.second = std::inner_product(w.weightsNegative_begin(),
                                                  w.weightsNegative_end(),
                                                  s.begin(), 0.0f);
        return featureValues;
    }

    /**
     * Sets the values of the single rectangle feature space.
     * If scale > 1, the Haar wavelet streaches right and down.
     * @param w
     * @param sum integral image of the original image.
     * @param srfsVector where the resulting values will be writen to. srfsVector.size() must be equal to w.dimensions().
     * @param scale how mutch w should be scaled.
     */
    template <typename floating_point_type>
    void srfs(const AbstractHaarWavelet & w, const cv::Mat & sum, std::vector<floating_point_type> &srfsVector, const float scale = 1.0) const
    {
        int i = 0;
        for (std::vector<cv::Rect>::const_iterator it = w.rects_begin(); it != w.rects_end(); ++it)
        {
            cv::Rect r = *it;
            r.x *= scale;
            r.y *= scale;
            r.height *= scale;
            r.width  *= scale;
            srfsVector[i] = singleRectangleValue(r, sum);

            //SRFS works with normalized means (Pavani et al., 2010, section 2.3).
            //AFAIK, Pavani's classifier only normalized things by the maximum numeric value of each pixel.
            srfsVector[i] /= it->area() * std::numeric_limits<unsigned char>::max(); //TODO it is probably best to use a fixed number
            ++i;
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
                             const float scale = 1.0) const
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
                             const float scale = 1.0) const
    {
        std::vector<float> s(w.dimensions());
        srfs(w, sum, squareSum, s, scale);

        std::transform(s.begin(), s.end(),
                       w.means_begin(),
                       s.begin(),
                       std::minus<float>());

        return std::abs(std::inner_product(w.weights_begin(), w.weights_end(),
                                           s.begin(), 0.0));
    }

    template <typename floating_point_type>
    void srfs(const AbstractHaarWavelet & w, const cv::Mat & sum, const cv::Mat & squareSum, std::vector<floating_point_type> &srfsVector, const float scale = 1.0) const
    {
        //Viola and Jones perform a variance normalization. This is better explained in Lienhart, Maydt, 2002, section 2.2.
        const double area = (sum.cols - 1) * (sum.rows - 1); //area of the original image
        const double mean = sum.at<double>(sum.rows - 1, sum.cols - 1) / area; //mean value of all pixels inside the image that originated the integral image
        const double stdDev = std::sqrt( std::abs(
                            (squareSum.at<double>(sum.rows - 1, sum.cols - 1) / area ) - (mean * mean)
                        )); //Viola and Jones' paper show a wrong equation?
                            //Correct is: STD_DEV = SQRT(E[X^2] - E[X]^2)

        if (stdDev)
        {
            int i = 0;
            for(std::vector<cv::Rect>::const_iterator it = w.rects_begin(); it != w.rects_end(); ++it, ++i)
            {
                cv::Rect r = *it;
                r.x *= scale;
                r.y *= scale;
                r.height *= scale;
                r.width  *= scale;

                //Can't divide by zero. If the subwindow standard deviation is 0, then all rectangles have the same value.
                //If this happens, then (singleRectangleValue(r, sum) - mean * r.area()) == 0. See the ELSE clause below.
                srfsVector[i] = (singleRectangleValue(r, sum) - (mean * r.area())) / (2.0 * stdDev);
            }
        }
        else
        {
            std::fill(srfsVector.begin(), srfsVector.end(), .0);
        }
    }
};



#endif // HAARWAVELETEVALUATORS_H
