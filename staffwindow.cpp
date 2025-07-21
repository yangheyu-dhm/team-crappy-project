#include "staffwindow.h"
#include "ui_staffwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStackedWidget>
StaffWindow::StaffWindow(int userId,QWidget *parent) :
    QMainWindow(parent),m_userId(userId),
    ui(new Ui::StaffWindow)
{
    ui->setupUi(this);
    setupMenus();
}

StaffWindow::~StaffWindow()
{
    delete ui;
}
void StaffWindow::setupMenus(){
    QMenuBar* menuBar = ui->menubar;
    menuBar->clear();

    QMenu* staffMenu = menuBar->addMenu("工作记录");
    QAction* actStaffManage = staffMenu->addAction("考勤记录");
    QAction* actLeave = staffMenu->addAction("工作进程");
    QAction* actAttendance = staffMenu->addAction("工作日志");
}
