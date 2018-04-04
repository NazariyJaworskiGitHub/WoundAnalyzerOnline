#ifndef MAINNWINDOW
#define MAINNWINDOW

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

#include "Web/UI/windowintro.h"
#include "Web/UI/windowimageedit.h"

/// \todo
#define _LINK_ "localhost:8080"

using namespace Wt;

namespace Web
{
namespace Ui
{
class MainWindow : public WContainerWidget
{
    public : WNavigationBar *myWNavigationBar = nullptr;
    public : WContainerWidget *mainContainer = nullptr;

    public : WPushButton *intoPushButton = nullptr;
    public : WindowIntro *windowIntro = nullptr;

    public : WPushButton *loginPushButton = nullptr;
    public : WindowImageEdit *windowImageEdit = nullptr;

    public : MainWindow(WContainerWidget *parent) : WContainerWidget(parent)
    {
        myWNavigationBar = new WNavigationBar(this);
        myWNavigationBar->setResponsive(true);
        myWNavigationBar->setMargin(0);

        mainContainer = new WContainerWidget(this);

        intoPushButton = new WPushButton(this);
        intoPushButton->setIcon(WLink("icons/MainIcon.png"));
        intoPushButton->setToolTip("Intro page");
        intoPushButton->clicked().connect(std::bind([=](){
            for(auto child : mainContainer->children())
                delete child;
            windowIntro = new WindowIntro(mainContainer);
        }));
        myWNavigationBar->addWidget(intoPushButton, AlignLeft);

        loginPushButton = new WPushButton(this);
        loginPushButton->setIcon(WLink("icons/Log-In.png"));
        loginPushButton->setToolTip("Log in");
        loginPushButton->clicked().connect(std::bind([=](){
            for(auto child : mainContainer->children())
                delete child;
            windowImageEdit = new WindowImageEdit(mainContainer);
        }));
        myWNavigationBar->addWidget(loginPushButton, AlignRight);

        windowIntro = new WindowIntro(mainContainer);
    }
};
}
}


#endif // MAINNWINDOW

