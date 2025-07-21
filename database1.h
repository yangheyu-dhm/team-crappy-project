#ifndef DATEBASE1_H
#define DATEBASE1_H
#include <QString>
#include <QSqlDatabase>
#include <QVariantList>
#include <QMap>
#include <QSqlQuery>
#include <QVariant>
class database1
{
public:
    explicit database1();
    ~database1();
    bool openDB(const QString& dbPath);
    void closeDB();
    QSqlQuery getUserByUsername(const QString& username);
    bool addUser(const QString username,const QString& password,int status,const QString& name,const QString& department,const QString& position,int role);
    QSqlQuery query(const QString& sql, const QVariantList& args);
private:
    QSqlDatabase m_db;
};


#endif  //DATEBASE1_H
