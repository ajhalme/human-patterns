#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include "opencv2/opencv.hpp"
#include <opencv2/aruco.hpp>

#include "config.h"

class HPFrameProcessor
{
public:
    HPFrameProcessor(HPConfig *config);
    ~HPFrameProcessor();

    void ProcessRaw(cv::Mat frame, cv::Mat *out);
    void ProcessPlayArea(cv::Mat frame, cv::Mat *out);

    void CacheState(cv::Mat frame,
                    vector<int> ids,
                    vector<vector<Point2f>> corners);

    void applySquareTransform(Mat input, Mat *output, vector<vector<Point2f>> points);

protected:
    HPConfig *config;
    cv::Ptr<cv::aruco::Dictionary> dict;
    vector<int> markerIds;
    vector<vector<Point2f>> playAreaCorners;
    cv::Mat baseline;

private:
    void cacheState();

};

#endif // FRAMEPROCESSOR_H
