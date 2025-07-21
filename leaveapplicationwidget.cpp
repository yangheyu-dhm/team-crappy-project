#include "leaveapplicationwidget.h"
#include "ui_leaveapplicationwidget.h"

LeaveApplicationWidget::LeaveApplicationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeaveApplicationWidget)
{
    ui->setupUi(this);
    model=new QSqlQueryModel();
    tm=new QSqlQueryModel();
    model->setQuery("select start_time,end_time,reason,approval_status from LeaveRequests ");
    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(0, 300);
    ui->tableView->setColumnWidth(1, 300);
    ui->tableView->setColumnWidth(2, 300);
    ui->tableView->setColumnWidth(3, 300);

    ui->tableView->verticalHeader()->hide();

    ui->tableView->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");
    QSet<QString> st1,st2,st3,st4;
    for(int i=0;i<model->rowCount();i++){
        st1.insert(model->data(model-> index (i, 0)).toString());
        st2.insert(model->data(model-> index (i, 1)).toString());
        st3.insert(model->data(model-> index (i, 2)).toString());
        st4.insert(model->data(model-> index (i, 3)).toString());
    }
    ui->b1->addItems(st1.values());
    ui->b2->addItems(st2.values());
    ui->b3->addItems(st3.values());
    ui->b4->addItems(st4.values());

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

LeaveApplicationWidget::~LeaveApplicationWidget()
{
    delete ui;
}

void LeaveApplicationWidget::setsql(){
    QString sql="select start_time,end_time,reason,approval_status from LeaveRequests";
    int flag=0;
    QString id[4]={};
    id[0]=ui->b1->currentText();
    id[1]=ui->b2->currentText();
    id[2]=ui->b3->currentText();
    id[3]=ui->b4->currentText();
    QString col[4]={"start_time","end_time","reason","approval_status"};
    for(int i=0;i<4;i++){

        if(id[i][0]=="请")continue;
        if(flag==0){flag=1;sql+=" where ";}else{sql+=" AND ";}
        sql+=col[i]+" = '"+id[i]+"' ";

    }
#ifndef NDEBUG
        qDebug()<<sql;
#endif
    QSqlQuery query;
    query.prepare(sql);
    query.exec();
    model->setQuery(query);
}

void LeaveApplicationWidget::setcbox(){
    ui->b1->clear();
    ui->b2->clear();
    ui->b3->clear();
    ui->b4->clear();
    ui->b1->addItem("请选择开始时间");
    ui->b2->addItem("请选择结束时间");
    ui->b3->addItem("请选择原因");
    ui->b4->addItem("请选择状态");
    tm->setQuery("select start_time,end_time,reason,approval_status from LeaveRequests ");
    QSet<QString> st1,st2,st3,st4;
    for(int i=0;i<tm->rowCount();i++){
        st1.insert(tm->data(tm-> index (i, 0)).toString());
        st2.insert(tm->data(tm-> index (i, 1)).toString());
        st3.insert(tm->data(tm-> index (i, 2)).toString());
        st4.insert(tm->data(tm-> index (i, 3)).toString());
    }
    ui->b1->addItems(st1.values());
    ui->b2->addItems(st2.values());
    ui->b3->addItems(st3.values());
    ui->b4->addItems(st4.values());
}

void LeaveApplicationWidget::on_b1_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void LeaveApplicationWidget::on_b2_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void LeaveApplicationWidget::on_b3_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void LeaveApplicationWidget::on_b4_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
