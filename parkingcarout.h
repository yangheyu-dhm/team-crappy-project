#ifndef PARKINGCAROUT_H
#define PARKINGCAROUT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Parkingcarout; }
QT_END_NAMESPACE

class Parkingcarout : public QWidget
{
    Q_OBJECT

public:
    explicit Parkingcarout(QWidget *parent = nullptr);
    ~Parkingcarout();

private slots:
    void queryCar();
    void onResultTimeout();

private:
    Ui::Parkingcarout *ui;
    QSqlDatabase db;

    QLabel *plateNumberLabel;
    QLineEdit *plateNumberEdit;
    QPushButton *queryButton;
    QLabel *resultLabel;
    QTableWidget *infoTable;
    QTimer* resultTimer;

    bool openDatabase();
    void createControls();
    void setupLayout();
    void connectSignals();
    void showParkingInfo(const QString &plateNumber, const QDateTime &startTime, const QDateTime &endTime, int hours, double fee);
    void clearTable();
    bool updateTemporaryParking(const QString &plateNumber); // 新增函数声明
};
#endif // PARKINGCAROUT_H
