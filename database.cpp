#include "database.h"

Database::Database()
{

}


bool Database::openDB(const QString& dbpath){
    if(db.isOpen())return true;
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbpath);
    if(!db.open()){
        qDebug()<<"failed :"<<dbpath<<db.lastError().text();
        return false;
    }
    return true;
}

Database::~Database()
{

}

QSqlQuery Database::getpw(const QString& acc){
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE username = ?");
    query.addBindValue(acc);

    if(!query.exec()) {
        qDebug() << "Execute failed:" << query.lastError().text();
        return QSqlQuery();
    }

    return query;
}
