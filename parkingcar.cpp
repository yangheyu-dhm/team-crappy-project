#include "parkingcar.h"
#include "ui_parkingcar.h"
#include "parkingcarin.h"

Parkingcar::Parkingcar(int userId,QWidget *parent) :
    QMainWindow(parent),int m_userId(userId),
    ui(new Ui::Parkingcar)
{
    ui->setupUi(this);

    parkingcarin = new  Parkingcarin(this);
}

Parkingcar::~Parkingcar()
{
    delete ui;
}
