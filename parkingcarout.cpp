#include "parkingcarout.h"
#include "ui_parkingcarout.h"
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>

Parkingcarout::Parkingcarout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Parkingcarout)
{
    // 动态创建UI控件
    createControls();
    setupLayout();
    connectSignals();

    // 打开数据库
    if (!openDatabase()) {
        QMessageBox::critical(this, "数据库错误", "无法连接到数据库!");
        queryButton->setEnabled(false);
    }
}

Parkingcarout::~Parkingcarout()
{
    delete ui;

    // 关闭数据库连接
    if (db.isOpen()) {
        db.close();
    }
}

bool Parkingcarout::openDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("smartupc.db");

    if (!db.open()) {
        qDebug() << "数据库错误:" << db.lastError().text();
        return false;
    }

    return true;
}

void Parkingcarout::createControls()
{
    // 创建UI控件
    plateNumberLabel = new QLabel("车牌号:", this);
    plateNumberEdit = new QLineEdit(this);
    plateNumberEdit->setPlaceholderText("请输入车牌号");

    queryButton = new QPushButton("识别", this);

    resultLabel = new QLabel(this);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet("font-size: 16px;");
    resultTimer = new QTimer(this);

    infoTable = new QTableWidget(this);
    infoTable->setColumnCount(4);
    infoTable->setHorizontalHeaderLabels({"车牌号", "入场时间", "停车时长", "停车费用"});
    infoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    infoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void Parkingcarout::setupLayout()
{
    // 设置布局
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(plateNumberLabel);
    inputLayout->addWidget(plateNumberEdit);
    inputLayout->addWidget(queryButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(resultLabel);
    mainLayout->addWidget(infoTable);

    // 设置布局比例
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 1);
    mainLayout->setStretch(2, 8);

    setLayout(mainLayout);
}

void Parkingcarout::connectSignals()
{
    // 连接信号和槽
    connect(resultTimer, &QTimer::timeout, this, &Parkingcarout::onResultTimeout);
    connect(queryButton, &QPushButton::clicked, this, &Parkingcarout::queryCar);
    connect(plateNumberEdit, &QLineEdit::returnPressed, this, &Parkingcarout::queryCar);
}

bool Parkingcarout::updateTemporaryParking(const QString &plateNumber)
{
    if (plateNumber.isEmpty())
        return false;

    QSqlQuery query(db);
    query.prepare("UPDATE tem_parking SET car_number = NULL, occupied = 'No' WHERE car_number = :plateNumber");
    query.bindValue(":plateNumber", plateNumber);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "更新临时停车位失败:" << query.lastError().text();
        return false;
    }
}

void Parkingcarout::queryCar()
{
    QString plateNumber = plateNumberEdit->text().trimmed();

    if (plateNumber.isEmpty()) {
        resultLabel->setText("请输入车牌号!");
        resultLabel->setStyleSheet("color: red; font-size: 16px;");
        resultTimer->start(3000);
        return;
    }

    // 先查询usrs表
    QSqlQuery queryUsrs(db);
    queryUsrs.prepare("SELECT * FROM users WHERE car = :plateNumber");
    queryUsrs.bindValue(":plateNumber", plateNumber);

    if (queryUsrs.exec() && queryUsrs.next()) {
        // 小区车辆：检查ParkingSpots表中对应车位状态（新增逻辑）
        QSqlQuery checkSpotQuery(db);
        checkSpotQuery.prepare("SELECT status FROM ParkingSpots WHERE carID = :plateNumber");
        checkSpotQuery.bindValue(":plateNumber", plateNumber);

        if (!checkSpotQuery.exec()) {
            qDebug() << "查询ParkingSpots表失败：" << checkSpotQuery.lastError().text();
            resultLabel->setText("系统错误");
            resultLabel->setStyleSheet("color: red; font-size: 16px;");
            resultTimer->start(3000);
            return;
        }

        if (!checkSpotQuery.next()) {
            resultLabel->setText("错误！小区车辆未分配固定车位");
            resultLabel->setStyleSheet("color: red; font-size: 16px;");
            resultTimer->start(3000);
            return;
        }

        QString status = checkSpotQuery.value(0).toString();

        if (status == "占用") {
            // 占用车位：更新为空闲并抬杆
            db.transaction();
            QSqlQuery updateQuery(db);
            updateQuery.prepare("UPDATE ParkingSpots SET status = '空闲' WHERE carID = :plateNumber");
            updateQuery.bindValue(":plateNumber", plateNumber);

            if (!updateQuery.exec()) {
                db.rollback();
                qDebug() << "更新车位状态失败：" << updateQuery.lastError().text();
                resultLabel->setText("系统错误");
                resultLabel->setStyleSheet("color: red; font-size: 16px;");
                resultTimer->start(3000);
                return;
            }

            if (!db.commit()) {
                qDebug() << "提交事务失败：" << db.lastError().text();
                resultLabel->setText("系统错误");
                resultLabel->setStyleSheet("color: red; font-size: 16px;");
                resultTimer->start(3000);
                return;
            }

            resultLabel->setText("抬杆");
            resultLabel->setStyleSheet("color: green; font-size: 16px;");
        }
        else if (status == "空闲") {
            // 已空闲：显示错误
            resultLabel->setText("错误！！！车辆已出库");
            resultLabel->setStyleSheet("color: red; font-size: 16px;");
        }
        else {
            // 状态值异常
            qDebug() << "ParkingSpots表status值异常：" << status;
            resultLabel->setText("系统错误");
            resultLabel->setStyleSheet("color: red; font-size: 16px;");
        }
        resultTimer->start(3000);
        clearTable();
        return;
    }

    // 再查询Parkingfares表（非小区车辆处理逻辑，保持不变）
    QSqlQuery queryFares(db);
    queryFares.prepare("SELECT * FROM Parkingfares WHERE carID = :plateNumber AND end_time IS NULL");
    queryFares.bindValue(":plateNumber", plateNumber);

    if (queryFares.exec() && queryFares.next()) {
        QDateTime startTime = queryFares.value("start_time").toDateTime();
        QDateTime endTime = QDateTime::currentDateTime();

        int seconds = startTime.secsTo(endTime);
        int hours = qMax(1, (seconds + 3599) / 3600);

        double price = queryFares.value("price").toDouble();
        double fee = hours * price;

        bool tempUpdateSuccess = updateTemporaryParking(plateNumber);

        QSqlQuery updateQuery(db);
        updateQuery.prepare("UPDATE Parkingfares SET end_time = :endTime, parkingfee = :fee WHERE carID = :plateNumber AND end_time IS NULL");
        updateQuery.bindValue(":endTime", endTime);
        updateQuery.bindValue(":fee", fee);
        updateQuery.bindValue(":plateNumber", plateNumber);

        if (updateQuery.exec()) {
            QString resultText = "抬杆\n停车费用: " + QString::number(fee, 'f', 2) + "元";
            if (!tempUpdateSuccess) {
                resultText += "\n临时停车位更新失败";
            }
            resultLabel->setText(resultText);
            resultLabel->setStyleSheet("color: green; font-size: 16px;");

            showParkingInfo(plateNumber, startTime, endTime, hours, fee);
        } else {
            resultLabel->setText("更新记录失败: " + updateQuery.lastError().text());
            resultLabel->setStyleSheet("color: red; font-size: 16px;");
        }
        resultTimer->start(3000);
    } else {
        resultLabel->setText("车辆不在地下车库中");
        resultLabel->setStyleSheet("color: red; font-size: 16px;");
        resultTimer->start(3000);
        clearTable();
    }
}

void Parkingcarout::showParkingInfo(const QString &plateNumber, const QDateTime &startTime, const QDateTime &endTime, int hours, double fee)
{
    clearTable();

    infoTable->insertRow(0);
    infoTable->setItem(0, 0, new QTableWidgetItem(plateNumber));
    infoTable->setItem(0, 1, new QTableWidgetItem(startTime.toString("yyyy-MM-dd hh:mm:ss")));
    infoTable->setItem(0, 2, new QTableWidgetItem(QString("%1小时").arg(hours)));
    infoTable->setItem(0, 3, new QTableWidgetItem(QString("%1元").arg(fee, 0, 'f', 2)));
}

void Parkingcarout::clearTable()
{
    while (infoTable->rowCount() > 0) {
        infoTable->removeRow(0);
    }
}

void Parkingcarout::onResultTimeout()
{
    // 3秒后重置为初始状态
    resultLabel->setText("");
    resultLabel->setStyleSheet("font-size: 20px;");
    resultTimer->stop();
}
