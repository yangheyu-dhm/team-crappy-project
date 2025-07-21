#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

namespace Ui {
class StaffManageWidget;
}

class StaffManageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StaffManageWidget(QWidget *parent = nullptr);
    ~StaffManageWidget();
private slots:
    void onAdd();
    void onEdit();
    void onDelete();
    void refreshTable();
private:
    Ui::StaffManageWidget *ui;
    QTableWidget* table;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;
    void loadStaff();
};
