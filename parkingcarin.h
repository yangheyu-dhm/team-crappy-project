// parkingcarin.h
#ifndef PARKINGCAR_H
#define PARKINGCAR_H
#include "database1.h"
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlError>

namespace Ui {
class ParkingCarin;
}

class ParkingCarin : public QWidget
{
    Q_OBJECT

public:
    explicit ParkingCarin(QWidget *parent = nullptr);
    ~ParkingCarin();

private slots:
    void onQueryButtonClicked();
    void onResultTimeout();

private:
    QLineEdit* plateNumberEdit;
    QPushButton* queryButton;
    QLabel* resultLabel;
    QSqlDatabase db;
    QTimer* resultTimer;
    bool initDatabase();
    void setupUI();
    bool assignTemporaryParkingSpace(const QString& plateNumber);
    bool isDatabaseAvailable();
};

#endif // PARKINGCAR_H
