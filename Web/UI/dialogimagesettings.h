#ifndef DIALOGIMAGESETTINGS_H
#define DIALOGIMAGESETTINGS_H

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <Wt/WDialog>
#include <Wt/WBreak>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WCssDecorationStyle>

#include "Web/UI/windowimageedit.h"

#define COLOR_BUTTON_WIDTH  50
#define COLOR_BUTTON_HEIGHT 20

using namespace Wt;

namespace Web
{
namespace Ui
{

class DialogImageSettings : public WDialog
{
    private: WindowImageEdit *_parent;

    WPushButton *_polygonEdgeColorButton    = nullptr;
    WPushButton *_polygonColorButton        = nullptr;
    WPushButton *_polygonTextColorButton    = nullptr;
    WPushButton *_rulerColorButton          = nullptr;
    WPushButton *_rulerTextButton           = nullptr;
    WPushButton *_applyButton               = nullptr;

    public: DialogImageSettings(WindowImageEdit *parent):
        _parent(parent),
        WDialog((WObject*)parent)
    {
        _polygonEdgeColorButton = new WPushButton(this->contents());
        _polygonEdgeColorButton->setThemeStyleEnabled(false);
        _polygonEdgeColorButton->decorationStyle().setBackgroundColor(WColor(_parent->polygonEdgeColor[0],_parent->polygonEdgeColor[1],_parent->polygonEdgeColor[2],_parent->polygonEdgeColor[3]));
        _polygonEdgeColorButton->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT));
        _polygonEdgeColorButton->setValueText("<input type=""color"">");
        this->contents()->addWidget(new WLabel("Polygon edge color"));
        this->contents()->addWidget(new WBreak());

        _polygonColorButton = new WPushButton(this->contents());
        _polygonColorButton->setThemeStyleEnabled(false);
        _polygonColorButton->decorationStyle().setBackgroundColor(WColor(_parent->polygonColor[0],_parent->polygonColor[1],_parent->polygonColor[2],_parent->polygonColor[3]));
        _polygonColorButton->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT));
        this->contents()->addWidget(new WLabel("Polygon color"));
        this->contents()->addWidget(new WBreak());

        _polygonTextColorButton = new WPushButton(this->contents());
        _polygonTextColorButton->setThemeStyleEnabled(false);
        _polygonTextColorButton->decorationStyle().setBackgroundColor(WColor(_parent->polygonTextColor[0],_parent->polygonTextColor[1],_parent->polygonTextColor[2],_parent->polygonTextColor[3]));
        _polygonTextColorButton->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT));
        this->contents()->addWidget(new WLabel("Polygon text color"));
        this->contents()->addWidget(new WBreak());

        _rulerColorButton = new WPushButton(this->contents());
        _rulerColorButton->setThemeStyleEnabled(false);
        _rulerColorButton->decorationStyle().setBackgroundColor(WColor(_parent->rulerColor[0],_parent->rulerColor[1],_parent->rulerColor[2],_parent->rulerColor[3]));
        _rulerColorButton->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT));
        this->contents()->addWidget(new WLabel("Ruler color"));
        this->contents()->addWidget(new WBreak());

        _rulerTextButton = new WPushButton(this->contents());
        _rulerTextButton->setThemeStyleEnabled(false);
        _rulerTextButton->decorationStyle().setBackgroundColor(WColor(_parent->rulerTextColor[0],_parent->rulerTextColor[1],_parent->rulerTextColor[2],_parent->rulerTextColor[3]));
        _rulerTextButton->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT));
        this->contents()->addWidget(new WLabel("Ruler text color"));
        this->contents()->addWidget(new WBreak());

        _applyButton = new WPushButton("Apply", this->contents());
        _applyButton->setFloatSide(Side::Right);
        _applyButton->setStyleClass("btn-primary");
        _applyButton->clicked().connect(this, &accept);
    }
    public: ~DialogImageSettings(){}
};
}
}

//namespace Ui {
//class SettingsWidget;
//}

//// Interact with ImageInterface through parent()->ui->label
//class SettingsWidget : public QDialog
//{
//    Q_OBJECT

//public:
//    explicit SettingsWidget(ImageInterface* ptr, QWidget *parent = 0);
//    ~SettingsWidget();

//private: ImageInterface* _ptr = nullptr;
//private: void _colorUpdate(QColor &_refToParentColor, QPushButton *_refToWidgetButton);

//private Q_SLOTS:
//    void on_polygonEdge_clicked();

//    void on_polygonColor_clicked();

//    void on_rulerColor_clicked();

//    void on_rulerNodesColor_clicked();

//    void on_polygonEdgeThickness_valueChanged(int arg1);

//    void on_rulerThickness_valueChanged(int arg1);

//    void on_polygonTextColor_clicked();

//    void on_rulerTextColor_clicked();

//private:
//    Ui::SettingsWidget *ui;
//};


#endif // DIALOGIMAGESETTINGS_H
