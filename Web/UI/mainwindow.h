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
#include <Wt/WMessageBox>

#include "Web/UI/authwidgets.h"
#include "Web/UI/windowintro.h"
#include "Web/UI/windowimageedit.h"

#include "Utilities/Logger/logger.h"

/// \todo
#define _LINK_ "localhost:8080"

using namespace Wt;

namespace Web
{
namespace Ui
{
class MainWindow : public WContainerWidget
{
    public : WNavigationBar *navigationBar = nullptr;
    public : WContainerWidget *mainContainer = nullptr;

    public : WPushButton *intoPushButton = nullptr;
    public : WindowIntro *windowIntro = nullptr;

    public : WPushButton *logInOutPushButton = nullptr;
    public : WindowImageEdit *windowImageEdit = nullptr;
    private: bool _logInOutState;

    public : void _showIntroContent()
    {
        for(auto child : mainContainer->children())
        {
            child->hide();
            delete child;
        }
        windowIntro = new WindowIntro(mainContainer);
    }
    public :void _showImageEditContent()
    {
        for(auto child : mainContainer->children())
        {
            child->hide();
            delete child;
        }
        windowImageEdit = new WindowImageEdit(mainContainer);
    }
    void _showIntroPushButton()
    {
        intoPushButton->setIcon(WLink("icons/MainIcon.png"));
        intoPushButton->setToolTip("Intro page");
    }
    void _showLogInPushButton()
    {
        logInOutPushButton->setIcon(WLink("icons/Log-In.png"));
        logInOutPushButton->setToolTip("Log in");
    }

    public :void _showLogOutPushButton()
    {
        logInOutPushButton->setIcon(WLink("icons/Log-Out.png"));
        logInOutPushButton->setToolTip("Log Out");
    }

    MainWindow(WContainerWidget *parent) : WContainerWidget(parent)
    {
        navigationBar = new WNavigationBar(this);
        navigationBar->setResponsive(true);

        mainContainer = new WContainerWidget(this);

        intoPushButton = new WPushButton(this);
        intoPushButton->clicked().connect(std::bind([=](){
            _showIntroContent();
        }));
        _showIntroPushButton();
        navigationBar->addWidget(intoPushButton, AlignLeft);

        logInOutPushButton = new WPushButton(this);
        logInOutPushButton->clicked().connect(std::bind([=](){
            if(_logInOutState)
            {
                LogInForm *logInForm = new LogInForm(this);
                logInForm->show();
                logInForm->finished().connect(std::bind([=](WDialog::DialogCode code){
                    if(code == WDialog::Accepted)
                    {
                        Log::GlobalLogger.msg(
                                    Log::INFO,
                                    "[Authentication manager] User <" +
                                    logInForm->getUserName().toUTF8() +
                                    "> Logged in\n");
                        _logInOutState = false;
                        _showLogOutPushButton();
                        _showImageEditContent();
                    }
                    delete logInForm;
                },std::placeholders::_1));
            }
            else
            {
                if(WMessageBox::show(
                            "Warning",
                            "Do you really want to log out?",
                            Yes | No) == Yes)
                {
                    _logInOutState = true;
                    _showLogInPushButton();
                    _showIntroContent();
                }
            }
        }));
        _logInOutState = true;
        _showLogInPushButton();
        navigationBar->addWidget(logInOutPushButton, AlignRight);

        windowIntro = new WindowIntro(mainContainer);

        navigationBar->setMargin(0);
    }
};
}
}


#endif // MAINNWINDOW

