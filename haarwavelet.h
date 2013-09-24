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
     * Returns the value of this Haar wavelet when applied to an image in a certain position.
     * If scale > 1, the Haar wavelet streaches right and down.
     */
    float value(const cv::Mat & sum, const cv::Mat & squareSum/*, const cv::Mat & tilted*/, const float scale = 1.0f) const;

    /**
     * Sets the values of the single rectangle feature space.
     * If scale > 1, the Haar wavelet streaches right and down.
     */
    template <typename floating_point_type>
    void srfs(const cv::Mat & sum, std::vector<floating_point_type> &srfsVector, const float scale = 1.0f) const
    {
        assert(sum.data); //TODO convert into exception?

        const int dim = dimensions();
        for (int i = 0; i < dim; ++i)
        {
            cv::Rect r = rects[i];
            r.x *= scale;
            r.y *= scale;
            r.height *= scale;
            r.width  *= scale;
            srfsVector[i] = singleRectangleValue(r, sum);

            //SRFS works with normalized means (Pavani et al., 2010, section 2.3).
            //AFAIK, Pavani's classifier only normalized things by the maximum numeric value of each pixel.
            //Viola and Jones perform a variance normalization.
            srfsVector[i] /= (((floating_point_type)rects[i].area()) * std::numeric_limits<unsigned char>::max());
        }
    }

    /**
     * Reads som data and sets this HaarWavelet with it.
     */
    bool read(std::istream &input);

    /**
     * Writes this Haar wavelet into the given std::ostream.
     */
    bool write(std::ostream &output) const;

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

protected:

    /**
     * Calculates the sum of pixels inside a rectangular area of the image.
     */
    float singleRectangleValue(const cv::Rect &r, const cv::Mat &s) const;

    /**
     * Each rectangle and its associated weight of this Haar wavelet
     */
    std::vector<cv::Rect> rects;
    std::vector<float> weights;
};



class ViolaJonesHaarWavelet : public HaarWavelet
{
public:

    /**
     * Returns the value of this Haar wavelet when applied to an image in a certain position.
     * If scale > 1, the Haar wavelet streaches right and down.
     *
     * The only normalization mentioned in Pavani et al. is the "intensity normalization" (section 2.3).
     * Viola and Jones perform a variance normalization that might be more resilient to lightning conditions.
     * This function implements what Viola and Jones did.
     */
    float value(const cv::Mat & sum, const cv::Mat & squareSum/*, const cv::Mat & tilted*/, const float scale = 1.0f) const;

};

#endif // HAARWAVELET_H
