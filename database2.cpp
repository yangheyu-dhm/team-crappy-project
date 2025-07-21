#include "database2.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

DataBase2* DataBase2::g_sharedDatabase = nullptr;//单例

DataBase2::DataBase2()
{

   if (QSqlDatabase::contains("qt_sql_default_connection")) {
        m_db = QSqlDatabase::database("qt_sql_default_connection");
  } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    }
}


DataBase2::~DataBase2()
{
    if (m_db.isValid() && m_db.isOpen()) {
        m_db.close();
    }
}

DataBase2* DataBase2::sharedInstance()
{
   if (!g_sharedDatabase) {
      g_sharedDatabase = new DataBase2();
   }
   return g_sharedDatabase;
}

bool DataBase2::openDB(const QString& dbPath)
{
    m_db.setDatabaseName(dbPath);
    if (!m_db.open()) {
        qDebug() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }
    return true;
}

void DataBase2::closeDB()
{
    if (m_db.isValid() && m_db.isOpen()) {
        m_db.close();
    }
}

QSqlError DataBase2::lastError() const
{
    return m_db.lastError();
}

QSqlQuery DataBase2::getUserInfo(int userId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Users1 WHERE UserID = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "getUserInfo failed:" << query.lastError().text();
    }
    return query;
}

bool DataBase2::insertUserInfo(int userId, const QString& name, const QString& phone,
                             int role, const QString& password, const QString& address)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Users1 (UserID, name, phone, role, password, address) "
                  "VALUES (:userId, :name, :phone, :role, :password, :address)");
    query.bindValue(":userId", userId);
    query.bindValue(":name", name);
    query.bindValue(":phone", phone);
    query.bindValue(":role", role);
    query.bindValue(":password", password);
    query.bindValue(":address", address);

    if (!query.exec()) {
        qDebug() << "Insert user failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase2::updateUserInfo(int userId, const QString& name, const QString& phone,
                             const QString& address)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Users1 SET name = :name, phone = :phone, address = :address "
                  "WHERE UserID = :userId");
    query.bindValue(":userId", userId);
    query.bindValue(":name", name);
    query.bindValue(":phone", phone);
    query.bindValue(":address", address);

    if (!query.exec()) {
        qDebug() << "Update user failed:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQuery DataBase2::searchUsersByAddress(const QString& address)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT UserID, name, phone, address FROM Users1 WHERE address = :address");
    query.bindValue(":address", address);
    if (!query.exec()) {
        qDebug() << "searchUsersByAddress failed:" << query.lastError().text();
    }
    return query;
}

QSqlQuery DataBase2::searchUsers(const QString& condition)
{
    QSqlQuery query(m_db);
    QString sql = "SELECT * FROM Users1 WHERE " + condition;
    if (!query.exec(sql)) {
        qDebug() << "searchUsers failed:" << query.lastError().text();
    }
    return query;
}

QSqlQuery DataBase2::getUserByUsername(const QString& username)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Users1 WHERE name = :username");
    query.bindValue(":username", username);
    if (!query.exec()) {
        qDebug() << "getUserByUsername failed:" << query.lastError().text();
    }
    return query;
}

bool DataBase2::isUserIdUnique(int userId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM Users1 WHERE UserID = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec() || !query.next()) {
        qDebug() << "isUserIdUnique failed:" << query.lastError().text();
        return false;
    }
    return query.value(0).toInt() == 0;
}


QSqlQuery DataBase2::getRepairInfo(int userId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM RepairRequests WHERE UserID = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "getRepairInfo failed:" << query.lastError().text();
    }
    return query;
}


QSqlQuery DataBase2::getRepairRecordsByUserId(int userId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT RepairID, UserID, DeviceID, description, status, create_time "
                  "FROM repair "
                  "WHERE UserID = :userId "
                  "ORDER BY create_time DESC");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "getRepairRecordsByUserId failed:" << query.lastError().text();
    }
    return query;
}

QSqlQuery DataBase2::getAllPaymentRecords() {
    QSqlQuery query(m_db);
    query.exec("SELECT PaymentID, UserID, Amount, Type, Status, PayTime FROM Payments");
    return query;
}
QSqlQuery DataBase2::getPaymentInfo(int userId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Payments WHERE UserID = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "getPaymentInfo failed:" << query.lastError().text();
    }
    return query;
}

bool DataBase2::updatePaymentStatus(int paymentId, const QString& status)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Payments SET Status = :status WHERE PaymentID = :paymentId");
    query.bindValue(":status", status);
    query.bindValue(":paymentId", paymentId);

    if (!query.exec()) {
        qDebug() << "Update payment status failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase2::updatePaymentStatus(int paymentId, const QString& status, const QString& payTime)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Payments SET Status = :status, PaymentTime = :payTime "
                  "WHERE PaymentID = :paymentId");
    query.bindValue(":status", status);
    query.bindValue(":payTime", payTime);
    query.bindValue(":paymentId", paymentId);

    if (!query.exec()) {
        qDebug() << "Update payment status with time failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase2::insertPaymentRecord(int userId, double amount, const QString& type, const QString& status, const QString& payTime, const QString& payMethod)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Payments (UserID, Amount, Type, Status, PayTime) "
                  "VALUES (:userId, :amount, :type, :status, :payTime)");
    query.bindValue(":userId", userId);
    query.bindValue(":amount", amount);
    query.bindValue(":type", type);
    query.bindValue(":status", status);
    query.bindValue(":payTime", payTime);


    if (!query.exec()) {
        qDebug() << "insertPaymentRecord failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase2::updatePaymentRecord(int paymentId, int userId, double amount, const QString& type, const QString& status, const QString& payTime, const QString& payMethod)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Payments SET UserID = :userId, Amount = :amount, Type = :type, Status = :status, PayTime = :payTime, PayMethod = :payMethod "
                  "WHERE PaymentID = :paymentId");
    query.bindValue(":paymentId", paymentId);
    query.bindValue(":userId", userId);
    query.bindValue(":amount", amount);
    query.bindValue(":type", type);
    query.bindValue(":status", status);
    query.bindValue(":payTime", payTime);
    query.bindValue(":payMethod", payMethod);

    if (!query.exec()) {
        qDebug() << "updatePaymentRecord failed:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQuery DataBase2::getPaymentRecordsByUserId(int userId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT PaymentID, UserID, Amount, Type, Status, PayTime "
                  "FROM Payments "
                  "WHERE UserID = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "getPaymentRecordsByUserId failed:" << query.lastError().text();
    }
    return query;
}


bool DataBase2::insertParkingRequest(int userId, int spotId, const QString& startDate) {

    QSqlQuery checkQuery(m_db);
    checkQuery.prepare(R"(
        SELECT status
        FROM ParkingSpots
        WHERE SpotID = :spotId
    )");
    checkQuery.bindValue(":spotId", spotId);
    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "车位状态查询失败：" << checkQuery.lastError().text();
        return false;
    }
    if (checkQuery.value("status").toString() != "空闲") {
        qDebug() << "车位已占用或不存在，SpotID:" << spotId;
        return false;
    }


    QSqlQuery updateQuery(m_db);
    updateQuery.prepare(R"(
        UPDATE ParkingSpots
        SET status = '待审核',
            UserID = :userId,
            startDate = :startDate
        WHERE SpotID = :spotId
    )");
    updateQuery.bindValue(":spotId", spotId);
    updateQuery.bindValue(":userId", userId);
    updateQuery.bindValue(":startDate", startDate); // 绑定起始日期

    if (!updateQuery.exec()) {
        qDebug() << "更新失败：" << updateQuery.lastError().text()
                 << "\nSQL:" << updateQuery.lastQuery();
        return false;
    }
    return true;
}


QSqlQuery DataBase2::getParkingRecordsByUserId(int userId) {
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT
            SpotID AS "车位ID",
            type AS "类型",
            location AS "位置",
            price AS "价格",
            status AS "申请状态",
            startDate AS "起始日期"
        FROM ParkingSpots
        WHERE UserID = :userId
        ORDER BY status DESC
    )");
    query.bindValue(":userId", userId);
    query.exec();
    return query;
}

bool DataBase2::updateParkingSpotStatus(int spotId, const QString& status)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE ParkingSpots "
                  "SET status = :status "
                  "WHERE SpotID = :spotId");
    query.bindValue(":status", status);
    query.bindValue(":spotId", spotId);

    if (!query.exec()) {
        qDebug() << "updateParkingSpotStatus failed:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQuery DataBase2::getAvailableParkingSpot(const QString& type,
                                           const QString& location,
                                           const QString& status) {
    QSqlQuery query(m_db);
    query.prepare("SELECT SpotID, type, location, price "
                  "FROM ParkingSpots "
                  "WHERE type = :type "
                  "  AND location = :location "
                  "  AND status = :status");
    query.bindValue(":type", type);
    query.bindValue(":location", location);
    query.bindValue(":status", status);
    if (!query.exec()) {
        qDebug() << "getAvailableParkingSpot 执行失败："
                 << query.lastError().text()
                 << "\nSQL:" << query.lastQuery()
                 << "\n绑定的参数:" << query.boundValues();
    }
    return query;
}

QSqlQuery DataBase2::getEvaluableRepairs(int ownerId) {
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT r.RepairID, r.UserID, r.DeviceID, r.description, r.status
        FROM repair r
        LEFT JOIN RepairFeedback rf ON r.RepairID = rf.RepairID
        WHERE r.UserID = :ownerId
          AND r.status = '已完成'
          AND (rf.rating IS NULL OR rf.feedback IS NULL)
    )");
    query.bindValue(":ownerId", ownerId);

    if (!query.exec()) {
        qDebug() << "Error in getEvaluableRepairs:" << query.lastError().text();

    }

    return query;
}

bool DataBase2::markRepairAsCompleted(int repairId) {
    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE repair
        SET status = '已完成',
            completion_time = CURRENT_TIMESTAMP
        WHERE RepairID = :repairId
    )");
    query.bindValue(":repairId", repairId);
    return query.exec();
}


bool DataBase2::insertRepairRequest(int repairId, int userId, int deviceId, const QString& desc) {
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO repair (RepairID, UserID, DeviceID, description, status, create_time)
        VALUES (:repairId, :userId, :deviceId, :desc, '待维修', CURRENT_TIMESTAMP)
    )");
    query.bindValue(":repairId", repairId);
    query.bindValue(":userId", userId);
    query.bindValue(":deviceId", deviceId);
    query.bindValue(":desc", desc);

    bool success = query.exec();
    if (!success) {
        qDebug() << "SQL 错误：" << query.lastError().text();
        qDebug() << "执行 SQL：" << query.lastQuery();
    }
    return success;
}



QSqlQuery DataBase2::getUnratedRepairs(int ownerId) {
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT r.RepairID, r.UserID, r.DeviceID, r.description, r.status
        FROM repair r
        LEFT JOIN RepairFeedback rf ON r.RepairID = rf.RepairID
        WHERE r.UserID = :ownerId
          AND r.status = '已完成'
          AND rf.RepairID IS NULL
    )");
    query.bindValue(":ownerId", ownerId);
    query.exec();
    return query;
}

bool DataBase2::submitRepairFeedback(int repairId, int rating, const QString& comment) {
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO RepairFeedback (RepairID, Rating, Comment)
        VALUES (:repairId, :rating, :comment)
    )");
    query.bindValue(":repairId", repairId);
    query.bindValue(":rating", rating);
    query.bindValue(":comment", comment);
    return query.exec();
}

QSqlQuery DataBase2::getParkingSpotsInfo()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT SpotID, type, location, price, status, start_date FROM ParkingSpots");
    if (!query.exec()) {
        qDebug() << "getParkingSpotsInfo failed:" << query.lastError().text();
    }
    return query;
}

QSqlQuery DataBase2::execQuery(const QString& sql) {
    QSqlQuery query(m_db);
    if (!query.exec(sql)) {
        qDebug() << "DataBase Error:" << query.lastError().text()
                 << "\nSQL:" << sql;
    }
    return query;
}

int DataBase2::getEventCurrentParticipants(int eventId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM EventRegistrations WHERE EventID = :eventId");
    query.bindValue(":eventId", eventId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    qDebug() << "获取活动报名人数失败:" << query.lastError().text();
    return 0; // 默认返回0（查询失败时）
}

bool DataBase2::isEventRegistered(int ownerId, int eventId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM EventRegistrations "
                  "WHERE OwnerID = :ownerId AND EventID = :eventId");
    query.bindValue(":ownerId", ownerId);
    query.bindValue(":eventId", eventId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    qDebug() << "检查报名状态失败:" << query.lastError().text();
    return false;
}

bool DataBase2::submitEventRegistration(int ownerId, int eventId)
{
    m_db.transaction();

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO EventRegistrations (OwnerID, EventID, RegisterTime) "
                  "VALUES (:ownerId, :eventId, CURRENT_TIMESTAMP)");
    query.bindValue(":ownerId", ownerId);
    query.bindValue(":eventId", eventId);

    if (query.exec()) {
        m_db.commit();
        return true;
    } else {
        m_db.rollback();
        qDebug() << "报名失败:" << query.lastError().text();
        return false;
    }
}

