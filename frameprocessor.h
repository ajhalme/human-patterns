#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include "opencv2/opencv.hpp"
#include <opencv2/aruco.hpp>

#include "config.h"

class HPFrameProcessor
{
public:
    HPFrameProcessor();
    ~HPFrameProcessor();

    cv::Mat ProcessRaw(cv::Mat frame, HPConfig *config);
    cv::Mat ProcessPlayArea(cv::Mat frame, HPConfig *config);

    void CacheState(cv::Mat frame,
                    vector<int> ids,
                    vector<vector<Point2f>> corners,
                    HPConfig *config);
protected:
    cv::Ptr<cv::aruco::Dictionary> dict;

    vector<int> markerIds;
    vector<vector<Point2f>> playAreaCorners;
    cv::Mat baseline;
private:
    void cacheState();
};

#endif // FRAMEPROCESSOR_H
