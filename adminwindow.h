#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H
#pragma once
#include <QMainWindow>
#include <QStackedWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
    class AdminWindow;
}
QT_END_NAMESPACE
class StaffManageWidget;
class LeaveApproveWidget;
class AttendanceWidget;
class PaymentmanageWidget;
class CarmanageWidget;
class AccessWidget;

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(int userId,QWidget *parent = nullptr);
    ~AdminWindow();
private slots:
    void showStaffManage();
    void showLeaveApprove();
    void showAttendance();
    void showPayment();
    void showCar();
    void showAccess();
private:
    void setupToolBar();
    int m_userId;
    Ui::AdminWindow *ui;
    QStackedWidget* stackedWidget;
    StaffManageWidget* staffWidget;
    LeaveApproveWidget* leaveWidget;
    AttendanceWidget* attendanceWidget;
    PaymentmanageWidget* paymanageWidget;
    CarmanageWidget* carmanageWidget;
    AccessWidget* accessmanageWidget;
};

#endif // ADMINWINDOW_H
