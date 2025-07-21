#include "database1.h"
#include <QString>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QVariant>
database1::database1()
{

}
database1::~database1(){

}
bool database1::openDB(const QString& dbPath){
    if(m_db.isOpen()) return true;
     m_db=QSqlDatabase::addDatabase("QSQLITE");
     m_db.setDatabaseName(dbPath);
     if(!m_db.open())
     {
         qWarning()<<"Failed to open database:"<<m_db.lastError().text();
         return false;
     }
     return true;
}
void database1::closeDB()
{
    if(m_db.isOpen()) m_db.close();
}
bool database1::addUser(const QString username,const QString& password,int status,const QString& name,const QString& department,const QString& position,int role){
    QSqlQuery query;
    query.prepare("INSERT INTO users (username,password,status,name,department,position,role) VALUES(?,?,?,?,?,?,?)");
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(status);
    query.addBindValue(name);
    query.addBindValue(department);
    query.addBindValue(position);
    query.addBindValue(role);
    return query.exec();
}
QSqlQuery database1::getUserByUsername(const QString& username){
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = ?");
    query.addBindValue(username);
    query.exec();
    return query;
}
QSqlQuery database1::query(const QString& sql, const QVariantList& args){

    QSqlQuery query;
    query.prepare(sql);
    for (const auto& v : args) query.addBindValue(v);
    query.exec();
    return query;
}
