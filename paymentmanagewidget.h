#ifndef PAYMENTMANAGEWIDGET_H
#define PAYMENTMANAGEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "database1.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PaymentmanageWidget; }
QT_END_NAMESPACE

class PaymentmanageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaymentmanageWidget(QWidget *parent = nullptr);
    ~PaymentmanageWidget();

private slots:
    void onSearchButtonClicked();
    void onShowAllButtonClicked();
    void animateButton(QPushButton* button);

private:
    Ui::PaymentmanageWidget *ui;
    QTableWidget *tableWidget;
    QLineEdit *searchEdit;
    QPushButton *searchButton;
    QPushButton *showAllButton;

    QLabel *totalAmountLabel;
    QLabel *paidAmountLabel;
    QLabel *unpaidAmountLabel;

    void initUI();
    void loadPaymentmanageWidget(const QString& userId = "");
    void calculateSummary(const QString& userId = "");
};

#endif // PAYMENTMANAGEWIDGET_H
