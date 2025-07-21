#include "paymentmanagewidget.h"
#include "ui_paymentmanagewidget.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QFont>
#include <QSqlError>

PaymentmanageWidget::PaymentmanageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaymentmanageWidget)
{
    ui->setupUi(this);
    initUI();
}

void PaymentmanageWidget::initUI() {
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(6);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setHorizontalHeaderLabels(QStringList()
        << "PaymentID" << "UserID" << "金额" << "缴费类型" << "支付状态" << "缴费时间");
    tableWidget->setStyleSheet("QTableWidget { border: none; background: #f8f8f8; }"
                               "QHeaderView::section { background:#2d8cf0; color: white; font-weight: bold;font-size:18px;}"
                               "QTableWidget::item:selected { background:#cce6ff;}");

    QLabel* title = new QLabel("缴费记录管理", this);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);
    title->setStyleSheet("color:#2d8cf0; margin: 10px;");

    QHBoxLayout *searchLayout = new QHBoxLayout;
    QLabel *searchLabel = new QLabel("用户ID:", this);
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("请输入用户ID");

    searchButton = new QPushButton("查询", this);
    searchButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2d8cf0;"
        "    color: white;"
        "    border-radius: 5px;"
        "    padding: 5px 15px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #409eff;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1e70bf;"
        "}"
    );

    showAllButton = new QPushButton("显示全部", this);
    showAllButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #19be6b;"
        "    color: white;"
        "    border-radius: 5px;"
        "    padding: 5px 15px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #36d399;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #0e7490;"
        "}"
    );

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(showAllButton);

    QHBoxLayout *summaryLayout = new QHBoxLayout;
    QLabel *summaryTitle = new QLabel("缴费统计：", this);
    summaryTitle->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));

    totalAmountLabel = new QLabel("应收总金额：0.00 元", this);
    totalAmountLabel->setStyleSheet("color: #333;");

    paidAmountLabel = new QLabel("已收总金额：0.00 元", this);
    paidAmountLabel->setStyleSheet("color: green;");  // 已收绿色

    unpaidAmountLabel = new QLabel("未收总金额：0.00 元", this);
    unpaidAmountLabel->setStyleSheet("color: red;");    // 未收红色

    summaryLayout->addWidget(summaryTitle);
    summaryLayout->addWidget(totalAmountLabel);
    summaryLayout->addSpacing(20);
    summaryLayout->addWidget(paidAmountLabel);
    summaryLayout->addSpacing(20);
    summaryLayout->addWidget(unpaidAmountLabel);
    summaryLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addLayout(searchLayout);
    layout->addWidget(tableWidget);
    layout->addLayout(summaryLayout);
    setLayout(layout);

    connect(searchButton, &QPushButton::clicked, this, &PaymentmanageWidget::onSearchButtonClicked);
    connect(showAllButton, &QPushButton::clicked, this, &PaymentmanageWidget::onShowAllButtonClicked);
    connect(searchButton, &QPushButton::clicked, [this]() { animateButton(searchButton); });
    connect(showAllButton, &QPushButton::clicked, [this]() { animateButton(showAllButton); });

    loadPaymentmanageWidget();
}

void PaymentmanageWidget::animateButton(QPushButton* button) {
    QPropertyAnimation *animation = new QPropertyAnimation(button, "size");
    animation->setDuration(100);
    animation->setStartValue(button->size());
    animation->setEndValue(QSize(button->width() - 4, button->height() - 4));
    animation->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation *animation2 = new QPropertyAnimation(button, "size");
    animation2->setDuration(100);
    animation2->setStartValue(QSize(button->width() - 4, button->height() - 4));
    animation2->setEndValue(button->size());
    animation2->setEasingCurve(QEasingCurve::InQuad);

    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(button);
    group->addAnimation(animation);
    group->addAnimation(animation2);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void PaymentmanageWidget::onSearchButtonClicked() {
    QString userId = searchEdit->text().trimmed();
    loadPaymentmanageWidget(userId);
}

void PaymentmanageWidget::onShowAllButtonClicked() {
    searchEdit->clear();
    loadPaymentmanageWidget();
}

void PaymentmanageWidget::loadPaymentmanageWidget(const QString& userId) {
    tableWidget->setRowCount(0);

    QString sql = "SELECT * FROM Payments";
    QVariantList params;

    if (!userId.isEmpty()) {
        sql += " WHERE UserID = ?";
        params.append(userId);
    }

    database1 dbObj;
    QSqlQuery query = dbObj.query(sql, params);
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

        if (row == 0 && !userId.isEmpty()) {
            QMessageBox::information(this, "查询结果", "未找到该用户ID的缴费记录");
        }
    } else {
        QMessageBox::critical(this, "查询错误", "查询 Payments 表失败：" + query.lastError().text());
    }

    calculateSummary(userId);
}

void PaymentmanageWidget::calculateSummary(const QString& userId) {
    QString totalSql = "SELECT SUM(amount) FROM Payments";
    QString paidSql = "SELECT SUM(amount) FROM Payments WHERE status = '已支付'";
    QString unpaidSql = "SELECT SUM(amount) FROM Payments WHERE status != '已支付'";

    if (!userId.isEmpty()) {
        totalSql += " WHERE UserID = ?";
        paidSql += " AND UserID = ?";
        unpaidSql += " AND UserID = ?";
    }

    database1 dbObj;
    double total = 0.0, paid = 0.0, unpaid = 0.0;

    QSqlQuery totalQuery = dbObj.query(totalSql, userId.isEmpty() ? QVariantList() : QVariantList() << userId);
    if (totalQuery.next()) {
        total = totalQuery.value(0).toDouble();
    }

    QSqlQuery paidQuery = dbObj.query(paidSql, userId.isEmpty() ? QVariantList() : QVariantList() << userId);
    if (paidQuery.next()) {
        paid = paidQuery.value(0).toDouble();
    }

    QSqlQuery unpaidQuery = dbObj.query(unpaidSql, userId.isEmpty() ? QVariantList() : QVariantList() << userId);
    if (unpaidQuery.next()) {
        unpaid = unpaidQuery.value(0).toDouble();
    }

    totalAmountLabel->setText(QString("应收总金额：%1 元").arg(total, 0, 'f', 2));
    paidAmountLabel->setText(QString("已收总金额：%1 元").arg(paid, 0, 'f', 2));
    unpaidAmountLabel->setText(QString("未收总金额：%1 元").arg(unpaid, 0, 'f', 2));
}

PaymentmanageWidget::~PaymentmanageWidget() {
    delete ui;
}
