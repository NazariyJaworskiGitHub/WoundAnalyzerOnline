#ifndef USERSESSION_H
#define USERSESSION_H

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <QString>

#include <Wt/WCssTheme>
#include <Wt/WBootstrapTheme>
#include <Wt/WLocale>
#include <Wt/WApplication>
#include <Wt/WEnvironment>

#include "Utilities/Logger/logger.h"

#include "Web/UI/windowpatient.h"
#include "Web/UI/windowimageedit.h"

using namespace Wt;

namespace Web
{
class UserSession : public WApplication
{
    public : UserSession(const WEnvironment &env):WApplication(env)
    {
        Log::GlobalLogger.msg(Log::INFO, "[User session " + this->sessionId() + "] User environment parameters:\n"
                              "\t Client address:     " + env.clientAddress() + "\n"
                              "\t User agent:         " + env.userAgent() + "\n"
                              "\t Screen width:       " + QString::number(env.screenWidth()).toStdString() + "\n"
                              "\t Screen height:      " + QString::number(env.screenHeight()).toStdString() + "\n"
                              "\t Support cookies:    " + (env.supportsCookies()   ? "TRUE\n" : "FALSE\n") +
                              "\t Support WebGL:      " + (env.webGL()             ? "TRUE\n" : "FALSE\n") +
                              "\t Support JavaScript: " + (env.javaScript()        ? "TRUE\n" : "FALSE\n") +
                              "\t Localization:       " + env.locale().name() + "\n");

        setTitle("Test webserver");
        //setLocale(env.locale());
        setTheme(new WBootstrapTheme());
        //setCssTheme("polished");
        //setCssTheme("");
        enableUpdates(true);
//        Ui::WindowPatient *windowPatient = new Ui::WindowPatient(root());
//        root()->addWidget(windowPatient);
//        root()->setOverflow(WContainerWidget::OverflowHidden);
        Ui::WindowImageEdit *windowImageEdit = new Ui::WindowImageEdit(root());
        root()->addWidget(windowImageEdit);
    }
    public : ~UserSession(){}
};
}

#endif // USERSESSION_H
