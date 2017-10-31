#ifndef DIALOGPROGRESSBAR
#define DIALOGPROGRESSBAR

#include <Wt/WString>
#include <Wt/WDialog>
#include <Wt/WProgressBar>
#include <Wt/WTimer>

#include <stdio.h>

using namespace Wt;

namespace Web
{
namespace Ui
{
class DialogProgressbar : public WDialog
{
    private:
    WProgressBar    *_myProgressBar;
    WTimer          *_myWTimer;
    double          _prevProgressbarValue = 0;
    public:
    WProgressBar* getProgressBar() {return _myProgressBar;}
    Signal<>        signalStuck;
    DialogProgressbar(
            const Wt::WString& windowTitle,
            WObject *parent = 0,
            int stuckCheckInterval = 1000):
        WDialog(windowTitle, parent)
    {
        _myProgressBar = new WProgressBar(this->contents());

        _myProgressBar->progressCompleted().connect(this,&DialogProgressbar::accept);

        _myWTimer = new WTimer(this);
        _myWTimer->setInterval(stuckCheckInterval);
        _myWTimer->timeout().connect(std::bind([=] (){
            if(_myProgressBar->value()!=_prevProgressbarValue)
                _prevProgressbarValue = _myProgressBar->value();
            else
            {
                std::cout << _myProgressBar->value() << " " << _prevProgressbarValue << "\n";
                _myWTimer->stop();
                _myWTimer->start();
                 signalStuck.emit();
            }
        }));

        this->finished().connect(std::bind([=] () {delete this;}));

        _myWTimer->start();
    }
    ~DialogProgressbar()
    {
    }
};
}
}

#endif // DIALOGPROGRESSBAR

