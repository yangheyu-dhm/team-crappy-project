#include "parkingcarin.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

// 定义唯一的数据库连接名
static const QString DATABASE_CONNECTION_NAME = "ParkingSystemConnection";

ParkingCarin::ParkingCarin(QWidget *parent) :
    QWidget(parent)
{
    setupUI();

    if (!initDatabase()) {
        QMessageBox::critical(this, "数据库错误", "无法连接到数据库!");
    }
}

void ParkingCarin::setupUI() {
    // 原来的UI搭建逻辑，比如布局、控件创建等代码
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* inputLayout = new QHBoxLayout;

    plateNumberEdit = new QLineEdit(this);
    plateNumberEdit->setPlaceholderText("请输入车牌号");
    plateNumberEdit->setMinimumWidth(200);

    queryButton = new QPushButton("识别", this);
    inputLayout->addWidget(plateNumberEdit);
    inputLayout->addWidget(queryButton);

    resultLabel = new QLabel("等待识别中...", this);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setMinimumHeight(50);
    resultLabel->setStyleSheet("font-size: 20px;");

    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(resultLabel);
    mainLayout->addStretch();

    resultTimer = new QTimer(this);
    connect(resultTimer, &QTimer::timeout, this, &ParkingCarin::onResultTimeout);
    connect(queryButton, &QPushButton::clicked, this, &ParkingCarin::onQueryButtonClicked);

    setWindowTitle("车牌识别系统");
    setMinimumSize(400, 200);
}

ParkingCarin::~ParkingCarin()
{
    if (db.isOpen()) {
        db.close();
    }

    // 从连接管理器中移除连接
    if (QSqlDatabase::contains(DATABASE_CONNECTION_NAME)) {
        QSqlDatabase::removeDatabase(DATABASE_CONNECTION_NAME);
    }
}

bool ParkingCarin::initDatabase()
{
    // 检查是否已存在连接，如果存在则关闭并移除
    if (QSqlDatabase::contains(DATABASE_CONNECTION_NAME)) {
        db = QSqlDatabase::database(DATABASE_CONNECTION_NAME);
        if (db.isOpen()) {
            db.close();
        }
        QSqlDatabase::removeDatabase(DATABASE_CONNECTION_NAME);
    }

    // 创建新的数据库连接
    db = QSqlDatabase::addDatabase("QSQLITE", DATABASE_CONNECTION_NAME);
    db.setDatabaseName("smartupc.db");

    if (!db.open()) {
        qDebug() << "数据库打开失败:" << db.lastError().text();
        return false;
    }

    return true;
}

bool ParkingCarin::isDatabaseAvailable()
{
    if (!db.isOpen()) {
        qDebug() << "数据库连接已关闭，尝试重新打开";
        if (!db.open()) {
            qDebug() << "重新打开数据库失败:" << db.lastError().text();
            return false;
        }
    }
    return true;
}

bool ParkingCarin::assignTemporaryParkingSpace(const QString& plateNumber)
{
    if (!isDatabaseAvailable()) {
        return false;
    }

    // 启动事务确保数据一致性
    db.transaction();

    // 查询是否有空余临时车位
    QSqlQuery selectQuery(db); // 显式指定数据库连接
    selectQuery.prepare("SELECT id FROM tem_parking WHERE occupied = 'No' LIMIT 1");

    if (!selectQuery.exec() || !selectQuery.next()) {
        qDebug() << "未找到可用临时车位";
        db.rollback();
        return false;
    }

    // 获取可用车位ID
    int spaceId = selectQuery.value(0).toInt();

    // 更新车位状态为已占用
    QSqlQuery updateQuery(db); // 显式指定数据库连接
    updateQuery.prepare("UPDATE tem_parking SET car_number = :plate, occupied = 'Yes' WHERE id = :id");
    updateQuery.bindValue(":plate", plateNumber);
    updateQuery.bindValue(":id", spaceId);

    if (!updateQuery.exec()) {
        qDebug() << "更新车位状态失败:" << updateQuery.lastError().text();
        db.rollback();
        return false;
    }

    // 提交事务
    if (!db.commit()) {
        qDebug() << "提交事务失败:" << db.lastError().text();
        // 尝试重新打开数据库连接
        db.close();
        if (!db.open()) {
            qDebug() << "重新打开数据库失败:" << db.lastError().text();
        }
        return false;
    }

    qDebug() << "成功分配临时车位，ID:" << spaceId;
    return true;
}

void ParkingCarin::onQueryButtonClicked()
{
    if (!isDatabaseAvailable()) {
        resultLabel->setText("数据库连接错误");
        resultLabel->setStyleSheet("color: red; font-size: 24px;");
        resultTimer->start(3000);
        return;
    }

    QString plateNumber = plateNumberEdit->text().trimmed();

    if (plateNumber.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入车牌号!");
        return;
    }

    // 检查车辆是否已入场且未出库（原逻辑保留）
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT * FROM Parkingfares WHERE carID = :plate AND parkingfee IS NULL");
    checkQuery.bindValue(":plate", plateNumber);

    if (checkQuery.exec() && checkQuery.next()) {
        resultLabel->setText("错误！！！车辆未出库");
        resultLabel->setStyleSheet("color: red; font-size: 24px;");
        resultTimer->start(3000);
        return;
    }

    // 先查询是否为小区车辆（原逻辑保留，核心修改在此处）
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE car = :plate");
    query.bindValue(":plate", plateNumber);

    if (query.exec() && query.next()) {
        // 小区车辆：检查ParkingSpots表中对应车位状态（新增逻辑）
        QSqlQuery checkSpotQuery(db);
        checkSpotQuery.prepare("SELECT status FROM ParkingSpots WHERE carID = :plate");
        checkSpotQuery.bindValue(":plate", plateNumber);

        if (!checkSpotQuery.exec()) {
            qDebug() << "查询ParkingSpots表失败：" << checkSpotQuery.lastError().text();
            resultLabel->setText("系统错误");
            resultLabel->setStyleSheet("color: red; font-size: 24px;");
            resultTimer->start(3000);
            return;
        }

        if (!checkSpotQuery.next()) {
            resultLabel->setText("错误！小区车辆未分配固定车位");
            resultLabel->setStyleSheet("color: red; font-size: 24px;");
            resultTimer->start(3000);
            return;
        }

        QString status = checkSpotQuery.value(0).toString();

        if (status == "空闲") {
            // 空闲车位：更新为占用并抬杆
            db.transaction();
            QSqlQuery updateQuery(db);
            updateQuery.prepare("UPDATE ParkingSpots SET status = '占用' WHERE carID = :plate");
            updateQuery.bindValue(":plate", plateNumber);

            if (!updateQuery.exec()) {
                db.rollback();
                qDebug() << "更新车位状态失败：" << updateQuery.lastError().text();
                resultLabel->setText("系统错误");
                resultLabel->setStyleSheet("color: red; font-size: 24px;");
                resultTimer->start(3000);
                return;
            }

            if (!db.commit()) {
                qDebug() << "提交事务失败：" << db.lastError().text();
                resultLabel->setText("系统错误");
                resultLabel->setStyleSheet("color: red; font-size: 24px;");
                resultTimer->start(3000);
                return;
            }

            resultLabel->setText("抬杆");
            resultLabel->setStyleSheet("color: green; font-size: 24px;");
        }
        else if (status == "占用") {
            // 已占用：显示错误
            resultLabel->setText("错误！！！车辆已入库");
            resultLabel->setStyleSheet("color: red; font-size: 24px;");
        }
        else {
            // 状态值异常
            qDebug() << "ParkingSpots表status值异常：" << status;
            resultLabel->setText("系统错误");
            resultLabel->setStyleSheet("color: red; font-size: 24px;");
        }
        resultTimer->start(3000);
    } else {
        // 非小区车辆处理逻辑（原逻辑保留，无修改）
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,
            "非小区车辆",
            "该车辆非小区登记车辆，是否允许进入（按时收费）？",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            if (assignTemporaryParkingSpace(plateNumber)) {
                QSqlQuery insertQuery(db);
                insertQuery.prepare(
                    "INSERT INTO Parkingfares (carID, start_time) "
                    "VALUES (:car, :time)"
                );
                insertQuery.bindValue(":car", plateNumber);
                insertQuery.bindValue(":time", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

                if (insertQuery.exec()) {
                    resultLabel->setText("抬杆");
                    resultLabel->setStyleSheet("color: green; font-size: 24px;");
                } else {
                    QMessageBox::warning(this, "数据库错误",
                            QString("临时车辆登记失败！\n错误详情：%1").arg(insertQuery.lastError().text()));
                    resultLabel->setText("系统错误");
                }
            } else {
                resultLabel->setText("无空余临时车位");
                resultLabel->setStyleSheet("color: orange; font-size: 24px;");
            }
            resultTimer->start(3000);
        } else {
            resultLabel->setText("禁止入内");
            resultLabel->setStyleSheet("color: red; font-size: 24px;");
            resultTimer->start(3000);
        }
    }
}

void ParkingCarin::onResultTimeout()
{
    // 3秒后重置为初始状态
    resultLabel->setText("等待识别中...");
    resultLabel->setStyleSheet("font-size: 20px;");
    resultTimer->stop();
}
