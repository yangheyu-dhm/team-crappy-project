#include "staffmainwindow.h"
#include "ui_staffmainwindow.h"
#include "repairservicewidget.h"
#include "devicemonitorwidget.h"
#include "parkingmanagerwidget.h"
#include "leaveapplicationwidget.h"
#include "activitymanagerwidget.h"
#include "pronouncementwidget.h"
#include "aaddwidget.h"
#include "laddwidget.h"
#include "proaddwidget.h"
#include "parkingcars.h"
#include <QStackedWidget>
#include <QWidgetAction>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QToolButton>
#include <QMenuBar>
#include <QAction>
#include <QTimer>
#include <QDate>
#include <QMenu>

StaffMainWindow::StaffMainWindow(int id,QWidget *parent ) :
    QMainWindow(parent), id(id),
    ui(new Ui::StaffMainWindow)
{
    ui->setupUi(this);
    db.openDB("smartupc.db");

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
    connect(timer, &QTimer::timeout, this, &StaffMainWindow::updateTime);
    updateTime();
    timer->start(1000);

    toolBar = new QToolBar("导航工具栏", this);
    QMenu *m[6];
    for(int i=0;i<6;i++)m[i]=new QMenu();
    m[0]->addAction("处理报修请求");
    m[0]->addAction("更新报修状态");
    m[1]->addAction("更新设备状态");
    m[2]->addAction("审核车位申请");
    m[2]->addAction("车辆出入");
    m[3]->addAction("提交请假申请");
    m[4]->addAction("发布社区活动信息");
    m[5]->addAction("发布公告");
    QString s[6]={"repairicon.png","deviceicon.png","parkingicon.png","leaveicon.png","activitiyicon.png","anicon.png"};
    QString s2[6]={"维修服务","设备管理","车位管理","请销假","活动管理","公告管理"};
    QToolButton *b[6];
    QWidgetAction *w[6];
    for(int i=0;i<6;i++){
        b[i]=new QToolButton(this);
        w[i]=new QWidgetAction(this);
        b[i]->setText(s2[i]);
        b[i]->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        b[i]->setIcon(QIcon(":/images/"+s[i]));
        b[i]->setPopupMode(QToolButton::MenuButtonPopup);
        b[i]->setMenu(m[i]);
        w[i]->setDefaultWidget(b[i]);
        toolBar->addAction(w[i]);
    }
    toolBar->setIconSize(QSize(48, 48));
    toolBar->setMovable(false);



    addToolBar(Qt::TopToolBarArea, toolBar);

    stackedWidget = new QStackedWidget(this);

    //RepairServiceWidget
    //DeviceMonitorWidget
    //ParkingManagerWidget
    //LeaveApplicationWidget
    //ActivityManagerWidget
    //PronouncementWidget
    rsw = new RepairServiceWidget(this);
    dmw = new DeviceMonitorWidget(this);
    pmw = new ParkingManagerWidget(this);
    law = new LeaveApplicationWidget(this);
    amw = new ActivityManagerWidget(this);
    pro = new PronouncementWidget(this);
    aaw = new AaddWidget(this);
    ladw = new LaddWidget(this);
    pc = new ParkingCars(1,this);
    paddw =new ProaddWidget(this);

    stackedWidget->addWidget(rsw);
    stackedWidget->addWidget(dmw);
    stackedWidget->addWidget(pmw);
    stackedWidget->addWidget(law);
    stackedWidget->addWidget(amw);
    stackedWidget->addWidget(aaw);
    stackedWidget->addWidget(pro);
    stackedWidget->addWidget(ladw);
    stackedWidget->addWidget(pc);
    stackedWidget->addWidget(paddw);
    setCentralWidget(stackedWidget);
    connect(b[0], &QToolButton::clicked, this, &StaffMainWindow::torsw);
    connect(b[1], &QToolButton::clicked, this, &StaffMainWindow::todmw);
    connect(b[2], &QToolButton::clicked, this, &StaffMainWindow::topmw);
    connect(b[3], &QToolButton::clicked, this, &StaffMainWindow::tolaw);
    connect(b[4], &QToolButton::clicked, this, &StaffMainWindow::toamw);
    connect(b[5], &QToolButton::clicked, this, &StaffMainWindow::topro);

    connect(m[0]->actions()[0], &QAction::triggered, rsw,  [=]() {
        torsw();
        rsw->toprocess();

    });
    connect(m[0]->actions()[1], &QAction::triggered, rsw,  [=]() {
        torsw();
        rsw->toupdate();

    });
    connect(m[1]->actions()[0], &QAction::triggered, dmw,  [=]() {
        todmw();
        dmw->toupdate();

    });
    connect(m[2]->actions().first(), &QAction::triggered, pmw, [=]() {
        topmw();
        pmw->toapprove();

    });
    connect(m[3]->actions().first(), &QAction::triggered, pmw, [=]() {
        toladw();
    });

    connect(m[5]->actions().first(), &QAction::triggered, this, &StaffMainWindow::toproa);
    connect(m[4]->actions().first(), &QAction::triggered, this, &StaffMainWindow::toaaw);
    connect(m[3]->actions().first(), &QAction::triggered, this, &StaffMainWindow::toladw);
    connect(m[2]->actions()[1], &QAction::triggered, this, &StaffMainWindow::topc);


    torsw();
}

StaffMainWindow::~StaffMainWindow()
{
    delete ui;
}

void StaffMainWindow::torsw()
{
    stackedWidget->setCurrentWidget(rsw);
    toolBar->actions().at(0)->setChecked(true);
}

void StaffMainWindow::todmw()
{
    stackedWidget->setCurrentWidget(dmw);
    toolBar->actions().at(1)->setChecked(true);
}

void StaffMainWindow::topmw()
{
    stackedWidget->setCurrentWidget(pmw);
    toolBar->actions().at(2)->setChecked(true);
}

void StaffMainWindow::topc()
{
    stackedWidget->setCurrentWidget(pc);
    toolBar->actions().at(2)->setChecked(true);
}

void StaffMainWindow::tolaw()
{
    stackedWidget->setCurrentWidget(law);
    toolBar->actions().at(3)->setChecked(true);
}

void StaffMainWindow::toladw()
{
    stackedWidget->setCurrentWidget(ladw);
    toolBar->actions().at(3)->setChecked(true);
}
void StaffMainWindow::toamw()
{
    stackedWidget->setCurrentWidget(amw);
    toolBar->actions().at(4)->setChecked(true);
}

void StaffMainWindow::toaaw()
{
    stackedWidget->setCurrentWidget(aaw);
    toolBar->actions().at(4)->setChecked(true);

}
void StaffMainWindow::topro()
{
    stackedWidget->setCurrentWidget(pro);
    toolBar->actions().at(5)->setChecked(true);
}
void StaffMainWindow::toproa()
{
    stackedWidget->setCurrentWidget(paddw);
    toolBar->actions().at(5)->setChecked(true);
}


void StaffMainWindow::toedit()
{
    int t = stackedWidget->currentIndex();
    if(t==0){
        RepairServiceWidget *tw = new RepairServiceWidget();
        tw->edit(0,0);
    }
}
void StaffMainWindow::updateTime() {
    QString currentDateTime = QDate::currentDate().toString("yyyy-MM-dd") + " " +
                             QTime::currentTime().toString("hh:mm:ss");
    timeLabel->setText(currentDateTime);
}




