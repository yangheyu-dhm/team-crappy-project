#ifndef ATTENDANCEWIDGET_H
#define ATTENDANCEWIDGET_H
#include <QTableWidget>
#include <QWidget>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QSqlError>
#include <QVBoxLayout>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QDate>
#include "database1.h"

namespace Ui {
class AttendanceWidget;
}

class AttendanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AttendanceWidget(QWidget *parent = nullptr);
    ~AttendanceWidget();

private slots:
    void filterByStatus(const QString &status);
    void searchByUserId();
    void searchByDate();

private:
    Ui::AttendanceWidget *ui;
    QTableWidget *tableWidget;
    QComboBox *statusFilter;
    QLineEdit *userIdEdit;
    QPushButton *searchButton1;
    QPushButton *searchButton2;
    QDateEdit *dateEdit;
    void initUI();
    void loadAttendance(const QString &statusFilter = "",
                        const QString &userId = "",
                        const QDate &date = QDate());
};

#endif // ATTENDANCEWIDGET_H
