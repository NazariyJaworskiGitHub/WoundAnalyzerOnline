#include "databasemanagerwt.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QTextStream>
#include <QStringList>

#include "Web/usersession.h"
#include "Utilities/Logger/logger.h"
#include "configurationparameters.h"

void DatabaseManagerWt::connectToDatabase() throw (std::exception)
{
    if(!DatabaseManager::connectToDatabase(
                ConfigurationParameters::instance()->databaseParameters.HostName,
                ConfigurationParameters::instance()->databaseParameters.DatabaseName,
                ConfigurationParameters::instance()->databaseParameters.UserName,
                ConfigurationParameters::instance()->databaseParameters.UserPassword))
    {
        std::string errMsg = "[Database] Can't connect to database. The program should be terminated.\n";
        Log::GlobalLogger.msg(Log::ERROR, errMsg);
        throw std::runtime_error(errMsg);
    }
}

DatabaseModel* DatabaseManagerWt::prepareDatabaseModel(WContainerWidget *parent)
{
    if(!CURRENT_SESSION->currentUser)
    {
        Log::GlobalLogger.msg(
                    Log::ERROR,
                    "[Database] Error: unauthenticated user from session <" +
                    WApplication::instance()->sessionId() +
                    "> is trying to connect to database\n");
        return nullptr;
    }

    QSqlRecord record;
    QString str;
    DatabaseModel* DBmodel = new DatabaseModel(parent);

    /////////////////////////////////////////////////////////////////////////////////////
    {
        QSqlQuery doctorsQuery(QSqlDatabase::database(DATABASENAME));
        Log::GlobalLogger.msg(Log::TRACE, "[Database] searching id of <" + CURRENT_SESSION->currentUser->username + ">\n");
        str = ("SELECT ID, DoctorName, Notes FROM Doctors WHERE Login = '" + CURRENT_SESSION->currentUser->username + "';").data();
        if(doctorsQuery.exec(str))
            Log::GlobalLogger.msg(Log::TRACE, "[Database] id found\n");
        else
        {
            Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + doctorsQuery.lastError().text().toStdString() + "\n");
            delete DBmodel;
            return nullptr;
        }
        doctorsQuery.next();
        record = doctorsQuery.record();
        DatabaseModel::Doctor *newDoctor = new DatabaseModel::Doctor(
                    doctorsQuery.value(record.indexOf("ID")).toInt(),
                    doctorsQuery.value(record.indexOf("DoctorName")).toString().toStdString(),
                    doctorsQuery.value(record.indexOf("Notes")).toString().toStdString());
        DBmodel->tree->setTreeRoot(newDoctor);
        DBmodel->doctor = newDoctor;
    }
    //////////////////////////////////////////////////////////////////////////////////////
    {
        QSqlQuery patientsQuery(QSqlDatabase::database(DATABASENAME));
        Log::GlobalLogger.msg(Log::TRACE, "[Database] loading patients of <" + CURRENT_SESSION->currentUser->username + ">\n");
        str = "SELECT ID, PatientName, Notes FROM Patients WHERE DoctorID = " + QString::number(DBmodel->doctor->id) + " ORDER BY PatientName ASC;";
        if(patientsQuery.exec(str))
            Log::GlobalLogger.msg(Log::TRACE, "[Database] patients are loaded\n");
        else
        {
            Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + patientsQuery.lastError().text().toStdString() + "\n");
            delete DBmodel;
            return nullptr;
        }
        while(patientsQuery.next())
        {
            record = patientsQuery.record();
            DatabaseModel::Patient * patient = new DatabaseModel::Patient(
                        patientsQuery.value(record.indexOf("ID")).toInt(),
                        patientsQuery.value(record.indexOf("PatientName")).toString().toStdString(),
                        patientsQuery.value(record.indexOf("Notes")).toString().toStdString());
            DBmodel->doctor->addChildNode(patient);

            ///////////////////////////////////////////////////////////////////////////////
            {
                QSqlQuery woundsQuery(QSqlDatabase::database(DATABASENAME));
                Log::GlobalLogger.msg(Log::TRACE, "[Database] loading wounds of <" + patient->name + ">\n");
                str = "SELECT ID, WoundName, Notes FROM Wounds WHERE PatientID = " + QString::number(patient->id) + " ORDER BY WoundName ASC;";
                if(woundsQuery.exec(str))
                    Log::GlobalLogger.msg(Log::TRACE, "[Database] wounds are loaded\n");
                else
                {
                    Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + woundsQuery.lastError().text().toStdString() + "\n");
                    delete DBmodel;
                    return nullptr;
                }
                while(woundsQuery.next())
                {
                    record = woundsQuery.record();
                    DatabaseModel::Wound * wound = new DatabaseModel::Wound(
                                woundsQuery.value(record.indexOf("ID")).toInt(),
                                woundsQuery.value(record.indexOf("WoundName")).toString().toStdString(),
                                woundsQuery.value(record.indexOf("Notes")).toString().toStdString());
                    patient->addChildNode(wound);
                    ///////////////////////////////////////////////////////////////////////
                    {
                        QSqlQuery surveyQuery(QSqlDatabase::database(DATABASENAME));
                        Log::GlobalLogger.msg(Log::TRACE, "[Database] loading surveys of <" + wound->name + ">\n");
                        str = "SELECT ID, SurveyDate, Notes, WoundArea FROM Surveys WHERE WoundID = " + QString::number(wound->id) + " ORDER BY SurveyDate ASC;";
                        if(surveyQuery.exec(str))
                            Log::GlobalLogger.msg(Log::TRACE, "[Database] surveys are loaded\n");
                        else
                        {
                            Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + surveyQuery.lastError().text().toStdString() + "\n");
                            delete DBmodel;
                            return nullptr;
                        }
                        while(surveyQuery.next())
                        {
                            record = surveyQuery.record();
                            DatabaseModel::Survey * survey = new DatabaseModel::Survey(
                                        surveyQuery.value(record.indexOf("ID")).toInt(),
                                        surveyQuery.value(record.indexOf("SurveyDate")).toDateTime(),
                                        surveyQuery.value(record.indexOf("Notes")).toString().toStdString(),
                                        surveyQuery.value(record.indexOf("WoundArea")).toDouble());
                                wound->addChildNode(survey);
                        }
                    }
                    ///////////////////////////////////////////////////////////////////////
                }
            }
            ///////////////////////////////////////////////////////////////////////////////
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////
    if(CURRENT_SESSION->currentDatabaseModel) delete
        CURRENT_SESSION->currentDatabaseModel;
    CURRENT_SESSION->currentDatabaseModel = DBmodel;
    return DBmodel;
}

bool DatabaseManagerWt::loadSurveyWoundImage(DatabaseModel::Survey *target) const
{
    QSqlRecord record;
    QString str;
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::GlobalLogger.msg(Log::TRACE, "[Database] loading survey wound image of <" + target->date.toString("dd.MM.yyyy hh:mm").toStdString() + ">\n");
    str = "SELECT Image, Polygons, RulerPoints, RulerFactor, Zoom FROM Surveys WHERE ID = " + QString::number(target->id) +";";
    if(query.exec(str))
        Log::GlobalLogger.msg(Log::TRACE, "[Database] survey wound image is loaded\n");
    else
    {
        Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + query.lastError().text().toStdString() + "\n");
        return false;
    }
    query.next();
    record = query.record();
    QByteArray byteArray1 = query.value(record.indexOf("Image")).toByteArray();
    if(!byteArray1.isEmpty())
    {
        std::vector<char> v(byteArray1.data(), byteArray1.data() + byteArray1.length());
        target->image = cv::imdecode(v, cv::IMREAD_COLOR);
    }
    else
    {
        Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: survey wound image is empty\n");
        return false;
    }
    target->unpackPolygons(query.value(record.indexOf("Polygons")).toByteArray());
    target->unpackRulerPoints(query.value(record.indexOf("RulerPoints")).toByteArray());
    target->rulerFactor = query.value(record.indexOf("RulerFactor")).toDouble();
    target->zoom = query.value(record.indexOf("Zoom")).toDouble();
    return true;
}

bool DatabaseManagerWt::_updateUtil(
        const std::string &param,
        const std::string &name,
        const std::string &notes,
        const int id) const
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::GlobalLogger.msg(Log::TRACE, "[Database] updating <" + param + "> <" + name + ">\n");
    query.prepare(("UPDATE " + param + "s SET " + param + "Name = '" + name +
                  "', Notes = :notes WHERE ID = ").data() + QString::number(id) + ";");
    query.bindValue(":notes", notes.data());
    if(query.exec())
        Log::GlobalLogger.msg(Log::TRACE, "[Database] <" + param + "> is updated \n");
    else
    {
        Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + query.lastError().text().toStdString() + "\n");
        return false;
    }
    return true;
}

bool DatabaseManagerWt::update(DatabaseModel::Patient *target)
{
    return _updateUtil("patient", target->name, target->notes, target->id);
}

bool DatabaseManagerWt::update(DatabaseModel::Wound *target)
{
    return _updateUtil("wound", target->name, target->notes, target->id);
}

bool DatabaseManagerWt::update(DatabaseModel::Doctor *target)
{
    return _updateUtil("doctor", target->name, target->notes, target->id);
}

bool DatabaseManagerWt::update(DatabaseModel::Survey *target)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::GlobalLogger.msg(Log::TRACE, "[Database] updating survey <" + target->date.toString("dd.MM.yyyy hh:mm").toStdString() + ">\n");
    std::vector<unsigned char> v;
    cv::imencode(".jpg", target->image, v);
    query.prepare(
            "UPDATE Surveys SET "
            "SurveyDate = '" + target->date.toString("yyyy-MM-dd hh:mm:ss") + "', "
            "Notes = :notes, "
            "Image = :imageData, "
            "Polygons = :polygonsData, "
            "RulerPoints = :rulerPoints, "
            "RulerFactor = " + QString::number(target->rulerFactor) + ", "
            "WoundArea = " + QString::number(target->woundArea) + ", "
            "Zoom = " + QString::number(target->zoom) + " "
            "WHERE ID = " + QString::number(target->id) + ";");
    query.bindValue(":notes", target->notes.data());
    query.bindValue(":imageData", QByteArray(reinterpret_cast<const char*>(v.data()),v.size()));
    query.bindValue(":polygonsData", target->packPolygons());
    query.bindValue(":rulerPoints", target->packRulerPoints());
    if(query.exec())
        Log::GlobalLogger.msg(Log::TRACE, "[Database] survey is updated \n");
    else
    {
        Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + query.lastError().text().toStdString() + "\n");
        return false;
    }
    return true;
}

void DatabaseManagerWt::add(
        DatabaseModel::Doctor *parent,
        DatabaseModel::Patient *newTarget)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::GlobalLogger.msg(Log::TRACE, "[Database] adding new patient to doctor <" + parent->name + ">\n");
    query.prepare(
                "INSERT INTO Patients (DoctorID, PatientName)"
                "VALUES ('" + QString::number(parent->id) + "','New patient');");
    if(query.exec())
        Log::GlobalLogger.msg(Log::TRACE, "[Database] new patient is added \n");
    else
    {
        Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + query.lastError().text().toStdString() + "\n");
        return;
    }
    newTarget->id = query.lastInsertId().toInt();
}

void DatabaseManagerWt::add(
        DatabaseModel::Patient *parent,
        DatabaseModel::Wound   *newTarget)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::GlobalLogger.msg(Log::TRACE, "[Database] adding new wound to patient <" + parent->name + ">\n");
    query.prepare(
                "INSERT INTO Wounds (PatientID, WoundName)"
                "VALUES ('" + QString::number(parent->id) + "','New wound');");
    if(query.exec())
        Log::GlobalLogger.msg(Log::TRACE, "[Database] new wound is added \n");
    else
    {
        Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + query.lastError().text().toStdString() + "\n");
        return;
    }
    newTarget->id = query.lastInsertId().toInt();
}

void DatabaseManagerWt::add(
        DatabaseModel::Wound *parent,
        DatabaseModel::Survey *newTarget)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::GlobalLogger.msg(Log::TRACE, "[Database] adding new survey to wound <" + parent->name + ">\n");
    query.prepare(
            "INSERT INTO Surveys SET "
            "WoundID = "+ QString::number(parent->id) + ", " +
            "SurveyDate = '" + newTarget->date.toString("yyyy-MM-dd hh:mm:ss") + "', "
            "Notes = :notes, "
            "Image = :imageData, "
            "Polygons = :polygonsData, "
            "RulerPoints = :rulerPoints, "
            "RulerFactor = " + QString::number(newTarget->rulerFactor) + ", "
            "WoundArea = " + QString::number(newTarget->woundArea) + ", "
            "Zoom = " + QString::number(newTarget->zoom) + ";") ;
    std::vector<unsigned char> v;
    cv::imencode(".jpg", newTarget->image.clone(), v);
    query.bindValue(":notes", newTarget->notes.data());
    query.bindValue(":imageData", QByteArray(reinterpret_cast<const char*>(v.data()),v.size()));
    query.bindValue(":polygonsData", newTarget->packPolygons());
    query.bindValue(":rulerPoints", newTarget->packRulerPoints());
    if(query.exec())
        Log::GlobalLogger.msg(Log::TRACE, "[Database] new survey is added \n");
    else
    {
        Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + query.lastError().text().toStdString() + "\n");
        return;
    }
    newTarget->id = query.lastInsertId().toInt();
}

void DatabaseManagerWt::del(DatabaseModel::Survey *target)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::GlobalLogger.msg(Log::TRACE, "[Database] deleting survey <" + target->date.toString("dd.MM.yyyy hh:mm").toStdString() + ">\n");
    query.prepare( "DELETE FROM Surveys WHERE ID = " + QString::number(target->id));
    if(query.exec())
        Log::GlobalLogger.msg(Log::TRACE, "[Database] survey is deleted \n");
    else
    {
        Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + query.lastError().text().toStdString() + "\n");
    }
}

void DatabaseManagerWt::del(DatabaseModel::Wound *target)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::GlobalLogger.msg(Log::TRACE, "[Database] deleting wound <" + target->name + ">\n");
    query.prepare( "DELETE FROM Wounds WHERE ID = " + QString::number(target->id));
    if(query.exec())
        Log::GlobalLogger.msg(Log::TRACE, "[Database] wound is deleted \n");
    else
    {
        Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + query.lastError().text().toStdString() + "\n");
    }
}

void DatabaseManagerWt::del(DatabaseModel::Patient *target)
{
    QSqlQuery query(QSqlDatabase::database(DATABASENAME));
    Log::GlobalLogger.msg(Log::TRACE, "[Database] deleting patient " + target->name + "\n");
    query.prepare( "DELETE FROM Patients WHERE ID = " + QString::number(target->id));
    if(query.exec())
        Log::GlobalLogger.msg(Log::TRACE, "[Database] patient is deleted \n");
    else
    {
        Log::GlobalLogger.msg(Log::ERROR, "[Database] Error: " + query.lastError().text().toStdString() + "\n");
    }
}
