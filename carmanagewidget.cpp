#include "carmanagewidget.h"
#include "ui_carmanagewidget.h"

CarmanageWidget::CarmanageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarmanageWidget)
{
    ui->setupUi(this);
    initUI();
}

void CarmanageWidget::initUI() {
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(6);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "业主车位ID" << "车位类型" << "位置分区" << "车位价格（万）" << "使用情况" << "车牌号");
    tableWidget->setStyleSheet("QTableWidget { border: none; background: #f8f8f8; }"
                         "QHeaderView::section { background:#2d8cf0; color: white; font-weight: bold;font-size:18px;}"
                         "QTableWidget::item:selected { background:#cce6ff;}");

    tableWidget2 = new QTableWidget(this);
    tableWidget2->setColumnCount(6);
    tableWidget2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget2->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget2->setHorizontalHeaderLabels(QStringList() << "临时车位ID" << "车位类型" << "位置" << "费用（元/时）" << "是否被占用" << "车牌号");
    tableWidget2->setStyleSheet("QTableWidget { border: none; background: #f8f8f8; }"
                         "QHeaderView::section { background:#2d8cf0; color: white; font-weight: bold;font-size:18px;}"
                         "QTableWidget::item:selected { background:#cce6ff;}");

    QLabel* title = new QLabel("车位信息记录", this);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);
    title->setStyleSheet("color:#2d8cf0; margin: 10px;");

    QLabel* searchLabel = new QLabel("查询业主车位ID:", this);
    searchLineEdit = new QLineEdit(this);
    searchButton = new QPushButton("搜索", this);
    connect(searchButton, &QPushButton::clicked, this, &CarmanageWidget::onSearchSpotID);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addStretch();

    QLabel* filterLabel = new QLabel("筛选占用状态:", this);
    filterComboBox = new QComboBox(this);
    filterComboBox->addItems({"全部", "已占用", "未占用"});
    filterButton = new QPushButton("筛选", this);
    connect(filterButton, &QPushButton::clicked, this, &CarmanageWidget::onFilterOccupied);

    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterComboBox);
    filterLayout->addWidget(filterButton);
    filterLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(tableWidget2);
    setLayout(mainLayout);

    loadCarmanageWidget();
    loadTemParkingTable();
}

void CarmanageWidget::loadCarmanageWidget(const QString &condition) {
    tableWidget->setRowCount(0);
    QString queryStr = "SELECT * FROM ParkingSpots";
    QVariantList bindValues;

    if (!condition.isEmpty()) {
        queryStr += " WHERE SpotID = ?";
        bindValues.append(condition);
    }

    database1 dbObj;
    QSqlQuery query = dbObj.query(queryStr, bindValues);

    if (query.isActive()) {
        int row = 0;
        while (query.next()) {
            tableWidget->setRowCount(row + 1);
            for (int col = 0; col < 6; col++) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                tableWidget->setItem(row, col, item);
            }
            row++;
        }
    } else {
        QMessageBox::critical(this, "查询错误", "查询 ParkingSpots 表失败：" + query.lastError().text());
    }
}

void CarmanageWidget::loadTemParkingTable(const QString &condition) {
    tableWidget2->setRowCount(0); // 清空表格原有数据
    QString queryStr = "SELECT * FROM tem_parking";
    QVariantList bindValues;

    if (!condition.isEmpty()) {
        queryStr += " WHERE occupied = ?";
        bindValues.append(condition);
    }

    database1 dbObj;
    QSqlQuery query = dbObj.query(queryStr, bindValues);

    if (query.isActive()) {
        int row = 0;
        while (query.next()) {
            tableWidget2->setRowCount(row + 1);
            for (int col = 0; col < 6; col++) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                tableWidget2->setItem(row, col, item);
            }
            row++;
        }
    } else {
        QMessageBox::critical(this, "查询错误", "查询 tem_parking 表失败：" + query.lastError().text());
    }
}

void CarmanageWidget::onSearchSpotID() {
    QString searchText = searchLineEdit->text().trimmed();
    loadCarmanageWidget(searchText);
}

void CarmanageWidget::onFilterOccupied() {
    int index = filterComboBox->currentIndex();
    QString condition;

    switch (index) {
    case 0: // 全部
        condition = "";
        break;
    case 1: // 已占
        condition = "Yes";
        break;
    case 2: // 未占
        condition = "No";
        break;
    }

    loadTemParkingTable(condition);
}

CarmanageWidget::~CarmanageWidget()
{
    delete ui;
}
