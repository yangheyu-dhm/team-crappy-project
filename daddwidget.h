#ifndef DADDWIDGET_H
#define DADDWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDir>

namespace Ui {
class DaddWidget;
}

class DaddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DaddWidget(QWidget *parent = nullptr);
    ~DaddWidget();

private slots:
    void on_okButton_clicked();

private:
    Ui::DaddWidget *ui;

};

#endif // DADDWIDGET_H
