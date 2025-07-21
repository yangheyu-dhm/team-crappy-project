#ifndef AADDWIDGET_H
#define AADDWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

namespace Ui {
class AaddWidget;
}

class AaddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AaddWidget(QWidget *parent = nullptr);
    ~AaddWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AaddWidget *ui;
};

#endif // AADDWIDGET_H
