#ifndef LEAVEAPPLICATIONWIDGET_H
#define LEAVEAPPLICATIONWIDGET_H

#include <QSqlQueryModel>
#include <QTableWidget>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QWidget>
#include <QDebug>
#include <QDir>


namespace Ui {
class LeaveApplicationWidget;
}

class LeaveApplicationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeaveApplicationWidget(QWidget *parent = nullptr);
    ~LeaveApplicationWidget();

private slots:
    void on_b1_currentTextChanged(const QString &arg1);

    void on_b2_currentTextChanged(const QString &arg1);

    void on_b3_currentTextChanged(const QString &arg1);

    void on_b4_currentTextChanged(const QString &arg1);

private:
    Ui::LeaveApplicationWidget *ui;

    QSqlQuery *q;

    QSqlQueryModel *model;

    QSqlQueryModel *tm;

    void setsql();

    void setcbox();
};

#endif // LEAVEAPPLICATIONWIDGET_H
