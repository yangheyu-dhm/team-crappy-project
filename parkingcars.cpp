#include "parkingcars.h"
#include "ui_parkingcars.h"

ParkingCars::ParkingCars(int userId,QWidget *parent) :
    QMainWindow(parent), m_userId(userId),
    ui(new Ui::ParkingCars)
{
    ui->setupUi(this);

    stackedWidget = new QStackedWidget(this);
    parkingcarin = new ParkingCarin(this);
    parkingcarout = new Parkingcarout(this);

    stackedWidget->addWidget(parkingcarin);
    stackedWidget->addWidget(parkingcarout);

    setCentralWidget(stackedWidget);

    setupToolBar();
}

void ParkingCars::setupToolBar()
{
    QToolBar *mainToolBar = addToolBar("主工具栏");
    mainToolBar->setObjectName("mainToolBar");
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
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

    QActionGroup *ParkingcarGroup = new QActionGroup(this);
    ParkingcarGroup->setExclusive(true);

    QAction *actParkingcarin = new QAction(QIcon(":/images/tubiao8.png"), "驶入地下车库", this);
    actParkingcarin->setCheckable(true);
    actParkingcarin->setActionGroup(ParkingcarGroup);
    mainToolBar->addAction(actParkingcarin);

    QAction *actParkingcarout = new QAction(QIcon(":/images/tubiao7.png"), "驶出地下车库", this);
    actParkingcarout->setCheckable(true);
    actParkingcarout->setActionGroup(ParkingcarGroup);
    mainToolBar->addAction(actParkingcarout);

    connect(actParkingcarin, &QAction::triggered, this, &ParkingCars::showparkingcarin);
    connect(actParkingcarout, &QAction::triggered, this, &ParkingCars::showparkingcarout);

    actParkingcarin->setChecked(true);
    showparkingcarin();

}

void ParkingCars::showparkingcarin()
{
    stackedWidget->setCurrentWidget(parkingcarin);
}

void ParkingCars::showparkingcarout()
{
    stackedWidget->setCurrentWidget(parkingcarout);
}

ParkingCars::~ParkingCars()
{
    delete ui;
}
