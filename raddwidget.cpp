#include "raddwidget.h"
#include "ui_raddwidget.h"
#include<QString>
#include<QDate>
#include<QMessageBox>

RaddWidget::RaddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RaddWidget)
{
    ui->setupUi(this);
    ui->datel->setText("当前日期: " + QDate::currentDate().toString("yyyy-MM-dd"));
}

RaddWidget::~RaddWidget()
{
    delete ui;
}

void RaddWidget::on_okButton_clicked()
{
    QSqlQuery q;
    QString s1=ui->uidE->text();
    QString s2=ui->didE->text();
    QString s3=ui->deidE->text();
    QString s4=ui->sE->text();
    QDate currentDate = QDate::currentDate();
    QString s5 = currentDate.toString("yyyy-MM-dd");
    if(s1==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入用户id!");
        return ;
    }
    else if(s2==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入设备id!");
        return ;
    }
    else if(s3==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入报修事务!");
        return ;
    }
    else if(s4==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入状态!");
        return ;
    }
    QString sql="INSERT INTO repair (UserID,DeviceID,description,status,create_time) VALUES (?,?,?,?,?)";
    q.prepare(sql);
    q.addBindValue(s1.toInt());
    q.addBindValue(s2.toInt());
    q.addBindValue(s3);
    q.addBindValue(s4);
    q.addBindValue(s5);


    if (!q.exec()) {
        QMessageBox::information(  this,   "失败!",  "失败!");
        qDebug() << "执行错误：" << q.lastError().text();
        qDebug() << "SQL:" << q.lastQuery();
        qDebug() << "绑定值:" << q.boundValues();
    } else {
        QMessageBox::information(  this,   "成功!",  "成功加入数据!");

        qDebug() << "插入成功，ID:" << q.lastInsertId();
    }
    close();
}
