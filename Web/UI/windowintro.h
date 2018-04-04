#ifndef WINDOWINTRO
#define WINDOWINTRO

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <Wt/WContainerWidget>
#include <Wt/WNavigationBar>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WTimer>

#include <Web/UI/authmanager.h>

/// \todo
#define _LINK_ "localhost:8080"

using namespace Wt;

namespace Web
{
namespace Ui
{
class WindowIntro : public WContainerWidget
{
    public : std::vector<WImage*> myIntroImages;
    private: size_t _imageIndexToShow = 0;
    public : WTimer *hideIntroImageTimer = nullptr;
    public : WTimer *showInroImageTimer = nullptr;
    public : int introDuration = 10000;
    public : int introAnimationDuration = 1000;
    public : WindowIntro(WContainerWidget *parent) : WContainerWidget(parent)
    {
        //intro pictures should be named "image_#.jpg"
        for(int i=1;; ++i)
        {
            std::string str = "images/intro_"+std::to_string(i)+".jpg";
            FILE *fp;
            if(!(fp = fopen(str.data(),"r"))) break;
            fclose(fp);
            myIntroImages.push_back(new WImage(WLink(str),this));
            myIntroImages.back()->setAttributeValue("width","100%");
            myIntroImages.back()->setAttributeValue("height","auto");
            myIntroImages.back()->hide();
        }
        myIntroImages.front()->animateShow(
                    WAnimation(WAnimation::Fade, WAnimation::Linear, introAnimationDuration));
        _imageIndexToShow = 0;
        hideIntroImageTimer = new WTimer(this);
        hideIntroImageTimer->setInterval(introDuration);
        hideIntroImageTimer->timeout().connect(std::bind([=]()
        {
            myIntroImages[_imageIndexToShow]->animateHide(
                        WAnimation(WAnimation::Fade, WAnimation::Linear, introAnimationDuration));
            if(_imageIndexToShow+1<myIntroImages.size())
                ++_imageIndexToShow;
            else _imageIndexToShow = 0;
            showInroImageTimer->start();
            hideIntroImageTimer->stop();
        }));
        showInroImageTimer = new WTimer(this);
        showInroImageTimer->setInterval(introAnimationDuration);
        showInroImageTimer->timeout().connect(std::bind([=]()
        {
            myIntroImages[_imageIndexToShow]->animateShow(
                        WAnimation(WAnimation::Fade, WAnimation::Linear, introAnimationDuration));
            showInroImageTimer->stop();
            hideIntroImageTimer->start();
        }));
        hideIntroImageTimer->start();
    }
};
}
}

#endif // WINDOWINTRO

