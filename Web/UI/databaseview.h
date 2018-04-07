#ifndef DATABASEVIEW
#define DATABASEVIEW

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

using namespace Wt;

namespace Web
{
namespace Ui {
class DatabaseView : public WContainerWidget
{
    public:
    WTree *tree = nullptr;
    public : DatabaseView(WContainerWidget *parent) : WContainerWidget(parent)
    {
        WIconPair *doctorIcon = new WIconPair("icons/DatabaseView/Doctor.png",
                                          "icons/DatabaseView/Doctor.png",
                                          false,this);
        WIconPair *patientIcon = new WIconPair("icons/DatabaseView/Patient.png",
                                          "icons/DatabaseView/Patient.png",
                                          false,this);
        WIconPair *woundIcon = new WIconPair("icons/DatabaseView/Wound.png",
                                          "icons/DatabaseView/Wound.png",
                                          false,this);
        WIconPair *surveyIcon = new WIconPair("icons/DatabaseView/Survey.png",
                                          "icons/DatabaseView/Survey.png",
                                          false,this);
        tree = new WTree(this);
        tree->setSelectionMode(SingleSelection);

        WTreeNode *doctor = new WTreeNode("Doctor 1",doctorIcon);
        WTreeNode *patient1 = new WTreeNode("Patient 1",patientIcon);
        WTreeNode *patient2 = new WTreeNode("Patient 2",patientIcon);
        WTreeNode *wound1 = new WTreeNode("Wound 1",woundIcon);
        WTreeNode *wound2 = new WTreeNode("Wound 2",woundIcon);
        WTreeNode *wound3 = new WTreeNode("Wound 3",woundIcon);
        WTreeNode *survey1 = new WTreeNode("Survey 1",surveyIcon);

        doctor->addChildNode(patient1);
        doctor->addChildNode(patient2);
        patient1->addChildNode(wound1);
        patient1->addChildNode(wound2);
        patient2->addChildNode(wound3);
        wound1->addChildNode(survey1);
        tree->setTreeRoot(doctor);
    }
    public: ~DatabaseView(){}
};
}
}

#endif // DATABASEVIEW

