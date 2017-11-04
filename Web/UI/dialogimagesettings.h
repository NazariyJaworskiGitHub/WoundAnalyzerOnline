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
#include <Wt/WSpinBox>
#include <Wt/WCssDecorationStyle>
#include <Wt/WJavaScript>

#include "Web/UI/windowimageedit.h"

#define COLOR_BUTTON_WIDTH  70
#define COLOR_BUTTON_HEIGHT 25

using namespace Wt;

namespace Web
{
namespace Ui
{

void wcolorToOCVColor(const WColor &wcol, Color &col)
{
    col[0] = wcol.blue();
    col[1] = wcol.green();
    col[2] = wcol.red();
    col[3] = wcol.alpha();
}

std::string OCVColorToCSSColor(const Color &col)
{
    std::string str;
    str.resize(8); // #rrggbb+0
    std::sprintf(&str[0],"#%02x%02x%02x",(unsigned int)col[2],(unsigned int)col[1],(unsigned int)col[0]);
    return str;
}

class DialogImageSettings : public WDialog
{
    private: WindowImageEdit *_parent       = nullptr;  

    public: DialogImageSettings(WindowImageEdit *parent):
        WDialog((WObject*)parent),
        _parent(parent)
    {
        this->setWindowTitle("Image tools settings");

        // Wt hasn't own color picker implementation, so i've changed the line edit with input
        // "text" implementation to input "color" implementation
        WLineEdit *_polygonEdgeColorPicker = new WLineEdit();
        _polygonEdgeColorPicker->setAttributeValue("type", """color""");
        _polygonEdgeColorPicker->setAttributeValue("value", OCVColorToCSSColor(_parent->polygonEdgeColor));
        _polygonEdgeColorPicker->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT));
        _polygonEdgeColorPicker->changed().connect(std::bind([=] () {
            wcolorToOCVColor(WColor(_polygonEdgeColorPicker->valueText()),_parent->polygonEdgeColor);
            _parent->redrawWImage();
        }));
        this->contents()->addWidget(_polygonEdgeColorPicker);
        this->contents()->addWidget(new WLabel("Polygon edge color"));
        this->contents()->addWidget(new WBreak());

        WLineEdit *_polygonColorPicker = new WLineEdit();
        _polygonColorPicker->setAttributeValue("type", """color""");
        _polygonColorPicker->setAttributeValue("value", OCVColorToCSSColor(_parent->polygonColor));
        _polygonColorPicker->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT));
        _polygonColorPicker->changed().connect(std::bind([=] () {
            wcolorToOCVColor(WColor(_polygonColorPicker->valueText()),_parent->polygonColor);
            _parent->redrawWImage();
        }));
        this->contents()->addWidget(_polygonColorPicker);
        this->contents()->addWidget(new WLabel("Polygon color"));
        this->contents()->addWidget(new WBreak());

        WLineEdit *_polygonTextColorPicker = new WLineEdit();
        _polygonTextColorPicker->setAttributeValue("type", """color""");
        _polygonTextColorPicker->setAttributeValue("value", OCVColorToCSSColor(_parent->polygonTextColor));
        _polygonTextColorPicker->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT));
        _polygonTextColorPicker->changed().connect(std::bind([=] () {
            wcolorToOCVColor(WColor(_polygonTextColorPicker->valueText()),_parent->polygonTextColor);
            _parent->redrawWImage();
        }));
        this->contents()->addWidget(_polygonTextColorPicker);
        this->contents()->addWidget(new WLabel("Polygon text color"));
        this->contents()->addWidget(new WBreak());

        WSpinBox *_polygonEdgeThicknessSpinBox = new WSpinBox();
        _polygonEdgeThicknessSpinBox->setValue(_parent->polygonEdgeThickness);
        _polygonEdgeThicknessSpinBox->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT-4));//padding and margins
        _polygonEdgeThicknessSpinBox->setRange(1,THICKNESS_MAX);
        _polygonEdgeThicknessSpinBox->valueChanged().connect(std::bind([=] (int value) {
            _parent->polygonEdgeThickness = value;
            _parent->redrawWImage();
        },std::placeholders::_1));
        this->contents()->addWidget(_polygonEdgeThicknessSpinBox);
        this->contents()->addWidget(new WLabel("Polygon edge thickness"));
        this->contents()->addWidget(new WBreak());

        WLineEdit *_rulerColorButton = new WLineEdit();
        _rulerColorButton->setAttributeValue("type", """color""");
        _rulerColorButton->setAttributeValue("value", OCVColorToCSSColor(_parent->rulerColor));
        _rulerColorButton->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT));
        _rulerColorButton->changed().connect(std::bind([=] () {
            wcolorToOCVColor(WColor(_rulerColorButton->valueText()),_parent->rulerColor);
            _parent->redrawWImage();
        }));
        this->contents()->addWidget(_rulerColorButton);
        this->contents()->addWidget(new WLabel("Ruler color"));
        this->contents()->addWidget(new WBreak());

        WLineEdit *_rulerTextButton = new WLineEdit();
        _rulerTextButton->setAttributeValue("type", """color""");
        _rulerTextButton->setAttributeValue("value", OCVColorToCSSColor(_parent->rulerTextColor));
        _rulerTextButton->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT));
        _rulerTextButton->changed().connect(std::bind([=] () {
            wcolorToOCVColor(WColor(_rulerTextButton->valueText()),_parent->rulerTextColor);
            _parent->redrawWImage();
        }));
        this->contents()->addWidget(_rulerTextButton);
        this->contents()->addWidget(new WLabel("Ruler text color"));
        this->contents()->addWidget(new WBreak());

        WSpinBox *_rulerThicknessSpinBox = new WSpinBox();
        _rulerThicknessSpinBox->setValue(_parent->rulerThickness);
        _rulerThicknessSpinBox->resize(WLength(COLOR_BUTTON_WIDTH),WLength(COLOR_BUTTON_HEIGHT-4));//padding and margins
        _rulerThicknessSpinBox->setRange(1,THICKNESS_MAX);
        _rulerThicknessSpinBox->setMargin(0);
        _rulerThicknessSpinBox->valueChanged().connect(std::bind([=] (int value) {
            _parent->rulerThickness = value;
            _parent->redrawWImage();
        },std::placeholders::_1));
        this->contents()->addWidget(_rulerThicknessSpinBox);
        this->contents()->addWidget(new WLabel("Ruler edge thickness"));
        this->contents()->addWidget(new WBreak());

        WPushButton *_applyButton = new WPushButton("Apply", this->contents());
        _applyButton->setFloatSide(Side::Right);
        _applyButton->setStyleClass("btn-primary");
        _applyButton->clicked().connect(this, &DialogImageSettings::accept);
    }
    public: ~DialogImageSettings(){}
};
}
}

#endif // DIALOGIMAGESETTINGS_H
