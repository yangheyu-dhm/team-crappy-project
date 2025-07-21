#ifndef DATABASE2_H
#define DATABASE2_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlError>

class DataBase2 {
public:
   DataBase2();
  ~DataBase2();
   static DataBase2* sharedInstance();
   QSqlQuery execQuery(const QString& sql);

   bool openDB(const QString& dbPath);
   void closeDB();
   QSqlError lastError() const;

   QSqlQuery getUserInfo(int userId);
   bool insertUserInfo(int userId, const QString& name, const QString& phone,
                        int role, const QString& password, const QString& address);
   bool updateUserInfo(int userId, const QString& name, const QString& phone,
                        const QString& address);
   QSqlQuery searchUsersByAddress(const QString& address);
   QSqlQuery searchUsers(const QString& condition);
   QSqlQuery getUserByUsername(const QString& username);
   bool isUserIdUnique(int userId);

   QSqlQuery getRepairInfo(int userId);
   bool insertRepairRequest(int repairId, int userId, int deviceId, const QString& desc);
   QSqlQuery getRepairRecordsByUserId(int userId);
   QSqlQuery getEvaluableRepairs(int ownerId);
   bool updateRepairFeedback(int repairId, int ownerId, int rating, const QString& feedback);
   bool markRepairAsCompleted(int repairId);
   QSqlQuery getUnratedRepairs(int ownerId);
   bool submitRepairFeedback(int repairId, int rating, const QString& comment);

   QSqlQuery getPaymentInfo(int userId);
   bool insertPaymentRecord(int userId, double amount, const QString& type, const QString& status, const QString& payTime, const QString& payMethod);
   bool updatePaymentRecord(int paymentId, int userId, double amount, const QString& type, const QString& status, const QString& payTime, const QString& payMethod);
   bool updatePaymentStatus(int paymentId, const QString& status);
   bool updatePaymentStatus(int paymentId, const QString& status, const QString& payTime);
   QSqlQuery getPaymentRecordsByUserId(int userId);
   QSqlQuery getAllPaymentRecords();


    int getEventCurrentParticipants(int eventId);
    bool isEventRegistered(int ownerId, int eventId);
    QSqlQuery getCommunityEvents();
    bool submitEventRegistration(int ownerId, int eventId);

    QSqlQuery getParkingSpotsInfo();
    QSqlQuery getParkingSpotByTypeAndLocation(const QString& type, const QString& location);
    bool updateParkingSpotStatus(int spotId, const QString& status);
    bool insertParkingRequest(int userId, int spotId, const QString& startDate);
    QSqlQuery getAvailableParkingSpot(const QString& type, const QString& location, const QString& status);
    QSqlQuery getParkingRecordsByUserId(int userId);
public:
    QSqlDatabase m_db;
private:
    static DataBase2* g_sharedDatabase;
};
#endif // DATABASE_H
