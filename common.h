#ifndef COMMON_H
#define COMMON_H

#include "opencv2/opencv.hpp"
#include <QPixmap>
#include <QGraphicsView>

using namespace cv;

namespace hp {
    inline QPixmap frame2Img(Mat *frame)
    {
        QImage qimg(frame->data, frame->cols, frame->rows, static_cast<int>(frame->step),
                QImage::Format_RGB888);
        return QPixmap::fromImage(qimg.rgbSwapped());
    }

    inline QPixmap scaleToView(QGraphicsView *view, QPixmap pmap)
    {
        QSize sz = view->size();
        return pmap.scaled(sz.width() - 1, sz.height() - 4, Qt::KeepAspectRatio);
    }
}


enum HPGameMode {
    Free, // no pattern advance
    Simple, // easy scoring
    Challenge // standard scoring
};

#endif // COMMON_H
