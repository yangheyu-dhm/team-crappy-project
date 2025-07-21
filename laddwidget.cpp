#include "laddwidget.h"
#include "ui_laddwidget.h"
#include<QString>
#include<QDate>
#include<QMessageBox>

LaddWidget::LaddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LaddWidget)
{
    ui->setupUi(this);
}

LaddWidget::~LaddWidget()
{
    delete ui;
}

void LaddWidget::on_pushButton_clicked()
{
      int userid=1;
      QSqlQuery q;
      QDate d1 = ui->e1->date();
      QString s1=d1.toString("yyyy-MM-dd");
      QDate d2 = ui->e2->date();
      QString s2=d2.toString("yyyy-MM-dd");
      QString s3=ui->e3->text();
      if(s3==""){
          QMessageBox::information(  this,   "申请失败!",  "请输入原因!");
          return ;
      }
      QString sql="INSERT INTO LeaveRequests (UserID,start_time,end_time,reason,approval_status) VALUES (?,?,?,?,?)";
      q.prepare(sql);
      q.addBindValue(userid);
      q.addBindValue(s1);
      q.addBindValue(s2);
      q.addBindValue(s3);
      q.addBindValue("待审核");


      if (!q.exec()) {
          QMessageBox::information(  this,   "失败!",  "失败!");
      #ifdef QT_DEBUG
          qDebug() << "执行错误：" << q.lastError().text();
          qDebug() << "SQL:" << q.lastQuery();
          qDebug() << "绑定值:" << q.boundValues();
      #endif
      } else {
          QMessageBox::information(  this,   "成功!",  "申请成功!");
          qDebug() << "插入成功，ID:" << q.lastInsertId();
      }
      ui->e3->clear();
}
