#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include "database.h"
#include <QWidget>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class loginpage;
}

class loginpage : public QWidget
{
    Q_OBJECT

public:
    explicit loginpage(QWidget *parent = nullptr);

    ~loginpage();

    void towindow(int x,int id);

private:
    Ui::loginpage *ui;

    Database db;

    int getid(QString st1,QString st2);
private slots:
    void on_pwvB_toggled(bool checked);

    void on_logB_clicked();

};

#endif // LOGINPAGE_H
