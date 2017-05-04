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
#include <Wt/WScrollArea>
#include <Wt/WImage>
#include <Wt/WSlider>
#include <Wt/WPanel>
#include <Wt/WText>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WBorderLayout>
#include <Wt/WStackedWidget>

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

    WToolBar    *_myToolBar;

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
    WScrollArea *_myWImageScrollArea;

    WSlider     *_myWSliderZoom;
    WText       *_myWSliderZoomText;
    WSlider     *_myWSliderTransparency;
    WText       *_myWSliderTransparencyText;

    WPanel      *_myProgressBar;
    WText       *_myProgressBarText;

    public : void redrawWImage()
    {
        _myImageManagerWt->updateWMemoryResource();
        _myWImage->setResource(_myImageManagerWt->myWMemoryResource);
        _myWImage->resize(WLength::Auto,WLength::Auto);
        _myWImageScrollArea->resize(WLength::Auto,WLength(300));
    }

    public : WindowImageEdit(WContainerWidget *parent) : WContainerWidget(parent)
    {
        _myImageManagerWt = new ImageManagerWt(this);

        this->setOverflow(OverflowHidden);
        WVBoxLayout *mainLayout = new WVBoxLayout(this);

        _myToolBar = new Wt::WToolBar(this);

        _myOpenButton = new  WPushButton("Open", this);
        _myOpenButton->setIcon(WLink("icons/Open.png"));
        _myToolBar->addButton(_myOpenButton);

        _mySaveButton = new  WPushButton("Save", this);
        _mySaveButton->setIcon(WLink("icons/Save.png"));
        _myToolBar->addButton(_mySaveButton);

        _myExportResultsButton = new  WPushButton("Export results", this);
        _myExportResultsButton->setIcon(WLink("icons/Export.png"));
        _myToolBar->addButton(_myExportResultsButton);

        _myToolBar->addSeparator();

        _myPolygonButton = new  WPushButton("Polygon", this);
        _myPolygonButton->setIcon(WLink("icons/Polygon.png"));
        _myToolBar->addButton(_myPolygonButton);

        _myRulerButton = new  WPushButton("Ruler", this);
        _myRulerButton->setIcon(WLink("icons/Ruler.png"));
        _myToolBar->addButton(_myRulerButton);

        _myClearButton = new  WPushButton("Clear", this);
        _myClearButton->setIcon(WLink("icons/Clear.png"));
        _myToolBar->addButton(_myClearButton);

        _myToolBar->addSeparator();

        _myZoomInButton = new  WPushButton("Zoom-In", this);
        _myZoomInButton->setIcon(WLink("icons/Zoom-In.png"));
        _myToolBar->addButton(_myZoomInButton);

        _myZoomOutButton = new  WPushButton("Zoom-Out", this);
        _myZoomOutButton->setIcon(WLink("icons/Zoom-Out.png"));
        _myToolBar->addButton(_myZoomOutButton);

        _myToolBar->addSeparator();

        _myConfigureButton = new  WPushButton("Configure", this);
        _myConfigureButton->setIcon(WLink("icons/Configure.png"));
        _myToolBar->addButton(_myConfigureButton);

        _myToolBar->setCompact(true);
        _myToolBar->setOrientation(Horizontal);

        mainLayout->addWidget(_myToolBar);

        _myWFileUpload = new WFileUpload(this);
        _myWFileUpload->setFilters("image/*");
        _myWFileUpload->setProgressBar(new WProgressBar(this));

        // Upload automatically when the user entered a file.
        _myWFileUpload->changed().connect(std::bind([=] () {
            _myWFileUpload->upload();
        }));

        // React to a succesfull upload.
        _myWFileUpload->uploaded().connect(std::bind([=] () {
            _myImageManagerWt->openImage(_myWFileUpload->spoolFileName());
            redrawWImage();
        }));

        // React to a file upload problem.
        _myWFileUpload->fileTooLarge().connect(std::bind([=] () {
            mainLayout->addWidget(new WText("Some error!!!",this));
            /// \todo
        }));

        mainLayout->addWidget(_myWFileUpload);

        _myWImage = new WImage(this);
        _myWImageScrollArea = new WScrollArea(this);
        _myWImageScrollArea->setWidget(_myWImage);
        _myWImageScrollArea->setScrollBarPolicy(WScrollArea::ScrollBarAsNeeded);

        mainLayout->addWidget(_myWImageScrollArea,1);

        _myWSliderZoom = new WSlider(this);
        _myWSliderZoomText = new WText("100%",this);
        _myWSliderZoom->setMinimum(10);
        _myWSliderZoom->setMaximum(400);
        _myWSliderZoom->setValue(100);
        _myWSliderZoom->valueChanged().connect(std::bind([=] () {
            if(_myImageManagerWt->isImageOpened())
            {
                _myImageManagerWt->zoom(_myWSliderZoom->value());
                redrawWImage();
            }
            _myWSliderZoomText->setText(_myWSliderZoom->valueText() + "%");
        }));
        WHBoxLayout *zoomSliderLayout = new WHBoxLayout();
        zoomSliderLayout->addWidget(_myWSliderZoom,1);
        zoomSliderLayout->addWidget(_myWSliderZoomText);

        mainLayout->addLayout(zoomSliderLayout);

        _myWSliderTransparency = new WSlider(this);
        _myWSliderTransparency->setMinimum(10);
        _myWSliderTransparency->setMaximum(90);
        _myWSliderTransparency->setValue(50);
        _myWSliderTransparencyText = new WText("50%",this);
        WHBoxLayout *transparencySliderLayout = new WHBoxLayout();
        transparencySliderLayout->addWidget(_myWSliderTransparency,1);
        transparencySliderLayout->addWidget(_myWSliderTransparencyText);

        mainLayout->addLayout(transparencySliderLayout);

        _myProgressBar = new WPanel(this);
        _myProgressBarText = new WText("TEST STRING",this);
        _myProgressBar->setCentralWidget(_myProgressBarText);
        mainLayout->addWidget(_myProgressBar);
    }
    public : ~WindowImageEdit(){}
};
}
}

#endif // WINDOWIMAGEEDIT

