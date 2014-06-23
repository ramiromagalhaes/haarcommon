#define BOOST_TEST_MODULE HaarWaveletTest
#include <boost/test/unit_test.hpp>

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "haarwavelet.h"
#include "haarwaveletevaluators.h"



const cv::Mat getMockImage()
{
    //Values taken from file "integrals.ods" (in the test folder)
    cv::Mat image(5, 5, cv::DataType<unsigned char>::type);
    image.at<unsigned char>(0, 0) = 5;
    image.at<unsigned char>(0, 1) = 100;
    image.at<unsigned char>(0, 2) = 12;
    image.at<unsigned char>(0, 3) = 30;
    image.at<unsigned char>(0, 4) = 200;
    image.at<unsigned char>(1, 0) = 0;
    image.at<unsigned char>(1, 1) = 13;
    image.at<unsigned char>(1, 2) = 12;
    image.at<unsigned char>(1, 3) = 150;
    image.at<unsigned char>(1, 4) = 20;
    image.at<unsigned char>(2, 0) = 180;
    image.at<unsigned char>(2, 1) = 5;
    image.at<unsigned char>(2, 2) = 90;
    image.at<unsigned char>(2, 3) = 150;
    image.at<unsigned char>(2, 4) = 12;
    image.at<unsigned char>(3, 0) = 190;
    image.at<unsigned char>(3, 1) = 4;
    image.at<unsigned char>(3, 2) = 160;
    image.at<unsigned char>(3, 3) = 30;
    image.at<unsigned char>(3, 4) = 5;
    image.at<unsigned char>(4, 0) = 170;
    image.at<unsigned char>(4, 1) = 5;
    image.at<unsigned char>(4, 2) = 100;
    image.at<unsigned char>(4, 3) = 5;
    image.at<unsigned char>(4, 4) = 90;

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
    means[0] = .117647058824f;
    means[1] = .127450980392f;

    return MyHaarWavelet(rects, weight, means);
}



BOOST_AUTO_TEST_CASE(IntensityNormalizedWaveletEvaluatorTest)
{
    const cv::Mat image = getMockImage();
    cv::Mat integralSum(6, 6, cv::DataType<double>::type),
            integralSquare(6, 6, cv::DataType<double>::type);
    cv::integral(image, integralSum, integralSquare, cv::DataType<double>::type);

    IntensityNormalizedWaveletEvaluator evaluator;
    {
        const HaarWavelet wavelet = getHaarWavelet();
        BOOST_CHECK_CLOSE(evaluator(wavelet, integralSum, integralSquare), -.009803921569f, 0.0001f);
    }

    {
        const MyHaarWavelet wavelet = getMyWavelet();
        BOOST_CHECK_CLOSE(evaluator(wavelet, integralSum, integralSquare), 0, 0.00001f);
    }
}



BOOST_AUTO_TEST_CASE(VarianceNormalizedWaveletEvaluatorTest)
{
    const cv::Mat image = getMockImage();
    cv::Mat integralSum(6, 6, cv::DataType<double>::type),
            integralSquare(6, 6, cv::DataType<double>::type);
    cv::integral(image, integralSum, integralSquare, cv::DataType<double>::type);

    VarianceNormalizedWaveletEvaluator evaluator;
    {
        const HaarWavelet wavelet = getHaarWavelet();
        BOOST_CHECK_CLOSE( evaluator(wavelet, integralSum, integralSquare), -0.0689201209f, 0.0001f);
    }

    {
        const MyHaarWavelet wavelet = getMyWavelet();
        BOOST_CHECK_CLOSE( evaluator(wavelet, integralSum, integralSquare), 0.664229572f, 0.0001f);
    }
}
