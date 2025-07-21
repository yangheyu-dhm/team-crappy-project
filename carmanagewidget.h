#ifndef CARMANAGEWIDGET_H
#define CARMANAGEWIDGET_H
#include <QTableWidget>
#include <QWidget>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QSqlError>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "database1.h"

namespace Ui {
class CarmanageWidget;
}

class CarmanageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CarmanageWidget(QWidget *parent = nullptr);
    ~CarmanageWidget();

private slots:
    void onSearchSpotID();
    void onFilterOccupied();

private:
    Ui::CarmanageWidget *ui;
    QTableWidget *tableWidget;
    QTableWidget *tableWidget2;
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QComboBox *filterComboBox;
    QPushButton *filterButton;
    void initUI();
    void loadCarmanageWidget(const QString &condition = "");
    void loadTemParkingTable(const QString &condition = "");
};

#endif // CARMANAGEWIDGET_H
