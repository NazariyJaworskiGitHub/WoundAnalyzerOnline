#include "databasemodel.h"

#include <Wt/WMemoryResource>
#include <Wt/WImage>
#include <Wt/WPushButton>
#include <Wt/WBreak>
#include <Wt/WMessageBox>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WDialog>
#include <Wt/WDateEdit>
#include <Wt/WTimeEdit>
#include <Wt/WTextArea>

#include "databasemanagerwt.h"
#include "configurationparameters.h"
#include "Web/usersession.h"
#include "Web/UI/windowimageedit.h"

#define TREE (static_cast<Web::Ui::DatabaseModel*>(this->tree()->parent()->parent()))

#define TREE_VIEW_WIDTH 350
#define RESERVED_HEIGHT 176
#define BUTTONS_HEIGHT  48
#define POPUP_DELTA 64

WImage * Web::Ui::DatabaseModel::Survey::convertFromCVMatToWImage()
{
    WMemoryResource *myWMemoryResource = new WMemoryResource("image/jpg",this);
    vector<unsigned char> imageDecodedData;
    vector<int> param = vector<int>(2);
    param[0] = CV_IMWRITE_JPEG_QUALITY;
    param[1] = ConfigurationParameters::instance()->jpegQuality;
    imencode(".jpg", image, imageDecodedData, param);
    myWMemoryResource->setData(imageDecodedData);
    WImage *img = new WImage();
    img->setResource(myWMemoryResource);
    return img;
}

Web::Ui::DatabaseModel::Survey::Survey(
        int ID,
        const QDateTime &date,
        const std::string &notes,
        const double woundarea,
        Wt::WContainerWidget *parent,
        const std::string &iconPath):
    WTreeNode(
        date.toString("dd.MM.yyyy hh:mm").toStdString() + " " +
        ((woundarea != 0) ?
             (QString::number(woundarea,'f',2).toStdString() + "sm<sup>2</sup>") :
             ("")),
        //should do this becouse cant access to labelText_
        new WIconPair(iconPath.data(),iconPath.data(),false,parent)),
    id(ID),
    date(date),
    notes(notes),
    woundArea(woundarea)
{
    this->selected().connect(std::bind([=](){
        TREE->viewContainer->clear();
        TREE->buttonsContainer->clear();

        DatabaseManagerWt::instance()->loadSurveyWoundImage(this);
        TREE->viewContainer->addWidget(convertFromCVMatToWImage());

        WPushButton *editButton = new WPushButton("Edit",TREE->buttonsContainer);
        editButton->setIcon(WLink("icons/DatabaseView/Edit.png"));
        editButton->clicked().connect(std::bind([=](){
            WDialog *d = new WDialog(TREE->viewContainer);
            d->contents()->setMaximumSize(
                        CURRENT_SESSION->userScreenWidth - POPUP_DELTA,
                        CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT - POPUP_DELTA);
            d->contents()->resize(
                        CURRENT_SESSION->userScreenWidth - POPUP_DELTA,
                        CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT - POPUP_DELTA);

            WDateEdit *de = new WDateEdit(d->contents());
            de->setDate(WDate(
                            this->date.date().year(),
                            this->date.date().month(),
                            this->date.date().day()));
            WTimeEdit *te = new WTimeEdit(d->contents());
            te->setTime(WTime(
                            this->date.time().hour(),
                            this->date.time().minute(),
                            this->date.time().second()));
            d->contents()->addWidget(new WBreak());

            WindowImageEdit *w = new WindowImageEdit(d->contents());
            w->copyAllDataFrom(image,polygons,rulerPoints,woundArea,rulerFactor);
            d->contents()->addWidget(new WBreak());

            WTextArea *ta = new WTextArea(d->contents());
            ta->setText(this->notes.data());
            d->contents()->addWidget(new WBreak());

            WPushButton *accept = new WPushButton("Accept",d->contents());
            accept->clicked().connect(d,&WDialog::accept);
            accept->setFloatSide(Side::Right);
            accept->setStyleClass("btn-primary");
            WPushButton *cancel = new WPushButton("Cancel",d->contents());
            cancel->clicked().connect(d,&WDialog::reject);
            cancel->setStyleClass("btn-primary");
            cancel->setFloatSide(Side::Right);

            d->show();
            d->finished().connect(std::bind([=](WDialog::DialogCode code){
                if(code == WDialog::Accepted)
                {
                    w->copyAllDataTo(image,polygons,rulerPoints,woundArea,rulerFactor);
                    this->date.setDate(QDate(
                                           de->date().year(),
                                           de->date().month(),
                                           de->date().day()));
                    this->date.setTime(QTime(
                                           te->time().hour(),
                                           te->time().minute(),
                                           te->time().second()));
                    this->notes = ta->text().toUTF8();
                    DatabaseManagerWt::instance()->update(this);
                    this->label()->setText(
                                date.toString("dd.MM.yyyy hh:mm").toStdString() + " " +
                                ((woundarea != 0) ?
                                (QString::number(woundarea,'f',2).toStdString() + "sm<sup>2</sup>") :
                                ("")));
                    this->label()->show();
                    TREE->viewContainer->clear();
                    TREE->viewContainer->addWidget(convertFromCVMatToWImage());
                }
            },std::placeholders::_1));
        }));

        WPushButton *deleteButton = new WPushButton("Delete",TREE->buttonsContainer);
        deleteButton->setIcon(WLink("icons/DatabaseView/Delete.png"));
        deleteButton->clicked().connect(std::bind([=](){
            if(WMessageBox::show(
                        "Warning",
                        "Do you really want to delete this survey?",
                        Yes | No) == Yes)
            {
                TREE->viewContainer->clear();
                TREE->buttonsContainer->clear();
                DatabaseManagerWt::instance()->del(this);
                TREE->treeContainer->show();
            }
        }));
    }));
}

QByteArray Web::Ui::DatabaseModel::Survey::packPolygons() const
{
    QByteArray arr;
    char intSize = sizeof(int);
    arr.append(&intSize, 1);                            // size of int
    char doubleSize = sizeof(double);
    arr.append(&doubleSize, 1);                         // size of double
    int count = polygons.size();
    arr.append((char*)&count, sizeof(int));             // number of polygons <int>
    for(PolygonF p: polygons)
    {
        int countP = p.size();
        arr.append((char*)&countP, sizeof(int));        // number of nodes in polygons <int>
    }
    for(PolygonF p: polygons)
        for(Point2d n: p)
        {
            arr.append((char*)&n.x, sizeof(double));    // nodes of polygons <double>
            arr.append((char*)&n.y, sizeof(double));
        }
    return arr;
}

void Web::Ui::DatabaseModel::Survey::unpackPolygons(QByteArray buf)
{
    for(auto p = polygons.begin(); p!= polygons.end(); ++p)
        p->clear();
    polygons.clear();
    if(!buf.isEmpty())
    {
        char * ptr = buf.data();
        size_t intSize = *ptr;
        ptr += sizeof(char);
        size_t doubleSize = *ptr;
        ptr += sizeof(char);
        int polygonsCount = *(int*)(ptr);
        ptr += intSize;
        polygons.resize(polygonsCount);
        for(auto p = polygons.begin(); p!= polygons.end(); ++p)
        {
            int nodesCount = *(int*)(ptr);
            ptr += intSize;
            p->resize(nodesCount);
        }
        for(auto p = polygons.begin(); p!= polygons.end(); ++p)
            for(auto n = p->begin(); n!= p->end(); ++n)
            {
                n->x = *(double*)(ptr);
                ptr += doubleSize;
                n->y = *(double*)(ptr);
                ptr += doubleSize;
            }
    }
}

QByteArray Web::Ui::DatabaseModel::Survey::packRulerPoints() const
{
    QByteArray arr;
    char intSize = sizeof(int);
    arr.append(&intSize, 1);                            // size of int
    char doubleSize = sizeof(double);
    arr.append(&doubleSize, 1);                         // size of double
    int count = rulerPoints.size();
    arr.append((char*)&count, sizeof(int));             // number of nodes <int>
    for(auto n: rulerPoints)
    {
        arr.append((char*)&n.x, sizeof(double));        // nodes of polygon <double>
        arr.append((char*)&n.y, sizeof(double));
    }
    return arr;
}

void Web::Ui::DatabaseModel::Survey::unpackRulerPoints(QByteArray buf)
{
    rulerPoints.clear();
    if(!buf.isEmpty())
    {
        char * ptr = buf.data();
        size_t intSize = *ptr;
        ptr += sizeof(char);
        size_t doubleSize = *ptr;
        ptr += sizeof(char);
        int nodesCount = *(int*)(ptr);
        ptr += intSize;
        rulerPoints.resize(nodesCount);
        for(auto n = rulerPoints.begin(); n!= rulerPoints.end(); ++n)
        {
            n->x = *(double*)(ptr);
            ptr += doubleSize;
            n->y = *(double*)(ptr);
            ptr += doubleSize;
        }
    }
}

void Web::Ui::DatabaseModel::Survey::setPolygonsAndRulerPoints(const std::vector<PolygonF> &p, const PolygonF &r)
{
    for(auto _p = polygons.begin(); _p!= polygons.end(); ++_p)
        _p->clear();
    polygons.clear();
    rulerPoints.clear();

    polygons.resize(p.size());
    for(size_t i=0 ; i < polygons.size(); ++i)
        polygons[i] = p[i];
    rulerPoints = r;
}

Web::Ui::DatabaseModel::Survey::~Survey()
{
    for(auto p = polygons.begin(); p!= polygons.end(); ++p)
        p->clear();
    polygons.clear();
    rulerPoints.clear();
}


Web::Ui::DatabaseModel::DatabaseModel(WContainerWidget *parent) : WContainerWidget(parent)
{
    WHBoxLayout *layout1 = new WHBoxLayout();
    WVBoxLayout *layout2 = new WVBoxLayout();
    WVBoxLayout *layout3 = new WVBoxLayout();
    layout1->addLayout(layout2);
    layout1->addLayout(layout3);
    this->setLayout(layout1);

    treeContainer = new WContainerWidget(this);
    treeContainer->setStyleClass("thumbnail");
    treeContainer->setOverflow(OverflowAuto);
    treeContainer->setMaximumSize(
                TREE_VIEW_WIDTH,
                CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT);
    treeContainer->resize(
                TREE_VIEW_WIDTH,
                CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT);
    layout2->addWidget(treeContainer);

    tree = new WTree(treeContainer);
    tree->setSelectionMode(SingleSelection);

    viewContainer = new WContainerWidget(this);
    viewContainer->setStyleClass("thumbnail");
    viewContainer->setOverflow(OverflowAuto);
    viewContainer->setMaximumSize(
                CURRENT_SESSION->userScreenWidth - TREE_VIEW_WIDTH,
                CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT - BUTTONS_HEIGHT);
    viewContainer->resize(
                CURRENT_SESSION->userScreenWidth - TREE_VIEW_WIDTH,
                CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT - BUTTONS_HEIGHT);
    layout3->addWidget(viewContainer);

    buttonsContainer = new WContainerWidget(this);
    buttonsContainer->setMaximumSize(
                CURRENT_SESSION->userScreenWidth - TREE_VIEW_WIDTH,
                BUTTONS_HEIGHT);
    buttonsContainer->resize(
                CURRENT_SESSION->userScreenWidth - TREE_VIEW_WIDTH,
                BUTTONS_HEIGHT);
    layout3->addWidget(buttonsContainer);
}
