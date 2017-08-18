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
    WProgressBar    *_myUploadProgressBar;
    WTimer          *_myWTimer;
    double          _prevProgressbarValue = 0;
    public:
    WProgressBar* getProgressBar() {return _myUploadProgressBar;}
    Signal<>        signalStuck;
    DialogProgressbar(
            const Wt::WString& windowTitle,
            WObject *parent = 0,
            int stuckCheckInterval = 1000):
        WDialog(windowTitle, parent)
    {
        _myUploadProgressBar = new WProgressBar(this->contents());

        _myUploadProgressBar->progressCompleted().connect(this,&DialogProgressbar::accept);

        _myWTimer = new WTimer(this);
        _myWTimer->setInterval(stuckCheckInterval);
        _myWTimer->timeout().connect(std::bind([=] (){
            if(_myUploadProgressBar->value()!=_prevProgressbarValue)
                _prevProgressbarValue = _myUploadProgressBar->value();
            else
            {
                std::cout << _myUploadProgressBar->value() << " " << _prevProgressbarValue << "\n";
                signalStuck.emit();
            }
        }));

//        this->finished().connect(std::bind([=] () {delete this;}));

//        _myWTimer->start();
    }
    ~DialogProgressbar()
    {
//        _myWTimer->stop();
    }
};
}
}

#endif // DIALOGPROGRESSBAR

