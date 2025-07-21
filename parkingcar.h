#ifndef PARKINGCAR_H
#define PARKINGCAR_H

#include <QMainWindow>
#include "parkingcarin.h"

namespace Ui {
class Parkingcar;
}

class ParkingCarin;
class ParkingCarout;

class Parkingcar : public QMainWindow
{
    Q_OBJECT

public:
    explicit Parkingcar(int userId,QWidget *parent = nullptr);
    ~Parkingcar();
private slots:
    void showparkingin();
    void showparkingout();

private:
    Ui::Parkingcar *ui;
    ParkingCarin* parkingcarin;
    void setupToolBar();
    int m_userId;
};

#endif // PARKINGCAR_H
