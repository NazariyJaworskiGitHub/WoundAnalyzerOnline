#ifndef WINDOWIMAGEEDIT
#define WINDOWIMAGEEDIT

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <Wt/WContainerWidget>
#include <Wt/WToolBar>
#include <Wt/WPushButton>
#include <Wt/WFileUpload>
#include <Wt/WImage>
#include <Wt/WSlider>
#include <Wt/WPanel>
#include <Wt/WText>
#include <Wt/WDoubleSpinBox>
#include <Wt/WLabel>

#include <Wt/WTable>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

#include <Wt/WJavaScript>

#include <Wt/WMessageBox>

#include "Image/imagemanagerwt.h"
#include "Web/UI/dialogprogressbar.h"

#define MIN_IMAGE_SIZE "400px"

#define ZOOM_MIN            10
#define ZOOM_MAX            400
#define ZOOM_BUTTON_STEP    10

#define TRANSP_MIN          10
#define TRANSP_MAX          90
#define TRANSP_BUTTON_STEP  10

#define THICKNESS_MAX       5

#define RULER_FACTOR_MIN     0.01
#define RULER_FACTOR_MAX    10.0

using namespace Wt;

namespace Web
{
namespace Ui
{

class DialogImageSettings;

class WindowImageEdit : public WContainerWidget
{
    private:
    ImageManagerWt  *_myImageManagerWt          = nullptr;

    WTable          *_myWTable                  = nullptr;

    WToolBar        *_myHeaderToolBar           = nullptr;

    WFileUpload     *_myWFileUpload             = nullptr;
    WLabel          *_myOpenButton              = nullptr; // see _onLoadPrepareImageUploader();

    WPushButton     *_myPolygonButton           = nullptr;
    WPushButton     *_myRulerButton             = nullptr;
    WPushButton     *_myClearButton             = nullptr;

    WPushButton     *_myZoomInButton            = nullptr;
    WPushButton     *_myZoomOutButton           = nullptr;

    WPushButton     *_myTransparencyUpButton    = nullptr;
    WPushButton     *_myTransparencyDownButton  = nullptr;

    WPushButton     *_myConfigureButton         = nullptr;
    DialogImageSettings *_myDialogImageSettings = nullptr;


    DialogProgressbar *_myProgressBarDialog     = nullptr;

    WImage          *_myWImage                  = nullptr;

    WContainerWidget*_myFooterToolBar           = nullptr;

    WLabel          *_myWSliderZoomLabel        = nullptr;
    WSlider         *_myWSliderZoom;
    WText           *_myWSliderZoomText         = nullptr;

    WLabel          *_myWSliderTransparencyLabel= nullptr;
    WSlider         *_myWSliderTransparency     = nullptr;
    WText           *_myWSliderTransparencyText = nullptr;

    WPanel          *_myProgressBar             = nullptr;
    WText           *_myProgressBarText         = nullptr;
    WDoubleSpinBox  *_myRulerFactorSpinBox      = nullptr;
    WLineEdit       *_myTotalAreaLineEdit       = nullptr;

    enum Mode{POLYGON_MODE, RULER_MODE, EDIT_MODE};

    Mode            mode                        = EDIT_MODE;
    bool            isCreateNewPolygon          = true;
    std::vector<PolygonF>   polygons;
    PolygonF                rulerPoints;

    double          woundsArea;
    double          rulerDistance;

    Point2d         *_nodeToMove                    = nullptr;
    PolygonF        *_ptrToPolygonWhereNodeIsFound  = nullptr;
    PolygonF        *_ptrToPolygonWhereLineIsFound  = nullptr;

    public: // public for DialogImageSettings
    Color polygonEdgeColor            = Color(255, 255, 255);
    Color polygonColor                = Color(127, 127, 127);
    Color polygonTextColor            = Color(127, 255, 127);
    int   polygonEdgeThickness        = 1;
    Color rulerColor                  = Color(255, 255,   0);
    Color rulerTextColor              = Color(  0, 255, 255);
    int   rulerThickness              = 1;


    private: void _clearPolygonsAndRuler();

    public : void redrawWImage();   // public for DialogImageSettings

    private: void _onLoadPrepareToolbar();

    private: void _onLoadPrepareImageArea();

    private: void _onLoadPrepareImageUploader();

    private: void _onLoadPrepareFooter();

    private: void _onLoadPrepareZoomSlider();

    private: void _disableUI();

    private: void _enableUI();

    private: void _changeZoom(int value);

    private: void _onLoadPrepareTransparencySlider();

    private: void _changeTransparency(int value);

    Point2d *_findNodeWithPosInPolygons(const Point2d &pos);

    Point2d *_findNodeWithPosInRuler(const Point2d &pos);

    bool _findLineWithPosInPolygons(Point2d **ptrToA, Point2d **ptrToB, const Point2d &pos);

    private: void _onMouseWentDownEvent(WMouseEvent e);

    private: void _onMouseWentUpEvent(WMouseEvent e);

    private: void _onMouseDraggedEvent(WMouseEvent e);

    private: void _initializeMouseControl();

    public : void setImage(const cv::Mat &img);

    public : void copyAllDataTo(
            cv::Mat &img,
            std::vector<PolygonF> &pols,
            PolygonF &pol,
            double &a,
            double &rf) const;

    public : void copyAllDataFrom(
        const cv::Mat &img,
        const std::vector<PolygonF> &pols,
        const PolygonF &pol,
        const double &a,
        const double &rf);

    public: WindowImageEdit(WContainerWidget *parent);
    public : ~WindowImageEdit();
};
}
}

#endif // WINDOWIMAGEEDIT

