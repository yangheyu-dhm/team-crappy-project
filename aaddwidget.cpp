#include "aaddwidget.h"
#include "ui_aaddwidget.h"
#include<QString>
#include<QDate>
#include<QMessageBox>

AaddWidget::AaddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AaddWidget)
{
    ui->setupUi(this);
}

AaddWidget::~AaddWidget()
{
    delete ui;
}

void AaddWidget::on_pushButton_clicked()
{
     QSqlQuery q;
     QString s1=ui->e1->text();
     QString s2=ui->e2->text();
     QDate d1 = ui->e3->date();
     QString s3=d1.toString("yyyy-MM-dd");
     QString s4=ui->e4->text();
     QString s5=ui->e5->text();
     QString s6=ui->e6->text();
     QDate d2 = ui->e7->date();
     QString s7=d2.toString("yyyy-MM-dd");
     if(s1==""){
         QMessageBox::information(  this,   "增加失败!",  "请输入活动名称!");
         return ;
     }
     else if(s2==""){
         QMessageBox::information(  this,   "增加失败!",  "请输入活动类型!");
         return ;
     }
     else if(s3==""){
         QMessageBox::information(  this,   "增加失败!",  "请输入活动时间!");
         return ;
     }
     else if(s4==""){
         QMessageBox::information(  this,   "增加失败!",  "请输入活动位置!");
         return ;
     }
     else if(s5==""){
         QMessageBox::information(  this,   "增加失败!",  "请输入活动内容!");
         return ;
     }
     else if(s6==""){
         QMessageBox::information(  this,   "增加失败!",  "请输入最大人数!");
         return ;
     }
     else if(s7==""){
         QMessageBox::information(  this,   "增加失败!",  "请输入报名截止时间!");
         return ;
     }
     QString sql="INSERT INTO CommunityEvents "
                 "(title,type,time,location,description,max_participants,registration_deadline) "
                 "VALUES (?,?,?,?,?,?,?)";
     q.prepare(sql);
     q.addBindValue(s1);
     q.addBindValue(s2);
     q.addBindValue(s3);
     q.addBindValue(s4);
     q.addBindValue(s5);
     q.addBindValue(s6);
     q.addBindValue(s7);


     if (!q.exec()) {
         QMessageBox::information(  this,   "失败!",  "失败!");
     #ifdef QT_DEBUG
         qDebug() << "执行错误：" << q.lastError().text();
         qDebug() << "SQL:" << q.lastQuery();
         qDebug() << "绑定值:" << q.boundValues();
     #endif
     } else {
         QMessageBox::information(  this,   "成功!",  "发布社区活动成功!");
         qDebug() << "插入成功，ID:" << q.lastInsertId();
     }
     ui->e1->clear();
     ui->e2->clear();
     ui->e4->clear();
     ui->e5->clear();
     ui->e6->clear();
}
