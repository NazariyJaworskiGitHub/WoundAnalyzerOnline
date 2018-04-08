#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

/// \warning don't forget qsqlmysql.dll and libmysql.dll

///// \warning make shure that you are using compiler native libstdc...dll,
///// but not QT libstdc...dll, or it causes errors
///// see http://forum.qt.io/topic/45306/solved-runtime-error-with-mingw-ifstream-qt-creator/11

#include <QObject>
#include <QtSql/QSqlError>

#define DATABASENAME "myDatabase"

class DatabaseManager : public QObject
{
    Q_OBJECT

        /// This stupid Qt provides access to QSqlDatabase only by the instances,
        /// so i have to create unique names instead unique objects
        /// if i connected then true, else false, if true at destruction then i have
        /// to destroy QSqlDatabase instance with _myDatabaseConnectionName
    private: bool _isConnectedFlag;
    public : bool isConnected() const {return _isConnectedFlag;}
        /// Here should be QSqlDatabase::lastError()
    public : QSqlError lastError() const;
        /// Connect to database
        /// Try to connect to database by given data, returns false if fail or true
        /// opened connection will has name _myDatabaseConnectionName
    public : bool connectToDatabase(
                QString hName, QString dbName, QString uName, QString password);

    protected: DatabaseManager(QObject *parent = nullptr);

    protected: ~DatabaseManager();
};

#endif // DATABASEMANAGER_H
