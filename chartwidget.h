#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include "database.h" // 假设这是你的数据库操作类头文件
namespace Ui {
class chartwidget;
}

class chartwidget : public QWidget
{
    Q_OBJECT

public:
    explicit chartwidget(QWidget *parent = nullptr);
    ~chartwidget();

private:
    Ui::chartwidget *ui;
    Database *db;
};

#endif // CHARTWIDGET_H
