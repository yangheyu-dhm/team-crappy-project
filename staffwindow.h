#ifndef STAFFWINDOW_H
#define STAFFWINDOW_H
#include <QStackedWidget>
#include <QMainWindow>
#include "adminwindow.h"
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDateTime>
#include "datebase1.h"


namespace Ui {
class StaffWindow;
}

class StaffWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StaffWindow(int userId,QWidget *parent = nullptr);
    ~StaffWindow();

private:
    int m_userId;
    void setupMenus();
    Ui::StaffWindow *ui;
};

#endif // STAFFWINDOW_H
