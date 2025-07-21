#ifndef LADDWIDGET_H
#define LADDWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

namespace Ui {
class LaddWidget;
}

class LaddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LaddWidget(QWidget *parent = nullptr);
    ~LaddWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::LaddWidget *ui;
};

#endif // LADDWIDGET_H
