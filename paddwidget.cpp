#include "paddwidget.h"
#include "ui_paddwidget.h"
#include<QString>
#include<QDateTime>
#include<QMessageBox>

PaddWidget::PaddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaddWidget)
{
    ui->setupUi(this);
}

PaddWidget::~PaddWidget()
{
    delete ui;
}

void PaddWidget::on_okButton_clicked()
{
    QSqlQuery q;
    QString s1=ui->e1->text();
    QString s2=ui->e2->text();
    QString s3=ui->e3->text();
    QString s4=ui->e4->text();
    QString s5=ui->e5->text();
    if(s1==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入车位类型!");
        return ;
    }
    else if(s2==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入车位位置!");
        return ;
    }
    else if(s3==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入价格!");
        return ;
    }
    else if(s4==""){
        QMessageBox::information(  this,   "增加失败!",  "请输入状态!");
        return ;
    }
    QString sql="INSERT INTO ParkingSpots (type,location,price,status,carID) VALUES (?,?,?,?,?)";
    q.prepare(sql);
    q.addBindValue(s1);
    q.addBindValue(s2);
    q.addBindValue(s3.toInt());
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
    #ifdef QT_DEBUG
        qDebug() << "插入成功，ID:" << q.lastInsertId();
    #endif
    }
    close();
}
