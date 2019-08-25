#ifndef DATABASE_H
#define DATABASE_H

#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>
#include <QApplication>
#include <QTime>

class DataBaseSettings
{
public:
    static const QString type;
    static const QString hostname;
    static const QString name;
};

class ActivityTable
{
public:
    static const QString nameTable;

    static const QString id;
    static const QString date;
    static const QString time;
    static const QString description;
    static const QString category;
    static const QString duration;
    static const QString running;
};

enum RunningActivity
{
    NotRun,
    Run
};

class DataBase
{
public:
    explicit DataBase();

    ~DataBase();

    bool newActivity(const QString& description, const QString& category);

    bool deleteActivity(int id);

    bool pauseActivity(int id);

    bool continueActivity(int id);

    bool editActivity(int id, const QString& description, const QString& category);

private:
    QSqlDatabase db;
    int currentActivityId;
    QTime currentTime;
};

#endif // DATABASE_H
