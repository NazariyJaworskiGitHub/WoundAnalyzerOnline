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
#include <Wt/WTabWidget>
#include <Wt/WPushButton>
#include <Wt/WFileUpload>
#include <Wt/WImage>
#include <Wt/WSlider>
#include <Wt/WPanel>
#include <Wt/WText>
#include <Wt/WLabel>

#include <Wt/WTable>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

#include <Wt/WJavaScript>

#include <Image/imagemanagerwt.h>
#include <Web/UI/dialogprogressbar.h>
#include <Utilities/mathutilities.h>

#define MIN_IMAGE_SIZE "400px"

#define ZOOM_MIN            10
#define ZOOM_MAX            400
#define ZOOM_BUTTON_STEP    10

#define TRANSP_MIN          10
#define TRANSP_MAX          90
#define TRANSP_BUTTON_STEP  10

using namespace Wt;

namespace Web
{
namespace Ui
{
class WindowImageEdit : public WContainerWidget
{
    private:
    ImageManagerWt  *_myImageManagerWt  = nullptr;

    WTable          *_myWTable                  = nullptr;

    WToolBar        *_myHeaderToolBar           = nullptr;

    WPushButton     *_myOpenButton              = nullptr;
    WPushButton     *_mySaveButton              = nullptr;
    WPushButton     *_myExportButton            = nullptr;

    WPushButton     *_myPolygonButton           = nullptr;
    WPushButton     *_myRulerButton             = nullptr;
    WPushButton     *_myClearButton             = nullptr;

    WPushButton     *_myZoomInButton            = nullptr;
    WPushButton     *_myZoomOutButton           = nullptr;

    WPushButton     *_myTransparencyUpButton    = nullptr;
    WPushButton     *_myTransparencyDownButton  = nullptr;

    WPushButton     *_myConfigureButton         = nullptr;

    WFileUpload     *_myWFileUpload             = nullptr;
    Web::Ui::DialogProgressbar *_myProgressBarDialog    = nullptr;

    WImage          *_myWImage                  = nullptr;

    WContainerWidget*_myFooterToolBar           = nullptr;

    WLabel          *_myWSliderZoomLabel        = nullptr;
    WSlider         *_myWSliderZoom;
    WText           *_myWSliderZoomText         = nullptr;

    WLabel          *_myWSliderTransparencyLabel    = nullptr;
    WSlider         *_myWSliderTransparency     = nullptr;
    WText           *_myWSliderTransparencyText = nullptr;

    WPanel          *_myProgressBar             = nullptr;
    WText           *_myProgressBarText         = nullptr;

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

    Color polygonEdgeColor            = Color(255, 255, 255);
    Color polygonColor                = Color(127, 127, 127);
    Color polygonTextColor            = Color(127, 255, 127);
    int   polygonEdgeThickness        = 1;
    Color rulerColor                  = Color(255, 255,   0);
    Color rulerTextColor              = Color(  0, 255, 255);
    int   rulerThickness              = 1;


    private: void _clearPolygonsAndRuler()
    {
        for(auto p = polygons.begin(); p!= polygons.end(); ++p)
            p->clear();
        polygons.clear();
        rulerPoints.clear();
        isCreateNewPolygon = true;
        _redrawWImage();
    }

    private: void _redrawWImage()
    {
        _myImageManagerWt->clearDrawingLayer();

        rulerDistance = _myImageManagerWt->drawRuler(
                        rulerPoints,
                        rulerColor,
                        rulerTextColor,
                        rulerThickness);

        woundsArea = 0;
        for(auto p: polygons)
            woundsArea += _myImageManagerWt->drawPolygon(
                        p,
                        polygonEdgeColor,
                        polygonColor,
                        polygonTextColor,
                        polygonEdgeThickness);

        _myImageManagerWt->updateWMemoryResource();
        _myWImage->setResource(_myImageManagerWt->myWMemoryResource);
        _myWImage->resize(_myImageManagerWt->getZoomedWidth(),_myImageManagerWt->getZoomedHeight());
        _myWImage->setMaximumSize(_myImageManagerWt->getZoomedWidth(),_myImageManagerWt->getZoomedHeight());
    }

    private : void _onLoadPrepareToolbar()
    {
        _myHeaderToolBar = new WToolBar(this);

        _myOpenButton = new  WPushButton("Open", this);
        _myOpenButton->setIcon(WLink("icons/Open.png"));
        _myHeaderToolBar->addButton(_myOpenButton);

        _mySaveButton = new  WPushButton("Save", this);
        _mySaveButton->setIcon(WLink("icons/Save.png"));
        _myHeaderToolBar->addButton(_mySaveButton);

        _myExportButton = new  WPushButton("Export", this);
        _myExportButton->setIcon(WLink("icons/Export.png"));
        _myHeaderToolBar->addButton(_myExportButton);

        _myHeaderToolBar->addSeparator();

        _myPolygonButton = new  WPushButton("Polygon", this);
        _myPolygonButton->setIcon(WLink("icons/Polygon.png"));
        _myPolygonButton->clicked().connect(std::bind([=] () {
            if(this->mode == POLYGON_MODE)
            {
                this->mode = EDIT_MODE;
                _myPolygonButton->setStyleClass("btn");
            }
            else
            {
                this->mode = POLYGON_MODE;
                isCreateNewPolygon = true;
                _myPolygonButton->setStyleClass("btn-success");
                _myRulerButton->setStyleClass("btn");
            }
        }));
        _myHeaderToolBar->addButton(_myPolygonButton);

        _myRulerButton = new  WPushButton("Ruler", this);
        _myRulerButton->setIcon(WLink("icons/Ruler.png"));
        _myRulerButton->clicked().connect(std::bind([=] () {
            if(this->mode == RULER_MODE)
            {
                this->mode = EDIT_MODE;
                _myRulerButton->setStyleClass("btn");
            }
            else
            {
                this->mode = RULER_MODE;
                _myRulerButton->setStyleClass("btn-success");
                _myPolygonButton->setStyleClass("btn");
            }
        }));
        _myHeaderToolBar->addButton(_myRulerButton);

        _myClearButton = new  WPushButton("Clear", this);
        _myClearButton->setIcon(WLink("icons/Clear.png"));
        _myClearButton->clicked().connect(std::bind([=] () {
            _clearPolygonsAndRuler();
        }));
        _myHeaderToolBar->addButton(_myClearButton);

        _myHeaderToolBar->addSeparator();

        _myZoomOutButton = new  WPushButton("Zoom-Out", this);
        _myZoomOutButton->setIcon(WLink("icons/Zoom-Out.png"));
        _myHeaderToolBar->addButton(_myZoomOutButton);

        _myZoomInButton = new  WPushButton("Zoom-In", this);
        _myZoomInButton->setIcon(WLink("icons/Zoom-In.png"));
        _myHeaderToolBar->addButton(_myZoomInButton);        

        _myTransparencyDownButton = new  WPushButton("Light-Dn", this);
        _myTransparencyDownButton->setIcon(WLink("icons/Transparency-Down.png"));
        _myHeaderToolBar->addButton(_myTransparencyDownButton);

        _myTransparencyUpButton = new  WPushButton("Light-Up", this);
        _myTransparencyUpButton->setIcon(WLink("icons/Transparency-Up.png"));
        _myHeaderToolBar->addButton(_myTransparencyUpButton);

        _myHeaderToolBar->addSeparator();

        _myConfigureButton = new  WPushButton("Configure", this);
        _myConfigureButton->setIcon(WLink("icons/Configure.png"));
        _myHeaderToolBar->addButton(_myConfigureButton);

        _myWTable->elementAt(0, 0)->addWidget(_myHeaderToolBar);
    }

    private: void _onLoadPrepareImageArea()
    {
        _myWImage = new WImage(this);
        _myWImage->decorationStyle().setCursor(Wt::CrossCursor);
        _myWImage->setJavaScriptMember("oncontextmenu","function() {return false;}");

        _myWFileUpload = new WFileUpload(this);
        _myWFileUpload->setFilters("image/*");

        // Upload automatically when the user entered a file.
        _myWFileUpload->changed().connect(std::bind([=] () {
            if(_myWFileUpload->canUpload())
            {
                _myProgressBarDialog = new Web::Ui::DialogProgressbar("Upload progress",this);
                // dialg accept will be emmited on 100%, see _myProgressBarDialog
                _myWFileUpload->setProgressBar(_myProgressBarDialog->getProgressBar());
                _myProgressBarDialog->show();
                _myWFileUpload->upload();
//                _myProgressBarDialog->signalStuck.connect(std::bind([=] () {
//                    std::cout << "new attempt\n";
//                    _myWFileUpload->stealSpooledFile();
//                    _myWFileUpload->upload();
//                }));
            }
        }));

        // React to a succesfull upload.
        _myWFileUpload->uploaded().connect(std::bind([=] () {
            _myWFileUpload->hide();
            _myImageManagerWt->openImage(_myWFileUpload->spoolFileName());
            _redrawWImage();
            _myWImage->show();
        }));

        // React to a file upload problem.
        _myWFileUpload->fileTooLarge().connect(std::bind([=] () {
            Log::GlobalLogger.msg(Log::ERROR,"[Image upload] fileTooLarge() signal\n");
        }));

        _myWTable->elementAt(2, 0)->addWidget(_myWFileUpload);
        _myWTable->elementAt(2, 0)->addWidget(_myWImage);
        //_myWTable->elementAt(2, 0)->setOverflow(OverflowScroll);
        //_myWTable->elementAt(2, 0)->resize(WLength("100%"),WLength("100%"));
        //_myWTable->elementAt(2, 0)->setMinimumSize(WLength(MIN_IMAGE_SIZE),WLength(MIN_IMAGE_SIZE));

    }

    private: void _onLoadPrepareFooter()
    {
        _myFooterToolBar = new WContainerWidget(this);

        WVBoxLayout *footerLayout = new WVBoxLayout();

        _myWSliderZoomLabel = new WLabel(new WImage(WLink("icons/ZoomSmall.png"),this),this);
        _myWSliderZoom = new WSlider(this);
        _myWSliderZoomText = new WText(std::to_string(ZOOM_INIT)+"%",this);

        WHBoxLayout *zoomSliderLayout = new WHBoxLayout();
        zoomSliderLayout->addWidget(_myWSliderZoomLabel);
        zoomSliderLayout->addWidget(_myWSliderZoom,1);
        zoomSliderLayout->addWidget(_myWSliderZoomText);
        footerLayout->addLayout(zoomSliderLayout);

        _myWSliderTransparencyLabel = new WLabel(new WImage(WLink("icons/Transparency.png"),this),this);
        _myWSliderTransparency = new WSlider(this);
        _myWSliderTransparencyText = new WText(std::to_string(TRANSP_INIT)+"%",this);

        WHBoxLayout *transparencySliderLayout = new WHBoxLayout();
        transparencySliderLayout->addWidget(_myWSliderTransparencyLabel);
        transparencySliderLayout->addWidget(_myWSliderTransparency,1);
        transparencySliderLayout->addWidget(_myWSliderTransparencyText);
        footerLayout->addLayout(transparencySliderLayout);

        _myProgressBar = new WPanel(this);
        _myProgressBarText = new WText("TEST STRING",this);
        _myProgressBar->setCentralWidget(_myProgressBarText);
        footerLayout->addWidget(_myProgressBar);

        _myFooterToolBar->setLayout(footerLayout);

        _myWTable->elementAt(3, 0)->addWidget(_myFooterToolBar);
    }

    private: void _onLoadPrepareZoomSlider()
    {
        _myWSliderZoom->setMinimum(ZOOM_MIN);
        _myWSliderZoom->setMaximum(ZOOM_MAX);
        _myWSliderZoom->valueChanged().connect(this,&WindowImageEdit::_changeZoom);
        _myWSliderZoom->setValue(ZOOM_INIT);
        _myZoomInButton->clicked().connect(std::bind([=] () {
                _changeZoom(_myWSliderZoom->value() + ZOOM_BUTTON_STEP);
        }));
        _myZoomOutButton->clicked().connect(std::bind([=] () {
                _changeZoom(_myWSliderZoom->value() - ZOOM_BUTTON_STEP);
        }));
    }

    private: void _changeZoom(int value)
    {
        // slider has automatic min/max control
        _myWSliderZoom->setValue(value);
        if(_myImageManagerWt->isImageOpened())
        {
            double delta = _myWSliderZoom->value()/100.0 - _myImageManagerWt->getZoomFactor();
            for(auto p = polygons.begin(); p!= polygons.end(); ++p)
                for(auto n = p->begin(); n!= p->end(); ++n)
                    *n *= 1.0 + delta / _myImageManagerWt->getZoomFactor();
            for(auto n = rulerPoints.begin(); n!= rulerPoints.end(); ++n)
                *n *= 1.0 + delta / _myImageManagerWt->getZoomFactor();

            _myImageManagerWt->zoom(_myWSliderZoom->value());
            _redrawWImage();
        }
        _myWSliderZoomText->setText(_myWSliderZoom->valueText() + "%");
    }

    private: void _onLoadPrepareTransparencySlider()
    {
        _myWSliderTransparency->setMinimum(TRANSP_MIN);
        _myWSliderTransparency->setMaximum(TRANSP_MAX);
        _myWSliderTransparency->valueChanged().connect(this,&WindowImageEdit::_changeTransparency);
        _myWSliderTransparency->setValue(TRANSP_INIT);
        _myTransparencyUpButton->clicked().connect(std::bind([=] () {
                _changeTransparency(_myWSliderTransparency->value() + TRANSP_BUTTON_STEP);
        }));
        _myTransparencyDownButton->clicked().connect(std::bind([=] () {
                _changeTransparency(_myWSliderTransparency->value() - TRANSP_BUTTON_STEP);
        }));

    }

    private: void _changeTransparency(int value)
    {
        // slider has automatic min/max control
        _myWSliderTransparency->setValue(value);
        if(_myImageManagerWt->isImageOpened())
        {
            _myImageManagerWt->setDrawingLayerTransparency(_myWSliderTransparency->value()/100.0);
            _redrawWImage();
        }
        _myWSliderTransparencyText->setText(_myWSliderTransparency->valueText() + "%");
    }

    Point2d *_findNodeWithPosInPolygons(const Point2d &pos)
    {
        Point2d *ptr = nullptr;
        for(auto p = polygons.begin(); p!= polygons.end(); ++p)
        {
            bool found = false;
            for(auto n = p->begin(); n!= p->end(); ++n)
                if(MathUtilities::length(*n, pos) < polygonEdgeThickness + LINE_RADIUS_THICKNESS_DELTA)
                {
                    ptr = &*n;
                    _ptrToPolygonWhereNodeIsFound = &*p;
                    found = true;
                    break;
                }
            if(found)break;
        }
        return ptr;
    }

    Point2d *_findNodeWithPosInRuler(const Point2d &pos)
    {
        Point2d *ptr = nullptr;
        if(rulerPoints.size() == 1)
        {
            if(MathUtilities::length(rulerPoints[0],pos) < rulerThickness+LINE_RADIUS_THICKNESS_DELTA)
                ptr = &rulerPoints[0];
        }
        else if(rulerPoints.size() > 1)
        {
            if(MathUtilities::length(rulerPoints[0],pos) < rulerThickness+LINE_RADIUS_THICKNESS_DELTA)
                ptr = &rulerPoints[0];
            else if(MathUtilities::length(rulerPoints[1],pos) < rulerThickness+LINE_RADIUS_THICKNESS_DELTA)
                ptr = &rulerPoints[1];
        }
        return ptr;
    }

    bool _findLineWithPosInPolygons(
            Point2d **ptrToA,
            Point2d **ptrToB,
            const Point2d &pos)
    {
        bool found = false;
        for(auto p = polygons.begin(); p!= polygons.end(); ++p)
        {
            if(p->size()>1)
            {
                auto n = p->begin();            // begin
                *ptrToA = &*n;                  // a = first
                for(++n; n!= p->end(); ++n)     // move to next, so a become prev
                {
                    *ptrToB = &*n;              // b = cur
                    if(MathUtilities::isOnSegment(**ptrToA, **ptrToB, pos))
                    {
                        found = true;
                        break;
                    }
                    *ptrToA = &*n;              // a = cur
                }
                if(!found)  // check first-last
                {
                    *ptrToB =  &p->front();
                    *ptrToA =  &p->back();
                    if(MathUtilities::isOnSegment(**ptrToA, **ptrToB, pos))
                        found = true;
                }
            }
            if(found)
            {
                _ptrToPolygonWhereLineIsFound = &*p;
                break;
            }
            else
            {
                *ptrToA = nullptr;
                *ptrToB = nullptr;
            }
        }
        return found;
    }

    private: void _onMouseWentDownEvent(WMouseEvent e)
    {
        if(_myImageManagerWt->isImageOpened())
        {
            Point2d crd = {e.widget().x, e.widget().y};
            if(e.button() == WMouseEvent::LeftButton)
            {
                switch (mode) {
                case POLYGON_MODE:
                    if(isCreateNewPolygon)
                    {
                        isCreateNewPolygon = false;
                        PolygonF p;
                        p.push_back(crd);
                        polygons.push_back(p);
                    }
                    else
                        polygons.back().push_back(crd);
                    _redrawWImage();
                    break;
                case RULER_MODE:
                    if(rulerPoints.size() == 0)
                        rulerPoints.push_back(crd);
                    else if(rulerPoints.size() == 1)
                        rulerPoints.push_back(crd);
                    else if(rulerPoints.size() == 2)
                    {
                        rulerPoints[0] = rulerPoints[1];
                        rulerPoints[1] = crd;
                    }
                    _redrawWImage();
                    break;
                case EDIT_MODE:
                    _nodeToMove = _findNodeWithPosInPolygons(crd);
                    if(!_nodeToMove)
                        _nodeToMove = _findNodeWithPosInRuler(crd);
                    break;
                }
            }
            else if(e.button() == WMouseEvent::RightButton)
            {
                if(mode == EDIT_MODE)
                {
                    Point2d *nodeToDelete = _findNodeWithPosInPolygons(crd);
                    if(nodeToDelete)
                    {
                        _ptrToPolygonWhereNodeIsFound->erase(static_cast<PolygonF::iterator>(nodeToDelete));
                        _nodeToMove = nullptr;
                    }
                    else
                    {
                        Point2d *a;
                        Point2d *b;
                        if(_findLineWithPosInPolygons(&a,&b,crd))
                        {
                            _ptrToPolygonWhereLineIsFound->insert(static_cast<PolygonF::iterator>(b),crd);
                            _nodeToMove = nullptr;
                        }
                    }
                    _redrawWImage();
                }
            }
        }
    }

    private: void _onMouseWentUpEvent(WMouseEvent e)
    {
        if(this->mode == EDIT_MODE)
        {
            _nodeToMove = nullptr;
        }

    }

    private: void _onMouseDraggedEvent(WMouseEvent e)
    {
        if(_myImageManagerWt->isImageOpened())
        {
            if(this->mode == EDIT_MODE)
            {
                if(_nodeToMove)
                    *_nodeToMove = {e.widget().x, e.widget().y};
                _redrawWImage();
            }
        }
    }

    private: void _initializeMouseControl()
    {
        _myWImage->mouseWentDown().connect(this, &WindowImageEdit::_onMouseWentDownEvent);
        _myWImage->mouseWentUp().connect(this, &WindowImageEdit::_onMouseWentUpEvent);
        _myWImage->mouseDragged().connect(this, &WindowImageEdit::_onMouseDraggedEvent);
    //        _myWImage->mouseWheel().connect(std::bind([=] (WMouseEvent &e) {
    //            _changeZoom(_myWSliderZoom->value() + e.wheelDelta());
    //        },std::placeholders::_1));
    }

    public :WindowImageEdit(WContainerWidget *parent) : WContainerWidget(parent)
    {

        _myImageManagerWt = new ImageManagerWt(this);

        _myWTable = new WTable(this);

        _onLoadPrepareToolbar();
        _onLoadPrepareImageArea();
        _onLoadPrepareFooter();

        _onLoadPrepareZoomSlider();
        _onLoadPrepareTransparencySlider();

        _initializeMouseControl();
    }
    public : ~WindowImageEdit()
    {
        for(auto p = polygons.begin(); p!= polygons.end(); ++p)
            p->clear();
        polygons.clear();
        rulerPoints.clear();
    }
};
}
}

#endif // WINDOWIMAGEEDIT

