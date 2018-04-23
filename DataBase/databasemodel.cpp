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
#include <Wt/WLineEdit>

#include <Wt/Chart/WCartesianChart>
#include <Wt/Chart/WDataSeries>
#include <Wt/WStandardItemModel>

#include "databasemanagerwt.h"
#include "configurationparameters.h"
#include "Web/usersession.h"
#include "Web/UI/windowimageedit.h"

#define TREE (static_cast<Web::Ui::DatabaseModel*>(this->tree()->parent()->parent()))

#define TREE_VIEW_WIDTH 350 //random
#define RESERVED_HEIGHT 224 //browser top + start menu bottom + some reserved
#define BUTTONS_HEIGHT  48
#define POPUP_DELTA 64      //random

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

/// \todo move it to separated class
WDialog * Web::Ui::DatabaseModel::Survey::callSurveyEditDialog(
        WObject *parent,
        WTextArea *textArea,
        bool addToTree,
        DatabaseModel::Wound *caller)
{
    WDialog *d = new WDialog(parent);
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
    ta->resize(WindowImageEdit_WIDTH, POPUP_DELTA*2);
    d->contents()->addWidget(new WBreak());

    WPushButton *accept = new WPushButton("Accept",d->contents());
    accept->clicked().connect(std::bind([=]()
    {
        if(w->isImageOpened())
            d->accept();
        else
            WMessageBox::show("Warning", "Survey should have an image", Ok);
    }));
    accept->setFloatSide(Side::Right);
    accept->setStyleClass("btn-primary");

    WPushButton *cancel = new WPushButton("Cancel",d->contents());
    cancel->clicked().connect(d,&WDialog::reject);
    cancel->setStyleClass("btn-primary");
    cancel->setFloatSide(Side::Right);

    d->finished().connect(std::bind([=](WDialog::DialogCode code){
        if(code == WDialog::Accepted)
        {
            w->changeZoom(100);
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

            this->label()->setText(
                        this->date.toString("dd.MM.yyyy hh:mm").toStdString() + " " +
                        ((this->woundArea > 0) ?
                        (QString::number(this->woundArea,'f',2).toStdString() + "sm<sup>2</sup>") :
                        ("")));
            if(addToTree)
            {
                DatabaseManagerWt::instance()->add(caller,this);
                // sort
                bool inserted = false;
                for(unsigned i=0; i<caller->childNodes().size(); ++i)
                    if(((DatabaseModel::Survey*)caller->childNodes()[i])->date>this->date)
                    {
                        caller->insertChildNode(i,this);
                        inserted = true;
                        break;
                    }
                if(!inserted)
                    caller->addChildNode(this);
                caller->expand();
                caller->tree()->select(this);
            }
            else
            {
                DatabaseManagerWt::instance()->update(this);
                //sort
                WTreeNode *parentNode = this->parentNode();
                parentNode->tree()->clearSelection();
                parentNode->removeChildNode(this);
                bool inserted = false;
                for(unsigned i=0; i<parentNode->childNodes().size(); ++i)
                    if(((DatabaseModel::Survey*)parentNode->childNodes()[i])->date>this->date)
                    {
                        parentNode->insertChildNode(i,this);
                        inserted = true;
                        break;
                    }
                if(!inserted)
                    parentNode->addChildNode(this);
                parentNode->tree()->select(this);
            }
        }
    },std::placeholders::_1));

    d->show();

    return d;
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
        ((woundarea > 0) ?
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
        TREE->notesContainer->setText(this->notes.data());
        TREE->buttonsContainer->clear();

        DatabaseManagerWt::instance()->loadSurveyWoundImage(this);
        TREE->viewContainer->addWidget(convertFromCVMatToWImage());       

        WPushButton *deleteButton = new WPushButton("Delete survey",TREE->buttonsContainer);
        deleteButton->setIcon(WLink("icons/DatabaseView/Delete.png"));
        deleteButton->setFloatSide(Side::Left);
        deleteButton->clicked().connect(std::bind([=](){
            if(WMessageBox::show(
                        "Warning",
                        "Do you really want to delete this survey?",
                        Yes | No) == Yes)
            {
                DatabaseManagerWt::instance()->del(this);
                this->tree()->clearSelection();
                TREE->viewContainer->clear();
                TREE->notesContainer->setText("");
                TREE->buttonsContainer->clear();
                this->parentNode()->removeChildNode(this);
                delete this;
            }
        }));

        WPushButton *editButton = new WPushButton("Edit survey",TREE->buttonsContainer);
        editButton->setIcon(WLink("icons/DatabaseView/Edit.png"));
        editButton->setFloatSide(Side::Right);
        editButton->clicked().connect(std::bind([=](){
            callSurveyEditDialog(TREE->viewContainer,TREE->notesContainer,false,nullptr);
            TREE->viewContainer->clear();
            TREE->viewContainer->addWidget(convertFromCVMatToWImage());
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

Web::Ui::DatabaseModel::Survey::~Survey()
{
    for(auto p = polygons.begin(); p!= polygons.end(); ++p)
        p->clear();
    polygons.clear();
    rulerPoints.clear();
}

/// \todo move it to separated class
template <class T1, class T2> WDialog * Web::Ui::DatabaseModel::callNotesEditDialog(
        T1 *caller, T2 *target,const std::string &title, bool addToTree, WTextArea *textArea, WObject *parent)
{
    WDialog *d = new WDialog(parent);
    d->contents()->setMaximumSize(
                CURRENT_SESSION->userScreenWidth - POPUP_DELTA,
                CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT - POPUP_DELTA);
    d->contents()->resize(
                CURRENT_SESSION->userScreenWidth - POPUP_DELTA,
                CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT - POPUP_DELTA);

    WLabel *lel = new WLabel((title + " name").data(),d->contents());
    WLineEdit *le = new WLineEdit(d->contents());
    le->setText(target->name.data());
    lel->setBuddy(le);
    le->resize( CURRENT_SESSION->userScreenWidth - POPUP_DELTA*2,
                WLength::Auto);

    WTextArea *ta = new WTextArea(d->contents());
    ta->setText(target->notes.data());
    ta->resize(CURRENT_SESSION->userScreenWidth - POPUP_DELTA*2, POPUP_DELTA*2);
    d->contents()->addWidget(new WBreak());

    WPushButton *accept = new WPushButton("Accept",d->contents());
    accept->clicked().connect(std::bind([=]()
    {
        if(!le->text().empty())
            d->accept();
        else
            WMessageBox::show("Warning", (title + " should have a name").data(), Ok);
    }));
    accept->setFloatSide(Side::Right);
    accept->setStyleClass("btn-primary");
    WPushButton *cancel = new WPushButton("Cancel",d->contents());
    cancel->clicked().connect(d,&WDialog::reject);
    cancel->setStyleClass("btn-primary");
    cancel->setFloatSide(Side::Right);

    d->finished().connect(std::bind([=](WDialog::DialogCode code){
        if(code == WDialog::Accepted)
        {
            target->name = le->text().toUTF8();
            target->notes = ta->text().toUTF8();

            target->label()->setText(target->name.data());
            if(addToTree)
            {
                DatabaseManagerWt::instance()->add(caller,target);
                // sort
                bool inserted = false;
                for(unsigned i=0; i<caller->childNodes().size(); ++i)
                    if((caller->childNodes()[i])->label()->text()>target->label()->text())
                    {
                        caller->insertChildNode(i,target);
                        inserted = true;
                        break;
                    }
                if(!inserted)
                    caller->addChildNode(target);
                caller->expand();
                caller->tree()->select(target);
            }
            else
            {
                DatabaseManagerWt::instance()->update(target);
                // sort
                WTreeNode *parentNode = target->parentNode();
                parentNode->tree()->clearSelection();
                parentNode->removeChildNode(target);
                bool inserted = false;
                for(unsigned i=0; i<parentNode->childNodes().size(); ++i)
                    if((parentNode->childNodes()[i])->label()->text() > target->label()->text())
                    {
                        parentNode->insertChildNode(i,target);
                        inserted = true;
                        break;
                    }
                if(!inserted)
                    parentNode->addChildNode(target);
                parentNode->tree()->select(target);
            }
        }
    },std::placeholders::_1));

    d->show();
    return d;
}

DatabaseModel::Wound::Wound(int ID, const string &title, const string &notes, WContainerWidget *parent, const string &iconPath):
    WTreeNode(title.data(), new WIconPair(iconPath.data(),iconPath.data(),false,parent)),
    id(ID),
    name(title),
    notes(notes)
{
    this->setLoadPolicy(LoadPolicy::NextLevelLoading);
    this->selected().connect(std::bind([=](){
        TREE->viewContainer->clear();
        TREE->notesContainer->setText(this->notes.data());
        TREE->buttonsContainer->clear();

        if(this->childNodes().size()>1)
        {
            WStandardItemModel *model = new WStandardItemModel(this->childNodes().size(),2,this);
            model->setHeaderData(1, Wt::WString("Wounds area (sm2)"));
            int i=0;
            for(auto *s : this->childNodes())
            {
                WDateTime wdt;
                wdt.setDate(WDate(
                                ((DatabaseModel::Survey*)s)->date.date().year(),
                                ((DatabaseModel::Survey*)s)->date.date().month(),
                                ((DatabaseModel::Survey*)s)->date.date().day()));
                wdt.setTime(WTime(
                                ((DatabaseModel::Survey*)s)->date.time().hour(),
                                ((DatabaseModel::Survey*)s)->date.time().minute(),
                                ((DatabaseModel::Survey*)s)->date.time().second()));
                model->setData(i, 0, wdt);
                model->setData(i, 1, ((DatabaseModel::Survey*)s)->woundArea);
                ++i;
            }
            Chart::WCartesianChart *chart = new Chart::WCartesianChart(
                        Chart::ScatterPlot,TREE->viewContainer);
            chart->setModel(model);
            chart->setXSeriesColumn(0);
            chart->setLegendEnabled(true);
            chart->axis(Chart::Axis::XAxis).setScale(Chart::AxisScale::DateTimeScale);
            chart->setPlotAreaPadding(160, Side::Right);

            Chart::WDataSeries ser(1,Chart::SeriesType::LineSeries);
            chart->addSeries(ser);
            chart->resize(
                        CURRENT_SESSION->userScreenWidth - TREE_VIEW_WIDTH -POPUP_DELTA,
                        CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT - BUTTONS_HEIGHT*3 -POPUP_DELTA);
        }
        else
        {
            TREE->viewContainer->addWidget(new WLabel("Add more surveys to see the dynamics"));
        }

        WPushButton *deleteButton = new WPushButton("Delete wound",TREE->buttonsContainer);
        deleteButton->setIcon(WLink("icons/DatabaseView/Delete.png"));
        deleteButton->setFloatSide(Side::Left);
        deleteButton->clicked().connect(std::bind([=](){
            if(WMessageBox::show(
                        "Warning",
                        "Do you really want to delete this wound?",
                        Yes | No) == Yes)
            {
                DatabaseManagerWt::instance()->del(this);
                this->tree()->clearSelection();
                TREE->viewContainer->clear();
                TREE->notesContainer->setText("");
                TREE->buttonsContainer->clear();
                this->parentNode()->removeChildNode(this);
                delete this;
            }
        }));

        WPushButton *editButton = new WPushButton("Edit wound",TREE->buttonsContainer);
        editButton->setIcon(WLink("icons/DatabaseView/Edit.png"));
        editButton->setFloatSide(Side::Right);
        editButton->clicked().connect(std::bind([=](){
            DatabaseModel::callNotesEditDialog<DatabaseModel::Patient,DatabaseModel::Wound>(
                        nullptr, this, "Wound", false, TREE->notesContainer, TREE->viewContainer);
        }));

        WPushButton *addButton = new WPushButton("Add survey",TREE->buttonsContainer);
        addButton->setIcon(WLink("icons/DatabaseView/Add.png"));
        addButton->setFloatSide(Side::Right);
        addButton->clicked().connect(std::bind([=](){
            DatabaseModel::Survey *survey = new DatabaseModel::Survey(
                        -1, QDateTime::currentDateTime(), "", -1);
            WDialog *d = survey->callSurveyEditDialog(TREE->viewContainer,TREE->notesContainer,true,this);
            d->finished().connect(std::bind([=](WDialog::DialogCode code){
                if(code != WDialog::Accepted)
                    delete survey;
            },std::placeholders::_1));
        }));
    }));
}

Web::Ui::DatabaseModel::Patient::Patient(int ID, const string &title, const string &notes, WContainerWidget *parent, const string &iconPath):
    WTreeNode(title.data(), new WIconPair(iconPath.data(),iconPath.data(),false,parent)),
    id(ID),
    name(title),
    notes(notes)
{
    this->selected().connect(std::bind([=](){
        TREE->viewContainer->clear();
        TREE->notesContainer->setText(this->notes.data());
        TREE->buttonsContainer->clear();

        WPushButton *deleteButton = new WPushButton("Delete patient",TREE->buttonsContainer);
        deleteButton->setIcon(WLink("icons/DatabaseView/Delete.png"));
        deleteButton->setFloatSide(Side::Left);
        deleteButton->clicked().connect(std::bind([=](){
            if(WMessageBox::show(
                        "Warning",
                        "Do you really want to delete this patient?",
                        Yes | No) == Yes)
            {
                DatabaseManagerWt::instance()->del(this);
                this->tree()->clearSelection();
                TREE->viewContainer->clear();
                TREE->notesContainer->setText("");
                TREE->buttonsContainer->clear();
                this->parentNode()->removeChildNode(this);
                delete this;
            }
        }));

        WPushButton *editButton = new WPushButton("Edit patient",TREE->buttonsContainer);
        editButton->setIcon(WLink("icons/DatabaseView/Edit.png"));
        editButton->setFloatSide(Side::Right);
        editButton->clicked().connect(std::bind([=](){
            DatabaseModel::callNotesEditDialog<DatabaseModel::Doctor, DatabaseModel::Patient>(
                        nullptr, this, "Patient", false, TREE->notesContainer, TREE->viewContainer);
        }));

        WPushButton *addButton = new WPushButton("Add wound",TREE->buttonsContainer);
        addButton->setIcon(WLink("icons/DatabaseView/Add.png"));
        addButton->setFloatSide(Side::Right);
        addButton->clicked().connect(std::bind([=](){
            DatabaseModel::Wound *wound = new DatabaseModel::Wound(-1, "New wound", "");
            WDialog *d = DatabaseModel::callNotesEditDialog<DatabaseModel::Patient, DatabaseModel::Wound>(
                        this, wound, "Wound", true, TREE->notesContainer, TREE->viewContainer);
            d->finished().connect(std::bind([=](WDialog::DialogCode code){
                if(code != WDialog::Accepted)
                    delete wound;
            },std::placeholders::_1));
        }));
    }));
}

DatabaseModel::Doctor::Doctor(int ID, const string &title, const string &notes, WContainerWidget *parent, const string &iconPath):
    WTreeNode(title.data(), new WIconPair(iconPath.data(),iconPath.data(),false,parent)),
    id(ID),
    name(title),
    notes(notes)
{
    this->selected().connect(std::bind([=](){
        TREE->viewContainer->clear();
        TREE->notesContainer->setText(this->notes.data());
        TREE->buttonsContainer->clear();

        WPushButton *editButton = new WPushButton("Edit doctor",TREE->buttonsContainer);
        editButton->setIcon(WLink("icons/DatabaseView/Edit.png"));
        editButton->setFloatSide(Side::Right);
        editButton->clicked().connect(std::bind([=](){
            DatabaseModel::callNotesEditDialog<DatabaseModel::Doctor, DatabaseModel::Doctor>(
                        nullptr, this, "Doctor", false, TREE->notesContainer, TREE->viewContainer);
        }));

        WPushButton *addButton = new WPushButton("Add patient",TREE->buttonsContainer);
        addButton->setIcon(WLink("icons/DatabaseView/Add.png"));
        addButton->setFloatSide(Side::Right);
        addButton->clicked().connect(std::bind([=](){
            DatabaseModel::Patient *patient = new DatabaseModel::Patient(-1,"New patient","");
            WDialog *d = DatabaseModel::callNotesEditDialog<DatabaseModel::Doctor, DatabaseModel::Patient>(
                        this, patient, "Wound", true, TREE->notesContainer, TREE->viewContainer);
            d->finished().connect(std::bind([=](WDialog::DialogCode code){
                if(code != WDialog::Accepted)
                    delete patient;
            },std::placeholders::_1));
        }));
    }));
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
                CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT - BUTTONS_HEIGHT*3);
    viewContainer->resize(
                CURRENT_SESSION->userScreenWidth - TREE_VIEW_WIDTH,
                CURRENT_SESSION->userScreenHeight - RESERVED_HEIGHT - BUTTONS_HEIGHT*3);
    layout3->addWidget(viewContainer);

    notesContainer = new WTextArea(this);
    notesContainer->setStyleClass("thumbnail");
    //notesContainer->setOverflow(OverflowAuto);
    notesContainer->setReadOnly(true);
    notesContainer->setMaximumSize(
                CURRENT_SESSION->userScreenWidth - TREE_VIEW_WIDTH,
                BUTTONS_HEIGHT*2 - BUTTONS_HEIGHT/3);
    notesContainer->resize(
                CURRENT_SESSION->userScreenWidth - TREE_VIEW_WIDTH,
                BUTTONS_HEIGHT*2 - BUTTONS_HEIGHT/3);
    layout3->addWidget(notesContainer);


    buttonsContainer = new WContainerWidget(this);
    buttonsContainer->setMaximumSize(
                CURRENT_SESSION->userScreenWidth - TREE_VIEW_WIDTH,
                BUTTONS_HEIGHT);
    buttonsContainer->resize(
                CURRENT_SESSION->userScreenWidth - TREE_VIEW_WIDTH,
                BUTTONS_HEIGHT);
    layout3->addWidget(buttonsContainer);
}
