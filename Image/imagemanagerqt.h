#ifndef IMAGEMANAGERQT
#define IMAGEMANAGERQT

#include "Image/imagemanager.h"

#include <QObject>
#include <QImage>
#include <QPixmap>

class ImageManagerQt : public QObject, public ImageManager
{
    Q_OBJECT

    public : static QImage Mat2QImage(const Mat &src)
    {
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);

        const float scale = 255.0;

        if (src.depth() == CV_8U) {
            if (src.channels() == 1) {
                for (int i = 0; i < src.rows; ++i) {
                    for (int j = 0; j < src.cols; ++j) {
                        int level = src.at<quint8>(i, j);
                        dest.setPixel(j, i, qRgb(level, level, level));
                    }
                }
            } else if (src.channels() == 3) {
                for (int i = 0; i < src.rows; ++i) {
                    for (int j = 0; j < src.cols; ++j) {
                        Vec3b bgr = src.at<Vec3b>(i, j);
                        dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
                    }
                }
            }
        } else if (src.depth() == CV_32F) {
            if (src.channels() == 1) {
                for (int i = 0; i < src.rows; ++i) {
                    for (int j = 0; j < src.cols; ++j) {
                        int level = scale * src.at<float>(i, j);
                        dest.setPixel(j, i, qRgb(level, level, level));
                    }
                }
            } else if (src.channels() == 3) {
                for (int i = 0; i < src.rows; ++i) {
                    for (int j = 0; j < src.cols; ++j) {
                        Vec3f bgr = scale * src.at<Vec3f>(i, j);
                        dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
                    }
                }
            }
        }

        return dest;
    }

    public : const QPixmap getImageAsQPixmap() const
    {
        return QPixmap::fromImage(Mat2QImage(getBlendedLayers()));
    }

    public : const QImage getImageAsQImage() const
    {
        return Mat2QImage(getBlendedLayers());
    }

    public : ImageManagerQt(QObject *parent):
        QObject(parent),
        ImageManager(){}
    public : ~ImageManagerQt(){}
};


#endif // IMAGEMANAGERQT

