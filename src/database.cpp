#include "database.h"
#include <QDebug>
#include <QDate>

const QString DataBaseSettings::type = "QSQLITE";
const QString DataBaseSettings::hostname = "localhost";
const QString DataBaseSettings::name = "db.db";

const QString ActivityTable::nameTable = "act_table";

const QString ActivityTable::id = "act_id";
const QString ActivityTable::date = "act_date";
const QString ActivityTable::time = "act_time";
const QString ActivityTable::description = "act_descr";
const QString ActivityTable::category = "act_cat";
const QString ActivityTable::duration = "act_dur";
const QString ActivityTable::running = "act_run";

DataBase::DataBase()
{
    db = QSqlDatabase::addDatabase(DataBaseSettings::type);
    db.setHostName(DataBaseSettings::hostname);
    db.setDatabaseName(qApp->applicationDirPath() + "/" + DataBaseSettings::name);

    if(db.open())
        qDebug() << "DB opened";
    else
        qDebug() << "DB don't opened";

    QSqlQuery queryTable;
    queryTable.prepare(
        "CREATE TABLE " + ActivityTable::nameTable + " (" +
        ActivityTable::id + " INTEGER PRIMARY KEY AUTOINCREMENT, " +
        ActivityTable::date + " DATE, " +
        ActivityTable::time + " TIME, " +
        ActivityTable::description + " TEXT, " +
        ActivityTable::category + " TEXT, " +
        ActivityTable::duration + " TIME, " +
        ActivityTable::running + " INTEGER" +
                                                      ")"

                      );

    if(!queryTable.exec())
    {
        qDebug() << "Error create table";
        qDebug() << queryTable.lastError().text();
    }
    else
        qDebug() << "Created table";

    currentActivityId = 0;
}

DataBase::~DataBase()
{
}

bool DataBase::newActivity(const QString& description, const QString& category)
{
    if(currentActivityId != 0)
    {
        bool pausedActivity = false;
        pausedActivity = pauseActivity(currentActivityId);
        if(pausedActivity)
        {
            qDebug() << "DataBase::newActivity(): Preview activity is paused";
        }
        else
        {
            qDebug() << "DataBase::newActivity(): Error preview activity pausing";
            return false;
        }
    }

    QSqlQuery queryInsert;
    queryInsert.prepare(
        "INSERT INTO " + ActivityTable::nameTable + " (" +
        ActivityTable::date + ", " +
        ActivityTable::time + ", " +
        ActivityTable::description + ", " +
        ActivityTable::category + ", " +
        ActivityTable::duration + ", " +
        ActivityTable::running +                     ") " +
        "VALUES (:Date, :Time, :Description, :Category, :Duration, :Running)"
                       );
    queryInsert.bindValue(":Date", QDate::currentDate());
    queryInsert.bindValue(":Time", QTime::currentTime());
    queryInsert.bindValue(":Description", description);
    queryInsert.bindValue(":Category", category);
    queryInsert.bindValue(":Duration", QTime(0, 0, 0));
    queryInsert.bindValue(":Running", RunningActivity::Run);

    if(!queryInsert.exec())
    {
        qDebug() << "DataBase::newActivity(): Error insert new activity into table";
        qDebug() << queryInsert.lastError().text();
        return false;
    }
    else
    {
        qDebug() << "DataBase::newActivity(): Inserted new activity into table";
        currentTime = QTime::currentTime();
        currentTime.start();
        currentActivityId = queryInsert.lastInsertId().toInt();
        qDebug() << "DataBase::newActivity(): currentActivityId = " << currentActivityId;
        return true;
    }

    return false;
}

bool DataBase::deleteActivity(int id)
{
    QSqlQuery queryDelete;
    queryDelete.prepare(
        "DELETE FROM " + ActivityTable::nameTable +
        " WHERE " + ActivityTable::id + " = :Id"
                       );
    queryDelete.bindValue(":Id", id);

    if(!queryDelete.exec())
    {
        qDebug() << "DataBase::deleteActivity(): Error delete activity from table";
        qDebug() << queryDelete.lastError().text();
        return false;
    }
    else
    {
        qDebug() << "DataBase::deleteActivity(): Deleted activity from table";
        if(currentActivityId == id)
            currentActivityId = 0;
        qDebug() << "DataBase::deleteActivity(): currentActivityId = " << currentActivityId;
        return true;
    }

    return false;
}

bool DataBase::pauseActivity(int id)
{
    QSqlQuery querySelect;
    querySelect.prepare(
        "SELECT " + ActivityTable::duration +
        " FROM " + ActivityTable::nameTable +
        " WHERE " + ActivityTable::id + " = :Id"
                       );
    querySelect.bindValue(":Id", id);

    QTime duration;

    if(!querySelect.exec())
    {
        qDebug() << "DataBase::pauseActivity(): Error select duration from table";
        qDebug() << querySelect.lastError().text();
        return false;
    }
    else
    {
        qDebug() << "DataBase::pauseActivity(): Selected duration from table";
        while(querySelect.next())
            duration = querySelect.value(0).toTime();
    }

    duration = duration.addMSecs(currentTime.elapsed());

    QSqlQuery queryUpdate;
    queryUpdate.prepare(
        "UPDATE " + ActivityTable::nameTable + " SET " +
        ActivityTable::duration + " = :Duration, " +
        ActivityTable::running + " = :Running " +
        "WHERE " + ActivityTable::id + " = :Id"
                       );
    queryUpdate.bindValue(":Duration", duration);
    queryUpdate.bindValue(":Running", RunningActivity::NotRun);
    queryUpdate.bindValue(":Id", id);

    if(!queryUpdate.exec())
    {
        qDebug() << "DataBase::pauseActivity(): Error update activity in table";
        qDebug() << queryUpdate.lastError().text();
        return false;
    }
    else
    {
        qDebug() << "DataBase::pauseActivity(): Updated activity in table";
        currentTime = QTime(0, 0, 0);
        currentActivityId = 0;
        qDebug() << "DataBase::pauseActivity(): currentActivityId = " << currentActivityId;
        return true;
    }

    return false;
}

bool DataBase::continueActivity(int id)
{
    qDebug() << "DataBase::continueActivity(): Start";
    if(currentActivityId != 0)
    {
        bool pausedActivity = false;
        pausedActivity = pauseActivity(currentActivityId);
        if(pausedActivity)
        {
            qDebug() << "DataBase::continueActivity(): Preview activity is paused";
        }
        else
        {
            qDebug() << "DataBase::continueActivity(): Error preview activity pausing";
            return false;
        }
    }

    QSqlQuery queryUpdate;
    queryUpdate.prepare(
        "UPDATE " + ActivityTable::nameTable + " SET " +
        ActivityTable::running + " = :Running " +
        "WHERE " + ActivityTable::id + " = :Id"
                       );
    queryUpdate.bindValue(":Running", RunningActivity::Run);
    queryUpdate.bindValue(":Id", id);

    if(!queryUpdate.exec())
    {
        qDebug() << "DataBase::continueActivity(): Error update activity in table";
        qDebug() << queryUpdate.lastError().text();
        return false;
    }
    else
    {
        qDebug() << "DataBase::continueActivity(): Updated activity in table";
        currentTime = QTime::currentTime();
        currentActivityId = id;
        qDebug() << "DataBase::continueActivity(): currentActivityId = " << currentActivityId;
        return true;
    }
}

bool DataBase::editActivity(int id, const QString& description, const QString& category)
{
    QSqlQuery queryUpdate;
    queryUpdate.prepare(
        "UPDATE " + ActivityTable::nameTable + " SET " +
        ActivityTable::description + " = :Description, " +
        ActivityTable::category + " = :Category " +
        "WHERE " + ActivityTable::id + " = :Id"
                       );
    queryUpdate.bindValue(":Description", description);
    queryUpdate.bindValue(":Category", category);
    queryUpdate.bindValue(":Id", id);

    if(!queryUpdate.exec())
    {
        qDebug() << "DataBase::editActivity(): Error update activity in table";
        qDebug() << queryUpdate.lastError().text();
        return false;
    }
    else
    {
        qDebug() << "DataBase::editActivity(): Updated activity in table";
        qDebug() << "DataBase::editActivity(): currentActivityId = " << currentActivityId;
        return true;
    }

    return false;
}
