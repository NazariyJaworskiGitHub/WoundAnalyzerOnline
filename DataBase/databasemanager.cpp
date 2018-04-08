#include "databasemanager.h"

#include <QtSql/QSqlDatabase>

#include "Utilities/Logger/logger.h"

DatabaseManager::DatabaseManager(QObject *parent):
    QObject(parent),
    _isConnectedFlag(false)
{
}

bool DatabaseManager::connectToDatabase(
        QString hName, QString dbName, QString uName, QString password)
{
    Log::GlobalLogger.msg(Log::INFO,"[Database] connecting to <" +
                          dbName.toStdString() + "> at <" +
                          hName.toStdString() + "> with login <" +
                          uName.toStdString() + "> ...\n");

    QSqlDatabase _db = QSqlDatabase::addDatabase(
                "QMYSQL",DATABASENAME);
//    _db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=WoundData.mdb");
    _db.setHostName(hName);
    _db.setDatabaseName(dbName);
    _db.setUserName(uName);
    _db.setPassword(password);
    _db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
    _isConnectedFlag = _db.open();

    if(_isConnectedFlag)
        Log::GlobalLogger.msg(Log::INFO,"[Database] ... connection succeed\n");
    else
        Log::GlobalLogger.msg(Log::ERROR, "[Database] <FAIL> "
                                         + lastError().text().toStdString() + "\n");

    return _isConnectedFlag;
}

QSqlError DatabaseManager::lastError() const
{
    return QSqlDatabase::database(DATABASENAME, false).lastError();
}

DatabaseManager::~DatabaseManager()
{
    if(_isConnectedFlag)
    {
        {   //barcet statement for local variable _db
            QSqlDatabase _db = QSqlDatabase::database(DATABASENAME, false);
            _db.close();
        }
        QSqlDatabase::removeDatabase(DATABASENAME);
    }
}
