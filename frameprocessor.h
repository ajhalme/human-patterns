#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include "opencv2/opencv.hpp"
#include <opencv2/aruco.hpp>

class HPFrameProcessor
{
public:
    HPFrameProcessor();
    ~HPFrameProcessor();

    cv::Mat ProcessFrame(cv::Mat frame);
protected:
    cv::Ptr<cv::aruco::Dictionary> dict;
};

#endif // FRAMEPROCESSOR_H
