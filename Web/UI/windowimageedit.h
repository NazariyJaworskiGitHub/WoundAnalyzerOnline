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
#include <Wt/WProgressBar>
#include <Wt/WFileUpload>
#include <Wt/WImage>
#include <Wt/WSlider>
#include <Wt/WText>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WBorderLayout>

#include <Image/imagemanagerwt.h>

using namespace Wt;

namespace Web
{
namespace Ui
{
class WindowImageEdit : public WContainerWidget
{
    private:
    ImageManagerWt *_myImageManagerWt;

    WToolBar    *_myFileToolBar;
    WToolBar    *_myEditToolBar;
    WToolBar    *_myViewToolBar;
    WToolBar    *_myConfToolBar;

    WPushButton *_myOpenButton;
    WPushButton *_mySaveButton;
    WPushButton *_myExportResultsButton;

    WPushButton *_myPolygonButton;
    WPushButton *_myRulerButton;
    WPushButton *_myClearButton;

    WPushButton *_myZoomInButton;
    WPushButton *_myZoomOutButton;

    WPushButton *_myConfigureButton;

    WFileUpload *_myWFileUpload;
    WImage      *_myWImage;

    WSlider     *_myWSliderZoom;
    WText       *_myWSliderZoomText;
    WSlider     *_myWSliderTransparency;

    WText       *_myProgressBar;

    public : WindowImageEdit(WContainerWidget *parent) : WContainerWidget(parent)
    {
        _myImageManagerWt = new ImageManagerWt(this);

        WBorderLayout *mainLayout = new WBorderLayout();
        WHBoxLayout *toolBarLayout = new WHBoxLayout();

        _myFileToolBar = new Wt::WToolBar(this);
        _myEditToolBar = new Wt::WToolBar(this);
        _myViewToolBar = new Wt::WToolBar(this);
        _myConfToolBar = new Wt::WToolBar(this);

        _myOpenButton = new  WPushButton("Open", this);
        _myOpenButton->setIcon(WLink("icons/Open.png"));
        _myFileToolBar->addButton(_myOpenButton);

        _mySaveButton = new  WPushButton("Save", this);
        _mySaveButton->setIcon(WLink("icons/Save.png"));
        _myFileToolBar->addButton(_mySaveButton);

        _myExportResultsButton = new  WPushButton("Export results", this);
        _myExportResultsButton->setIcon(WLink("icons/Export.png"));
        _myFileToolBar->addButton(_myExportResultsButton);

        toolBarLayout->addWidget(_myFileToolBar);

        _myPolygonButton = new  WPushButton("Polygon", this);
        _myPolygonButton->setIcon(WLink("icons/Polygon.png"));
        _myEditToolBar->addButton(_myPolygonButton);

        _myRulerButton = new  WPushButton("Ruler", this);
        _myRulerButton->setIcon(WLink("icons/Ruler.png"));
        _myEditToolBar->addButton(_myRulerButton);

        _myClearButton = new  WPushButton("Clear", this);
        _myClearButton->setIcon(WLink("icons/Clear.png"));
        _myEditToolBar->addButton(_myClearButton);

        toolBarLayout->addWidget(_myEditToolBar);

        _myZoomInButton = new  WPushButton("Zoom-In", this);
        _myZoomInButton->setIcon(WLink("icons/Zoom-In.png"));
        _myViewToolBar->addButton(_myZoomInButton);

        _myZoomOutButton = new  WPushButton("Zoom-Out", this);
        _myZoomOutButton->setIcon(WLink("icons/Zoom-Out.png"));
        _myViewToolBar->addButton(_myZoomOutButton);

        toolBarLayout->addWidget(_myViewToolBar);

        _myConfigureButton = new  WPushButton("Configure", this);
        _myConfigureButton->setIcon(WLink("icons/Configure.png"));
        _myConfToolBar->addButton(_myConfigureButton);

        toolBarLayout->addWidget(_myConfToolBar);
        mainLayout->add(toolBarLayout, WBorderLayout::North);
        //mainLayout->addWidget(new WBreak(this));

        WVBoxLayout *centerLayout = new WVBoxLayout();
        _myWFileUpload = new WFileUpload(this);
        _myWFileUpload->setProgressBar(new WProgressBar(this));

        // Upload automatically when the user entered a file.
        _myWFileUpload->changed().connect(std::bind([=] () {
            _myWFileUpload->upload();
        }));

        // React to a succesfull upload.
        _myWFileUpload->uploaded().connect(std::bind([=] () {
            _myImageManagerWt->openImage(_myWFileUpload->spoolFileName());
            _myImageManagerWt->updateWMemoryResource();
            _myWImage->setResource(_myImageManagerWt->myWMemoryResource);
            _myWImage->resize(_myImageManagerWt->getWidth(),_myImageManagerWt->getHeight());
        }));

        // React to a file upload problem.
        _myWFileUpload->fileTooLarge().connect(std::bind([=] () {
            /// \todo
        }));

        centerLayout->addWidget(_myWFileUpload);

        _myWImage = new WImage(this);
        centerLayout->addWidget(_myWImage);
        mainLayout->add(centerLayout, WBorderLayout::Center);

        WVBoxLayout *southLayout = new WVBoxLayout();
        _myWSliderZoom = new WSlider(this);
        _myWSliderZoomText = new WText("100%",this);
        _myWSliderZoom->setMinimum(10);
        _myWSliderZoom->setMaximum(100);
        _myWSliderZoom->setValue(100);
        _myWSliderZoom->valueChanged().connect(std::bind([=] () {
            double delta = _myWSliderZoom->value()/100.0 - _myImageManagerWt->zoomFactor;
            _myImageManagerWt->zoomFactor += delta;

            _myImageManagerWt->cleanDrawingLayer();
            _myImageManagerWt->updateWMemoryResource();
            /// \todo

            _myWImage->setResource(_myImageManagerWt->myWMemoryResource);
            _myWImage->resize(_myImageManagerWt->getWidth(),_myImageManagerWt->getHeight());
            _myWSliderZoomText->setText(_myWSliderZoom->valueText() + "%");
        }));
        southLayout->addWidget(_myWSliderZoom,1);
        southLayout->addWidget(_myWSliderZoomText);

        _myWSliderTransparency  = new WSlider(this);
        southLayout->addWidget(_myWSliderTransparency);

        _myProgressBar = new WText("TEST STRING",this);
        southLayout->addWidget(_myProgressBar);
        mainLayout->add(southLayout, WBorderLayout::South);

        setLayout(mainLayout);
    }
    public : ~WindowImageEdit(){}
};
}
}

#endif // WINDOWIMAGEEDIT

