#include "mainrecorddb.h"
#include "ui_mainrecorddb.h"
#include <QStandardItemModel>
MainRecordDB::MainRecordDB(globeset *globek,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainRecordDB)
{
    ui->setupUi(this);
    globe = globek;
    qDebug()<<"mainRecordDB is begin";
}

MainRecordDB::~MainRecordDB()
{
    delete ui;
}

void MainRecordDB::on_ShowButton_clicked()
{
    ui->treeView->setColumnWidth(0,100);//日期
    ui->treeView->setColumnWidth(1,50);//时间
    ui->treeView->setColumnWidth(2,100);//事项名
    ui->treeView->setColumnWidth(3,80);//持续时长
    ui->treeView->setColumnWidth(4,100);//事件备注
    ui->treeView->setColumnWidth(5,50);//分类代号
    QStandardItemModel    *Model = new QStandardItemModel(0,6);
    Model->setHeaderData(0, Qt::Horizontal, trUtf8("日期"));
    Model->setHeaderData(1, Qt::Horizontal, trUtf8("时间"));
    Model->setHeaderData(2, Qt::Horizontal, trUtf8("事项名"));
    Model->setHeaderData(3, Qt::Horizontal, trUtf8("持续时长"));
    Model->setHeaderData(4, Qt::Horizontal, trUtf8("事件备注"));
    Model->setHeaderData(5, Qt::Horizontal, trUtf8("分类代号"));


    /**************************************************
     * <xml大杀器，高能慎入>
     *************************************************/

    QFile wojiuluanqiminglezhadi(globe->RecordGetAndPost);
    wojiuluanqiminglezhadi.open(QIODevice::ReadOnly);

    QDomDocument    doc;
    doc.setContent(&wojiuluanqiminglezhadi);
    QDomElement     Day,Record,VarDate;
    Day    =        doc.firstChildElement("root");

    Day    =        Day.firstChildElement("Day");//对于为什么会有Day=0的情况心有疑问，但急着上厕所，就不先追究了&&

    Day    =        Day.nextSiblingElement("Day");
    int             RowCount=0;//记录行数
    QSize           RowSize;//利用QSize设定行高，应该有其他方法，第二版改进&&
    RowSize.setHeight(20);
    while(!Day.isNull())
    {
        QList<QStandardItem *>      ItemList;
        Record =       Day.firstChildElement("Record");
        VarDate=       Record.firstChildElement("Date");
        ItemList.append(new QStandardItem(VarDate.text()));
        qDebug()<<VarDate.text();
        while(!Record.isNull())
        {
            QList<QStandardItem *> ItemChildList;
            ItemChildList.append(new QStandardItem(" "));
            VarDate=       Record.firstChildElement("Time");
            ItemChildList.append(new QStandardItem(VarDate.text()));
            VarDate=       Record.firstChildElement("Thing");
            ItemChildList.append(new QStandardItem(VarDate.text()));
            VarDate=       Record.firstChildElement("Minute");
            ItemChildList.append(new QStandardItem(VarDate.text()));
            VarDate=       Record.firstChildElement("ThingRem");

            ItemChildList.append(new QStandardItem(VarDate.text()));

            ItemChildList.at(0)->setSizeHint(RowSize);//设定行高
            ItemChildList.at(1)->setSizeHint(RowSize);
            ItemChildList.at(2)->setSizeHint(RowSize);
            ItemChildList.at(3)->setSizeHint(RowSize);


            ItemList.at(0)->appendRow(ItemChildList);
            Record   =     Record.nextSiblingElement("Record");
        }
            RowCount++;
            Day      =     Day.nextSiblingElement("Day");


            Model->appendRow(ItemList);
    }

    ui->treeView->setModel(Model);
    /**************************************************
     * </xml大杀器，高能慎入>
     *************************************************/
}
