#ifndef IMAGEMANAGERWT
#define IMAGEMANAGERWT

#include "Image/imagemanager.h"

#include <Wt/WMemoryResource>
#include <Wt/WLength>

#include <QImage>
#include <QPixmap>

#include "configurationparameters.h"

using namespace Wt;

class ImageManagerWt : public WObject, public ImageManager
{
    public : WMemoryResource *myWMemoryResource = nullptr;
    public : vector<unsigned char> imageDecodedData;
    public : void updateWMemoryResource()
    {
        imageDecodedData.clear();
        vector<int> param = vector<int>(2);
        param[0] = CV_IMWRITE_JPEG_QUALITY;
        param[1] = ConfigurationParameters::instance()->jpegQuality;
        imencode(".jpg", getBlendedLayers(), imageDecodedData, param);
        myWMemoryResource->setData(imageDecodedData);
    }
    public : WLength getWidth() const {return WLength(_myImage.cols);}
    public : WLength getHeight() const {return WLength(_myImage.rows);}
    public : WLength getZoomedWidth() const {return WLength(_myDrawingLayer.cols);}
    public : WLength getZoomedHeight() const {return WLength(_myDrawingLayer.rows);}
    public : ImageManagerWt(WObject *parent = 0): WObject(parent), ImageManager()
    {
        myWMemoryResource = new WMemoryResource("image/jpg",this);
    }
    public : ~ImageManagerWt(){}
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
};


#endif // IMAGEMANAGERWT

