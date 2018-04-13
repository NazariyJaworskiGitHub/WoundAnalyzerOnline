#ifndef DATABASEMANAGERWT_H
#define DATABASEMANAGERWT_H

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <Wt/WContainerWidget>

#include "DataBase/databasemanager.h"
#include "DataBase/databasemodel.h"

using namespace Wt;
using namespace Web::Ui;

class DatabaseManagerWt : public DatabaseManager
{
    Q_OBJECT

    public : void connectToDatabase() throw (std::exception);
    public : DatabaseModel* prepareDatabaseModel(WContainerWidget *parent);
    public : bool loadSurveyWoundImage(DatabaseModel::Survey *target) const;
    private: bool _updateUtil(
            const std::string &param,
            const std::string &name,
            const std::string &notes,
            const int id) const;

    public : bool update(DatabaseModel::Patient   *target);
    public : bool update(DatabaseModel::Wound     *target);
    public : bool update(DatabaseModel::Doctor    *target);
    public : bool update(DatabaseModel::Survey    *target);

    public : void add(DatabaseModel::Doctor  *parent, DatabaseModel::Doctor  *newTarget){} //stub
    public : void add(DatabaseModel::Doctor  *parent, DatabaseModel::Patient *newTarget);
    public : void add(DatabaseModel::Patient *parent, DatabaseModel::Wound   *newTarget);
    public : void add(DatabaseModel::Wound   *parent, DatabaseModel::Survey  *newTarget);

    public : void del(DatabaseModel::Survey  *target);
    public : void del(DatabaseModel::Wound   *target);
    public : void del(DatabaseModel::Patient *target);

    private: DatabaseManagerWt(QObject *parent = nullptr): DatabaseManager(parent){}
    public : static DatabaseManagerWt *instance()
    {
        static DatabaseManagerWt _instanceOfDatabaseManager;
        return &_instanceOfDatabaseManager;
    }
};

#endif // DATABASEMANAGERWT_H

