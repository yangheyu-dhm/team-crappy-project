#include "attendancewidget.h"
#include "ui_attendancewidget.h"

AttendanceWidget::AttendanceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttendanceWidget)
{
    ui->setupUi(this);
    initUI();
}

void AttendanceWidget::initUI(){
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "AttendanceID" << "UserID" << "签到时间" << "签退时间" << "出勤状态" );
    tableWidget->setStyleSheet("QTableWidget { border: none; background: #f8f8f8; }"
                         "QHeaderView::section { background:#2d8cf0; color: white; font-weight: bold;font-size:18px;}"
                         "QTableWidget::item:selected { background:#cce6ff;}");

    QLabel* title = new QLabel("出勤记录管理",this);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);
    title->setStyleSheet("color:#2d8cf0; margin: 10px;");

    QLabel* statusLabel = new QLabel("筛选状态:", this);
    statusFilter = new QComboBox(this);
    statusFilter->addItem("全部", "");
    statusFilter->addItem("正常", "normal");
    statusFilter->addItem("迟到", "late");
    statusFilter->addItem("早退", "early");
    statusFilter->addItem("缺勤", "absent");

    QLabel* userIdLabel = new QLabel("查询UserID:", this);
    userIdEdit = new QLineEdit(this);
    userIdEdit->setPlaceholderText("输入UserID");

    QLabel* dateLabel = new QLabel("查询日期:", this);
    dateEdit = new QDateEdit(this);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate());


    searchButton1 = new QPushButton("查询", this);
    searchButton2 = new QPushButton("查询日期", this);


    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(statusLabel);
    filterLayout->addWidget(statusFilter);
    filterLayout->addSpacing(10);
    filterLayout->addWidget(userIdLabel);
    filterLayout->addWidget(userIdEdit);
    filterLayout->addSpacing(10);
    filterLayout->addWidget(searchButton1);
    filterLayout->addWidget(dateLabel);
    filterLayout->addWidget(dateEdit);
    filterLayout->addWidget(searchButton2);

    filterLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(tableWidget);
    setLayout(mainLayout);

    connect(statusFilter, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(filterByStatus(const QString&)));
    connect(searchButton1, SIGNAL(clicked()),
            this, SLOT(searchByUserId()));
    connect(searchButton2, SIGNAL(clicked()),
            this, SLOT(searchByDate()));
    connect(userIdEdit, SIGNAL(returnPressed()),
            this, SLOT(searchByUserId()));

    loadAttendance();
}

void AttendanceWidget::loadAttendance(const QString &statusFilter,
                                     const QString &userId,
                                     const QDate &date)
{
    tableWidget->setRowCount(0);

    QString queryStr = "SELECT * FROM Attendance";
    QVariantList queryParams;
    bool hasWhere = false;

    if (!statusFilter.isEmpty()) {
        queryStr += " WHERE status = :status";
        queryParams.append(statusFilter);
        hasWhere = true;
    }

    if (!userId.isEmpty()) {
        queryStr += hasWhere ? " AND" : " WHERE";
        queryStr += " UserID = :userId";
        queryParams.append(userId);
        hasWhere = true;
    }

    if (!date.isNull()) {
        queryStr += hasWhere ? " AND" : " WHERE";
        queryStr += " DATE(check_in) = :date";
        queryParams.append(date.toString("yyyy-MM-dd"));
    }

    database1 dbObj;
    QSqlQuery query = dbObj.query(queryStr, queryParams);

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
        QMessageBox::critical(this, "查询错误", "查询 Attendance 表失败：" + query.lastError().text());
    }
}

void AttendanceWidget::filterByStatus(const QString &status)
{
    QString statusValue = statusFilter->currentData().toString();
    loadAttendance(statusValue, userIdEdit->text()/*, dateEdit->date()*/);
}

void AttendanceWidget::searchByUserId()
{
    loadAttendance(statusFilter->currentData().toString(),
                  userIdEdit->text());
}

void AttendanceWidget::searchByDate()
{
    QDate selectedDate = dateEdit->date();
    dateEdit->setSpecialValueText("");

    loadAttendance(statusFilter->currentData().toString(),
                  userIdEdit->text(),
                  selectedDate);
}


AttendanceWidget::~AttendanceWidget()
{
    delete ui;
}
