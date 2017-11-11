#include "Web/UI/windowimageedit.h"

#include "Web/UI/dialogimagesettings.h"
#include "Utilities/mathutilities.h"

void Web::Ui::WindowImageEdit::_clearPolygonsAndRuler()
{
    for(auto p = polygons.begin(); p!= polygons.end(); ++p)
        p->clear();
    polygons.clear();
    rulerPoints.clear();
    isCreateNewPolygon = true;
    redrawWImage();
}

void Web::Ui::WindowImageEdit::redrawWImage()
{
    if(_myImageManagerWt->isImageOpened())
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

        char c[32];
        std::sprintf(c,"%.2f",woundsArea);
        _myTotalAreaLineEdit->setText(c);

        _myImageManagerWt->updateWMemoryResource();
        _myWImage->setResource(_myImageManagerWt->myWMemoryResource);
        _myWImage->resize(_myImageManagerWt->getZoomedWidth(),_myImageManagerWt->getZoomedHeight());
        _myWImage->setMaximumSize(_myImageManagerWt->getZoomedWidth(),_myImageManagerWt->getZoomedHeight());
    }
}

void Web::Ui::WindowImageEdit::_onLoadPrepareToolbar()
{
    _myHeaderToolBar = new WToolBar(this);

    /// I can't change input=file in html and can't bind the button,
    /// so i have just bond the label
    _myOpenButton = new WLabel("Open",this);
    _myOpenButton->setImage(new WImage(WLink("icons/Open.png")));
    _myOpenButton->setStyleClass("btn");
    _onLoadPrepareImageUploader();
    _myHeaderToolBar->addWidget(_myOpenButton);
    _myHeaderToolBar->setCompact(true);

//    _mySaveButton = new  WPushButton("Save", this);
//    _mySaveButton->setIcon(WLink("icons/Save.png"));
//    _myHeaderToolBar->addButton(_mySaveButton);

//    _myExportButton = new  WPushButton("Export", this);
//    _myExportButton->setIcon(WLink("icons/Export.png"));
//    _myHeaderToolBar->addButton(_myExportButton);

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
    _myConfigureButton->clicked().connect(std::bind([=] () {
        _myDialogImageSettings = new DialogImageSettings(this);
        _myDialogImageSettings->show();
    }));
    _myHeaderToolBar->addButton(_myConfigureButton);

    _myWTable->elementAt(0, 0)->addWidget(_myHeaderToolBar);
}

void Web::Ui::WindowImageEdit::_onLoadPrepareImageArea()
{
    _myWImage = new WImage(this);
    _myWImage->decorationStyle().setCursor(Wt::CrossCursor);
    _myWImage->setJavaScriptMember("oncontextmenu","function() {return false;}");
    _myWTable->elementAt(1, 0)->addWidget(_myWImage);
}

void Web::Ui::WindowImageEdit::_onLoadPrepareImageUploader()
{
    if(_myWFileUpload) delete _myWFileUpload;

    _myWFileUpload = new WFileUpload(this);
    _myWFileUpload->setFilters("image/*");

    // Upload automatically when the user entered a file.
    _myWFileUpload->changed().connect(std::bind([=] () {
        if(_myWFileUpload->canUpload())
        {
            _myProgressBarDialog = new Web::Ui::DialogProgressbar("Upload progress",this);
            // dialog accept will be emmited on 100%, see _myProgressBarDialog
            // _myProgressBarDialog will be destroed automatically on finished() signal;
            _myWFileUpload->setProgressBar(_myProgressBarDialog->getProgressBar());
            _myProgressBarDialog->show();
            _myWFileUpload->upload();
            //                _myProgressBarDialog->signalStuck.connect(std::bind([=] () {
            //                    std::cout << "new attempt\n";
            //                    //_myWFileUpload->stealSpooledFile();
            //                    _myWFileUpload->upload();
            //                }));
            //                _myWFileUpload->dataReceived().connect(std::bind([=] (long cur, long tot) {
            //                    std::cout << "current " << cur << " total " << tot << std::endl;
            //                }, std::placeholders::_1, std::placeholders::_1));
        }
    }));

    // React to a succesfull upload.
    _myWFileUpload->uploaded().connect(std::bind([=] () {
        _myImageManagerWt->openImage(_myWFileUpload->spoolFileName());
        _onLoadPrepareImageUploader();
        redrawWImage();
        _myWImage->show();
        _enableUI();
    }));

    // React to a file upload problem.
    _myWFileUpload->fileTooLarge().connect(std::bind([=] () {
        Log::GlobalLogger.msg(Log::ERROR,"[Image upload] fileTooLarge() signal\n");
        _myProgressBarDialog->reject();
        WMessageBox::show("", "File too large", Ok);
        _onLoadPrepareImageUploader();

    }));
    /// I can't just hide it! see https://tympanus.net/codrops/2015/09/15/styling-customizing-file-inputs-smart-way/
    //_myWFileUpload->hide();
    _myWFileUpload->setAttributeValue("style","width:0px;height:0px;opacity:0;visibility:hidden;overflow:hidden;position:absolute;");

    /// I can't change input=file in html and can't bind the button,
    /// so i have just bond the label
    WPushButton *_fakeBuddy = new WPushButton(this);
    _fakeBuddy->setAttributeValue("style","width:0px;height:0px;opacity:0;visibility:hidden;overflow:hidden;position:absolute;");
    _myOpenButton->setBuddy(_fakeBuddy);
    /// just can't use reinterpret_cast
    _myOpenButton->setAttributeValue("for","in"+_myWFileUpload->id()); // override buddy
    //delete _fakeBuddy;
}

void Web::Ui::WindowImageEdit::_onLoadPrepareFooter()
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

    WLabel *l1 = new WLabel("Ruler factor: ",this);
    _myRulerFactorSpinBox = new WDoubleSpinBox(this);
    _myRulerFactorSpinBox->setRange(RULER_FACTOR_MIN,RULER_FACTOR_MAX);
    _myRulerFactorSpinBox->setValue(RULER_FACTOR_INIT);
    _myRulerFactorSpinBox->setSingleStep(0.01);
    _myRulerFactorSpinBox->valueChanged().connect(std::bind([=] (double val) {
        if(_myImageManagerWt->isImageOpened())
        {
            _myImageManagerWt->setRulerFactor(val);
            redrawWImage();
        }
        }, std::placeholders::_1));
    WLabel *l2 = new WLabel("Total wounds area: ",this);
    _myTotalAreaLineEdit  = new WLineEdit(this);
    _myTotalAreaLineEdit->setReadOnly(true);
    WLabel *l3 = new WLabel("sm<sup>2</sup>",this);
    _myProgressBarText = new WText("",this);

    WHBoxLayout *progressBarLayout = new WHBoxLayout();
    progressBarLayout->addWidget(l1);
    progressBarLayout->addWidget(_myRulerFactorSpinBox);
    progressBarLayout->addWidget(l2);
    progressBarLayout->addWidget(_myTotalAreaLineEdit);
    progressBarLayout->addWidget(l3);
    progressBarLayout->addWidget(_myProgressBarText);

    footerLayout->addLayout(progressBarLayout);

    _myFooterToolBar->setLayout(footerLayout);

    _myWTable->elementAt(2, 0)->addWidget(_myFooterToolBar);
}

void Web::Ui::WindowImageEdit::_onLoadPrepareZoomSlider()
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

void Web::Ui::WindowImageEdit::_disableUI()
{
//    _myOpenButton->disable();
//    _mySaveButton->disable();
//    _myExportButton->disable();

    _myPolygonButton->disable();
    _myRulerButton->disable();
    _myClearButton->disable();

    _myZoomInButton->disable();
    _myZoomOutButton->disable();

    _myTransparencyUpButton->disable();
    _myTransparencyDownButton->disable();

    _myConfigureButton->disable();

    _myWSliderZoomLabel->disable();
    _myWSliderZoom->disable();
    _myWSliderZoomText->disable();

    _myWSliderTransparencyLabel->disable();
    _myWSliderTransparency->disable();
    _myWSliderTransparencyText->disable();

    _myRulerFactorSpinBox->disable();
}

void Web::Ui::WindowImageEdit::_enableUI()
{
    //    _myOpenButton->enable();
    //    _mySaveButton->enable();
    //    _myExportButton->enable();

    _myPolygonButton->enable();
    _myRulerButton->enable();
    _myClearButton->enable();

    _myZoomInButton->enable();
    _myZoomOutButton->enable();

    _myTransparencyUpButton->enable();
    _myTransparencyDownButton->enable();

    _myConfigureButton->enable();

    _myWSliderZoomLabel->enable();
    _myWSliderZoom->enable();
    _myWSliderZoomText->enable();

    _myWSliderTransparencyLabel->enable();
    _myWSliderTransparency->enable();
    _myWSliderTransparencyText->enable();

    _myRulerFactorSpinBox->enable();
}

void Web::Ui::WindowImageEdit::_changeZoom(int value)
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
        redrawWImage();
    }
    _myWSliderZoomText->setText(_myWSliderZoom->valueText() + "%");
}

void Web::Ui::WindowImageEdit::_onLoadPrepareTransparencySlider()
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

void Web::Ui::WindowImageEdit::_changeTransparency(int value)
{
    // slider has automatic min/max control
    _myWSliderTransparency->setValue(value);
//    if(_myImageManagerWt->isImageOpened())
//    {
        _myImageManagerWt->setDrawingLayerTransparency(_myWSliderTransparency->value()/100.0);
        redrawWImage();
//    }
    _myWSliderTransparencyText->setText(_myWSliderTransparency->valueText() + "%");
}

Point2d *Web::Ui::WindowImageEdit::_findNodeWithPosInPolygons(const Point2d &pos)
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

Point2d *Web::Ui::WindowImageEdit::_findNodeWithPosInRuler(const Point2d &pos)
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

bool Web::Ui::WindowImageEdit::_findLineWithPosInPolygons(Point2d **ptrToA, Point2d **ptrToB, const Point2d &pos)
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

void Web::Ui::WindowImageEdit::_onMouseWentDownEvent(WMouseEvent e)
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
                redrawWImage();
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
                redrawWImage();
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
                redrawWImage();
            }
        }
    }
}

void Web::Ui::WindowImageEdit::_onMouseWentUpEvent(WMouseEvent e)
{
    if(this->mode == EDIT_MODE)
    {
        _nodeToMove = nullptr;
    }

}

void Web::Ui::WindowImageEdit::_onMouseDraggedEvent(WMouseEvent e)
{
    if(_myImageManagerWt->isImageOpened())
    {
        if(this->mode == EDIT_MODE)
        {
            if(_nodeToMove)
                *_nodeToMove = {e.widget().x, e.widget().y};
            redrawWImage();
        }
    }
}

void Web::Ui::WindowImageEdit::_initializeMouseControl()
{
    _myWImage->mouseWentDown().connect(this, &WindowImageEdit::_onMouseWentDownEvent);
    _myWImage->mouseWentUp().connect(this, &WindowImageEdit::_onMouseWentUpEvent);
    _myWImage->mouseDragged().connect(this, &WindowImageEdit::_onMouseDraggedEvent);
    //        _myWImage->mouseWheel().connect(std::bind([=] (WMouseEvent &e) {
    //            _changeZoom(_myWSliderZoom->value() + e.wheelDelta());
    //        },std::placeholders::_1));
}

Web::Ui::WindowImageEdit::WindowImageEdit(WContainerWidget *parent) : WContainerWidget(parent)
{

    _myImageManagerWt = new ImageManagerWt(this);

    _myWTable = new WTable(this);

    _onLoadPrepareImageArea();

    _onLoadPrepareToolbar();  

//    _onLoadPrepareImageUploader(); // see _onLoadPrepareToolbar();

    _onLoadPrepareFooter();

    _onLoadPrepareZoomSlider();
    _onLoadPrepareTransparencySlider();

    _initializeMouseControl();

    _disableUI();
}

Web::Ui::WindowImageEdit::~WindowImageEdit()
{
    for(auto p = polygons.begin(); p!= polygons.end(); ++p)
        p->clear();
    polygons.clear();
    rulerPoints.clear();
}
