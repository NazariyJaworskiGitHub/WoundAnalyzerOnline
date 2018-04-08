#ifndef DATABASMODEL_H
#define DATABASMODEL_H

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <Wt/WContainerWidget>
#include <Wt/WIconPair>
#include <Wt/WTree>
#include <Wt/WTreeNode>

#include <QDateTime>
#include <QByteArray>

#include "Image/imagemanager.h"

//#define SURVEY_TYPE     1001
//#define WOUND_TYPE      1002
//#define PATIENT_TYPE    1003
//#define DOCTOR_TYPE     1004

using namespace Wt;

namespace Web
{
namespace Ui {

class DatabaseModel : public WContainerWidget
{
    public : class Survey : public WTreeNode
    {
        public : int id;
        public : QDateTime date;
        public : std::string notes;
        public : cv::Mat image;
        public : std::vector<PolygonF> polygons;
        public : PolygonF rulerPoints;
        public : double rulerFactor = 1.0;
        public : double woundArea = -1;
        public : Survey(
                int ID,
                const QDateTime &date,
                const std::string &notes,
                const double woundArea,
                WContainerWidget *parent = 0,
                const std::string &iconPath = "icons/DatabaseView/Survey.png"):
            WTreeNode(
                date.toString("dd.MM.yyyy hh:mm").toStdString() + " " +
                ((woundArea != 0) ? (QString::number(woundArea,'f',2).toStdString() + "sm<sup>2</sup>") : ("")),
                new WIconPair(iconPath.data(),iconPath.data(),false,parent)),
            id(ID),
            date(date),
            notes(notes),
            woundArea(woundArea){}
        public : QByteArray packPolygons() const
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
        public : void unpackPolygons(QByteArray buf)
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
        public : QByteArray packRulerPoints() const
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
        public : void unpackRulerPoints(QByteArray buf)
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
        public : void setPolygonsAndRulerPoints(
                const std::vector<PolygonF> &p,
                const PolygonF &r)
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
        public :~Survey()
        {
            for(auto p = polygons.begin(); p!= polygons.end(); ++p)
                p->clear();
            polygons.clear();
            rulerPoints.clear();
        }

//        public : int type() const override { return SURVEY_TYPE;}
    };

    public : class Wound : public WTreeNode
    {
        public : int id;
        public : std::string name;
        public : std::string notes;
        public : Wound(
                int ID,
                const std::string &title,
                const std::string &notes,
                WContainerWidget *parent = 0,
                const std::string &iconPath = "icons/DatabaseView/Wound.png"):
            WTreeNode(title.data(), new WIconPair(iconPath.data(),iconPath.data(),false,parent)),
            id(ID),
            name(title),
            notes(notes){}
//        public : int type() const override { return WOUND_TYPE;}
    };

    public : class Patient : public WTreeNode
    {
        public : int id;
        public : std::string name;
        public : std::string notes;
        public : Patient(
                int ID,
                const std::string &title,
                const std::string &notes,
                WContainerWidget *parent = 0,
                const std::string &iconPath = "icons/DatabaseView/Patient.png"):
            WTreeNode(title.data(), new WIconPair(iconPath.data(),iconPath.data(),false,parent)),
            id(ID),
            name(title),
            notes(notes){}
//        public : int type() const override { return PATIENT_TYPE;}
    };

    public : class Doctor : public WTreeNode
    {
        public : int id;
        public : std::string name;
        public : std::string notes;
        public : Doctor(
                int ID,
                const std::string &title,
                const std::string &notes,
                WContainerWidget *parent = 0,
                const std::string &iconPath = "icons/DatabaseView/Doctor.png"):
            WTreeNode(title.data(), new WIconPair(iconPath.data(),iconPath.data(),false,parent)),
            id(ID),
            name(title),
            notes(notes){}
//        public : int type() const override { return DOCTOR_TYPE;}
    };

    public : WTree *tree = nullptr;
    public : Doctor *doctor = nullptr;
    public : DatabaseModel(WContainerWidget *parent) : WContainerWidget(parent)
    {
        tree = new WTree(this);
        tree->setSelectionMode(SingleSelection);
    }
    public: ~DatabaseModel(){}
};
}
}

#endif // DATABASMODEL_H

