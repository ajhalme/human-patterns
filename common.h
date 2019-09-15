#ifndef COMMON_H
#define COMMON_H

#include "opencv2/opencv.hpp"
#include <QPixmap>

using namespace cv;

namespace hp {
    QPixmap static frame2Img(Mat *frame)
    {
        QImage qimg(frame->data, frame->cols, frame->rows, static_cast<int>(frame->step),
                    QImage::Format_RGB888);
        return QPixmap::fromImage(qimg.rgbSwapped());
    }
}


#endif // COMMON_H
