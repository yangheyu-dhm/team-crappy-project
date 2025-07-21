#ifndef PADDWIDGET_H
#define PADDWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>


namespace Ui {
class PaddWidget;
}

class PaddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaddWidget(QWidget *parent = nullptr);
    ~PaddWidget();

private slots:
    void on_okButton_clicked();

private:
    Ui::PaddWidget *ui;

};

#endif // PADDWIDGET_H
