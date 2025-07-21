#include "ownerrepairevalwidget.h"
#include "ui_ownerrepairevalwidget.h"
#include <QString>
#include <QSqlQuery>
#include <QMessageBox>


OwnerRepairEvalWidget::OwnerRepairEvalWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OwnerRepairEvalWidget)
{
    ui->setupUi(this);
}

OwnerRepairEvalWidget::~OwnerRepairEvalWidget()
{
    delete ui;
}

void OwnerRepairEvalWidget::on_btnSubmitEvaluation_clicked()
{
    QString st1,st2,st3;
    st1=ui->comboBoxRepairOrderId->currentText();
    st2=ui->textEditEvaluation->toPlainText();
    st3=ui->lineEdit->text();
    if(st1==""||st2==""){
        QMessageBox::information(this,"提示","请输入id和评价");
    }
    QSqlQuery q;
    QString sql="update RepairFeedback set Comment = ?,score = ? where id = ?";
    q.prepare(sql);
    q.addBindValue(st1);
    q.addBindValue(st3);
    q.addBindValue(st2);
    q.exec();
    QMessageBox::information(this,"提示","评价成功!");
}
