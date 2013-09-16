#include "detector.h"



Detector::Detector() : baseSize(PAVANI_BASE_SIZE),
    scalingFactor(DEFAULT_SCALING_FACTOR) {}



void Detector::scan(const cv::Mat image, std::vector<cv::Rect> detections)
{
    float currentScale = 1;      //the current scaling factor in use.
    cv::Mat integralImage;       //the integral image calculated over the whole image.
    cv::Rect currentRoi;         //stores both the current size and position of the scanning window.
    cv::Mat currentIntegralImage;//references the integral image data that's being scanned.

    //TODO a loop for the scaling factor
    for(int w = 0; w < image.cols - currentRoi.width; ++w)
    {
        for(int h = 0; h < image.rows - currentRoi.height; ++h)
        {
            cv::integral(image, integralImage);
            currentIntegralImage = image(currentRoi);

            //deliver to the cascade of classifiers (or the strongclassifier) the integral image
            int classification;
            if (classification)
            {
                detections.push_back(currentRoi);
            }
        }
    }

    //TODO integrate all detected ROIs as explained in Viola and Jones' 2004 paper (section 5.6).
    for(int i = 0; i < detections.size(); ++i)
    {
        for(int j = i; j < detections.size(); )
        {
            if (detections[i] & detections[j])
            {
                detections[i].x = (detections[i].x + detections[j].x) / 2;
                detections[i].y = (detections[i].y + detections[j].y) / 2;
                detections.erase(detections.begin() + j);
            }
            else
            {
                ++j;
            }
        }
    }
}



/*
bool Detector::setSubwindow(const int left, const int top)
{
    currentRoi.width
    currentRoi.height

    if( left < 0 || top < 0 ||
        left + baseSize.width >= image.cols ||
        top + baseSize.height >= image.rows )
    {
        return false;
    }

    currentRoi.x = left;
    currentRoi.y = top;

    const cv::Rect integralsRoi(currentRoi.x, currentRoi.y, currentRoi.width + 1, currentRoi.height + 1);
    currentIntegralImage = integralImage(integralsRoi);

    return true;
}

bool Detector::setSubwindow(const cv::Point topLeftPosition)
{
    setSubwindow(topLeftPosition.x, topLeftPosition.y);
    return true;
}


void Detector::scaleUp()
{
    //TODO verify if this operation is possible
    currentScale *= scalingFactor;
}

void Detector::scaleDown()
{
    //TODO verify if this operation is possible
    currentScale /= scalingFactor;
}
*/
