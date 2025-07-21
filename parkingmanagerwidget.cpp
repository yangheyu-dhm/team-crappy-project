#include "parkingmanagerwidget.h"
#include "ui_parkingmanagerwidget.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlRecord>
#include <QDebug>
#include <QSet>

ParkingManagerWidget::ParkingManagerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParkingManagerWidget)
{
    ui->setupUi(this);
    model=new QSqlQueryModel();
    tm=new QSqlQueryModel();
    model->setQuery("select SpotID,carID,location,price,status from ParkingSpots ");
    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(0, 240);
    ui->tableView->setColumnWidth(1, 240);
    ui->tableView->setColumnWidth(2, 240);
    ui->tableView->setColumnWidth(3, 240);
    ui->tableView->setColumnWidth(4, 240);

    ui->tableView->verticalHeader()->hide();

    ui->tableView->setStyleSheet("QHeaderView::section {"
                                 " background-color: darkblue; color: white; "
                                 "}"
                                 "QTableView::item:selected {"
                                 " selection-background-color: lightblue; "
                                 "}");
    QSet<QString> st1,st2,st4;
    for(int i=0;i<model->rowCount();i++){
        st1.insert(model->data(model-> index (i, 1)).toString());
        st2.insert(model->data(model-> index (i, 2)).toString());
        st4.insert(model->data(model-> index (i, 4)).toString());
    }
    ui->b1->addItems(st1.values());
    ui->b2->addItems(st2.values());
    ui->b4->addItems(st4.values());

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

void ParkingManagerWidget::setsql(){
    QString sql="select SpotID,carID,location,price,status from ParkingSpots ";
    int flag=0;
    QString id[5]={};
    id[0]=ui->e->text();
    id[1]=ui->b1->currentText();
    id[2]=ui->b2->currentText();
    id[3]=ui->e2->text();
    id[4]=ui->b4->currentText();
    QString col[6]={"SpotID","carID","location","price","status"};
    for(int i=0;i<5;i++){
        if(i==0||i==3){
            if(id[i]=="")continue;
            if(flag==0){flag=1;sql+=" where ";}else{sql+=" AND ";}
            sql+=col[i]+" = '"+id[i]+"' ";
        }
        else{
            if(id[i][0]=="请")continue;
            if(flag==0){flag=1;sql+=" where ";}else{sql+=" AND ";}
            sql+=col[i]+" = '"+id[i]+"' ";
        }
    }
#ifndef NDEBUG
        qDebug()<<sql;
#endif
    QSqlQuery query;
    query.prepare(sql);
    query.exec();
    model->setQuery(query);
}

void ParkingManagerWidget::setcbox(){
    ui->b1->clear();
    ui->b2->clear();
    ui->b4->clear();
    ui->b1->addItem("请选择车牌号");
    ui->b2->addItem("请选择设备种类");
    ui->b4->addItem("请选择状态");
    tm->setQuery("select SpotID,carID,location,price,status from ParkingSpots ");
    QSet<QString> st1,st2,st4;
    for(int i=0;i<tm->rowCount();i++){
        st1.insert(tm->data(tm-> index (i, 1)).toString());
        st2.insert(tm->data(tm-> index (i, 2)).toString());
        st4.insert(tm->data(tm-> index (i, 4)).toString());
    }
    ui->b1->addItems(st1.values());
    ui->b2->addItems(st2.values());
    ui->b4->addItems(st4.values());
}

ParkingManagerWidget::~ParkingManagerWidget()
{
    delete ui;
}

void ParkingManagerWidget::on_e_editingFinished(){setsql();}
void ParkingManagerWidget::on_b1_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void ParkingManagerWidget::on_b2_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void ParkingManagerWidget::on_b3_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void ParkingManagerWidget::on_b4_currentTextChanged(const QString &arg1){Q_UNUSED(arg1);setsql();}
void ParkingManagerWidget::on_cB_clicked(){}
void ParkingManagerWidget::on_addB_clicked(){}

#include "showcustominputdialog.h"
void ParkingManagerWidget::edit(int x,int y,QWidget *parent)
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
        paw = new PaddWidget();
        paw->setAttribute(Qt::WA_DeleteOnClose);
        connect(paw, &ParkingManagerWidget::destroyed, this, [this]() {
            setsql();
            setcbox();
        });
        paw->show();
    }
    else if (msgBox.clickedButton() == editBtn) {
        if(x==0){
            QMessageBox::information(  this, "修改失败!","无法修改车位id!");
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
        QString sql="update ParkingSpots set "+tn+" = ";
        sql+=" '"+tst+"' ";
        sql+=" where SpotID = "+idValue.toString();

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
        QString sql="delete from ParkingSpots where SpotID = ";
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
#include "approveparkingspotdialog.h"
void ParkingManagerWidget::toapprove(){
    ui->b4->setCurrentText("待审核");
    auto result = parkingdialog(this);
    QString accepted = result.first;
    QString tst = result.second;
    q=new QSqlQuery();
    QString tn;
    if(accepted=="approved") tn = "通过";
    else if(accepted=="rejected") tn = "拒绝";
    else {setcbox();return;}

    QString sql="update ParkingSpots set status = ";
    sql+=" '"+tn+"' ";
    sql+=" where SpotID = "+tst;

#ifndef NDEBUG
    qDebug()<<sql;
#endif
    q->prepare(sql);
    if(q->exec())
        QMessageBox::information(  this, "审核成功!","已审核!");
    else
    {
        QMessageBox::information(  this, "审核失败!","error!");
        return ;
    }
    if(accepted=="approved"){
        QString sql="update users set parking = "+tst+" where car = ";
        QSqlQuery q;
        q.prepare("select CarID from ParkingSpots where SpotID = "+tst);
        q.exec();
        QString id=q.value(0).toString();
        sql+=id;
        q.prepare(sql);
        q.exec();
    }


    setsql();
    setcbox();
}






