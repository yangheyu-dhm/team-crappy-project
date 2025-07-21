#include "ownerwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDebug>
#include "database2.h"
#include <QDateTime>
#include <QObject>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QWidgetAction>
#include <QRandomGenerator>
#include <QLabel>
#include <QSqlTableModel>
#include "ownerrepairevalwidget.h"
#include "pronouncementwidget.h"

//PronouncementWidget* w=new PronouncementWidget();
OwnerWindow::OwnerWindow(int ownerId, QWidget *parent)
    : QMainWindow(parent)
   , m_ownerId(ownerId)
   , g_db(DataBase2::sharedInstance()) //单例
   , selectedRepairId(1)
   , tableRepairFeedback(nullptr)
   , spinBoxRating(nullptr)
   , txtFeedback(nullptr)
   , m_selectedEventId(-1)
   , tableEvents(nullptr)
{
    ui.setupUi(this);
    setWindowTitle("业主管理");


    if (!g_db) {
        QMessageBox::critical(this, "初始化错误", "数据库单例创建失败，程序将退出");
        close();
        return;
    }
    if (!g_db->openDB("smartupc.db")) {
        QMessageBox::critical(this, "数据库错误",
            QString("无法连接数据库：%1\n程序将退出").arg(g_db->lastError().text()));
        close();
        return;
    }

    timeLabel = new QLabel(this);
    timeLabel->setStyleSheet("QLabel {"
        "   background-color: white;"
        "   border-radius: 6px;"
        "   padding: 3px 10px;"
        "   min-width: 200px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   color: darkblue"
        "}"
    );
    statusBar()->addPermanentWidget(timeLabel);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OwnerWindow::updateTime);
    updateTime();
    timer->start(1000);

        initRepairTableHeaders();
        initRepairFeedbackTableHeaders();
        initParkingTableHeaders();

        setupMenus();
        setupStackedWidget();


        loadUserInfo();
        loadRepairFeedbackTable();
        loadParkingSpotsInfo();
        loadParkingRecords(m_ownerId);
        loadEvents();
        ui.dateEditStartDate->setDate(QDate::currentDate());
        showRegistration();
        ui.tableParkingSpots->setStyleSheet("QPushButton { background-color: #4a6fa5; color: white; border-radius: 5px; padding: 6px; }"
                                       "QPushButton:hover { background-color: #3a5a85; }");


        ui.tableWidget_searchResult->setStyleSheet(R"(
            QHeaderView::section {
                background-color: #2c3e50;
                color: white;
                padding: 8px;
                border-radius: 0px;
                font-weight: bold;
                border: 1px solid #34495e;
            }
            QTableView {
                gridline-color: #ecf0f1;
                selection-background-color: #3498db;
                selection-color: white;
                background-color: #f8f9fa;
                alternate-background-color: #f5f5f5;
                border-radius: 5px;
                border: 1px solid #ddd;
            }
            QTableView::item {
                padding: 6px;
                border-bottom: 1px solid #ecf0f1;
            }
            QTableView::item:selected {
                border: none;
            }
            QScrollBar:vertical {
                background: #f5f5f5;
                width: 12px;
                margin: 0px;
            }
            QScrollBar::handle:vertical {
                background: #bdc3c7;
                min-height: 20px;
                border-radius: 6px;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
        )");
        ui.tableRepairFeedback->setStyleSheet(R"(
            QHeaderView::section {
                background-color: #2c3e50;
                color: white;
                padding: 8px;
                border-radius: 0px;
                font-weight: bold;
                border: 1px solid #34495e;
            }
            QTableView {
                gridline-color: #ecf0f1;
                selection-background-color: #3498db;
                selection-color: white;
                background-color: #f8f9fa;
                alternate-background-color: #f5f5f5;
                border-radius: 5px;
                border: 1px solid #ddd;
            }
            QTableView::item {
                padding: 6px;
                border-bottom: 1px solid #ecf0f1;
            }
            QTableView::item:selected {
                border: none;
            }
            QScrollBar:vertical {
                background: #f5f5f5;
                width: 12px;
                margin: 0px;
            }
            QScrollBar::handle:vertical {
                background: #bdc3c7;
                min-height: 20px;
                border-radius: 6px;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
        )");

        ui.tableWidget_searchResult->setRowCount(0);
        ui.tableWidget_searchResult->setColumnCount(4);
        ui.tableWidget_searchResult->setHorizontalHeaderLabels({"UserID", "姓名", "电话", "地址"});

        ui.tableWidget_searchResult->setColumnWidth(0, 100);
        ui.tableWidget_searchResult->setColumnWidth(1, 120);
        ui.tableWidget_searchResult->setColumnWidth(2, 120);
        ui.tableWidget_searchResult->setColumnWidth(3, 180);

        ui.tableWidget_searchResult->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        ui.tableWidget_searchResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui.tableWidget_searchResult->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui.tableWidget_searchResult->setAlternatingRowColors(true);
        ui.tableWidget_searchResult->setShowGrid(true);
        ui.tableWidget_searchResult->setCornerButtonEnabled(false);
}

void OwnerWindow::updateTime(){
    QString currentTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    timeLabel->setText(currentTime);
}

OwnerWindow::~OwnerWindow()
{
    if (g_db && g_db->m_db.isOpen()) {
        g_db->closeDB();
    }

}
void OwnerWindow::setupMenus()
{
    QToolBar* toolBar = new QToolBar("导航工具栏", this);
    addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar->setIconSize(QSize(48, 48));
    toolBar->setMovable(false);

    struct MenuData {
        QString title;
        QString iconPath;
        QStringList actions;
        std::function<void()> slot;
        QList<std::function<void()>> subSlots;
    };

    QList<MenuData> menuDataList = {
        {
            "业主信息",
            ":/images/tubiao1.png",
            {"信息登记", "信息修改", "信息查询"},
            [this]() { showRegistration(); },
            {
                [this]() { showRegistration(); },
                [this]() { showModification(); },
                [this]() { showInquiry(); }
            }
        },
        {
            "服务申请",
            ":/images/parkingicon.png",
            {"报修申请", "车位申请", "报修评价"},
            [this]() { showRepair(); },
            {
                [this]() { showRepair(); },
                [this]() { showParking(); },
                [this]() { showRepairFeedback(); }
            }
        },
        {
            "缴费管理",
            ":/images/leaveicon.png",
            {"费用缴纳"},
            [this]() { showPayment(); },
            {
                [this]() { showPayment(); }
            }
        },
        {
            "社区活动",
            ":/images/activitiyicon.png",
            {"活动列表","公告列表"},
            [this]() { showCommunityEvents(); },
            {
                [this]() { showCommunityEvents(); },
                [this]() {PronouncementWidget* w=new PronouncementWidget();
                w->show();},
            }
        }
    };

    for (const auto& data : menuDataList) {
        QMenu* menu = new QMenu(data.title, this);

        for (int i = 0; i < data.actions.size(); ++i) {
            QAction* action = menu->addAction(data.actions[i]);
            if (i < data.subSlots.size()) {
                connect(action, &QAction::triggered, data.subSlots[i]);
            }
        }

        QToolButton* button = new QToolButton(this);
        button->setText(data.title);
        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        button->setIcon(QIcon(data.iconPath));
        button->setPopupMode(QToolButton::MenuButtonPopup);
        button->setMenu(menu);
\
        if (data.slot) {
            connect(button, &QToolButton::clicked, data.slot);
        }

        QWidgetAction* widgetAction = new QWidgetAction(this);
        widgetAction->setDefaultWidget(button);
        toolBar->addAction(widgetAction);
    }
}


void OwnerWindow::setupStackedWidget()
{
    auto setupTableStyle = [](QTableView* tableView) {
        tableView->verticalHeader()->hide();
        tableView->setStyleSheet(
            "QHeaderView::section { background-color: darkblue; color: white; }"
            "QTableView::item:selected { selection-background-color: lightblue; }"
        );
    };
    auto setupTableWidgetStyle = [](QTableWidget* tableWidget) {
        tableWidget->verticalHeader()->hide();
        tableWidget->setStyleSheet(
            "QHeaderView::section { background-color: darkblue; color: white; }"
            "QTableWidget::item:selected { selection-background-color: lightblue; }"
        );
    };

    auto setupLabelStyle = [](QLabel* label, const QString& title) {
        label->setText(title);
        QFont font = label->font();
        font.setPointSize(14);
        font.setBold(true);
        label->setFont(font);
    };

    if (ui.label_registration) {
        setupLabelStyle(ui.label_registration, "业主信息登记（ID: " + QString::number(m_ownerId) + "）");
    }

    ui.tableWidget_3->setColumnCount(4);
    ui.tableWidget_3->setHorizontalHeaderLabels({"UserID", "姓名", "电话", "地址"});
    setupTableWidgetStyle(ui.tableWidget_3);
    ui.tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui.tableWidget_modify->setColumnCount(4);
    ui.tableWidget_modify->setHorizontalHeaderLabels({"UserID", "姓名", "电话", "地址"});
    setupTableWidgetStyle(ui.tableWidget_modify);
    ui.tableWidget_modify->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui.comboBox_searchType->addItems({"全部", "姓名", "UserID"});
    ui.spinBoxRepairId->setRange(1, 9999);
    ui.spinBoxRepairId->setValue(1);

    connect(ui.comboBox_searchType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &OwnerWindow::searchOwnerInfo);
    connect(ui.btnSearch, &QPushButton::clicked, this, &OwnerWindow::searchOwnerInfo);
    connect(ui.btnSubmint, &QPushButton::clicked, this, &OwnerWindow::submitRegistration);
    connect(ui.btnSaveModify, &QPushButton::clicked, this, &OwnerWindow::saveModification);
    connect(ui.btnRepair, &QPushButton::clicked, this, &OwnerWindow::submitRepair);
    connect(ui.btnAddPayment, &QPushButton::clicked, this, &OwnerWindow::submitAddPayment);
    connect(ui.btnParking, &QPushButton::clicked, this, &OwnerWindow::submitParkingRequest);
    connect(ui.pushButtonSubmit, &QPushButton::clicked, this, &OwnerWindow::submitRepairFeedback);

    QWidget* eventPage = new QWidget();
    QVBoxLayout* eventLayout = new QVBoxLayout(eventPage);
    eventLayout->setContentsMargins(10, 10, 10, 10);
    eventLayout->setSpacing(10);

    QLabel* lblEventTitle = new QLabel("社区活动列表", eventPage);
    setupLabelStyle(lblEventTitle, "社区活动列表");
    eventLayout->addWidget(lblEventTitle);

    initEventTableHeaders();
    setupTableWidgetStyle(tableEvents);
    tableEvents->setColumnWidth(0, 100);
    tableEvents->setColumnWidth(1, 200);
    tableEvents->setColumnWidth(2, 150);
    tableEvents->setColumnWidth(3, 150);
    tableEvents->setColumnWidth(4, 100);
    eventLayout->addWidget(tableEvents);

    QWidget* myEventsPage = nullptr;
    for (int i = 0; i < ui.stackedWidget->count(); ++i) {
        if (ui.stackedWidget->widget(i)->objectName() == "myEventsPage") {
            myEventsPage = ui.stackedWidget->widget(i);
            break;
        }
    }
    if (myEventsPage) {
        QPushButton* btnRefresh_2 = myEventsPage->findChild<QPushButton*>("btnRefresh_2");
        if (btnRefresh_2) {
        }
    }

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnRefresh = new QPushButton("刷新列表", eventPage);
    QPushButton* btnSignUp = new QPushButton("报名活动", eventPage);

    btnRefresh->setStyleSheet("QPushButton { background-color: #4a6fa5; color: white; border-radius: 5px; padding: 6px; }"
                             "QPushButton:hover { background-color: #3a5a85; }");
    btnSignUp->setStyleSheet("QPushButton { background-color: #4a6fa5; color: white; border-radius: 5px; padding: 6px; }"
                            "QPushButton:hover { background-color: #3a5a85; }");

    btnRefresh->setObjectName("btnRefreshEvents");
    btnSignUp->setObjectName("btnSignUpEvent");
    btnLayout->addWidget(btnRefresh);
    btnLayout->addWidget(btnSignUp);
    eventLayout->addLayout(btnLayout);

    connect(btnRefresh, &QPushButton::clicked, this, &OwnerWindow::on_btnRefreshEvents_clicked);
    connect(btnSignUp, &QPushButton::clicked, this, &OwnerWindow::on_btnSignUpEvent_clicked);
    connect(tableEvents, &QTableWidget::itemClicked, this, &OwnerWindow::on_tableEvents_itemClicked);

    ui.stackedWidget->addWidget(eventPage);

    QWidget *repairFeedbackPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(repairFeedbackPage);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);

    QLabel *lblTitle = new QLabel("业主维修评价", repairFeedbackPage);
    setupLabelStyle(lblTitle, "业主维修评价");

    tableRepairFeedback = new QTableWidget(repairFeedbackPage);


    tableRepairFeedback->setColumnCount(5);
    tableRepairFeedback->setHorizontalHeaderLabels({
        "报修ID", "业主ID", "设备ID", "故障描述", "状态"
    });
    setupTableWidgetStyle(tableRepairFeedback);
    tableRepairFeedback->setColumnWidth(0, 100);
    tableRepairFeedback->setColumnWidth(1, 100);
    tableRepairFeedback->setColumnWidth(2, 100);
    tableRepairFeedback->setColumnWidth(3, 300);
    tableRepairFeedback->setColumnWidth(4, 100);

    QLabel *lblRating = new QLabel("评分：", repairFeedbackPage);
    spinBoxRating = new QSpinBox(repairFeedbackPage);
    spinBoxRating->setRange(1, 5);
    spinBoxRating->setValue(3);

    QLabel *lblFeedback = new QLabel("评价：", repairFeedbackPage);
    txtFeedback = new QTextEdit(repairFeedbackPage);
    txtFeedback->setPlaceholderText("请输入评价内容...");

    QPushButton *btnSubmitFeedback = new QPushButton("提交评价", repairFeedbackPage);
    btnSubmitFeedback->setStyleSheet("QPushButton { background-color: #4a6fa5; color: white; border-radius: 5px; padding: 6px; }"
                                   "QPushButton:hover { background-color: #3a5a85; }");
    connect(btnSubmitFeedback, &QPushButton::clicked, this, &OwnerWindow::submitRepairFeedback);

    layout->addWidget(lblTitle);
    layout->addWidget(tableRepairFeedback);

    QHBoxLayout* ratingLayout = new QHBoxLayout();
    ratingLayout->addWidget(lblRating);
    ratingLayout->addWidget(spinBoxRating);
    ratingLayout->addStretch();
    layout->addLayout(ratingLayout);

    layout->addWidget(lblFeedback);
    layout->addWidget(txtFeedback);
    layout->addWidget(btnSubmitFeedback);

    ui.stackedWidget->addWidget(repairFeedbackPage);

    if (tableRepairFeedback) {
        connect(tableRepairFeedback, &QTableWidget::itemClicked,
                this, &OwnerWindow::on_tableRepairFeedback_itemClicked);
    } else {
        qDebug() << "Warning: tableRepairFeedback 初始化失败";
    }
}

void OwnerWindow::submitRegistration()
{
    QString defaultAddress = ui.lineEdit_address->text().trimmed();
    if (defaultAddress.isEmpty()) {
        QSqlQuery query = g_db->getUserInfo(m_ownerId);
        if (query.next()) {
            defaultAddress = query.value("address").toString();
            ui.lineEdit_address->setText(defaultAddress);
        }
    }

    QString userIdStr = ui.lineEdit_userId->text().trimmed();
    QString name = ui.lineEdit_name->text().trimmed();
    QString phone = ui.lineEdit_phone->text().trimmed();
    QString address = defaultAddress;

    if (userIdStr.isEmpty() || name.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("输入错误");
        msgBox.setText("UserID和姓名不能为空！");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: #f8f9fa;
                border-radius: 8px;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
            }
            QLabel {
                color: #495057;
                font-weight: 500;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: 500;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton#approveButton {
                background: darkblue;
                color: white;
            }
            QPushButton#approveButton:hover {
                background: lightblue;
            }
        )");
        msgBox.exec();
        return;
    }

    bool isUserIdValid;
    int userId = userIdStr.toInt(&isUserIdValid);
    if (!isUserIdValid) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("输入错误");
        msgBox.setText("UserID必须是有效整数！");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: #f8f9fa;
                border-radius: 8px;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
            }
            QLabel {
                color: #495057;
                font-weight: 500;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: 500;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton#approveButton {
                background: darkblue;
                color: white;
            }
            QPushButton#approveButton:hover {
                background: lightblue;
            }
        )");
        msgBox.exec();
        return;
    }

    QSqlQuery checkQuery = g_db->getUserInfo(userId);
    if (checkQuery.next()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("登记失败");
        msgBox.setText(QString("UserID %1已存在，请更换！").arg(userId));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: #f8f9fa;
                border-radius: 8px;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
            }
            QLabel {
                color: #495057;
                font-weight: 500;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: 500;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton#approveButton {
                background: darkblue;
                color: white;
            }
            QPushButton#approveButton:hover {
                background: lightblue;
            }
        )");
        msgBox.exec();
        return;
    }

    if (g_db->insertUserInfo(userId, name, phone, 2, QString::number(userId), address)) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("登记成功");
        msgBox.setText("家庭成员信息已成功添加！");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: #f8f9fa;
                border-radius: 8px;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
            }
            QLabel {
                color: #495057;
                font-weight: 500;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: 500;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton#approveButton {
                background: darkblue;
                color: white;
            }
            QPushButton#approveButton:hover {
                background: lightblue;
            }
        )");
        msgBox.exec();
        ui.tableWidget_3->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");
        loadSameAddressUsers(address, ui.tableWidget_3);

        ui.lineEdit_userId->clear();
        ui.lineEdit_name->clear();
        ui.lineEdit_phone->clear();
    } else {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("登记失败");
        msgBox.setText(QString("数据库错误：%1").arg(g_db->lastError().text()));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: #f8f9fa;
                border-radius: 8px;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
            }
            QLabel {
                color: #495057;
                font-weight: 500;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: 500;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton#approveButton {
                background: darkblue;
                color: white;
            }
            QPushButton#approveButton:hover {
                background: lightblue;
            }
        )");
        msgBox.exec();
    }
}
void OwnerWindow::loadSameAddressUsers(const QString& address, QTableWidget* targetTable)
{
    targetTable->setRowCount(0);
    targetTable->setColumnCount(4);
    targetTable->setHorizontalHeaderLabels({"UserID", "姓名", "电话", "地址"});
    targetTable->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");

    QSqlQuery query = g_db->searchUsersByAddress(address);
    int row = 0;
    while (query.next()) {
        targetTable->insertRow(row);
        targetTable->setItem(row, 0, new QTableWidgetItem(query.value("UserID").toString()));
        targetTable->setItem(row, 1, new QTableWidgetItem(query.value("name").toString()));
        targetTable->setItem(row, 2, new QTableWidgetItem(query.value("phone").toString()));
        targetTable->setItem(row, 3, new QTableWidgetItem(query.value("address").toString()));
        row++;
    }

    if (row == 0) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("提示");
        msgBox.setText("暂无同地址业主信息");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: #f8f9fa;
                border-radius: 8px;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
            }
            QLabel {
                color: #495057;
                font-weight: 500;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: 500;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton#approveButton {
                background: darkblue;
                color: white;
            }
            QPushButton#approveButton:hover {
                background: lightblue;
            }
        )");
        msgBox.exec();
    }
}

void OwnerWindow::showRegistration()
{
    ui.stackedWidget->setCurrentIndex(0);
    QSqlQuery query = g_db->getUserInfo(m_ownerId);
    if (query.next()) {
        QString address = query.value("address").toString();
        ui.lineEdit_address->setText(address);
        loadSameAddressUsers(address, ui.tableWidget_3);
    }
}

void OwnerWindow::saveModification()
{
    QString userIdStr = ui.lineEdit_modifyUserId->text().trimmed();
    if (userIdStr.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("输入错误");
        msgBox.setText("请输入要修改信息的 UserID");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: #f8f9fa;
                border-radius: 8px;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
            }
            QLabel {
                color: #495057;
                font-weight: 500;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: 500;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton#approveButton {
                background: darkblue;
                color: white;
            }
            QPushButton#approveButton:hover {
                background: lightblue;
            }
        )");
        msgBox.exec();
        return;
    }

    bool isUserIdValid;
    int userId = userIdStr.toInt(&isUserIdValid);
    if (!isUserIdValid) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("输入错误");
        msgBox.setText("UserID 必须是有效整数");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: #f8f9fa;
                border-radius: 8px;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
            }
            QLabel {
                color: #495057;
                font-weight: 500;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: 500;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton#approveButton {
                background: darkblue;
                color: white;
            }
            QPushButton#approveButton:hover {
                background: lightblue;
            }
        )");
        msgBox.exec();
        return;
    }

    QString newName = ui.lineEdit_modifyName->text().trimmed();
    QString newPhone = ui.lineEdit_modifyPhone->text().trimmed();
    QString newAddress = ui.lineEdit_modifyAddress->text().trimmed();

    if (g_db->updateUserInfo(userId, newName, newPhone, newAddress)) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("成功");
        msgBox.setText("信息修改完成");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: #f8f9fa;
                border-radius: 8px;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
            }
            QLabel {
                color: #495057;
                font-weight: 500;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: 500;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton#approveButton {
                background: darkblue;
                color: white;
            }
            QPushButton#approveButton:hover {
                background: lightblue;
            }
        )");
        msgBox.exec();

        ui.tableWidget_modify->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");
        loadSameAddressUsers(newAddress, ui.tableWidget_modify);
    } else {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("失败");
        msgBox.setText("信息修改失败，请检查数据库");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: #f8f9fa;
                border-radius: 8px;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
            }
            QLabel {
                color: #495057;
                font-weight: 500;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: 500;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton#approveButton {
                background: darkblue;
                color: white;
            }
            QPushButton#approveButton:hover {
                background: lightblue;
            }
        )");
        msgBox.exec();
    }
}

void OwnerWindow::loadUserInfo()
{
    QSqlQuery query = g_db->getUserInfo(m_ownerId);
    if (query.next()) {
        ui.lineEdit_address->setText(query.value("address").toString());
        ui.lineEdit_modifyName->setText(query.value("name").toString());
        ui.lineEdit_modifyPhone->setText(query.value("phone").toString());
        ui.lineEdit_modifyAddress->setText(query.value("address").toString());
    }
}

void OwnerWindow::showInquiry()
{
    ui.stackedWidget->setCurrentIndex(2);
}

void OwnerWindow::showRepair()
{
    ui.stackedWidget->setCurrentIndex(3);
    loadRepairRecords(m_ownerId);
}
void OwnerWindow::loadRepairRecords(int userId) {
    QSqlQuery query = g_db->getRepairRecordsByUserId(userId);
    ui.tableRepair->setRowCount(0);
    ui.tableRepair->setColumnCount(6);
    ui.tableRepair->setHorizontalHeaderLabels({
        "报修ID", "业主ID", "设备ID", "故障描述", "状态", "创建时间" // 对应 create_time
    });

    int row = 0;
    while (query.next()) {
        ui.tableRepair->insertRow(row);
        for (int col = 0; col < 6; col++) {
            ui.tableRepair->setItem(row, col,
                new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}


void OwnerWindow::submitRepair() {
    int userId = m_ownerId;
    int repairId = ui.spinBoxRepairId->value();
    int deviceId = ui.spinBoxDeviceId->value();
    QString description = ui.textEditRepairDesc->toPlainText().trimmed();

    if (repairId <= 0) {
        QMessageBox::warning(this, "输入错误", "报修 ID 需为正整数！");
        return;
    }
    if (description.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "故障描述不能为空！");
        return;
    }

    if (g_db->insertRepairRequest(repairId, userId, deviceId, description)) {
        QMessageBox::information(this, "提交成功", "维修申请已提交（状态：待维修）");

        loadRepairRecords(userId);
        ui.textEditRepairDesc->clear();

        QTimer::singleShot(5000, [this, repairId]() {
            if (g_db->markRepairAsCompleted(repairId)) {
                qDebug() << "报修单" << repairId << "已标记为“已完成”";
                loadRepairFeedbackTable();
            }
        });
    } else {
        QMessageBox::critical(this, "提交失败", "数据库错误：" + g_db->lastError().text());
    }
}

void OwnerWindow::showPayment()
{
    ui.stackedWidget->setCurrentIndex(6);
    loadPaymentRecords(m_ownerId);
}

void OwnerWindow::submitAddPayment()
{
    int userId = ui.lineEditUserId->text().toInt();
    double amount = ui.lineEditAmount->text().toDouble();
    QString type = ui.comboBoxPayType->currentText();
    QString status = ui.comboBoxStatus->currentText();
    QString payTime = ui.dateEditPayTime->date().toString("yyyy-MM-dd");
    QString payMethod = ui.comboBoxPayMethod->currentText();

    if (userId == 0 || amount <= 0) {
        QMessageBox::warning(this, "输入错误", "用户 ID 和金额不能为空！");
        return;
    }

    if (payMethod == "微信" || payMethod == "支付宝") {
        QMessageBox qrMsg(QMessageBox::Information,
                         "扫码支付",
                         QString("请使用%1扫码支付").arg(payMethod),
                         QMessageBox::Ok,
                         this);

        QIcon qrIcon(QString(":/images/QRcode.png"));
        qrMsg.setIconPixmap(qrIcon.pixmap(200, 200));  // 设置合适的大小

        qrMsg.exec();
        status = "已支付";
    } else if (payMethod == "银行卡") {
        bool ok;
        QString cardInfo = QInputDialog::getMultiLineText(this, "银行卡支付",
                                                          "请填写银行卡信息：",
                                                          "卡号：\n密码：", &ok);
        if (!ok || cardInfo.isEmpty()) {
            QMessageBox::warning(this, "支付取消", "银行卡信息未填写，支付取消！");
            return;
        }
        qDebug() << "银行卡信息：" << cardInfo;
    }

    if (g_db->insertPaymentRecord(userId, amount, type, status, payTime, payMethod)) {
        QMessageBox::information(this, "添加成功", "费用记录已添加！");
        loadPaymentRecords(userId);
        ui.lineEditUserId->clear();
        ui.lineEditAmount->clear();
        ui.comboBoxPayType->setCurrentIndex(0);
        ui.comboBoxStatus->setCurrentIndex(0);
        ui.dateEditPayTime->setDate(QDate::currentDate());
        ui.comboBoxPayMethod->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "添加失败",
                             QString("数据库错误：%1").arg(g_db->lastError().text()));
    }
}

void OwnerWindow::submitEditPayment()
{
    QModelIndexList selectedRows = ui.tableWidgetPayments->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "操作错误", "请选择要修改的费用记录！");
        return;
    }
    int row = selectedRows.first().row();
    int paymentId = ui.tableWidgetPayments->item(row, 0)->text().toInt();

    int userId = ui.lineEditUserId->text().toInt();
    double amount = ui.lineEditAmount->text().toDouble();
    QString type = ui.comboBoxPayType->currentText();
    QString status = ui.comboBoxStatus->currentText();
    QString payTime = ui.dateEditPayTime->date().toString("yyyy-MM-dd");
    QString payMethod = ui.comboBoxPayMethod->currentText();

    if (payMethod == "微信" || payMethod == "支付宝") {
        QMessageBox qrMsg(QMessageBox::Information, "扫码支付",
                          "请使用" + payMethod + "扫码支付：\n\n"
                          "<img src='QRcode.png' width='200' height='200'/>",
                          QMessageBox::Ok, this);
        qrMsg.exec();
        status = "已支付";
    } else if (payMethod == "银行卡") {
        bool ok;
        QString cardInfo = QInputDialog::getMultiLineText(this, "银行卡支付",
                                                          "请填写银行卡信息：",
                                                          "卡号：\n密码：", &ok);
        if (!ok || cardInfo.isEmpty()) {
            QMessageBox::warning(this, "支付取消", "银行卡信息未填写，支付取消！");
            return;
        }
        qDebug() << "银行卡信息：" << cardInfo;
    }

    if (g_db->updatePaymentRecord(paymentId, userId, amount, type, status, payTime, payMethod)) {
        QMessageBox::information(this, "修改成功", "费用记录已更新！");
        loadPaymentRecords(userId);
    } else {
        QMessageBox::critical(this, "修改失败",
                             QString("数据库错误：%1").arg(g_db->lastError().text()));
    }
}


void OwnerWindow::loadPaymentRecords(int userId)
{
    QSqlQuery query = g_db->getPaymentRecordsByUserId(userId);

    ui.tableWidgetPayments->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");

    ui.tableWidgetPayments->setRowCount(0);
    ui.tableWidgetPayments->setColumnCount(6);
    ui.tableWidgetPayments->setHorizontalHeaderLabels({"费用ID", "用户ID", "金额", "类型", "状态", "交付时间"});

    int row = 0;
    while (query.next()) {
        ui.tableWidgetPayments->insertRow(row);
        ui.tableWidgetPayments->setItem(row, 0, new QTableWidgetItem(query.value("PaymentID").toString()));
        ui.tableWidgetPayments->setItem(row, 1, new QTableWidgetItem(query.value("UserID").toString()));
        ui.tableWidgetPayments->setItem(row, 2, new QTableWidgetItem(query.value("Amount").toString()));
        ui.tableWidgetPayments->setItem(row, 3, new QTableWidgetItem(query.value("Type").toString()));
        ui.tableWidgetPayments->setItem(row, 4, new QTableWidgetItem(query.value("Status").toString()));
        ui.tableWidgetPayments->setItem(row, 5, new QTableWidgetItem(query.value("PayTime").toString()));
        row++;
    }


}

void OwnerWindow::showParking()
{
    ui.stackedWidget->setCurrentIndex(5);
    loadParkingRecords(m_ownerId);
}
void OwnerWindow::searchOwnerInfo()
{
    QString searchType = ui.comboBox_searchType->currentText();
    QString searchText = ui.lineEdit_search->text().trimmed();

    QString condition;
    if (searchType == "姓名") {
        condition = QString("name = '%1'").arg(searchText);
    } else if (searchType == "UserID") {
        condition = QString("UserID = %1").arg(searchText.toInt());
    } else if (searchType == "地址") {
        condition = QString("address LIKE '%%1%'").arg(searchText);
    } else if (searchType == "全部" && searchText.isEmpty()) {
        condition = "";
    } else if (searchType == "全部" && !searchText.isEmpty()) {
        condition = QString("name LIKE '%%1%' OR UserID LIKE '%%1%' OR address LIKE '%%1%'")
                   .arg(searchText);
    }

    QSqlQuery query = g_db->searchUsers(condition);



    int row = 0;
    while (query.next()) {
        ui.tableWidget_searchResult->insertRow(row);
        for (int col = 0; col < 4; col++) {
            QTableWidgetItem* item = new QTableWidgetItem(query.value(col).toString());
            item->setTextAlignment(Qt::AlignCenter);
            QFont font = item->font();
            font.setPointSize(10);
            item->setFont(font);
            ui.tableWidget_searchResult->setItem(row, col, item);
        }
        row++;
    }

    if (row == 0) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("查询结果");
        msgBox.setText("未找到匹配的业主信息");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background: white;
                border-radius: 8px;
                border: 1px solid #ddd;
                padding: 20px;
                font-family: "Microsoft YaHei", sans-serif;
                box-shadow: 0 4px 8px rgba(0,0,0,0.1);
            }
            QLabel {
                color: #333;
                font-weight: normal;
                font-size: 14px;
                padding: 5px 0;
            }
            QPushButton {
                background-color: #3498db;
                color: white;
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 14px;
                min-width: 80px;
                transition: all 0.2s;
            }
            QPushButton:hover {
                background-color: #2980b9;
                transform: translateY(-1px);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            QPushButton:pressed {
                transform: translateY(0px);
                box-shadow: none;
            }
        )");
        msgBox.exec();
    }
}

void OwnerWindow::submitParkingRequest() {
    int userId = ui.lineEditUserId_2->text().toInt();
    QString type = ui.comboBoxType_2->currentText();
    QString location = ui.comboBoxLocation->currentText();
    QString startDate = ui.dateEditStartDate->date().toString("yyyy-MM-dd");

    QSqlQuery spotQuery = g_db->getAvailableParkingSpot(type, location, "空闲");
    if (!spotQuery.next()) {
        QMessageBox::warning(this, "申请失败", "无符合条件的空闲车位！");
        return;
    }
    int spotId = spotQuery.value("SpotID").toInt();

    if (g_db->insertParkingRequest(userId, spotId, startDate)) {
        QMessageBox::information(this, "提交成功", "车位申请已提交！");
        loadParkingRecords(userId);
    } else {
        QMessageBox::critical(this, "提交失败", "数据库错误：" + g_db->lastError().text());
    }
}

void OwnerWindow::loadParkingRecords(int userId) {
    QSqlQuery query = g_db->getParkingRecordsByUserId(userId);
    ui.tableParkingSpots->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");
    ui.tableParkingSpots->setColumnCount(6);
    ui.tableParkingSpots->setHorizontalHeaderLabels({
        "车位ID", "类型", "位置", "价格", "申请状态", "起始日期"
    });

    int row = 0;
    while (query.next()) {
        ui.tableParkingSpots->insertRow(row);
        ui.tableParkingSpots->setItem(row, 0, new QTableWidgetItem(query.value("车位ID").toString()));
        ui.tableParkingSpots->setItem(row, 1, new QTableWidgetItem(query.value("类型").toString()));
        ui.tableParkingSpots->setItem(row, 2, new QTableWidgetItem(query.value("位置").toString()));
        ui.tableParkingSpots->setItem(row, 3, new QTableWidgetItem(query.value("价格").toString()));
        ui.tableParkingSpots->setItem(row, 4, new QTableWidgetItem(query.value("申请状态").toString()));
        ui.tableParkingSpots->setItem(row, 5, new QTableWidgetItem(query.value("起始日期").toString()));
        row++;
    }

}

void OwnerWindow::showModification()
{
    ui.stackedWidget->setCurrentIndex(1);
    loadUserInfo();
}

void OwnerWindow::loadOwnerTable()
{
    QSqlQuery query = g_db->getUserInfo(m_ownerId);

    ui.tableWidget_3->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");

    if (query.next()) {
        ui.tableWidget_3->setRowCount(1);
        ui.tableWidget_3->setItem(0, 0, new QTableWidgetItem(query.value("UserID").toString()));
        ui.tableWidget_3->setItem(0, 1, new QTableWidgetItem(query.value("name").toString()));
        ui.tableWidget_3->setItem(0, 2, new QTableWidgetItem(query.value("phone").toString()));
        ui.tableWidget_3->setItem(0, 3, new QTableWidgetItem(query.value("address").toString()));
    }
}
void OwnerWindow::loadRepairInfo()
{
    loadRepairRecords(m_ownerId);
}
void OwnerWindow::loadPaymentInfo()
{
    loadPaymentRecords(m_ownerId);
}

void OwnerWindow::loadParkingSpotsInfo()
{
    QSqlQuery query = g_db->getParkingSpotsInfo();
    ui.tableParkingSpots->setRowCount(0);
    ui.tableParkingSpots->setColumnCount(6);
    ui.tableParkingSpots->setHorizontalHeaderLabels({
        "车位ID", "类型", "位置", "价格", "状态", "起始日期"
    });

    int row = 0;
    while (query.next()) {
        ui.tableParkingSpots->insertRow(row);
        ui.tableParkingSpots->setItem(row, 0, new QTableWidgetItem(query.value("SpotID").toString()));
        ui.tableParkingSpots->setItem(row, 1, new QTableWidgetItem(query.value("type").toString()));
        ui.tableParkingSpots->setItem(row, 2, new QTableWidgetItem(query.value("location").toString()));
        ui.tableParkingSpots->setItem(row, 3, new QTableWidgetItem(query.value("price").toString()));
        ui.tableParkingSpots->setItem(row, 4, new QTableWidgetItem(query.value("status").toString()));
        ui.tableParkingSpots->setItem(row, 5, new QTableWidgetItem(query.value("start_date").toString()));
        row++;
    }
}

void OwnerWindow::submitParking()
{
    submitParkingRequest();
}

void OwnerWindow::submitPayment()
{
    submitAddPayment();
}

void OwnerWindow::initOwnerTableHeaders()
{

    ui.tableWidget_3->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");

    ui.tableWidget_3->setColumnCount(4);
    ui.tableWidget_3->setHorizontalHeaderLabels({"UserID", "姓名", "电话", "地址"});
    ui.tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void OwnerWindow::initPaymentTable() {
    ui.tableWidgetPayments->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");

    ui.tableWidgetPayments->setColumnCount(6);
    ui.tableWidgetPayments->setHorizontalHeaderLabels({
        "费用ID", "用户ID", "金额", "类型", "状态", "缴费时间"
    });

    ui.tableWidgetPayments->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableWidgetPayments->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableWidgetPayments->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void OwnerWindow::initParkingTableHeaders() {
    ui.tableParkingSpots->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");

    ui.tableParkingSpots->setColumnCount(6);
    ui.tableParkingSpots->setHorizontalHeaderLabels({
        "车位ID", "类型", "位置", "价格", "申请状态", "起始日期"
    });

    ui.tableParkingSpots->setColumnWidth(0, 100); // 车位ID列宽
    ui.tableParkingSpots->setColumnWidth(1, 80);  // 类型列宽
    ui.tableParkingSpots->setColumnWidth(2, 120); // 位置列宽
    ui.tableParkingSpots->setColumnWidth(3, 80);  // 价格列宽
    ui.tableParkingSpots->setColumnWidth(4, 100); // 申请状态列宽
    ui.tableParkingSpots->setColumnWidth(5, 120); // 起始日期列宽

    ui.tableParkingSpots->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui.tableParkingSpots->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableParkingSpots->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void OwnerWindow::submitRepairFeedback() {
    if (selectedRepairId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要评价的报修单！");
        return;
    }

    int rating = spinBoxRating->value();
    QString feedback = txtFeedback->toPlainText().trimmed();

    if (feedback.isEmpty()) {
        QMessageBox::warning(this, "提示", "评价内容不能为空！");
        return;
    }

    if (g_db->submitRepairFeedback(selectedRepairId, rating, feedback)) {
        QMessageBox::information(this, "成功", "评价提交成功！");
        selectedRepairId = -1;
        txtFeedback->clear();
        loadRepairFeedbackTable();
    } else {
        QMessageBox::critical(this, "失败", "提交失败：" + g_db->lastError().text());
    }
}

void OwnerWindow::showRepairFeedback()
{
    OwnerRepairEvalWidget* w=new OwnerRepairEvalWidget();
    w->show();
}

void OwnerWindow::on_tableRepairFeedback_itemClicked(QTableWidgetItem *item) {
    int row = item->row();

    QTableWidgetItem* repairIdItem = tableRepairFeedback->item(row, 0);
    if (repairIdItem) {
        selectedRepairId = repairIdItem->text().toInt();
        qDebug() << "选中的报修单ID:" << selectedRepairId;

        if (tableRepairFeedback->columnCount() > 5) {
            QTableWidgetItem* ratingItem = tableRepairFeedback->item(row, 5);
            QTableWidgetItem* commentItem = tableRepairFeedback->item(row, 6);

            if (ratingItem && !ratingItem->text().isEmpty()) {
                spinBoxRating->setValue(ratingItem->text().toInt());
            }

            if (commentItem && !commentItem->text().isEmpty()) {
                txtFeedback->setText(commentItem->text());
            }
        }
    }
}

void OwnerWindow::initRepairTableHeaders() {
    ui.tableRepair->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");

    ui.tableRepair->setColumnCount(6);
    ui.tableRepair->setHorizontalHeaderLabels({
        "报修ID", "业主ID", "设备ID", "故障描述", "状态", "提交时间"
    });

    ui.tableRepair->setColumnWidth(0, 100); // 报修ID列宽
    ui.tableRepair->setColumnWidth(1, 80);  // 业主ID列宽
    ui.tableRepair->setColumnWidth(2, 80);  // 设备ID列宽
    ui.tableRepair->setColumnWidth(3, 200); // 故障描述列宽
    ui.tableRepair->setColumnWidth(4, 80);  // 状态列宽
    ui.tableRepair->setColumnWidth(5, 120); // 提交时间列宽
    ui.tableRepair->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui.tableRepair->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableRepair->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void OwnerWindow::initRepairFeedbackTableHeaders() {
    if (tableRepairFeedback) {
        tableRepairFeedback->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");

        tableRepairFeedback->setColumnCount(5);
        tableRepairFeedback->setHorizontalHeaderLabels({
            "报修ID", "业主ID", "设备ID", "故障描述", "状态"
        });

        tableRepairFeedback->setColumnWidth(0, 100); // 报修ID列宽
        tableRepairFeedback->setColumnWidth(1, 80);  // 业主ID列宽
        tableRepairFeedback->setColumnWidth(2, 80);  // 设备ID列宽
        tableRepairFeedback->setColumnWidth(3, 200); // 故障描述列宽
        tableRepairFeedback->setColumnWidth(4, 80);  // 状态列宽
        tableRepairFeedback->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        tableRepairFeedback->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableRepairFeedback->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
}

void OwnerWindow::loadRepairFeedbackTable() {
    if (!tableRepairFeedback) return;

    tableRepairFeedback->setRowCount(0);

    QSqlQuery query = g_db->getUnratedRepairs(m_ownerId);

    while (query.next()) {
        int row = tableRepairFeedback->rowCount();
        tableRepairFeedback->insertRow(row);

        for (int col = 0; col < 5; col++) {
            QTableWidgetItem* item = new QTableWidgetItem(query.value(col).toString());
            tableRepairFeedback->setItem(row, col, item);
        }
    }
}

void OwnerWindow::initEventTableHeaders()
{
    tableEvents = new QTableWidget(this);
    tableEvents->setColumnCount(7);
    tableEvents->setHorizontalHeaderLabels({
        "活动ID", "标题", "类型", "时间", "地点", "最大名额", "剩余名额"
    });
    tableEvents->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableEvents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableEvents->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void OwnerWindow::loadEvents()
{
    tableEvents->clearContents();
    tableEvents->setRowCount(0);

    QSqlQuery query = g_db->execQuery("SELECT * FROM CommunityEvents ORDER BY time ASC");
    if (!query.isActive()) {
        QMessageBox::warning(this, "错误", "加载活动列表失败");
        return;
    }

    int row = 0;
    while (query.next()) {
        int eventId = query.value("EventID").toInt();
        QString title = query.value("title").toString();
        QString type = query.value("type").toString();
        QString time = query.value("time").toString();
        QString location = query.value("location").toString();
        int maxParticipants = query.value("max_participants").toInt();
        QString deadline = query.value("registration_deadline").toString();

        int currentParticipants = g_db->getEventCurrentParticipants(eventId);
        int remaining = maxParticipants - currentParticipants;

        tableEvents->insertRow(row);
        tableEvents->setItem(row, 0, new QTableWidgetItem(QString::number(eventId)));
        tableEvents->setItem(row, 1, new QTableWidgetItem(title));
        tableEvents->setItem(row, 2, new QTableWidgetItem(type));
        tableEvents->setItem(row, 3, new QTableWidgetItem(time));
        tableEvents->setItem(row, 4, new QTableWidgetItem(location));
        tableEvents->setItem(row, 5, new QTableWidgetItem(QString::number(maxParticipants)));
        tableEvents->setItem(row, 6, new QTableWidgetItem(QString::number(remaining)));

        if (currentParticipants >= maxParticipants) {
            for (int i = 0; i < tableEvents->columnCount(); i++) {
                tableEvents->item(row, i)->setBackgroundColor(QColor(255, 200, 200));
            }
        } else if (QDateTime::fromString(deadline, "yyyy-MM-dd") < QDateTime::currentDateTime()) {
            for (int i = 0; i < tableEvents->columnCount(); i++) {
                tableEvents->item(row, i)->setBackgroundColor(QColor(200, 200, 200));
            }
        }

        row++;
    }

    if (row == 0) {
        QMessageBox::information(this, "提示", "暂无社区活动");
    }
}

void OwnerWindow::showCommunityEvents()
{
    for (int i = 0; i < ui.stackedWidget->count(); i++) {
        if (ui.stackedWidget->widget(i)->objectName() == "eventPage") {
            ui.stackedWidget->setCurrentIndex(i);
            loadEvents();
            return;
        }
    }

    ui.stackedWidget->setCurrentIndex(ui.stackedWidget->count() - 2); // 假设倒数第二个是活动页面
    loadEvents();
}

void OwnerWindow::on_btnRefreshEvents_clicked()
{
    loadEvents();
    QMessageBox::information(this, "提示", "活动列表已刷新");
}

void OwnerWindow::on_tableEvents_itemClicked(QTableWidgetItem *item)
{
    m_selectedEventId = tableEvents->item(item->row(), 0)->text().toInt();
}

void OwnerWindow::on_btnSignUpEvent_clicked()
{
    if (m_selectedEventId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要报名的活动");
        return;
    }

    QSqlQuery query = g_db->execQuery(
        QString("SELECT * FROM CommunityEvents WHERE EventID = %1").arg(m_selectedEventId)
    );

    if (!query.next()) {
        QMessageBox::warning(this, "错误", "未找到该活动信息");
        return;
    }

    QDateTime deadline = QDateTime::fromString(
        query.value("registration_deadline").toString(),
        "yyyy-MM-dd"
    );

    if (deadline < QDateTime::currentDateTime()) {
        QMessageBox::warning(this, "提示", "该活动报名已截止");
        return;
    }

    int maxParticipants = query.value("max_participants").toInt();
    int currentParticipants = g_db->getEventCurrentParticipants(m_selectedEventId);

    if (currentParticipants >= maxParticipants) {
        QMessageBox::warning(this, "提示", "该活动名额已满");
        return;
    }

    if (g_db->isEventRegistered(m_ownerId, m_selectedEventId)) {
        QMessageBox::warning(this, "提示", "您已报名参加该活动");
        return;
    }

    if (QMessageBox::question(
        this,
        "确认报名",
        QString("您确定要报名参加活动 \"%1\" 吗？").arg(query.value("title").toString())
    ) != QMessageBox::Yes) {
        return;
    }

    if (g_db->submitEventRegistration(m_ownerId, m_selectedEventId)) {
        QMessageBox::information(this, "成功", "报名成功！");
        loadEvents(); // 刷新活动列表
    } else {
        QMessageBox::critical(this, "失败", "报名失败，请稍后再试");
    }
}

void OwnerWindow::on_btnCancelEvent_clicked()
{
    QPushButton* senderBtn = qobject_cast<QPushButton*>(sender());
    if (!senderBtn) return;

    int registrationId = senderBtn->property("registrationId").toInt();
    if (registrationId == -1) {
        QMessageBox::warning(this, "错误", "无效的报名记录");
        return;
    }

    if (QMessageBox::question(this, "确认取消", "确定要取消该活动报名吗？") != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query = g_db->execQuery(
        QString("DELETE FROM EventRegistrations WHERE RegistrationID = %1")
       .arg(registrationId)
    );
    if (query.exec()) {
        QMessageBox::information(this, "成功", "取消报名成功！");
    } else {
        QMessageBox::critical(this, "失败", "取消报名失败：" + query.lastError().text());
    }
}

