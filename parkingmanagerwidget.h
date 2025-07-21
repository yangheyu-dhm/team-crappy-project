#ifndef PARKINGMANAGERWIDGET_H
#define PARKINGMANAGERWIDGET_H

#include "paddwidget.h"
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>
#include <QDebug>
#include <QDir>


namespace Ui {
class ParkingManagerWidget;
}

class ParkingManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParkingManagerWidget(QWidget *parent = nullptr);
    ~ParkingManagerWidget();
    void edit(int x,int y,QWidget *parent = nullptr);

private slots:
    void on_e_editingFinished();

    void on_b1_currentTextChanged(const QString &arg1);

    void on_b2_currentTextChanged(const QString &arg1);

    void on_b3_currentTextChanged(const QString &arg1);

    void on_b4_currentTextChanged(const QString &arg1);

    void on_cB_clicked();

    void on_addB_clicked();

public slots:
    void toapprove();


private:
    Ui::ParkingManagerWidget *ui;

    QSqlQuery *q;

    QSqlQueryModel *model;

    QSqlQueryModel *tm;

    void setsql();

    void setcbox();

    PaddWidget*paw;
};

#endif // PARKINGMANAGERWIDGET_H
