#ifndef RADDWIDGET_H
#define RADDWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

namespace Ui {
class RaddWidget;
}

class RaddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RaddWidget(QWidget *parent = nullptr);
    ~RaddWidget();

private slots:
    void on_okButton_clicked();

private:
    Ui::RaddWidget *ui;




};

#endif // RADDWIDGET_H
