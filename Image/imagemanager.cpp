#include "imagemanager.h"

void ImageManager::cleanDrawingLayer()
{
    _myDrawingLayer = Mat(_myImage.rows, _myImage.cols, _myImage.type());
    resize(_myDrawingLayer,_myDrawingLayer,Size(),zoomFactor,zoomFactor);
}

void ImageManager::highlightCircle(const Point2d &p, const Color &col, int radius)
{
    circle(
                _myDrawingLayer,
                p,
                radius + NODE_RADIUS_THICKNESS_DELTA,
                col,
                CV_FILLED);
}

void ImageManager::highlightLine(
        const Point2d &a, const Point2d &b, const Color &col, int thickness)
{
    line(
            _myDrawingLayer,
            a,
            b,
            col,
            thickness + LINE_RADIUS_THICKNESS_DELTA);
}

double ImageManager::drawPolygon(
        const PolygonF &polygon,
        const Color &polyEdgeCol,
        const Color &polyFillCol,
        const Color &polyTextCol,
        int thickness,
        bool drawText)
{
    double _area = -1;
    if(polygon.size() != 0)
    {
        for(unsigned i=0; i<polygon.size(); ++i)
        {
            circle(
                        _myDrawingLayer,
                        polygon[i],
                        thickness + LINE_RADIUS_THICKNESS_DELTA,
                        polyEdgeCol);
        }
        if(polygon.size() == 2)
            polylines(
                        _myDrawingLayer,
                        polygon,
                        false,
                        polyEdgeCol,
                        thickness);
        else if(polygon.size() > 2)
        {
            std::vector<std::vector<Point2d>> _p(1);
            _p[0] = polygon;
            fillPoly(
                        _myDrawingLayer,
                        _p,
                        polyFillCol);
            polylines(
                        _myDrawingLayer,
                        polygon,
                        true,
                        polyEdgeCol,
                        thickness);
            if(rulerLength != 0)
                _area = contourArea(polygon) *
                        (rulerFactor / rulerLength) *
                        (rulerFactor / rulerLength);
            if(drawText)
                putText(
                        _myDrawingLayer,
                        to_string(_area),
                        polygon[0],
                        FONT_HERSHEY_PLAIN,
                        1.0,
                        polyTextCol);
        }
    }
    return _area;
}

double ImageManager::drawRuler(
        const PolygonF &ruler,
        const Color &rulerCol,
        const Color &rulerTextCol,
        int thickness,
        bool drawText)
{
    double _distance = 0;
    if(ruler.size() != 0)
    {
        circle(
                    _myDrawingLayer,
                    ruler[0],
                    thickness + LINE_RADIUS_THICKNESS_DELTA,
                    rulerCol);
        if(ruler.size()==2)
        {
            line(
                        _myDrawingLayer,
                        ruler[0],
                        ruler[1],
                        rulerCol,
                        thickness);
            circle(
                        _myDrawingLayer,
                        ruler[1],
                        thickness + LINE_RADIUS_THICKNESS_DELTA,
                        rulerCol);
            _distance = std::sqrt((ruler[0].x-ruler[1].x)*(ruler[0].x-ruler[1].x) +
                    (ruler[0].y-ruler[1].y)*(ruler[0].y-ruler[1].y));

            if(drawText)
                putText(
                        _myDrawingLayer,
                        to_string(_distance) + "px",
                        Point(
                            ruler[0].x + (ruler[1].x - ruler[0].x)/2.0,
                            ruler[0].y + (ruler[1].y - ruler[0].y)/2.0),
                        FONT_HERSHEY_PLAIN,
                        1.0,
                        rulerTextCol);
        }
    }
    rulerLength = _distance;
    return _distance;
}

Mat ImageManager::_blendLayers() const
{
    Mat _result;
    cv::resize(_myImage,_result,cv::Size(),zoomFactor,zoomFactor);
    addWeighted(
                _result,
                1.0 - drawingLayerTransparency,
                _myDrawingLayer,
                drawingLayerTransparency,
                0.0,
                _result);
    return _result;
}

void ImageManager::_onLoadImageCleanup()
{
    if(!_myImage.empty())
    {
        cleanDrawingLayer();
        isImageOpened = true;
        zoomFactor = 1.0;
        rulerFactor = 1.0;
        rulerLength = 0;

        Log::GlobalLogger.msg(Log::TRACE, "[Image] image is loaded\n");
    }
    else
        Log::GlobalLogger.msg(Log::WARN, "[Image] image is not loaded\n");
}

void ImageManager::openImage(const Mat &image)
{
    Log::GlobalLogger.msg(Log::TRACE, "[Image] opening internal\n");
    _myImage = image.clone();
    _onLoadImageCleanup();
}

void ImageManager::openImage(const string &fileName)
{
    Log::GlobalLogger.msg(Log::TRACE, "[Image] opening " + fileName + "\n");
    _myImage = imread(fileName, IMREAD_COLOR);
    _onLoadImageCleanup();
}

void ImageManager::saveImage(const string &fileName) const
{
    Log::GlobalLogger.msg(Log::TRACE, "[Image] saving " + fileName + "\n");

    bool _result = imwrite(fileName,_blendLayers());

    if(_result)
        Log::GlobalLogger.msg(Log::TRACE, "[Image] image is saved\n");
    else
        Log::GlobalLogger.msg(Log::WARN, "[Image] image is not saved\n");
}

ImageManager::ImageManager(){}

ImageManager::~ImageManager(){}
