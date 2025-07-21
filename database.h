#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDir>

class Database
{
public:
    Database();

    ~Database();

    bool openDB(const QString& dbpath);

    QSqlQuery getpw(const QString& name);

private:
    QSqlDatabase db;
};



#endif // DATABASE_H
