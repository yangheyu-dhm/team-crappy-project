#ifndef LEAVEAPPROVEWIDGET_H
#define LEAVEAPPROVEWIDGET_H
#include "adminwindow.h"
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDateTime>
#include "database1.h"

namespace Ui {
class LeaveApproveWidget;
}

class LeaveApproveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeaveApproveWidget(QWidget *parent = nullptr);
    ~LeaveApproveWidget();

private:
    Ui::LeaveApproveWidget *ui;
    QTableWidget *tableWidget;
    void initUI();
    void loadLeaveRequests();
    void updateApprovalStatus(int row, const QString& newStatus);
    void handleApprovalAction(const QString& newStatus);

private slots:
    void onApproveButtonClicked();
    void onRejectButtonClicked();
};

#endif // LEAVEAPPROVEWIDGET_H
