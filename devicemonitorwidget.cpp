#include "devicemonitorwidget.h"
#include "ui_devicemonitorwidget.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlRecord>
#include <QDebug>
#include <QSet>

DeviceMonitorWidget::DeviceMonitorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceMonitorWidget)
{
    ui->setupUi(this);
    model=new QSqlQueryModel();
    tm=new QSqlQueryModel();
    model->setQuery("select DeviceID,name,type,location,status,Last_check from Devices ");
    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(0, 200);
    ui->tableView->setColumnWidth(1, 200);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 200);
    ui->tableView->setColumnWidth(4, 200);
    ui->tableView->setColumnWidth(5, 200);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setStyleSheet("QHeaderView::section { background-color: darkblue; color: white; }QTableView::item:selected { selection-background-color: lightblue; }");
    QSet<QString> dnset,dkset,lset,sset,tset;
    int col = model->record().indexOf("location");
    for(int i=0;i<model->rowCount();i++){
        dnset.insert(model->data(model-> index (i, col-2)).toString());
        dkset.insert(model->data(model-> index (i, col-1)).toString());
        lset.insert(model->data(model-> index (i, col)).toString());
        sset.insert(model->data(model-> index (i, col+1)).toString());
        tset.insert(model->data(model-> index (i, col+2)).toString());
    }
    ui->dnab->addItems(dnset.values());
    ui->dkb->addItems(dkset.values());
    ui->lcb->addItems(lset.values());
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

void DeviceMonitorWidget::setsql(){
    QString sql="SELECT DeviceID,name,type,location,status,Last_check from Devices";
    int flag=0;
    QString id[6]={};
    id[0]=ui->didE->text();
    id[1]=ui->dnab->currentText();
    id[2]=ui->dkb->currentText();
    id[3]=ui->lcb->currentText();
    id[4]=ui->scb->currentText();
    id[5]=ui->tcb->currentText();
    QString col[6]={"DeviceID","name","type","location","status","Last_check"};
    for(int i=0;i<1;i++){
        if(id[i]=="")continue;
        if(flag==0){flag=1;sql+=" where ";}else{sql+=" AND ";}
        sql+=col[i]+" = "+id[i]+" ";
    }
    for(int i=1;i<6;i++){
        if(id[i][0]=="请")continue;
        if(flag==0){flag=1;sql+=" where ";}else{sql+=" AND ";}
        sql+=col[i]+" = '"+id[i]+"' ";
    }

    QSqlQuery query;
    query.prepare(sql);
    query.exec();
    model->setQuery(query);
}

void DeviceMonitorWidget::setcbox(){
    ui->dnab->clear();
    ui->dkb->clear();
    ui->lcb->clear();
    ui->scb->clear();
    ui->tcb->clear();
    ui->dnab->addItem("请选择设备名");
    ui->dkb->addItem("请选择设备种类");
    ui->lcb->addItem("请选择设备地点");
    ui->scb->addItem("请选择状态");
    ui->tcb->addItem("请选择上一次检查时间");
    tm->setQuery("select DeviceID,name,type,location,status,Last_check from Devices");
    QSet<QString> dnset,dkset,lset,sset,tset;
    int col = model->record().indexOf("location");
    for(int i=0;i<model->rowCount();i++){
        dnset.insert(model->data(model-> index (i, col-2)).toString());
        dkset.insert(model->data(model-> index (i, col-1)).toString());
        lset.insert(model->data(model-> index (i, col)).toString());
        sset.insert(model->data(model-> index (i, col+1)).toString());
        tset.insert(model->data(model-> index (i, col+2)).toString());
    }
    ui->dnab->addItems(dnset.values());
    ui->dkb->addItems(dkset.values());
    ui->lcb->addItems(lset.values());
    ui->scb->addItems(sset.values());
    ui->tcb->addItems(tset.values());
}

DeviceMonitorWidget::~DeviceMonitorWidget()
{
    delete ui;
}

void DeviceMonitorWidget::on_didE_editingFinished(){setsql();}
void DeviceMonitorWidget::on_dnab_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void DeviceMonitorWidget::on_dkb_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void DeviceMonitorWidget::on_lcb_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void DeviceMonitorWidget::on_scb_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void DeviceMonitorWidget::on_tcb_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}

#include "showcustominputdialog.h"
void DeviceMonitorWidget::edit(int x,int y,QWidget *parent)
{
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
        daw = new DaddWidget();
        daw->setAttribute(Qt::WA_DeleteOnClose);
        connect(daw, &DaddWidget::destroyed, this, [this]() {
            setsql();
            setcbox();
        });
        daw->show();
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
        QString sql="update Devices set "+tn+" = ";
        sql+=" '"+tst+"' ";
        sql+=" where DeviceID = "+idValue.toString();

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
        QString sql="delete from Devices where DeviceID = ";
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

#include "deviceupdatedialog.h"
void DeviceMonitorWidget::toupdate(){
    twoedit result = showme1(this);
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
    QString sql="update Devices set status = ";
    sql+=" '"+st2+"' ";
    sql+=" where DeviceID = "+st1;

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


