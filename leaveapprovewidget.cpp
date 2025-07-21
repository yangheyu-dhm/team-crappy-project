#include "leaveapprovewidget.h"
#include "ui_leaveapprovewidget.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QHeaderView>
#include <QLabel>

LeaveApproveWidget::LeaveApproveWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LeaveApproveWidget)
{
    ui->setupUi(this);
    initUI();
}

LeaveApproveWidget::~LeaveApproveWidget()
{
    delete ui;
}

void LeaveApproveWidget::initUI()
{
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(7);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "LeaveID" << "UserID" << "开始时间" << "结束时间" << "原因" << "审批状态" << "审批时间");
    tableWidget->setStyleSheet("QTableWidget { border: none; background: #f8f8f8; }"
                         "QHeaderView::section { background:#2d8cf0; color: white; font-weight: bold;font-size:18px;}"
                         "QTableWidget::item:selected { background:#cce6ff;}");

    QPushButton *approveBtn = new QPushButton("审批通过", this);
    QPushButton *rejectBtn = new QPushButton("审批拒绝", this);

    QLabel* title = new QLabel("请销假审批",this);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);
    title->setStyleSheet("color:#2d8cf0; margin: 10px;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addWidget(tableWidget);
    layout->addWidget(approveBtn);
    layout->addWidget(rejectBtn);
    setLayout(layout);

    connect(approveBtn, &QPushButton::clicked, this, &LeaveApproveWidget::onApproveButtonClicked);
    connect(rejectBtn, &QPushButton::clicked, this, &LeaveApproveWidget::onRejectButtonClicked);

    loadLeaveRequests();
}

void LeaveApproveWidget::loadLeaveRequests()
{
    database1 dbObj;
    QSqlQuery query = dbObj.query("SELECT * FROM LeaveRequests", QVariantList());
    if (query.isActive()) {
        int row = 0;
        while (query.next()) {
            tableWidget->setRowCount(row + 1);
            for (int col = 0; col < 7; col++) { // 对应表格列数
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                tableWidget->setItem(row, col, item);
            }
            row++;
        }
    } else {
        QMessageBox::critical(this, "查询错误", "查询 LeaveRequests 表失败：" + query.lastError().text());
    }
}

void LeaveApproveWidget::onApproveButtonClicked()
{
    handleApprovalAction("approved");
}

void LeaveApproveWidget::onRejectButtonClicked()
{
    handleApprovalAction("rejected");
}

void LeaveApproveWidget::handleApprovalAction(const QString& newStatus)
{
    const int currentRow = tableWidget->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "提示", "请选择要审批的记录");
        return;
    }


    const QString actionText = (newStatus == "approved") ? "通过" : "拒绝";
    const QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认审批",
        QString("确定要将选中的请假申请审批为%1吗？").arg(actionText),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        updateApprovalStatus(currentRow, newStatus);
    }
}

void LeaveApproveWidget::updateApprovalStatus(int row, const QString& newStatus)
{
    database1 dbObj;
    QTableWidgetItem* leaveIdItem = tableWidget->item(row, 0);
    if (!leaveIdItem) {
        qCritical() << "Failed to get LeaveID at row:" << row;
        QMessageBox::critical(this, "错误", "获取记录ID失败，请重试");
        return;
    }

    const int leaveId = leaveIdItem->text().toInt();
    const QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QVariantList args;
    args << newStatus << currentTime << leaveId;

    QSqlQuery query = dbObj.query("UPDATE LeaveRequests SET approval_status =?, approval_time =? WHERE LeaveID =?", args);
    if (query.isActive() && query.numRowsAffected() > 0) {
        if (tableWidget->item(row, 5)) tableWidget->item(row, 5)->setText(newStatus);
        if (tableWidget->item(row, 6)) tableWidget->item(row, 6)->setText(currentTime);

        qInfo() << "Leave request approved successfully. LeaveID:" << leaveId
                << "New status:" << newStatus;

        QMessageBox::information(this, "成功", "审批状态更新成功");


    } else {
        qCritical() << "Failed to update approval status. LeaveID:" << leaveId
                   << "Error:" << query.lastError().text();

        QMessageBox::critical(this, "错误", "更新审批状态失败：" + query.lastError().text());
    }
}
