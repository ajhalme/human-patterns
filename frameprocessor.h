#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include "opencv2/opencv.hpp"

class HPFrameProcessor
{
public:
    HPFrameProcessor();
    ~HPFrameProcessor();

    cv::Mat ProcessFrame(cv::Mat frame);
};

#endif // FRAMEPROCESSOR_H
