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
    public : bool isImageOpened = false;

    protected: Mat _myImage;
    protected: Mat _myDrawingLayer;

    public : double drawingLayerTransparency = 0.5;

    public : double zoomFactor = 1.0;
    public : double rulerFactor = 1.0;
    public : double rulerLength = 0;

    public : void cleanDrawingLayer();

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
