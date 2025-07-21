#include "loginpage.h"
#include "ui_loginpage.h"
#include <QMessageBox>
#include "staffmainwindow.h"
#include "adminwindow.h"
#include "ownerwindow.h"
#include "pronouncementwidget.h"

loginpage::loginpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginpage)
{
    ui->setupUi(this);
    db.openDB("smartupc.db");
}

loginpage::~loginpage()
{
    delete ui;
}

void loginpage::towindow(int role,int id)
{
    if(role==1){
        StaffMainWindow *sw=new StaffMainWindow(1111);
        sw->show();
        this->hide();
        connect(sw, &StaffMainWindow::destroyed, this, [this]() {
            this->show();
        });
    }
    else if(role==0){
        AdminWindow *sw=new AdminWindow(id);
        sw->show();
        this->hide();
        connect(sw, &AdminWindow::destroyed, this, [this]() {
            this->show();
        });

    }
    else{
        OwnerWindow *sw=new OwnerWindow(id);
        PronouncementWidget* w=new PronouncementWidget();
        sw->show();
        w->show();
        this->hide();
        connect(sw, &AdminWindow::destroyed, this, [this]() {
            this->show();
        });
    }
}

void loginpage::on_pwvB_toggled(bool checked)
{
    if(checked)
        ui->pwE->setEchoMode(QLineEdit::Normal);
    else
        ui->pwE->setEchoMode(QLineEdit::Password);
}


void loginpage::on_logB_clicked()
{
    QString acc=ui->accE->text().trimmed();
    QString pw=ui->pwE->text().trimmed();
    QString cr=ui->chooseid->currentText();
    int crole;
    qDebug()<<cr;
    if(cr=="工作人员"){
        crole=1;
    }
    else if(cr=="管理员"){
        crole=0;
    }
    else{
        crole=2;
    }
    if(acc.isEmpty()||pw.isEmpty()){
        QMessageBox::warning(this,"提示","请输入用户名和密码");
        return;
    }

    QSqlQuery q=db.getpw(acc);
    if(q.next()){
        QString dbpw=q.value("password").toString();
        int role=q.value("role").toInt();
        int a=getid(acc,pw);
        if(pw==dbpw){
            if(role!=crole) {
                QMessageBox::warning(this,"提示","身份错误!");
                return;
            }
            QMessageBox::information(this,"提示","登录成功!");
            towindow(role,a);
        }
        else if(pw!=dbpw){
            QMessageBox::warning(this,"提示","密码错误!");
            return;
        }

    }
    else{
        QMessageBox::warning(this,"提示","用户名错误!");
        return;
    }
}

int loginpage::getid(QString st1, QString st2){
    QString sql="select id from users where username = ? and password = ?";
    QSqlQuery q;
    q.prepare(sql);
    q.addBindValue(st1);
    q.addBindValue(st2);
    q.exec();
    int a=q.value("id").toInt();
    return a;
}


