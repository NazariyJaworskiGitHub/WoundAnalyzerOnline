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
#include <Wt/WFileDropWidget>
#include <Wt/WImage>
#include <Wt/WSlider>
#include <Wt/WPanel>
#include <Wt/WText>
#include <Wt/WLabel>

#include <Wt/WTable>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WBorderLayout>
#include <Wt/WStackedWidget>

#include <Image/imagemanagerwt.h>
#include <Web/UI/dialogprogressbar.h>

#define MIN_IMAGE_SIZE "400px"

#define ZOOM_MIN            10
#define ZOOM_MAX            400
#define ZOOM_INIT           100
#define ZOOM_BUTTON_STEP    10

#define TRANSP_MIN          10
#define TRANSP_MAX          90
#define TRANSP_INIT         50
#define TRANSP_BUTTON_STEP  10

using namespace Wt;

namespace Web
{
namespace Ui
{
class WindowImageEdit : public WContainerWidget
{
    private:
    ImageManagerWt  *_myImageManagerWt;

    WTable          *_myWTable;

    WToolBar        *_myHeaderToolBar;

    WPushButton     *_myOpenButton;
    WPushButton     *_mySaveButton;
    WPushButton     *_myExportButton;

    WPushButton     *_myPolygonButton;
    WPushButton     *_myRulerButton;
    WPushButton     *_myClearButton;

    WPushButton     *_myZoomInButton;
    WPushButton     *_myZoomOutButton;

    WPushButton     *_myTransparencyUpButton;
    WPushButton     *_myTransparencyDownButton;

    WPushButton     *_myConfigureButton;

    WFileUpload     *_myWFileUpload;
    WFileDropWidget *_myWFileDrop;
    Web::Ui::DialogProgressbar *_myProgressBarDialog;

    WImage          *_myWImage;
    WScrollArea     *_myWImageScrollArea;

    WContainerWidget*_myFooterToolBar;

    WLabel          *_myWSliderZoomLabel;
    WSlider         *_myWSliderZoom;
    WText           *_myWSliderZoomText;

    WLabel          *_myWSliderTransparencyLabel;
    WSlider         *_myWSliderTransparency;
    WText           *_myWSliderTransparencyText;

    WPanel          *_myProgressBar;
    WText           *_myProgressBarText;

    WVBoxLayout     *_mainLayout;

    public : void redrawWImage()
    {
        _myImageManagerWt->updateWMemoryResource();
        _myWImage->setResource(_myImageManagerWt->myWMemoryResource);
        _myWImage->resize(_myImageManagerWt->getZoomedWidth(),_myImageManagerWt->getZoomedHeight());
        _myWImage->setMaximumSize(_myImageManagerWt->getZoomedWidth(),_myImageManagerWt->getZoomedHeight());
        //_myWImage->resize(WLength::Auto,WLength::Auto);
        //_myWImageScrollArea->resize(_myImageManagerWt->getZoomedWidth(),_myImageManagerWt->getZoomedHeight());
    }

    private : void _onLoadPrepareToolbar()
    {
        _myHeaderToolBar = new WToolBar(this);
        //_myHeaderToolBar->setPositionScheme(Absolute);

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
        _myHeaderToolBar->addButton(_myPolygonButton);

        _myRulerButton = new  WPushButton("Ruler", this);
        _myRulerButton->setIcon(WLink("icons/Ruler.png"));
        _myHeaderToolBar->addButton(_myRulerButton);

        _myClearButton = new  WPushButton("Clear", this);
        _myClearButton->setIcon(WLink("icons/Clear.png"));
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
        //_myWTable->elementAt(0, 0)->setPositionScheme(Fixed);
        //_myWTable->elementAt(0, 0)->resize(WLength("100%"),WLength("100%"));
        //_myWTable->elementAt(0, 0)->setMaximumSize(WLength("100%"),WLength("100%"));
    }

    private: void _onLoadPrepareImageArea()
    {
        _myWImage = new WImage(this);
//        _myWImageScrollArea = new WScrollArea(this);
//        _myWImageScrollArea->setWidget(_myWImage);
//        _myWImageScrollArea->setScrollBarPolicy(WScrollArea::ScrollBarAlwaysOn);
//        _myWImageScrollArea->setPositionScheme(Absolute);
        //_myWImage->setPositionScheme(Absolute);

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
            //_myWFileDrop->hide();

            _myImageManagerWt->openImage(_myWFileUpload->spoolFileName());
            redrawWImage();

            _myWImage->show();
        }));

        // React to a file upload problem.
        _myWFileUpload->fileTooLarge().connect(std::bind([=] () {
            Log::GlobalLogger.msg(Log::ERROR,"[Image upload] fileTooLarge() signal\n");
        }));

//        // Upload automatically when the user droped a file.
//        _myWFileDrop = new WFileDropWidget(this);
//        _myWFileDrop->drop().connect(std::bind([=] (
//                    const std::vector<WFileDropWidget::File*>& files) {
//            for (unsigned i=1; i < files.size(); i++)
//                _myWFileDrop->cancelUpload(files[i]);
//            _myWFileDrop->setAcceptDrops(false);
//            _myProgressBarDialog->show();
//        }, std::placeholders::_1));

//        // React to a succesfull upload.
//        //_myWFileDrop->uploaded().connect(_myProgressBarDialog->accept);
//        _myWFileDrop->uploaded().connect(std::bind([=] (WFileDropWidget::File* file) {
//            _myWFileUpload->hide();
//            _myWFileDrop->hide();

//            _myImageManagerWt->openImage(file->uploadedFile().spoolFileName());
//            redrawWImage();

//            _myWImage->show();
//        }, std::placeholders::_1));

//        // React to a file upload problem.
//        _myWFileDrop->tooLarge().connect(std::bind([=] (WFileDropWidget::File *file) {
//            Log::GlobalLogger.msg(Log::ERROR,"[Image upload] fileTooLarge() signal\n");
//        }, std::placeholders::_1));

//        _myWFileDrop->uploadFailed().connect(std::bind([=] (WFileDropWidget::File *file) {
//            Log::GlobalLogger.msg(Log::ERROR,"[Image upload] uploadFailed() signal\n");
//        }, std::placeholders::_1));

        _myWTable->elementAt(2, 0)->addWidget(_myWFileUpload);
//        _myWTable->elementAt(2, 0)->addWidget(_myWFileDrop);
        //_myWTable->elementAt(2, 0)->addWidget(_myWImageScrollArea);
        _myWTable->elementAt(2, 0)->addWidget(_myWImage);
        //_myWTable->elementAt(2, 0)->setPositionScheme(Absolute);
        _myWTable->elementAt(2, 0)->setOverflow(OverflowScroll);
        _myWTable->elementAt(2, 0)->resize(WLength("100%"),WLength("100%"));
        //_myWTable->elementAt(2, 0)->setMaximumSize(WLength("100%"),WLength("100%"));
        _myWTable->elementAt(2, 0)->setMinimumSize(WLength(MIN_IMAGE_SIZE),WLength(MIN_IMAGE_SIZE));

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
        //_myWTable->elementAt(3, 0)->setPositionScheme(Relative);
        //_myWTable->elementAt(3, 0)->setOffsets(WLength("0px"),Bottom);
        //_myWTable->elementAt(3, 0)->resize(WLength("100%"),WLength("100%"));
        //_myWTable->elementAt(3, 0)->setMaximumSize(WLength("100%"),WLength("100%"));
    }

    private: void _onLoadPrepareZoomSlider()
    {
        _myWSliderZoom->setMinimum(ZOOM_MIN);
        _myWSliderZoom->setMaximum(ZOOM_MAX);
        _myWSliderZoom->valueChanged().connect(this,&WindowImageEdit::_onZoomSliderValueChanged);
        _myWSliderZoom->setValue(ZOOM_INIT);
        _myZoomInButton->clicked().connect(std::bind([=] () {
                _myWSliderZoom->setValue(_myWSliderZoom->value() + ZOOM_BUTTON_STEP);
                _onZoomSliderValueChanged(_myWSliderZoom->value());
        }));
        _myZoomOutButton->clicked().connect(std::bind([=] () {
                _myWSliderZoom->setValue(_myWSliderZoom->value() - ZOOM_BUTTON_STEP);
                _onZoomSliderValueChanged(_myWSliderZoom->value());
        }));
    }

    private: void _onZoomSliderValueChanged(int value)
    {
        if(_myImageManagerWt->isImageOpened())
        {
            _myImageManagerWt->zoom(value);
            redrawWImage();
        }
        _myWSliderZoomText->setText(std::to_string(value) + "%");
    }

    private: void _onLoadPrepareTransparencySlider()
    {
        _myWSliderTransparency->setMinimum(TRANSP_MIN);
        _myWSliderTransparency->setMaximum(TRANSP_MAX);
        _myWSliderTransparency->valueChanged().connect(this,&WindowImageEdit::_onTransparencySliderValueChanged);
        _myWSliderTransparency->setValue(TRANSP_INIT);
        _myTransparencyUpButton->clicked().connect(std::bind([=] () {
                _myWSliderTransparency->setValue(_myWSliderTransparency->value() + TRANSP_BUTTON_STEP);
                _onTransparencySliderValueChanged(_myWSliderTransparency->value());
        }));
        _myTransparencyDownButton->clicked().connect(std::bind([=] () {
                _myWSliderTransparency->setValue(_myWSliderTransparency->value() - TRANSP_BUTTON_STEP);
                _onTransparencySliderValueChanged(_myWSliderTransparency->value());
        }));

    }

    private: void _onTransparencySliderValueChanged(int value)
    {
        if(_myImageManagerWt->isImageOpened())
        {
            _myImageManagerWt->setDrawingLayerTransparency(value/100.0);
            redrawWImage();
        }
        _myWSliderTransparencyText->setText(std::to_string(value) + "%");
    }

    public :WindowImageEdit(WContainerWidget *parent) : WContainerWidget(parent)
    {
        //this->resize(WLength("800px"),WLength("600px"));
        //this->resize(WLength("800px"),WLength("600px"));

        _myImageManagerWt = new ImageManagerWt(this);

        _myWTable = new WTable(this);

        _onLoadPrepareToolbar();
        _onLoadPrepareImageArea();
        _onLoadPrepareFooter();

        _onLoadPrepareZoomSlider();
        _onLoadPrepareTransparencySlider();
    }
    public : ~WindowImageEdit(){}
};
}
}

#endif // WINDOWIMAGEEDIT

