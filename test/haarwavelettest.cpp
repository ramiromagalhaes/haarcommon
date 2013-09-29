#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../haarwavelet.h"
#include "../haarwaveletevaluators.h"

const cv::Mat getMockImage()
{
    cv::Mat image(5, 5, cv::DataType<unsigned char>::type);
    image.at<unsigned char>(0, 0) = 5;
    image.at<unsigned char>(0, 1) = 5;
    image.at<unsigned char>(0, 2) = 5;
    image.at<unsigned char>(0, 3) = 5;
    image.at<unsigned char>(0, 4) = 5;
    image.at<unsigned char>(1, 0) = 4;
    image.at<unsigned char>(1, 1) = 4;
    image.at<unsigned char>(1, 2) = 4;
    image.at<unsigned char>(1, 3) = 4;
    image.at<unsigned char>(1, 4) = 4;
    image.at<unsigned char>(2, 0) = 3;
    image.at<unsigned char>(2, 1) = 3;
    image.at<unsigned char>(2, 2) = 3;
    image.at<unsigned char>(2, 3) = 3;
    image.at<unsigned char>(2, 4) = 3;
    image.at<unsigned char>(3, 0) = 4;
    image.at<unsigned char>(3, 1) = 4;
    image.at<unsigned char>(3, 2) = 4;
    image.at<unsigned char>(3, 3) = 4;
    image.at<unsigned char>(3, 4) = 4;
    image.at<unsigned char>(4, 0) = 5;
    image.at<unsigned char>(4, 1) = 5;
    image.at<unsigned char>(4, 2) = 5;
    image.at<unsigned char>(4, 3) = 5;
    image.at<unsigned char>(4, 4) = 5;

    return image;
}



const HaarWavelet getHaarWavelet()
{
    std::vector<cv::Rect> rects(2);
    rects[0].x = 1;
    rects[0].y = 1;
    rects[0].width = 2;
    rects[0].height = 2;

    rects[1].x = 3;
    rects[1].y = 3;
    rects[1].width = 2;
    rects[1].height = 2;

    std::vector<float> weight(2);
    weight[0] = 1;
    weight[1] = -1;

    return HaarWavelet(rects, weight);
}



const MyHaarWavelet getMyWavelet()
{
    std::vector<cv::Rect> rects(2);
    rects[0].x = 1;
    rects[0].y = 1;
    rects[0].width = 2;
    rects[0].height = 2;

    rects[1].x = 3;
    rects[1].y = 3;
    rects[1].width = 2;
    rects[1].height = 2;

    std::vector<float> weight(2);
    weight[0] = 1;
    weight[1] = -1;

    std::vector<float> means(2);
    means[0] = .013725490196f;
    means[1] = .017647058824f;

    return MyHaarWavelet(rects, weight, means);
}



int main()
{
    const cv::Mat image = getMockImage();
    cv::Mat integralSum(6, 6, cv::DataType<double>::type),
            integralSquare(6, 6, cv::DataType<double>::type);
    cv::integral(image, integralSum, integralSquare, cv::DataType<double>::type);

    {
        IntensityNormalizedWaveletEvaluator evaluator;
        {
            const HaarWavelet wavelet = getHaarWavelet();
            if (evaluator(wavelet, integralSum) != -0.00392156933f)
            {
                return 1;
            }
        }

        {
            const MyHaarWavelet wavelet = getMyWavelet();
            if (evaluator(wavelet, integralSum) != 0)
            {
                return 2;
            }
        }
    }

    {
        VarianceNormalizedWaveletEvaluator evaluator;
        {
            const HaarWavelet wavelet = getHaarWavelet();
            if (evaluator(wavelet, integralSum, integralSquare) != -0.66815114f)
            {
                return 3;
            }
        }

        {
            const MyHaarWavelet wavelet = getMyWavelet();
            if (evaluator(wavelet, integralSum, integralSquare) != -0.664229572)
            {
                return 4;
            }
        }
    }
    return 0;
}
