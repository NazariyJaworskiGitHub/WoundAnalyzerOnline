#ifndef WINDOWPATIENT_H
#define WINDOWPATIENT_H

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <Wt/WString>
#include <Wt/WContainerWidget>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WDateEdit>
#include <Wt/WTimeEdit>
#include <Wt/WTemplate>
#include <Wt/WPushButton>

#include <QFile>
#include <QTextStream>

#include <patient.h>

using namespace Wt;

namespace Web
{
namespace Ui
{
class WindowPatient : public WContainerWidget
{
    private:
    WTemplate *form;

    WLineEdit *ministry;
    WLineEdit *hospital;
    WLineEdit *hospitalYEDRPOU;
    WLineEdit *id;
    WDateEdit *hospDate;
    WTimeEdit *hospTime;
    WComboBox *sexKey;
    WLineEdit *lastName;
    WLineEdit *firstName;
    WLineEdit *fatherName;
    WDateEdit *birthDate;
    WLineEdit *age;
    WLineEdit *idDocType;
    WLineEdit *idDocNumber;
    WComboBox *citizenship;
    WComboBox *residenceKey;
    WLineEdit *residenceAddress;
    WLineEdit *resZipCode;
    WLineEdit *placeOfJob;
    WLineEdit *sender;
    WLineEdit *senderYEDRPOU;
    WLineEdit *hospDiag;
    WLineEdit *hospDiagMKX10;
    WLineEdit *hospDep;
    WLineEdit *wrOutDep;
    WComboBox *hospKey;
    WDateEdit *HIVobservDate;
    WLineEdit *bloodType;
    WLineEdit *RhAffiliation;
    WDateEdit *VasReactDate;
    WLineEdit *allergReact;
    WComboBox *hospInCurrentYearKey;
    WComboBox *hospRepeatedlyInMonthKey;
    WDateEdit *wrOutDate;
    WTimeEdit *wrOutTime;
    WLineEdit *hospDays;

    WPushButton *changeModeButton;

    bool _editMode;
    public : bool isEditMode() const noexcept {return _editMode;}

    public : void setEditMode()
    {
        // binding should automatically delete previous widgets
        form->bindWidget("_1",ministry);
        form->bindWidget("_2",hospital);
        form->bindWidget("_3",hospitalYEDRPOU);
        form->bindWidget("_4",id);
        form->bindWidget("_5",hospDate);
        form->bindWidget("_6",hospTime);
        form->bindWidget("A",sexKey);
        form->bindWidget("_7",lastName);
        form->bindWidget("_7_1",firstName);
        form->bindWidget("_7_2",fatherName);
        form->bindWidget("_8",birthDate);
        form->bindWidget("_9",age);
        form->bindWidget("_10",idDocType);
        form->bindWidget("_11",idDocNumber);
        form->bindWidget("_12",citizenship);
        form->bindWidget("B",residenceKey);
        form->bindWidget("_13",resZipCode);
        form->bindWidget("_14",residenceAddress);
        form->bindWidget("_15",placeOfJob);
        form->bindWidget("_16",sender);
        form->bindWidget("_17",senderYEDRPOU);
        form->bindWidget("_18",hospDiag);
        form->bindWidget("_19",hospDiagMKX10);
        form->bindWidget("_20",hospDep);
        form->bindWidget("_21",wrOutDep);
        form->bindWidget("C",hospKey);
        form->bindWidget("_22",HIVobservDate);
        form->bindWidget("_23",bloodType);
        form->bindWidget("_24",RhAffiliation);
        form->bindWidget("_25",VasReactDate);
        form->bindWidget("_26",allergReact);
        form->bindWidget("D",hospInCurrentYearKey);
        form->bindWidget("E",hospRepeatedlyInMonthKey);
        form->bindWidget("_27",wrOutDate);;
        form->bindWidget("_28",wrOutTime);
        form->bindWidget("_29",hospDays);
        for(auto w:form->widgets())
        {
            w->setWidth(WLength(100,WLength::Percentage));
        }
        id->setWidth(WLength::Auto);
        lastName->setWidth(WLength(30,WLength::Percentage));
        firstName->setWidth(WLength(30,WLength::Percentage));
        fatherName->setWidth(WLength(30,WLength::Percentage));
        hospDate->setWidth(WLength(70,WLength::Percentage));
        hospTime->setWidth(WLength(70,WLength::Percentage));
        birthDate->setWidth(WLength(70,WLength::Percentage));
        HIVobservDate->setWidth(WLength(70,WLength::Percentage));
        VasReactDate->setWidth(WLength(70,WLength::Percentage));
        wrOutDate->setWidth(WLength(70,WLength::Percentage));
        wrOutTime->setWidth(WLength(70,WLength::Percentage));
        _editMode = true;
        show();
    }

    public : void setInfoMode()
    {
        //unbind widgets, don't delete them
        form->takeWidget("_1");
        form->takeWidget("_2");
        form->takeWidget("_3");
        form->takeWidget("_4");
        form->takeWidget("_5");
        form->takeWidget("_6");
        form->takeWidget("_7");
        form->takeWidget("_7_1");
        form->takeWidget("_7_2");
        form->takeWidget("_8");
        form->takeWidget("_9");
        form->takeWidget("_10");
        form->takeWidget("_11");
        form->takeWidget("_12");
        form->takeWidget("_13");
        form->takeWidget("_14");
        form->takeWidget("_15");
        form->takeWidget("_16");
        form->takeWidget("_17");
        form->takeWidget("_18");
        form->takeWidget("_19");
        form->takeWidget("_20");
        form->takeWidget("_21");
        form->takeWidget("_22");
        form->takeWidget("_23");
        form->takeWidget("_24");
        form->takeWidget("_25");
        form->takeWidget("_26");
        form->takeWidget("_27");
        form->takeWidget("_28");
        form->takeWidget("_29");
        form->takeWidget("A");
        form->takeWidget("B");
        form->takeWidget("C");
        form->takeWidget("D");
        form->takeWidget("E");

        WLabel *l1, *l2, *l3, *l4;
        form->bindWidget("_1",new WLabel(ministry->text()));
        form->bindWidget("_2",new WLabel(hospital->text()));
        form->bindWidget("_3",new WLabel(hospitalYEDRPOU->text()));
        form->bindWidget("_4",l1 = new WLabel(id->text()));
        form->bindWidget("_5",new WLabel(hospDate->text()));
        form->bindWidget("_6",new WLabel(hospTime->text()));
        form->bindWidget("A",new WLabel(sexKey->currentText()));
        form->bindWidget("_7",l2 = new WLabel(lastName->text()));
        form->bindWidget("_7_1",l3 = new WLabel(firstName->text()));
        form->bindWidget("_7_2",l4 = new WLabel(fatherName->text()));
        form->bindWidget("_8",new WLabel(birthDate->text()));
        form->bindWidget("_9",new WLabel(age->text()));
        form->bindWidget("_10",new WLabel(idDocType->text()));
        form->bindWidget("_11",new WLabel(idDocNumber->text()));
        form->bindWidget("_12",new WLabel(citizenship->currentText()));
        form->bindWidget("B",new WLabel(residenceKey->currentText()));
        form->bindWidget("_13",new WLabel(resZipCode->text()));
        form->bindWidget("_14",new WLabel(residenceAddress->text()));
        form->bindWidget("_15",new WLabel(placeOfJob->text()));
        form->bindWidget("_16",new WLabel(sender->text()));
        form->bindWidget("_17",new WLabel(senderYEDRPOU->text()));
        form->bindWidget("_18",new WLabel(hospDiag->text()));
        form->bindWidget("_19",new WLabel(hospDiagMKX10->text()));
        form->bindWidget("_20",new WLabel(hospDep->text()));
        form->bindWidget("_21",new WLabel(wrOutDep->text()));
        form->bindWidget("C",new WLabel(hospKey->currentText()));
        form->bindWidget("_22",new WLabel(HIVobservDate->text()));
        form->bindWidget("_23",new WLabel(bloodType->text()));
        form->bindWidget("_24",new WLabel(RhAffiliation->text()));
        form->bindWidget("_25",new WLabel(VasReactDate->text()));
        form->bindWidget("_26",new WLabel(allergReact->text()));
        form->bindWidget("D",new WLabel(hospInCurrentYearKey->currentText()));
        form->bindWidget("E",new WLabel(hospRepeatedlyInMonthKey->currentText()));
        form->bindWidget("_27",new WLabel(wrOutDate->text()));
        form->bindWidget("_28",new WLabel(wrOutTime->text()));
        form->bindWidget("_29",new WLabel(hospDays->text()));
        for(auto w:form->widgets())
        {
            w->setWidth(WLength(100,WLength::Percentage));
        }
        l1->setWidth(WLength::Auto);
        l2->setWidth(WLength(30,WLength::Percentage));
        l3->setWidth(WLength(30,WLength::Percentage));
        l4->setWidth(WLength(30,WLength::Percentage));
        _editMode = false;
        show();
    }
    private: void _allocateWidgets()
    {
        ministry    = new WLineEdit(this);
        hospital    = new WLineEdit(this);
        hospitalYEDRPOU = new WLineEdit(this);
        id          = new WLineEdit(this);
        hospDate    = new WDateEdit(this);
        hospTime    = new WTimeEdit(this);
        hospDate->setFormat("dd.MM.yyyy");
        hospTime->setFormat("hh:mm");
        sexKey      = new WComboBox(this);
        sexKey->addItem("1");
        sexKey->addItem("2");
        lastName    = new WLineEdit(this);
        firstName   = new WLineEdit(this);
        fatherName  = new WLineEdit(this);
        birthDate   = new WDateEdit(this);
        birthDate->setFormat("dd.MM.yyyy");
        age         = new WLineEdit(this);
        age->setReadOnly(true);
        idDocType   = new WLineEdit(this);
        idDocNumber = new WLineEdit(this);
        citizenship = new WComboBox(this);
        for(auto c : Countries::instance().countriesList)
            citizenship->addItem(WString::fromUTF8(c.nameAlpha3));
        residenceKey      = new WComboBox(this);
        residenceKey->addItem(WString::fromUTF8("1"));
        residenceKey->addItem(WString::fromUTF8("2"));
        resZipCode      = new WLineEdit(this);
        residenceAddress = new WLineEdit(this);
        placeOfJob      = new WLineEdit(this);
        sender          = new WLineEdit(this);
        senderYEDRPOU   = new WLineEdit(this);
        hospDiag        = new WLineEdit(this);
        hospDiagMKX10   = new WLineEdit(this);
        hospDep         = new WLineEdit(this);
        wrOutDep        = new WLineEdit(this);
        hospKey      = new WComboBox(this);
        hospKey->addItem(WString::fromUTF8("1"));
        hospKey->addItem(WString::fromUTF8("2"));
        HIVobservDate   = new WDateEdit(this);
        HIVobservDate->setFormat("dd.MM.yyyy");
        bloodType       = new WLineEdit(this);
        RhAffiliation   = new WLineEdit(this);
        VasReactDate    = new WDateEdit(this);
        VasReactDate->setFormat("dd.MM.yyyy");
        allergReact     = new WLineEdit(this);
        hospInCurrentYearKey    = new WComboBox(this);
        hospInCurrentYearKey->addItem(WString::fromUTF8("1"));
        hospInCurrentYearKey->addItem(WString::fromUTF8("2"));
        hospRepeatedlyInMonthKey    = new WComboBox(this);
        hospRepeatedlyInMonthKey->addItem(WString::fromUTF8("1"));
        hospRepeatedlyInMonthKey->addItem(WString::fromUTF8("2"));
        wrOutDate   = new WDateEdit(this);
        wrOutDate->setFormat("dd.MM.yyyy");
        wrOutTime   = new WTimeEdit(this);
        wrOutTime->setFormat("hh:mm");
        hospDays    = new WLineEdit(this);
        hospDays->setReadOnly(true);
    }

    public : WindowPatient(WContainerWidget *parent) : WContainerWidget(parent)
    {
        // Load form ----------------------------------------------------------
        QFile _patientForm("F003_0.htm");
        _patientForm.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream _input(&_patientForm);
        WString str = WString::fromUTF8(_input.readAll().toStdString().data());
        _patientForm.close();

        // Prepare template ---------------------------------------------------
        form = new WTemplate(this);
        form->setTemplateText(str, Wt::XHTMLUnsafeText);

        // Allocate widgets ---------------------------------------------------
        _allocateWidgets();

        // Bind template widgets ----------------------------------------------
        setEditMode();

        // Show filled template
        this->addWidget(form);

        changeModeButton = new WPushButton("Change mode", this);
        changeModeButton->clicked().connect(std::bind([=] () {
            if(_editMode)setInfoMode();
            else setEditMode();
            }));
        this->addWidget(changeModeButton);
    }

    public : ~WindowPatient()
    {
        // bind widgets to telete them automatically
        if(!_editMode)
            setEditMode();
    }
};
}
}

#endif // WINDOWPATIENT_H
