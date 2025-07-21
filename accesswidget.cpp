#include "accesswidget.h"
#include "ui_accesswidget.h"

AccessWidget::AccessWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccessWidget)
{
    ui->setupUi(this);
    initUI();
}

void AccessWidget::initUI()
{
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "RecondID" << "UserID" << "门禁使用时间" << "门禁设备ID" << "通行状态");
    tableWidget->setStyleSheet("QTableWidget {  background: #f8f8f8; }"
                         "QHeaderView::section { background:#2d8cf0; color: white; font-weight: bold;font-size:18px;}"
                         "QTableWidget::item:selected { background:#cce6ff;}");

    QLabel* title = new QLabel("门禁记录管理",this);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);
    title->setStyleSheet("color:#2d8cf0; margin: 10px;");

    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("输入UserID查询");

    filterComboBox = new QComboBox(this);
    filterComboBox->addItems({"全部状态", "success", "failed"});

    searchButton = new QPushButton("查询", this);
    filterButton = new QPushButton("筛选", this);
    resetButton = new QPushButton("重置", this);

    connect(searchButton, &QPushButton::clicked, this, &AccessWidget::onSearchButtonClicked);
    connect(filterButton, &QPushButton::clicked, this, &AccessWidget::onFilterButtonClicked);
    connect(resetButton, &QPushButton::clicked, this, &AccessWidget::onResetButtonClicked);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(new QLabel("UserID:", this));
    controlLayout->addWidget(searchLineEdit);
    controlLayout->addWidget(searchButton);
    controlLayout->addWidget(new QLabel("通行状态:", this));
    controlLayout->addWidget(filterComboBox);
    controlLayout->addWidget(filterButton);
    controlLayout->addWidget(resetButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(tableWidget);
    setLayout(mainLayout);

    currentUserIDFilter = "";
    currentStatusFilter = "";
    loadAccessmanageWidget();
}

void AccessWidget::loadAccessmanageWidget()
{
    updateTable();
}

void AccessWidget::updateTable(const QString &userID, const QString &status)
{
    QString queryStr = "SELECT * FROM AccessRecords";
    QVariantList queryValues;
    bool hasWhereClause = false;

    if (!userID.isEmpty()) {
        queryStr += hasWhereClause ? " AND" : " WHERE";
        queryStr += " UserID = ?";
        queryValues.append(userID);
        hasWhereClause = true;
    }

    if (!status.isEmpty() && status != "全部状态") {
        queryStr += hasWhereClause ? " AND" : " WHERE";
        queryStr += " status = ?";
        queryValues.append(status);
        hasWhereClause = true;
    }

    database1 dbObj;
    QSqlQuery query = dbObj.query(queryStr, queryValues);

    tableWidget->setRowCount(0);

    if (query.isActive()) {
        int row = 0;
        while (query.next()) {
            tableWidget->setRowCount(row + 1);
            for (int col = 0; col < 5; col++) { // 对应表格列数
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                tableWidget->setItem(row, col, item);
            }
            row++;
        }
    } else {
        QMessageBox::critical(this, "查询错误", "查询 AccessRecords 表失败：" + query.lastError().text());
    }
}

void AccessWidget::onSearchButtonClicked()
{
    currentUserIDFilter = searchLineEdit->text();
    updateTable(currentUserIDFilter, currentStatusFilter);
}

void AccessWidget::onFilterButtonClicked()
{
    currentStatusFilter = filterComboBox->currentText();
    updateTable(currentUserIDFilter, currentStatusFilter);
}

void AccessWidget::onResetButtonClicked()
{
    searchLineEdit->clear();
    filterComboBox->setCurrentIndex(0);
    currentUserIDFilter = "";
    currentStatusFilter = "";
    updateTable();
}

AccessWidget::~AccessWidget()
{
    delete ui;
}
