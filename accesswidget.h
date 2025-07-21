#ifndef ACCESSWIDGET_H
#define ACCESSWIDGET_H
#include <QTableWidget>
#include <QWidget>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QSqlError>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include "database1.h"

namespace Ui {
class AccessWidget;
}

class AccessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AccessWidget(QWidget *parent = nullptr);
    ~AccessWidget();

private slots:
    void onSearchButtonClicked();
    void onFilterButtonClicked();
    void onResetButtonClicked();

private:
    Ui::AccessWidget *ui;
    QTableWidget *tableWidget;
    QLineEdit *searchLineEdit;
    QComboBox *filterComboBox;
    QPushButton *searchButton;
    QPushButton *filterButton;
    QPushButton *resetButton;

    QString currentUserIDFilter;
    QString currentStatusFilter;

    void initUI();
    void loadAccessmanageWidget();
    void updateTable(const QString &userID = "", const QString &status = "");
};

#endif // ACCESSWIDGET_H
