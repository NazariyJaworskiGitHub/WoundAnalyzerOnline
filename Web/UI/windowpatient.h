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

#include <vector>
#include <Wt/WString>
#include <Wt/WFormWidget>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WDateEdit>
#include <Wt/WTimeEdit>
#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WBreak>
#include <Wt/WComboBox>
#include <Wt/WStringListModel>
#include <patient.h>

using namespace Wt;

namespace Web
{
namespace Ui
{
class WindowPatient : public WContainerWidget
{
    private:
    WVBoxLayout *layout;

    WLabel *id_Label; WLineEdit *id_LineEdit;
    WLabel *hospitalisationDate_Label; WDateEdit *hospitalisationDate_DateEdit; WTimeEdit *hospitalisationDate_TimeEdit;
    WLabel *sex_Label; WButtonGroup *sex_ButtonGroup; WRadioButton *sex_Male_RadioButton; WRadioButton *sex_Female_RadioButton;
    WLabel *lastName_Label; WLineEdit *lastName_LineEdit;
    WLabel *firstName_Label; WLineEdit *firstName_LineEdit;
    WLabel *fatherName_Label; WLineEdit *fatherName_LineEdit;
    WLabel *dateOfBirth_Label; WDateEdit *dateOfBirth_DateEdit;
    WLabel *age_Label; WLineEdit *age_LineEdit;
    WLabel *idDocumentType_Label; WLineEdit *idDocumentType_LineEdit;
    WLabel *idDocumentNumber_Label; WLineEdit *idDocumentNumber_LineEdit;
    WLabel *citizenship_Label; WComboBox *citizenship_WComboBox;
    WLabel *permanentResidenceKey_Label; WButtonGroup *permanentResidenceKey_ButtonGroup; WRadioButton *permanentResidenceKey_City_RadioButton; WRadioButton *permanentResidenceKey_Country_RadioButton; WLineEdit *permanentResidenceAddress_LineEdit;
    WLabel *permanentResidenceZipCode_Label; WLineEdit *permanentResidenceZipCode_LineEdit;
    WLabel *placeOfJob_Label; WLineEdit *placeOfJob_LineEdit;
    WLabel *sender_Label; WLineEdit *sender_LineEdit; WLineEdit *senderCodeYEDRPOU_LineEdit;
    WLabel *hospitalisationDiagnosis_Label; WLineEdit *hospitalisationDiagnosis_LineEdit; WLineEdit *hospitalisationDiagnosisCodeMKX10_LineEdit;
    WLabel *hospitalisationDepartment_Label; WLineEdit *hospitalisationDepartment_LineEdit;
    WLabel *writingOutDepartment_Label; WLineEdit *writingOutDepartment_LineEdit;
    WLabel *hospitalisationKey_Label; WButtonGroup *hospitalisationKey_ButtonGroup; WRadioButton *hospitalisationKey_Urgent_RadioButton; WRadioButton *hospitalisationKey_Planned_RadioButton;
    WLabel *HIV_AIDSobservation_Label; WDateEdit *HIV_AIDSobservation_DateEdit;
    WLabel *bloodType_Label; WLineEdit *bloodType_LineEdit;
    WLabel *RhAffiliation_Label; WLineEdit *RhAffiliation_LineEdit;
    WLabel *VasermanaReaction_Label; WDateEdit *VasermanaReaction_DateEdit;
    WLabel *allergicReactions_Label; WLineEdit *allergicReactions_LineEdit;
    WLabel *hospitalisationInCurrentYear_Label; WButtonGroup *hospitalisationInCurrentYear_ButtonGroup; WRadioButton *hospitalisationInCurrentYear_Firstly_RadioButton; WRadioButton *hospitalisationInCurrentYear_Repeatedly_RadioButton;
    WLabel *hospitalisationRepeatedlyInMonth_Label; WButtonGroup *hospitalisationRepeatedlyInMonth_ButtonGroup; WRadioButton *hospitalisationRepeatedlyInMonth_Yes_RadioButton; WRadioButton *hospitalisationRepeatedlyInMonth_No_RadioButton;
    WLabel *writingOutDate_Label; WDateEdit *writingOutDate_DateEdit; WTimeEdit *writingOutDate_TimeEdit;
    WLabel *hospitalisationDays_Label; WLineEdit *hospitalisationDays_LineEdit;

    std::vector<WHBoxLayout*> _localLayouts;
    std::vector<WVBoxLayout*> _localLayoutsForTips;

    private: void _addTip(char *str, WWidget *w) noexcept
    {
        w->setToolTip(WString::fromUTF8(str));
        _localLayoutsForTips.push_back(new WVBoxLayout());
        _localLayoutsForTips.back()->addWidget(w);
        WLabel *l = new WLabel(WString::fromUTF8(str),this);
        if(str[0])_localLayoutsForTips.back()->addWidget(l);
        else _localLayoutsForTips.back()->addStretch(1);
        _localLayouts.back()->addLayout(_localLayoutsForTips.back());
    }

    public : WindowPatient(WContainerWidget *parent) : WContainerWidget(parent)
    {
        layout = new WVBoxLayout(this);

        id_Label    = new WLabel(WString::fromUTF8("МЕДИЧНА КАРТА СТАЦІОНАРНОГО ХВОРОГО №"), this);
        id_LineEdit = new WLineEdit(this);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(id_Label);
        _localLayouts.back()->addWidget(id_LineEdit);
        layout->addLayout(_localLayouts.back());

        hospitalisationDate_Label       = new WLabel(WString::fromUTF8("1. Дата госпіталізації"), this);
        hospitalisationDate_DateEdit  = new WDateEdit(this);
        hospitalisationDate_TimeEdit    = new WTimeEdit(this);
        hospitalisationDate_DateEdit->setFormat("dd.MM.yyyy");
        hospitalisationDate_TimeEdit->setFormat("hh:mm");
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(hospitalisationDate_Label);
        _addTip("(число, місяць, рік)", hospitalisationDate_DateEdit);
        _addTip("(година, хвилини)", hospitalisationDate_TimeEdit);
        sex_Label               = new WLabel(WString::fromUTF8("2. Стать"), this);
        sex_ButtonGroup         = new WButtonGroup(this);
        sex_Male_RadioButton    = new WRadioButton(WString::fromUTF8("чоловіча"), this);
        sex_Female_RadioButton  = new WRadioButton(WString::fromUTF8("жіноча"), this);
        sex_ButtonGroup->addButton(sex_Male_RadioButton);
        sex_ButtonGroup->addButton(sex_Female_RadioButton);
        _localLayouts.back()->addWidget(sex_Label);
        _localLayouts.back()->addWidget(sex_Male_RadioButton);
        _localLayouts.back()->addWidget(sex_Female_RadioButton);
        layout->addLayout(_localLayouts.back());

        lastName_Label      = new WLabel(WString::fromUTF8("3. Прізвище"), this);
        lastName_LineEdit   = new WLineEdit(this);
        firstName_Label     = new WLabel(WString::fromUTF8("Ім'я"), this);
        firstName_LineEdit  = new WLineEdit(this);
        fatherName_Label    = new WLabel(WString::fromUTF8("По батькові"), this);
        fatherName_LineEdit = new WLineEdit(this);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(lastName_Label);
        _localLayouts.back()->addWidget(lastName_LineEdit);
        _localLayouts.back()->addWidget(firstName_Label);
        _localLayouts.back()->addWidget(firstName_LineEdit);
        _localLayouts.back()->addWidget(fatherName_Label);
        _localLayouts.back()->addWidget(fatherName_LineEdit);
        layout->addLayout(_localLayouts.back());

        dateOfBirth_Label       = new WLabel(WString::fromUTF8("4. Дата народження"), this);
        dateOfBirth_DateEdit    = new WDateEdit(this);
        dateOfBirth_DateEdit->setFormat("dd.MM.yyyy");
        age_Label        = new WLabel(WString::fromUTF8("5. Вік"), this);
        age_LineEdit     = new WLineEdit(this);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(dateOfBirth_Label);
        _addTip("(число, місяць, рік)", dateOfBirth_DateEdit);
        _localLayouts.back()->addWidget(age_Label);
        _addTip("(кількість повних років)", age_LineEdit);
        layout->addLayout(_localLayouts.back());

        idDocumentType_Label        = new WLabel(WString::fromUTF8("6. Документ, що посвідчує особу"), this);
        idDocumentType_LineEdit     = new WLineEdit(this);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(idDocumentType_Label);
        _localLayouts.back()->addWidget(idDocumentType_LineEdit);
        _localLayouts.back()->addWidget(new WBreak(this));
        _localLayouts.back()->addWidget(new WBreak(this));
        layout->addLayout(_localLayouts.back());

        idDocumentNumber_Label        = new WLabel(WString::fromUTF8("6.1. Номер документа, що посвідчує особу"), this);
        idDocumentNumber_LineEdit     = new WLineEdit(this);
        citizenship_Label        = new WLabel(WString::fromUTF8("6.2. Громадянство"), this);
        citizenship_WComboBox    = new WComboBox(this);
        for(auto c : Countries::instance().countriesList)
            citizenship_WComboBox->addItem(WString::fromUTF8(c.nameAlpha3));
        citizenship_WComboBox->setCurrentIndex(UA_COUNTRY_INDEX);

        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(idDocumentNumber_Label);
        _localLayouts.back()->addWidget(idDocumentNumber_LineEdit);
        _localLayouts.back()->addWidget(citizenship_Label);
        _addTip("(код країни)", citizenship_WComboBox);
        layout->addLayout(_localLayouts.back());

        permanentResidenceKey_Label               = new WLabel(WString::fromUTF8("7. Місце проживання"), this);
        permanentResidenceKey_ButtonGroup         = new WButtonGroup(this);
        permanentResidenceKey_City_RadioButton    = new WRadioButton(WString::fromUTF8("місто"), this);
        permanentResidenceKey_Country_RadioButton = new WRadioButton(WString::fromUTF8("село"), this);
        permanentResidenceAddress_LineEdit        = new WLineEdit(this);
        permanentResidenceKey_ButtonGroup->addButton(permanentResidenceKey_City_RadioButton);
        permanentResidenceKey_ButtonGroup->addButton(permanentResidenceKey_Country_RadioButton);
        permanentResidenceZipCode_Label     = new WLabel(WString::fromUTF8("7.1. Поштовий індекс"), this);
        permanentResidenceZipCode_LineEdit  = new WLineEdit(this);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(permanentResidenceKey_Label);
        _localLayouts.back()->addWidget(permanentResidenceKey_City_RadioButton);
        _localLayouts.back()->addWidget(permanentResidenceKey_Country_RadioButton);
        _addTip("(область, район, населений пункт, вулиця, будинок, квартира)", permanentResidenceAddress_LineEdit);
        _localLayouts.back()->addWidget(permanentResidenceZipCode_Label);
        _addTip("", permanentResidenceZipCode_LineEdit);
        layout->addLayout(_localLayouts.back());

        placeOfJob_Label        = new WLabel(WString::fromUTF8("8. Місце роботи, посада"), this);
        placeOfJob_LineEdit     = new WLineEdit(this);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(placeOfJob_Label);
        _addTip("(для дітей, учнів, студентів – найменування навчального закладу, пільгова категорія; для інвалідів –  вид і група інвалідності)", placeOfJob_LineEdit);

        layout->addLayout(_localLayouts.back());

        sender_Label        = new WLabel(WString::fromUTF8("9. Ким направлений хворий"), this);
        sender_LineEdit     = new WLineEdit(this);
        senderCodeYEDRPOU_LineEdit     = new WLineEdit(this);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(sender_Label);
        _addTip("(найменування закладу охорони здоров’я)", sender_LineEdit);
        _addTip("(код за ЄДРПОУ)", senderCodeYEDRPOU_LineEdit);
        layout->addLayout(_localLayouts.back());

        hospitalisationDiagnosis_Label        = new WLabel(WString::fromUTF8("10. Діагноз при госпіталізації"), this);
        hospitalisationDiagnosis_LineEdit     = new WLineEdit(this);
        hospitalisationDiagnosisCodeMKX10_LineEdit     = new WLineEdit(this);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(hospitalisationDiagnosis_Label);
        _localLayouts.back()->addWidget(hospitalisationDiagnosis_LineEdit);
        _addTip("(код за МКХ-10)", hospitalisationDiagnosisCodeMKX10_LineEdit);
        layout->addLayout(_localLayouts.back());

        hospitalisationDepartment_Label        = new WLabel(WString::fromUTF8("11. Відділення при госпіталізації"), this);
        hospitalisationDepartment_LineEdit     = new WLineEdit(this);
        writingOutDepartment_Label        = new WLabel(WString::fromUTF8("12. Відділення при виписці"), this);
        writingOutDepartment_LineEdit     = new WLineEdit(this);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(hospitalisationDepartment_Label);
        _addTip("(код)", hospitalisationDepartment_LineEdit);
        _localLayouts.back()->addWidget(writingOutDepartment_Label);
        _addTip("(код)", writingOutDepartment_LineEdit);
        layout->addLayout(_localLayouts.back());

        hospitalisationKey_Label               = new WLabel(WString::fromUTF8("13. Госпіталізація"), this);
        hospitalisationKey_ButtonGroup         = new WButtonGroup(this);
        hospitalisationKey_Urgent_RadioButton  = new WRadioButton(WString::fromUTF8("ургентна"), this);
        hospitalisationKey_Planned_RadioButton = new WRadioButton(WString::fromUTF8("планова"), this);
        hospitalisationKey_ButtonGroup->addButton(hospitalisationKey_Urgent_RadioButton);
        hospitalisationKey_ButtonGroup->addButton(hospitalisationKey_Planned_RadioButton);
        HIV_AIDSobservation_Label       = new WLabel(WString::fromUTF8("14. Обстеження на ВІЛ-інфекцію"), this);
        HIV_AIDSobservation_DateEdit  = new WDateEdit(this);
        HIV_AIDSobservation_DateEdit->setFormat("dd.MM.yyyy");
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(hospitalisationKey_Label);
        _localLayouts.back()->addWidget(hospitalisationKey_Urgent_RadioButton);
        _localLayouts.back()->addWidget(hospitalisationKey_Planned_RadioButton);
        _localLayouts.back()->addWidget(HIV_AIDSobservation_Label);
        _addTip("(число, місяць, рік)", HIV_AIDSobservation_DateEdit);
        layout->addLayout(_localLayouts.back());

        bloodType_Label        = new WLabel(WString::fromUTF8("16. Група крові"), this);
        bloodType_LineEdit     = new WLineEdit(this);
        RhAffiliation_Label        = new WLabel(WString::fromUTF8("16. Резус-приналежність"), this);
        RhAffiliation_LineEdit     = new WLineEdit(this);
        VasermanaReaction_Label       = new WLabel(WString::fromUTF8("17. Реакція Васермана"), this);
        VasermanaReaction_DateEdit    = new WDateEdit(this);
        VasermanaReaction_DateEdit->setFormat("dd.MM.yyyy");
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(bloodType_Label);
        _localLayouts.back()->addWidget(bloodType_LineEdit);
        _localLayouts.back()->addWidget(RhAffiliation_Label);
        _localLayouts.back()->addWidget(RhAffiliation_LineEdit);
        _localLayouts.back()->addWidget(VasermanaReaction_Label);
        _addTip("(число, місяць, рік)", VasermanaReaction_DateEdit);
        layout->addLayout(_localLayouts.back());

        allergicReactions_Label        = new WLabel(WString::fromUTF8("18. Алергічні реакції, гіперчутливість чи непереносимість лікарського засобу"), this);
        allergicReactions_LineEdit     = new WLineEdit(this);
        layout->addWidget(allergicReactions_Label);
        layout->addWidget(allergicReactions_LineEdit);

        hospitalisationInCurrentYear_Label                  = new WLabel(WString::fromUTF8("19. Госпіталізація з приводу цього захворювання в цьому році"), this);
        hospitalisationInCurrentYear_ButtonGroup            = new WButtonGroup(this);
        hospitalisationInCurrentYear_Firstly_RadioButton    = new WRadioButton(WString::fromUTF8("вперше"), this);
        hospitalisationInCurrentYear_Repeatedly_RadioButton = new WRadioButton(WString::fromUTF8("повторно"), this);
        hospitalisationInCurrentYear_ButtonGroup->addButton(hospitalisationInCurrentYear_Firstly_RadioButton);
        hospitalisationInCurrentYear_ButtonGroup->addButton(hospitalisationInCurrentYear_Repeatedly_RadioButton);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(hospitalisationInCurrentYear_Label);
        _localLayouts.back()->addWidget(hospitalisationInCurrentYear_Firstly_RadioButton);
        _localLayouts.back()->addWidget(hospitalisationInCurrentYear_Repeatedly_RadioButton);
        layout->addLayout(_localLayouts.back());

        hospitalisationRepeatedlyInMonth_Label              = new WLabel(WString::fromUTF8("19.1. Повторна госпіталізація з приводу цього захворювання до 30 днів"), this);
        hospitalisationRepeatedlyInMonth_ButtonGroup        = new WButtonGroup(this);
        hospitalisationRepeatedlyInMonth_Yes_RadioButton    = new WRadioButton(WString::fromUTF8("так"), this);
        hospitalisationRepeatedlyInMonth_No_RadioButton     = new WRadioButton(WString::fromUTF8("ні"), this);
        hospitalisationRepeatedlyInMonth_ButtonGroup->addButton(hospitalisationRepeatedlyInMonth_Yes_RadioButton);
        hospitalisationRepeatedlyInMonth_ButtonGroup->addButton(hospitalisationRepeatedlyInMonth_No_RadioButton);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(hospitalisationRepeatedlyInMonth_Label);
        _localLayouts.back()->addWidget(hospitalisationRepeatedlyInMonth_Yes_RadioButton);
        _localLayouts.back()->addWidget(hospitalisationRepeatedlyInMonth_No_RadioButton);
        layout->addLayout(_localLayouts.back());

        writingOutDate_Label       = new WLabel(WString::fromUTF8("20. Дата виписки/смерті"), this);
        writingOutDate_DateEdit  = new WDateEdit(this);
        writingOutDate_TimeEdit    = new WTimeEdit(this);
        writingOutDate_DateEdit->setFormat("dd.MM.yyyy");
        writingOutDate_TimeEdit->setFormat("hh:mm");
        hospitalisationDays_Label        = new WLabel(WString::fromUTF8("21. Проведено ліжко-днів"), this);
        hospitalisationDays_LineEdit     = new WLineEdit(this);
        _localLayouts.push_back(new WHBoxLayout());
        _localLayouts.back()->addWidget(writingOutDate_Label);
        _addTip("(число, місяць, рік)", writingOutDate_DateEdit);
        _addTip("(година, хвилини)", writingOutDate_TimeEdit);
        _localLayouts.back()->addWidget(hospitalisationDays_Label);
        _localLayouts.back()->addWidget(hospitalisationDays_LineEdit);
        layout->addLayout(_localLayouts.back());

        this->setLayout(layout);
    }

    public : ~WindowPatient()
    {
        for(auto l:_localLayouts)delete l;
        for(auto l:_localLayoutsForTips)delete l;
    }
};
}
}

#endif // WINDOWPATIENT_H
