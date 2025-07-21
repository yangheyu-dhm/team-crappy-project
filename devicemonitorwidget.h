#ifndef DEVICEMONITORWIDGET_H
#define DEVICEMONITORWIDGET_H

#include "daddwidget.h"
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QWidget>
#include <QDebug>
#include <QDir>

namespace Ui {
class DeviceMonitorWidget;
}

class DeviceMonitorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceMonitorWidget(QWidget *parent = nullptr);
    ~DeviceMonitorWidget();
    void edit(int x,int y,QWidget *parent = nullptr);

private slots:
    void on_didE_editingFinished();

    void on_dnab_currentTextChanged(const QString &arg1);

    void on_dkb_currentTextChanged(const QString &arg1);

    void on_lcb_currentTextChanged(const QString &arg1);

    void on_scb_currentTextChanged(const QString &arg1);

    void on_tcb_currentTextChanged(const QString &arg1);

public slots:
    void toupdate();

private:
    Ui::DeviceMonitorWidget *ui;

    QSqlQuery *q;

    QSqlQueryModel *model;

    QSqlQueryModel *tm;

    void setsql();

    void setcbox();

    DaddWidget*daw;
};

#endif // DEVICEMONITORWIDGET_H
