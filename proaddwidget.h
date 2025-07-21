#ifndef PROADDWIDGET_H
#define PROADDWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

namespace Ui {
class ProaddWidget;
}

class ProaddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProaddWidget(QWidget *parent = nullptr);
    ~ProaddWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ProaddWidget *ui;

};

#endif // PROADDWIDGET_H
