#include "repairservicewidget.h"
#include "ui_repairservicewidget.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlRecord>
#include <QDebug>
#include <QSet>

RepairServiceWidget::RepairServiceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RepairServiceWidget)
{
    ui->setupUi(this);
    model=new QSqlQueryModel();
    tm=new QSqlQueryModel();
    model->setQuery("select RepairID,UserID,DeviceID,description,status,create_time from repair");
    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(0, 200);
    ui->tableView->setColumnWidth(1, 200);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 200);
    ui->tableView->setColumnWidth(4, 200);
    ui->tableView->setColumnWidth(5, 200);

    ui->tableView->verticalHeader()->hide();

    ui->tableView->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");
    QSet<QString> dset,sset,tset;
    int col = model->record().indexOf("description");
    for(int i=0;i<model->rowCount();i++){
        dset.insert(model->data(model-> index (i, col)).toString());
        sset.insert(model->data(model-> index (i, col+1)).toString());
        tset.insert(model->data(model-> index (i, col+2)).toString());
    }
    ui->ecb->addItems(dset.values());
    ui->scb->addItems(sset.values());
    ui->tcb->addItems(tset.values());

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, &QTableView::customContextMenuRequested,[this](const QPoint &pos) {
        QModelIndex index = ui->tableView->indexAt(pos);
        if (index.isValid()) {
            int row = index.row();
            int col = index.column();
            this->edit(col, row);
        }
    });
    connect(ui->tableView, &QTableView::doubleClicked, [this](const QModelIndex &index) {
        if (index.isValid()) {
            int row = index.row();
            int col = index.column();
            this->edit(col, row);
        }
    });
}
RepairServiceWidget::~RepairServiceWidget()
{
    delete ui;
}

void RepairServiceWidget::setsql(){
    QString sql="SELECT RepairID,UserID,DeviceID,description,status,create_time FROM repair ";
    int flag=0;
    QString id[6]={};
    id[0]=ui->uidE->text();
    id[1]=ui->didE->text();
    id[2]=ui->ridE->text();
    id[3]=ui->ecb->currentText();
    id[4]=ui->scb->currentText();
    id[5]=ui->tcb->currentText();
    QString col[6]={"RepairID","UserID","DeviceID","description","status","create_time"};
    for(int i=0;i<3;i++){
        if(id[i]=="")continue;
        if(flag==0){flag=1;sql+=" where ";}else{sql+=" AND ";}
        sql+=col[i]+" = "+id[i]+" ";
    }
    for(int i=3;i<6;i++){
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

void RepairServiceWidget::setcbox(){
    ui->ecb->clear();
    ui->scb->clear();
    ui->tcb->clear();
    ui->ecb->addItem("请选择报修事件");
    ui->scb->addItem("请选择状态");
    ui->tcb->addItem("请选择时间");
    tm->setQuery("select RepairID,UserID,DeviceID,description,status,create_time from repair");
    QSet<QString> dset,sset,tset;
    int col = tm->record().indexOf("description");
    for(int i=0;i<tm->rowCount();i++){
        dset.insert(tm->data(tm-> index (i, col)).toString());
        sset.insert(tm->data(tm-> index (i, col+1)).toString());
        tset.insert(tm->data(tm-> index (i, col+2)).toString());
    }
    ui->ecb->addItems(dset.values());
    ui->scb->addItems(sset.values());
    ui->tcb->addItems(tset.values());
}

#include "showcustominputdialog.h"

void RepairServiceWidget::edit(int x,int y,QWidget *parent)
{
//    qDebug()<<x<<' '<<y;
    QMessageBox msgBox(parent);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "   min-width: 800px;"
        "   max-width: 800px;"
        "   min-height: 600px;"
        "   max-height: 600px;"
        "}"
    );
    msgBox.setWindowTitle("编辑");
    msgBox.setText("请选择要进行的操作:");
    msgBox.setWindowIcon(QIcon(":/images/qqicon.png"));
    QAbstractButton *addBtn = msgBox.addButton("增加", QMessageBox::ActionRole);
    QAbstractButton *editBtn = msgBox.addButton("修改", QMessageBox::ActionRole);
    QAbstractButton *deleteBtn = msgBox.addButton("删除", QMessageBox::ActionRole);
    QAbstractButton *cancleBtn = msgBox.addButton("取消", QMessageBox::ActionRole);
    Q_UNUSED(cancleBtn);
    msgBox.exec();
    if (msgBox.clickedButton() == addBtn) {
        raw = new RaddWidget();
        raw->setAttribute(Qt::WA_DeleteOnClose);

        connect(raw, &RaddWidget::destroyed, this, [this]() {
            setsql();
            setcbox();
        });
        raw->show();
    }
    else if (msgBox.clickedButton() == editBtn) {
        if(x==0){
            QMessageBox::information(  this, "修改失败!","无法修改报修id!");
            return ;
        }
        auto result = showCustomInputDialog(this);
        bool accepted = result.first;
        QString tst = result.second;
        if (!accepted) {
             return;
        }else if (tst=="") {
            QMessageBox::information(  this, "修改失败!","未输入数据!");
            return ;
        }
        q=new QSqlQuery();
        QVariant idValue = model->data(model->index(y, 0));//唯一键
        QString tn = model->headerData(x, Qt::Horizontal).toString();//要修改的列名
        QString sql="update repair set "+tn+" = ";
        if(x<=1)sql+=tst;
        else sql+=" '"+tst+"' ";
        sql+=" where RepairID = "+idValue.toString();
#ifndef NDEBUG
        qDebug()<<sql;
#endif
        q->prepare(sql);
        if(q->exec())
            QMessageBox::information(  this, "修改成功!","已修改数据!");
        else
            QMessageBox::information(  this, "修改失败!","error!");
        setsql();
        setcbox();
    }
    else if (msgBox.clickedButton() == deleteBtn){
        QMessageBox tbox(parent);
        tbox.setWindowTitle("提示");
        tbox.setText("确定删除这组数据吗？");
        tbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        tbox.setButtonText(QMessageBox::Ok, "确认");
        tbox.setButtonText(QMessageBox::Cancel, "取消");
        tbox.setDefaultButton(QMessageBox::Cancel);
        if(tbox.exec() == QMessageBox::Cancel){
            return;
        }
        q=new QSqlQuery();
        QString sql="delete from repair where RepairID = ";
        QVariant idValue = model->data(model->index(y, 0));
        sql+=idValue.toString();
#ifndef NDEBUG
        qDebug()<<sql;
#endif
        q->prepare(sql);
        if(q->exec())
            QMessageBox::information(  this, "删除成功!","已删除数据!");
        else
            QMessageBox::information(  this, "删除失败!","error!");

        setsql();
        setcbox();
    }
}

void RepairServiceWidget::on_uidE_editingFinished(){setsql();}
void RepairServiceWidget::on_didE_editingFinished(){setsql();}
void RepairServiceWidget::on_ridE_editingFinished(){setsql();}
void RepairServiceWidget::on_ecb_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void RepairServiceWidget::on_scb_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void RepairServiceWidget::on_tcb_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}

#include "doubleeditdialog.h"
void RepairServiceWidget::toupdate(){
    twoedit result = showme(this);
    int flag=result.flag;
    QString st1 = result.st1;
    QString st2 = result.st2;
    q=new QSqlQuery();
    QString tn;
    if(flag==0) return;
    if(st1==""){
        QMessageBox::information(this,"提示","未输入编号");
        return;
    }
    if(st2==""){
        QMessageBox::information(this,"提示","未输入状态");
        return;
    }
    QString sql="update repair set status = ";
    sql+=" '"+st2+"' ";
    sql+=" where RepairID = "+st1;

#ifndef NDEBUG
    qDebug()<<sql;
#endif
    q->prepare(sql);
    if(q->exec())
        QMessageBox::information(  this, "更新成功!","已更新!");
    else
        QMessageBox::information(  this, "更新失败!","error!");

    setsql();
    setcbox();
}

#include "processrequest.h"
void RepairServiceWidget::toprocess(){
    ui->scb->setCurrentText("待审核");
    auto result = repairdialog(this);
    QString accepted = result.first;
    QString tst = result.second;
    q=new QSqlQuery();
    QString tn;
    if(accepted=="approved") tn = "通过";
    else if(accepted=="rejected") tn = "拒绝";
    else return;

    QString sql="update repair set status = ";
    sql+=" '"+tn+"' ";
    sql+=" where RepairID = "+tst;

#ifndef NDEBUG
    qDebug()<<sql;
#endif
    q->prepare(sql);
    if(q->exec())
        QMessageBox::information(  this, "处理成功!","已处理!");
    else
        QMessageBox::information(  this, "处理失败!","error!");

    setsql();
    setcbox();
}









