#ifndef ACTIVITYMANAGERWIDGET_H
#define ACTIVITYMANAGERWIDGET_H

#include <QSqlQueryModel>
#include <QTableWidget>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QWidget>
#include <QDebug>
#include <QDir>


namespace Ui {
class ActivityManagerWidget;
}

class ActivityManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActivityManagerWidget(QWidget *parent = nullptr);
    ~ActivityManagerWidget();

private slots:
    void on_b1_currentTextChanged(const QString &arg1);

    void on_b2_currentTextChanged(const QString &arg1);

    void on_b3_currentTextChanged(const QString &arg1);

    void on_b4_currentTextChanged(const QString &arg1);

    void on_b5_currentTextChanged(const QString &arg1);

    void on_b6_currentTextChanged(const QString &arg1);

    void on_b7_currentTextChanged(const QString &arg1);

private:
    Ui::ActivityManagerWidget *ui;

    QSqlQuery *q;

    QSqlQueryModel *model;

    QSqlQueryModel *tm;

    void setsql();

    void setcbox();
};

#endif // ACTIVITYMANAGERWIDGET_H
