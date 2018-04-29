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
#include <Wt/WTree>
#include <Wt/WTreeNode>
#include <Wt/WIconPair>
#include <Wt/WTextArea>

#include <QDateTime>
#include <QByteArray>

#include "Image/imagemanager.h"

using namespace Wt;

namespace Web
{
namespace Ui {

class DatabaseModel : public WContainerWidget
{
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
                const std::string &iconPath = "icons/DatabaseView/Doctor.png");
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
                const std::string &iconPath = "icons/DatabaseView/Patient.png");
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
                const std::string &iconPath = "icons/DatabaseView/Wound.png");
    };

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
        public : double zoom = 100;
        public : Survey(int ID,
                const QDateTime &date,
                const std::string &notes,
                const double woundarea,
                WContainerWidget *parent = 0,
                const std::string &iconPath = "icons/DatabaseView/Survey.png");
        public : QByteArray packPolygons() const;
        public : void unpackPolygons(QByteArray buf);
        public : QByteArray packRulerPoints() const;
        public : void unpackRulerPoints(QByteArray buf);
        public :~Survey();
        WImage * convertFromCVMatToWImage();
        WDialog *callSurveyEditDialog(WObject *parent, bool addToTree, Wound *caller);
    };

    public : template <class T1, class T2> static WDialog *callNotesEditDialog(
            T1 *caller,
            T2 *target,
            const std::string &title,
            bool addToTree,
            WObject *parent);

    public : WTree *tree = nullptr;
    public : Doctor *doctor = nullptr;
    public : WContainerWidget *treeContainer = nullptr;
    public : WContainerWidget *viewContainer = nullptr;
    public : WTextArea *notesContainer = nullptr;
    public : WContainerWidget *buttonsContainer = nullptr;
    public : DatabaseModel(WContainerWidget *parent);
    public: ~DatabaseModel(){}
};
}
}

#endif // DATABASMODEL_H

