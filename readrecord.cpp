#include "readrecord.h"
#include "ui_readrecord.h"

#include <QTextTable>
#include <QTextFrame>
#include <QTableWidget>
#include <QTableWidgetItem>//新希望啊这是http://hi.baidu.com/guzhou_diaoke/item/0460e6f33506230184d278fc
#include <QStandardItem>
/*******读取文件记录***********/
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QtXml/QDomDocument>
#include <QtDebug>
ReadRecord::ReadRecord(globeset *globek,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReadRecord)
{
    ui->setupUi(this);
    ui->ShowRecordDB->hide();
    //ui->AddToRecordDB->hide();
    globe=globek;
    QTableWidget    *table=ui->tableWidget;
    table->setColumnCount(7);
    QStringList         Header;
    Header.append(trUtf8("错误标记"));
    Header.append(trUtf8("日期"));
    Header.append(trUtf8("时间"));
    Header.append(trUtf8("已完成事项"));
    Header.append(trUtf8("已完成事项备注"));
    Header.append(trUtf8("待办事项"));
    Header.append(trUtf8("待办事项备注"));
    table->setHorizontalHeaderLabels(Header);
    table->setColumnWidth(0,60);
    table->setColumnWidth(1,90);
    table->setColumnWidth(2,50);




}

ReadRecord::~ReadRecord()
{
    delete ui;
}


void ReadRecord::on_ShowButton_clicked()
{
    SmsReader   outside_smsreader(*globe);
    outside_smsreader.Read(*globe);//处理短信记录
    QTableWidget    *table=ui->tableWidget;
    table->clear();
    QStringList         Header;

    Header.append(trUtf8("错误标记"));
    Header.append(trUtf8("日期"));
    Header.append(trUtf8("时间"));
    Header.append(trUtf8("已完成事项"));
    Header.append(trUtf8("已完成事项备注"));
    Header.append(trUtf8("待办事项"));
    Header.append(trUtf8("待办事项备注"));
    table->setHorizontalHeaderLabels(Header);
    table->setColumnWidth(0,60);
    table->setColumnWidth(1,90);
    table->setColumnWidth(2,50);


    QFile   _TaskFile("C:/RecXml.xml");
    _TaskFile.open(QIODevice::ReadOnly);
     QDomDocument    doc;
     doc.setContent(&_TaskFile);
     QDomElement                     root;
     QDomNode                        Record;
     root       =doc.firstChildElement("root");
     Record     =root.firstChildElement("Record");

     table->setRowCount(root.childNodes().count());//设定行数





     int i=0;//序列号
     while(!Record.isNull())
     {
         qDebug()<<i;
         table->setItem(i,0,new QTableWidgetItem(Record.firstChildElement("WrongFlag").text()));
         if(!Record.firstChildElement("WrongFlag").text().compare("True"))
         {
             table->item(i,0)->setBackgroundColor(QColor(100,100,100));
         }
         table->setItem(i,1,new QTableWidgetItem(Record.firstChildElement("Date").text()));
         table->setItem(i,2,new QTableWidgetItem(Record.firstChildElement("Time").text()));
         table->setItem(i,3,new QTableWidgetItem(Record.firstChildElement("Body").firstChildElement("LastThing").text()));
         table->setItem(i,4,new QTableWidgetItem(Record.firstChildElement("Body").firstChildElement("LastThingRemember").text()));
         table->setItem(i,5,new QTableWidgetItem(Record.firstChildElement("Body").firstChildElement("NextThing").text()));
         table->setItem(i,6,new QTableWidgetItem(Record.firstChildElement("Body").firstChildElement("NextThingRemember").text()));
         i++;
        Record=Record.nextSibling();
     }
     //ui->AddToRecordDB->show();
}



void ReadRecord::on_AddToRecordDB_clicked()
{
    RecordGetAndPost a;
    a.RecordReadFromFile(*globe);
    a.RecordAdd(*globe);
    a.RecordSave(*globe);
    ui->ShowRecordDB->show();
}

void ReadRecord::on_ShowRecordDB_clicked()
{

    MainRecordDB *a =new MainRecordDB(globe);
    a->show();
    this->hide();
}


