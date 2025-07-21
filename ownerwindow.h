#ifndef OWNERWINDOW_H
#define OWNERWINDOW_H

#include <QMainWindow>
#include "ui_ownerwindow.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QInputDialog>
#include <QDate>
#include <QTableWidgetItem>
#include <QDateTime>
#include "database2.h"

class OwnerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OwnerWindow(int ownerId, QWidget *parent = nullptr);
    ~OwnerWindow();

private slots:
    void loadOwnerTable();
    void loadPaymentRecords(int userId);
    void submitAddPayment();
    void submitEditPayment();
    void setupMenus();
    void setupStackedWidget();
    void loadUserInfo();
    void loadRepairInfo();
    void loadPaymentInfo();
    void loadParkingSpotsInfo();
    void loadSameAddressUsers(const QString& address, QTableWidget* targetTable);
    void showRegistration();
    void showModification();
    void showInquiry();
    void showRepair();
    void showPayment();
    void showParking();
    void loadRepairRecords(int userId);
    void loadParkingRecords(int userId);
    void submitParkingRequest();
    void submitParking();
    void submitRegistration();
    void saveModification();
    void searchOwnerInfo();
    void submitRepair();
    void submitPayment();
    void initOwnerTableHeaders();
    void initPaymentTable();
    void initRepairTableHeaders();
    void initParkingTableHeaders();

    void showRepairFeedback();
    void loadRepairFeedbackTable();
    void submitRepairFeedback();
    void on_tableRepairFeedback_itemClicked(QTableWidgetItem *item);
    void initRepairFeedbackTableHeaders();


    void showCommunityEvents();
    void on_btnRefreshEvents_clicked();
    void on_btnSignUpEvent_clicked();
    void on_tableEvents_itemClicked(QTableWidgetItem *item);
    void on_btnCancelEvent_clicked();
    void updateTime();
private:
    Ui::OwnerWindow ui;
    int m_ownerId;
    DataBase2* g_db;
    int selectedRepairId;
    QLabel *timeLabel;
    QTimer *timer;
    QSpinBox* spinBoxRating = nullptr;
    QTextEdit* txtFeedback = nullptr;
    QTableWidget* tableRepairFeedback = nullptr;


    QTableWidget* tableEvents;
    int m_selectedEventId = -1;

    void setupCommunityEventsPage();
    void initEventTableHeaders();
    void loadEvents();
    bool isRegistered(int eventId);
    int getCurrentParticipants(int eventId);
     QTableWidget* tableMyEvents;
};

#endif // OWNERWINDOW_H
