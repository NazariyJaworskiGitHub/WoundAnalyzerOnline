#ifndef IMAGEMANAGERWT
#define IMAGEMANAGERWT

#include "Image/imagemanager.h"
#include "Wt/WMemoryResource"
#include "Wt/WLength"

using namespace Wt;

class ImageManagerWt : public WObject, public ImageManager
{
    public : WMemoryResource *myWMemoryResource = nullptr;
    public : vector<unsigned char> imageDecodedData;
    public : void updateWMemoryResource()
    {
        imageDecodedData.clear();
        vector<int>param = vector<int>(2);
        param[0] = CV_IMWRITE_JPEG_QUALITY;
        param[1] = 95;
        imencode(".jpg", _blendLayers(), imageDecodedData, param);
        myWMemoryResource->setData(imageDecodedData);
    }
    public : WLength getWidth() const {return WLength(_myImage.cols);}
    public : WLength getHeight() const {return WLength(_myImage.rows);}

//    public : void openImage(const Mat &image) override
//    {
//        ImageManager::openImage(image);
//        updateWMemoryResource();
//    }
//    public : void openImage(const string &fileName) override
//    {
//        ImageManager::openImage(fileName);
//        updateWMemoryResource();
//    }

    public : ImageManagerWt(WObject *parent = 0): WObject(parent), ImageManager()
    {
        myWMemoryResource = new WMemoryResource("image/jpg",this);
    }
    public : ~ImageManagerWt(){}
};

#endif // IMAGEMANAGERWT

