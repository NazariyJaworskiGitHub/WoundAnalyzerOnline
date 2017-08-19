#include "imagemanager.h"

void ImageManager::zoom(double percentage)
{
    _zoomFactor += percentage/100.0 - _zoomFactor;
    clearDrawingLayer();
}

void ImageManager::clearDrawingLayer()
{
    _myDrawingLayer = Mat(_myImage.rows, _myImage.cols, _myImage.type());
    resize(_myDrawingLayer,_myDrawingLayer,Size(),_zoomFactor,_zoomFactor);
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
        std::vector<cv::Point> _tmpPoly;
        _tmpPoly.resize(polygon.size());

        for(unsigned i=0; i<polygon.size(); ++i)
        {
            _tmpPoly[i].x = polygon[i].x;
            _tmpPoly[i].y = polygon[i].y;
            circle(
                        _myDrawingLayer,
                        _tmpPoly[i],
                        thickness + LINE_RADIUS_THICKNESS_DELTA,
                        polyEdgeCol);
        }
        if(polygon.size() == 2)
            polylines(
                        _myDrawingLayer,
                        _tmpPoly,
                        false,
                        polyEdgeCol,
                        thickness);
        else if(polygon.size() > 2)
        {
            std::vector<std::vector<Point>> _p(1);
            _p[0] = _tmpPoly;
            fillPoly(
                        _myDrawingLayer,
                        _p,
                        polyFillCol);
            polylines(
                        _myDrawingLayer,
                        _tmpPoly,
                        true,
                        polyEdgeCol,
                        thickness);
            if(_rulerLength != 0)
            {
                _area = contourArea(_tmpPoly) *
                        (_rulerFactor / _rulerLength) *
                        (_rulerFactor / _rulerLength);
                if(drawText)
                    putText(
                            _myDrawingLayer,
                            to_string(_area),
                            _tmpPoly[0],
                            FONT_HERSHEY_PLAIN,
                            1.0,
                            polyTextCol);
            }
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
    _rulerLength = _distance;
    return _distance;
}

Mat ImageManager::_blendLayers() const
{
    Mat _result;
    cv::resize(_myImage,_result,cv::Size(),_zoomFactor,_zoomFactor);
    addWeighted(
                _result,
                _drawingLayerTransparency,
                _myDrawingLayer,
                1.0 - _drawingLayerTransparency,
                0.0,
                _result);
    return _result;
}

void ImageManager::_onLoadImageCleanup()
{
    if(!_myImage.empty())
    {
        clearDrawingLayer();
        _isImageOpened = true;
        _drawingLayerTransparency = TRANSP_INIT / 100.0;
        _zoomFactor = ZOOM_INIT / 100.0;
        _rulerFactor = RULER_FACTOR_INIT / 100.0;
        _rulerLength = RULER_INIT;
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
