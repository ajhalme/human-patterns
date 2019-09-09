#include "config.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

HPConfig::HPConfig(int edge)
{
    const Size s = Size(edge, edge);
    this->targetSize = s;
    this->targetShape = {
        Point2f(0,              0),
        Point2f(s.width,        0),
        Point2f(s.width, s.height),
        Point2f(0,       s.height)
    };
}

HPConfig::~HPConfig()
{
}
