#ifndef PARKINGCARS_H
#define PARKINGCARS_H
#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QScrollArea>
#include "parkingcarin.h"
#include "parkingcarout.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class ParkingCars;
}
QT_END_NAMESPACE
class ParkingCarin;
class Parkingcarout;

class ParkingCars : public QMainWindow
{
    Q_OBJECT

public:
    explicit ParkingCars(int useId,QWidget *parent = nullptr);
    ~ParkingCars();
    void showparkingcarin();
    void showparkingcarout();

private:
    int m_userId;
    Ui::ParkingCars *ui;
    void setupToolBar();
    QStackedWidget* stackedWidget;
    ParkingCarin* parkingcarin;
    Parkingcarout* parkingcarout;
};

#endif // PARKINGCARS_H
