#include "proaddwidget.h"
#include "ui_proaddwidget.h"
#include <QDateTime>
#include <QMessageBox>

ProaddWidget::ProaddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProaddWidget)
{
    ui->setupUi(this);
}

ProaddWidget::~ProaddWidget()
{
    delete ui;
}

void ProaddWidget::on_pushButton_clicked()
{
    QSqlQuery q;
    QString st1,st2,st3;
    st1=ui->lineEdit->text();
    st2=ui->textEdit->toPlainText();
    if(st1==""||st2==""){
        QMessageBox::information(this,"提示!","请输入标题和内容");
        return ;
    }
    QDateTime currentDateTime = QDateTime::currentDateTime();
    st3 = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString sql="insert into pronouncement (title,content,date) values(?,?,?)";
    q.prepare(sql);
    q.addBindValue(st1);
    q.addBindValue(st2);
    q.addBindValue(st3);
    q.exec();
    QMessageBox::information(this,"成功!","成功发布公告");
}
