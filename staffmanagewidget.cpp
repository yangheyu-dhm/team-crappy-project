#include "staffmanagewidget.h"
#include "database1.h"
#include "ui_staffmanagewidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QLabel>
#include <QSpacerItem>
#include <QStyle>
#include <QApplication>

StaffManageWidget::StaffManageWidget(QWidget *parent) : QWidget(parent),ui(new Ui::StaffManageWidget)
{
    ui->setupUi(this);
    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"ID","用户名","姓名","部门","岗位","状态"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);// 斑马线
    table->setStyleSheet("QTableWidget { border: none; background: #f8f8f8; }"
                         "QHeaderView::section { background:#2d8cf0; color: white; font-weight: bold;font-size:18px;}"
                         "QTableWidget::item:selected { background:#cce6ff;}");
    btnAdd = new QPushButton("新增",this);
    btnEdit = new QPushButton("编辑",this);
    btnDelete = new QPushButton("删除",this);

    QString btnStyle ="oPushButton { background-color:#2d8cf0; color: white; border-radius: 6px;min-width: 80px; min-height: 28px; font-size: 14px;}"
                      "QPushButton:hover { background-color:#1a73e8;}";

    btnAdd->setStyleSheet(btnStyle);
    btnEdit->setStyleSheet(btnStyle.replace("#2d8cf0","#19be6b").replace("#1a73e8","#13a456"));
    btnDelete->setStyleSheet(btnStyle.replace("#2d8cf0","#f56c6c").replace("#1a73e8","#d93025"));

    auto* btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnAdd);
    btnLayout->addSpacing(10);
    btnLayout->addWidget(btnEdit);
    btnLayout->addSpacing(10);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();

    QLabel* title = new QLabel("物业工作人员信息维护",this);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);
    title->setStyleSheet("color:#2d8cf0; margin: 10px;");

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(table);


    connect(btnAdd,&QPushButton::clicked, this,&StaffManageWidget::onAdd);
    connect(btnEdit,&QPushButton::clicked, this,&StaffManageWidget::onEdit);
    connect(btnDelete,&QPushButton::clicked, this,&StaffManageWidget::onDelete);

    refreshTable();
}

void StaffManageWidget::refreshTable(){

    table->setRowCount(0);

    database1 db;

    db.openDB("smartupc.db");
    QSqlQuery query("SELECT users.id, users.username, users.name, users.department, "
                    "users.position, users.status "
                    "FROM users "
                    "WHERE users.role = 1");
    bool success = query.exec("SELECT users.id, users.username, users.name, users.department, "
                              "users.position, users.status "
                              "FROM users "
                              "WHERE users.role = 1");
    if(success){
        int row=0;
        while (query.next()){
            table->insertRow(row);
            for (int col = 0;col < 6;++col){
                QString val = query.value(col).toString();

                if(col == 5){
                    if(val =="1")val="在职";
                    else val ="离职";
                }
                table->setItem(row,col, new QTableWidgetItem(val));
            }
            ++row;
        }
        table->resizeRowsToContents();
    }
}

void StaffManageWidget::onAdd(){
    bool ok;
    QString username = QInputDialog::getText(this,"新增工作人员","用户名：",QLineEdit::Normal,"",&ok);
    if (!ok || username.isEmpty()) return;
    QString password = QInputDialog::getText(this,"新增工作人员","密码：",QLineEdit::Normal,"",&ok);
    if (!ok || password.isEmpty()) return;
    QString name = QInputDialog::getText(this,"新增工作人员","姓名：",QLineEdit::Normal,"",&ok);
    if (!ok)return;
    QString department = QInputDialog::getText(this,"新增工作人员","部门：",QLineEdit::Normal,"",&ok);
    if (!ok) return;
    QString position = QInputDialog::getText(this,"新增工昨人员","岗位：",QLineEdit::Normal,"",&ok);
    if (!ok) return;
    database1 db;
    db.openDB("smartupc.db");
    if (!db.addUser (username, password, 1, name,department,position,1)){
        QMessageBox::warning(this,"失败","添加账号失败，用户名可能已存在！");
        return;
    }

    refreshTable();
}

void StaffManageWidget::onEdit(){
    int status;
    QStringList statusOptions;
    statusOptions << "在职" << "离职";
    int row = table->currentRow();
    if (row < 0) return;
    int user_id = table->item(row,0)->text().toInt();
    QString username = QInputDialog::getText(this,"编辑用户名","用户名：",QLineEdit::Normal,table->item(row, 1)->text().trimmed());
    QString name = QInputDialog::getText(this,"编辑姓名","姓名：",QLineEdit::Normal,table->item(row, 2)->text().trimmed());
    QString department = QInputDialog::getText(this,"编辑部门","部门：",QLineEdit::Normal,table->item(row,3)->text().trimmed());
    QString position = QInputDialog::getText(this,"编辑岗位","岗位：",QLineEdit::Normal,table->item(row,4)->text().trimmed());

    QString status1 = QInputDialog::getItem(this,"编辑状态","状态：",statusOptions,statusOptions.indexOf(table->item(row, 5)->text().trimmed()),false);
    if(status1=="在职"){
        status = 1;
    }else{
        status = 0;
    }
    database1 db;
    db.openDB("smartupc.db");
    QSqlQuery q1;
    q1.prepare("UPDATE users SET username=?, name=?, department=?, position=?,status=? WHERE id=?");
    q1.addBindValue(username);
    q1.addBindValue(name);
    q1.addBindValue(department);
    q1.addBindValue(position);
    q1.addBindValue(status);
    q1.addBindValue(user_id);

    q1.exec();
    refreshTable();
}

void StaffManageWidget::onDelete(){
    int row = table->currentRow();
    if (row < 0) return;
    int user_id = table->item(row,0)->text().toInt();
    if (QMessageBox::question(this,"确认","确定要删除该工作人员吗？") != QMessageBox::Yes)return;
    database1 db;
    db.openDB("smartupc.db");
    QSqlQuery q2;
    q2.prepare("DELETE FROM users WHERE id=?");
    q2.addBindValue(user_id);
    q2.exec();

    refreshTable();
}
StaffManageWidget::~StaffManageWidget()
{
    delete ui;
}

