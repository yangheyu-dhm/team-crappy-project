#include "activitymanagerwidget.h"
#include "ui_activitymanagerwidget.h"

ActivityManagerWidget::ActivityManagerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActivityManagerWidget)
{
    ui->setupUi(this);
    model=new QSqlQueryModel();
    tm=new QSqlQueryModel();
    model->setQuery("select title,type,time,location,description,max_participants,registration_deadline from CommunityEvents ");
    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(0, 150);
    ui->tableView->setColumnWidth(1, 150);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 150);
    ui->tableView->setColumnWidth(4, 200);
    ui->tableView->setColumnWidth(5, 150);
    ui->tableView->setColumnWidth(6, 200);

    ui->tableView->verticalHeader()->hide();

    ui->tableView->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");
    QSet<QString> st1,st2,st3,st4,st5,st6,st7;
    for(int i=0;i<model->rowCount();i++){
        st1.insert(model->data(model-> index (i, 0)).toString());
        st2.insert(model->data(model-> index (i, 1)).toString());
        st3.insert(model->data(model-> index (i, 2)).toString());
        st4.insert(model->data(model-> index (i, 3)).toString());
        st5.insert(model->data(model-> index (i, 4)).toString());
        st6.insert(model->data(model-> index (i, 5)).toString());
        st7.insert(model->data(model-> index (i, 6)).toString());
    }
    ui->b1->addItems(st1.values());
    ui->b2->addItems(st2.values());
    ui->b3->addItems(st3.values());
    ui->b4->addItems(st4.values());
    ui->b5->addItems(st5.values());
    ui->b6->addItems(st6.values());
    ui->b7->addItems(st7.values());

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

ActivityManagerWidget::~ActivityManagerWidget()
{
    delete ui;
}

void ActivityManagerWidget::setsql(){
    QString sql="select title,type,time,location,description,max_participants,registration_deadline from CommunityEvents ";
    int flag=0;
    QString id[7]={};
    id[0]=ui->b1->currentText();
    id[1]=ui->b2->currentText();
    id[2]=ui->b3->currentText();
    id[3]=ui->b4->currentText();
    id[4]=ui->b5->currentText();
    id[5]=ui->b6->currentText();
    id[6]=ui->b7->currentText();
    QString col[7]={"title","type","time","location","description","max_participants","registration_deadline"};
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

void ActivityManagerWidget::setcbox(){
    ui->b1->clear();
    ui->b2->clear();
    ui->b3->clear();
    ui->b4->clear();
    ui->b5->clear();
    ui->b6->clear();
    ui->b7->clear();
    ui->b1->addItem("请选择活动名");
    ui->b2->addItem("请选择活动类型");
    ui->b3->addItem("请选择活动时间");
    ui->b4->addItem("请选择活动地点");
    ui->b5->addItem("请选择活动内容");
    ui->b6->addItem("请选择最大人数");
    ui->b7->addItem("请选择报名截止时间");
    tm->setQuery("select title,type,time,location,"
                 "description,max_participants,registration_deadline from CommunityEvents ");
    QSet<QString> st1,st2,st3,st4,st5,st6,st7;
    for(int i=0;i<tm->rowCount();i++){
        st1.insert(tm->data(tm-> index (i, 0)).toString());
        st2.insert(tm->data(tm-> index (i, 1)).toString());
        st3.insert(tm->data(tm-> index (i, 2)).toString());
        st4.insert(tm->data(tm-> index (i, 3)).toString());
        st5.insert(tm->data(tm-> index (i, 4)).toString());
        st6.insert(tm->data(tm-> index (i, 5)).toString());
        st7.insert(tm->data(tm-> index (i, 6)).toString());
    }
    ui->b1->addItems(st1.values());
    ui->b2->addItems(st2.values());
    ui->b3->addItems(st3.values());
    ui->b4->addItems(st4.values());
    ui->b5->addItems(st5.values());
    ui->b6->addItems(st6.values());
    ui->b7->addItems(st7.values());
}

void ActivityManagerWidget::on_b1_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void ActivityManagerWidget::on_b2_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void ActivityManagerWidget::on_b3_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void ActivityManagerWidget::on_b4_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void ActivityManagerWidget::on_b5_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void ActivityManagerWidget::on_b6_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void ActivityManagerWidget::on_b7_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
