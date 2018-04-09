#include "databasemodel.h"

#include <Wt/WMemoryResource>
#include <Wt/WImage>

#include "databasemanagerwt.h"
#include "configurationparameters.h"
#include "Web/usersession.h"

#define TREE (static_cast<Web::Ui::DatabaseModel*>(this->tree()->parent()->parent()))

Web::Ui::DatabaseModel::Survey::Survey(
        int ID,
        const QDateTime &date,
        const std::string &notes,
        const double woundArea,
        Wt::WContainerWidget *parent,
        const std::string &iconPath):
    WTreeNode(
        date.toString("dd.MM.yyyy hh:mm").toStdString() + " " +
        ((woundArea != 0) ?
             (QString::number(woundArea,'f',2).toStdString() + "sm<sup>2</sup>") :
             ("")),
        new WIconPair(iconPath.data(),iconPath.data(),false,parent)),
    id(ID),
    date(date),
    notes(notes),
    woundArea(woundArea)
{
    this->selected().connect(std::bind([=](){
        TREE->viewContainer->clear();

        DatabaseManagerWt::instance()->loadSurveyWoundImage(this);
        WMemoryResource *myWMemoryResource = new WMemoryResource("image/jpg",this);
        vector<unsigned char> imageDecodedData;
        vector<int> param = vector<int>(2);
        param[0] = CV_IMWRITE_JPEG_QUALITY;
        param[1] = ConfigurationParameters::instance()->jpegQuality;
        imencode(".jpg", image, imageDecodedData, param);
        myWMemoryResource->setData(imageDecodedData);
        WImage *img = new WImage(TREE->viewContainer);
        img->setResource(myWMemoryResource);
        this->show();
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
    layout = new WHBoxLayout(this);

    treeContainer = new WContainerWidget(this);
    treeContainer->setStyleClass("thumbnail");
    treeContainer->setOverflow(OverflowAuto);
    treeContainer->setMaximumSize(350,350/*CURRENT_SESSION->userScreenHeight-80*/);
    layout->addWidget(treeContainer);

    tree = new WTree(treeContainer);
    tree->setSelectionMode(SingleSelection);

    viewContainer = new WContainerWidget(this);
    viewContainer->setStyleClass("thumbnail");
    viewContainer->setOverflow(OverflowAuto);
    layout->addWidget(viewContainer);
}
