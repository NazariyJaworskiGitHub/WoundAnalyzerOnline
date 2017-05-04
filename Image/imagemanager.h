#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <string>
#include <opencv2/opencv.hpp>

#include "Utilities/Logger/logger.h"

#define NODE_RADIUS_THICKNESS_DELTA 4
#define LINE_RADIUS_THICKNESS_DELTA 2

using namespace cv;
using namespace std;

typedef Scalar Color;
typedef std::vector<Point2d> PolygonF;

class ImageManager
{
    protected: Mat _myImage;
    protected: Mat _myDrawingLayer;

    private: bool _isImageOpened = false;
    public : bool isImageOpened() const noexcept {return _isImageOpened;}
    private: double _drawingLayerTransparency = 0.5;
    public : double getDrawingLayerTransparency() const noexcept {return _drawingLayerTransparency;}
    private: double _zoomFactor = 1.0;
    public : double getZoomFactor() const noexcept {return _zoomFactor;}
    public : void zoom(double percentage);
    private: double _rulerFactor = 1.0;
    public : double getRulerFactor() const noexcept {return _rulerFactor;}
    private: double _rulerLength = 0;
    public : double getRulerLength() const noexcept {return _rulerLength;}

    public : void clearDrawingLayer();

    public : void highlightCircle(
            const Point2d &p,
            const Color &col,
            int radius);

    public : void highlightLine(
            const Point2d &a,
            const Point2d &b,
            const Color &col,
            int thickness);

    public : double drawPolygon(// returns the area of polygon
            const PolygonF &polygon,
            const Color &polyEdgeCol,
            const Color &polyFillCol,
            const Color &polyTextCol,
            int thickness,
            bool drawText = true);

    public : double drawRuler(// returns the length of the ruler
            const PolygonF &ruler,
            const Color &rulerCol,
            const Color &rulerTextCol,
            int thickness,
            bool drawText = true);

    protected: Mat _blendLayers() const;

    protected: void _onLoadImageCleanup();

    public : virtual void openImage(const Mat &image);
    public : virtual void openImage(const string &fileName);

    public : void saveImage(const string &fileName) const;

    public : ImageManager();
    public : ~ImageManager();
};

#endif // IMAGEMANAGER_H
