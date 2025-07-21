#include "daddwidget.h"
#include "ui_daddwidget.h"
#include<QString>
#include<QDateTime>
#include<QMessageBox>

DaddWidget::DaddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DaddWidget)
{
    ui->setupUi(this);
}

DaddWidget::~DaddWidget()
{
    delete ui;
}

void DaddWidget::on_okButton_clicked()
{
    QSqlQuery q;
    QString s1=ui->dnaE->text();
    QString s2=ui->dkE->text();
    QString s3=ui->lcE->text();
    QString s4=ui->sE->text();
    QDateTime date = ui->dateTimeEdit->dateTime();
    QString s5 = date.toString("yyyy-MM-dd HH:mm");
    if(s1==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入设备名!");
        return ;
    }
    else if(s2==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入设备种类!");
        return ;
    }
    else if(s3==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入设备位置!");
        return ;
    }
    else if(s4==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入状态!");
        return ;
    }
    QString sql="INSERT INTO Devices (name,type,location,status,Last_check) VALUES (?,?,?,?,?)";
    q.prepare(sql);
    q.addBindValue(s1);
    q.addBindValue(s2);
    q.addBindValue(s3);
    q.addBindValue(s4);
    q.addBindValue(s5);


    if (!q.exec()) {
        QMessageBox::information(  this,   "失败!",  "失败!");
    #ifdef QT_DEBUG
        qDebug() << "执行错误：" << q.lastError().text();
        qDebug() << "SQL:" << q.lastQuery();
        qDebug() << "绑定值:" << q.boundValues();
    #endif
    } else {
        QMessageBox::information(  this,   "成功!",  "成功加入数据!");

        qDebug() << "插入成功，ID:" << q.lastInsertId();
    }
    close();
}
