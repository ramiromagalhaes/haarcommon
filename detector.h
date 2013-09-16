#ifndef DETECTOR_H
#define DETECTOR_H

#include "haarwavelet.h"
#include <opencv2/imgproc/imgproc.hpp>

#define PAVANI_BASE_SIZE cv::Size(20, 20)
#define DEFAULT_SCALING_FACTOR 1.25f

/**
 * This is an implementation of an image detector for the Pavani face classifier as described in the paper
 * "Haar-like features with optimally weighted rectangles for rapid object detection".
 *
 * While Viola and Jones mention a variance normalization to make the detector resilient against
 * lightning conditions, the only normalization mentioned in Pavani et al. is the "intensity
 * normalization" (section 2.3). This will be the only normalization implemented here.
 */
class Detector
{
public:
    Detector();
    void scan(const cv::Mat image, std::vector<cv::Rect> detections);

protected:
    /**
     * @brief baseSize the size of the detector when it is scaled by 1. It is the size of the images used during trainning.
     */
    const cv::Size baseSize;

    /**
     * @brief scalingFactor the factor with which the base detector will be scaled when it finishes iterating over the image.
     */
    const float scalingFactor;
};

#endif // DETECTOR_H
