#ifndef STAFFMAINWINDOW_H
#define STAFFMAINWINDOW_H

#include "database.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <QSqlError>
#include <QSqlQuery>
#include <QToolBar>
#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QDir>

namespace Ui {class StaffMainWindow;}
//RepairServiceWidget
//DeviceMonitorWidget
//ParkingManagerWidget
//LeaveApplicationWidget
//ActivityManagerWidget

class RepairServiceWidget;
class DeviceMonitorWidget;
class ParkingManagerWidget;
class LeaveApplicationWidget;
class ActivityManagerWidget;
class AaddWidget;
class LaddWidget;
class ParkingCars;
class PronouncementWidget;
class ProaddWidget;


class StaffMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StaffMainWindow( int id,QWidget *parent = nullptr);
    ~StaffMainWindow();

private slots:
    void torsw();
    void todmw();
    void topmw();
    void tolaw();
    void toamw();
    void toaaw();
    void toladw();
    void topc();
    void topro();
    void toproa();
    void toedit();
    void updateTime();


private:
    Ui::StaffMainWindow *ui;

    QToolBar *toolBar;

    QStackedWidget* stackedWidget;

    Database db;
    int id;
    RepairServiceWidget*rsw;
    DeviceMonitorWidget*dmw;
    ParkingManagerWidget*pmw;
    LeaveApplicationWidget*law;
    ActivityManagerWidget*amw;
    AaddWidget*aaw;
    LaddWidget*ladw;
    ParkingCars*pc;
    PronouncementWidget*pro;
    ProaddWidget*paddw;

    QTimer *timer;
    QLabel *timeLabel;
};

#endif // STAFFMAINWINDOW_H
