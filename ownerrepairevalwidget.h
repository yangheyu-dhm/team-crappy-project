#ifndef OWNERREPAIREVALWIDGET_H
#define OWNERREPAIREVALWIDGET_H

#include <QDialog>

namespace Ui {
class OwnerRepairEvalWidget;
}

class OwnerRepairEvalWidget : public QDialog
{
    Q_OBJECT

public:
    explicit OwnerRepairEvalWidget(QWidget *parent = nullptr);
    ~OwnerRepairEvalWidget();

private slots:
    void on_btnSubmitEvaluation_clicked();
    
private:
    Ui::OwnerRepairEvalWidget *ui;
};

#endif // OWNERREPAIREVALWIDGET_H
