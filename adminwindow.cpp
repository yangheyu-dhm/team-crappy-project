#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "staffmanagewidget.h"
#include "leaveapprovewidget.h"
#include "attendancewidget.h"
#include "paymentmanagewidget.h"
#include "carmanagewidget.h"
#include "accesswidget.h"
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QScrollArea>
#include <QStackedWidget>

AdminWindow::AdminWindow(int userId, QWidget *parent) :
    QMainWindow(parent), m_userId(userId),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);

    stackedWidget = new QStackedWidget(this);
    staffWidget = new StaffManageWidget(this);
    leaveWidget = new LeaveApproveWidget(this);
    attendanceWidget = new AttendanceWidget(this);
    paymanageWidget = new PaymentmanageWidget(this);
    carmanageWidget = new CarmanageWidget(this);
    accessmanageWidget = new AccessWidget(this);

    stackedWidget->addWidget(staffWidget);
    stackedWidget->addWidget(leaveWidget);
    stackedWidget->addWidget(attendanceWidget);
    stackedWidget->addWidget(paymanageWidget);
    stackedWidget->addWidget(carmanageWidget);
    stackedWidget->addWidget(accessmanageWidget);

    setCentralWidget(stackedWidget);

    setupToolBar();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::setupToolBar()
{
    QToolBar *mainToolBar = addToolBar("主工具栏");
    mainToolBar->setObjectName("mainToolBar");
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 图标在文字旁边
    mainToolBar->setIconSize(QSize(48, 48));

    mainToolBar->setStyleSheet(R"(
        QToolBar {
            background-color: #f0f0f0;
            border: none;
            padding: 4px;
        }
        QToolButton {
            border-radius: 4px;
            padding: 6px 12px;
            margin: 4px;
        }
        QToolButton:hover {
            background-color: #e0e0e0;
        }
        QToolButton:pressed, QToolButton:checked {
            background-color: #d0d0d0;
        }
    )");

    QActionGroup *staffGroup = new QActionGroup(this);
    staffGroup->setExclusive(true);

    QAction *actStaffManage = new QAction(QIcon(":/images/tubiao1.png"), "物业工作人员维护", this);
    actStaffManage->setCheckable(true);
    actStaffManage->setActionGroup(staffGroup);
    mainToolBar->addAction(actStaffManage);

    QAction *actLeave = new QAction(QIcon(":/images/tubiao2.png"), "请销假审批", this);
    actLeave->setCheckable(true);
    actLeave->setActionGroup(staffGroup);
    mainToolBar->addAction(actLeave);

    QAction *actAttendance = new QAction(QIcon(":/images/tubiao3.png"), "出勤记录管理", this);
    actAttendance->setCheckable(true);
    actAttendance->setActionGroup(staffGroup);
    mainToolBar->addAction(actAttendance);

    QAction *actFinanceManage = new QAction(QIcon(":/images/tubiao4.png"), "业主缴费情况", this);
    actFinanceManage->setCheckable(true);
    actFinanceManage->setActionGroup(staffGroup);
    mainToolBar->addAction(actFinanceManage);

    QAction *actCarManage = new QAction(QIcon(":/images/tubiao5.png"), "车位信息记录", this);
    actCarManage->setCheckable(true);
    actCarManage->setActionGroup(staffGroup);
    mainToolBar->addAction(actCarManage);

    QAction *actAccessManage = new QAction(QIcon(":/images/tubiao6.png"), "门禁记录管理", this);
    actAccessManage->setCheckable(true);
    actAccessManage->setActionGroup(staffGroup);
    mainToolBar->addAction(actAccessManage);

    connect(actStaffManage, &QAction::triggered, this, &AdminWindow::showStaffManage);
    connect(actLeave, &QAction::triggered, this, &AdminWindow::showLeaveApprove);
    connect(actAttendance, &QAction::triggered, this, &AdminWindow::showAttendance);
    connect(actFinanceManage, &QAction::triggered, this, &AdminWindow::showPayment);
    connect(actCarManage, &QAction::triggered, this, &AdminWindow::showCar);
    connect(actAccessManage, &QAction::triggered, this, &AdminWindow::showAccess);

    actStaffManage->setChecked(true);
    showStaffManage();
}

void AdminWindow::showStaffManage()
{
    stackedWidget->setCurrentWidget(staffWidget);
}

void AdminWindow::showLeaveApprove()
{
    stackedWidget->setCurrentWidget(leaveWidget);
}

void AdminWindow::showAttendance()
{
    stackedWidget->setCurrentWidget(attendanceWidget);
}

void AdminWindow::showPayment()
{
    stackedWidget->setCurrentWidget(paymanageWidget);
}

void AdminWindow::showCar()
{
    stackedWidget->setCurrentWidget(carmanageWidget);
}

void AdminWindow::showAccess()
{
    stackedWidget->setCurrentWidget(accessmanageWidget);
}
