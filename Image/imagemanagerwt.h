#ifndef IMAGEMANAGERWT
#define IMAGEMANAGERWT

#include "Image/imagemanager.h"

#include "Wt/WImage"
#include "Wt/WRasterImage"

using namespace Wt;

class ImageManagerWt : public WObject, public ImageManager
{
    private: WRasterImage *_myWRasterImage = nullptr;
    public : void updateWImage()
    {
        if(
                !_myWRasterImage ||
                _myWRasterImage->width() != _myImage.cols ||
                _myWRasterImage->height() != _myImage.rows)
        {
            if(_myWRasterImage) delete _myWRasterImage;

            _myWRasterImage = new WRasterImage(
                        "jpeg",_myDrawingLayer.cols,_myDrawingLayer.rows,this);
        }
        Mat src = _blendLayers();

        const float scale = 255.0;

        if (src.depth() == CV_8U) {
            if (src.channels() == 1) {
                for (int i = 0; i < src.rows; ++i) {
                    for (int j = 0; j < src.cols; ++j) {
                        int level = src.at<unsigned char>(i, j);
                        _myWRasterImage->setPixel(j, i, WColor(level, level, level));
                    }
                }
            } else if (src.channels() == 3) {
                for (int i = 0; i < src.rows; ++i) {
                    for (int j = 0; j < src.cols; ++j) {
                        Vec3b bgr = src.at<Vec3b>(i, j);
                        _myWRasterImage->setPixel(j, i, WColor(bgr[2], bgr[1], bgr[0]));
                    }
                }
            }
        } else if (src.depth() == CV_32F) {
            if (src.channels() == 1) {
                for (int i = 0; i < src.rows; ++i) {
                    for (int j = 0; j < src.cols; ++j) {
                        int level = scale * src.at<float>(i, j);
                        _myWRasterImage->setPixel(j, i, WColor(level, level, level));
                    }
                }
            } else if (src.channels() == 3) {
                for (int i = 0; i < src.rows; ++i) {
                    for (int j = 0; j < src.cols; ++j) {
                        Vec3f bgr = scale * src.at<Vec3f>(i, j);
                        _myWRasterImage->setPixel(j, i, WColor(bgr[2], bgr[1], bgr[0]));
                    }
                }
            }
        }
    }

    public : void openImage(const Mat &image) override
    {
        ImageManager::openImage(image);
        updateWImage();
    }
    public : void openImage(const string &fileName) override
    {
        ImageManager::openImage(fileName);
        updateWImage();
    }

    public : ImageManagerWt(WObject *parent = 0): WObject(parent), ImageManager(){}
    public : ~ImageManagerWt(){}
};

#endif // IMAGEMANAGERWT

