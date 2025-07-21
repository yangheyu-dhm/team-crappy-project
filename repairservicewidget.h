#ifndef REPAIRSERVICEWIDGET_H
#define REPAIRSERVICEWIDGET_H

#include "raddwidget.h"
#include <QWidget>
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

namespace Ui {
class RepairServiceWidget;
}

class RaddWidget;

class RepairServiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RepairServiceWidget(QWidget *parent = nullptr);
    ~RepairServiceWidget();
    void edit(int x,int y,QWidget *parent = nullptr);

private slots:
    void on_uidE_editingFinished();

    void on_didE_editingFinished();

    void on_ridE_editingFinished();

    void on_ecb_currentTextChanged(const QString &arg1);

    void on_tcb_currentTextChanged(const QString &arg1);

    void on_scb_currentTextChanged(const QString &arg1);

public slots:
    void toprocess();

    void toupdate();

private:
    Ui::RepairServiceWidget *ui;

    QSqlQuery *q;

    QSqlQueryModel *model;

    QSqlQueryModel *tm;

    void setsql();

    void setcbox();

    RaddWidget*raw;
};

#endif // REPAIRSERVICEWIDGET_H
