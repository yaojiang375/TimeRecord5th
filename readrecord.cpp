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
    test    =   true;
    Time    =   clock();
    QIcon       icon("./ini/www.ico");
    QWidget::setWindowIcon(icon);
    ui->setupUi(this);
    this->setWindowTitle(trUtf8("时间管理器5th"));
    //ui->AddToRecordDB->hide();
    globe=globek;
    QTableWidget    *table=ui->tableWidget;
    table->setColumnCount(7);
    table->clear();
    SmsReader   outside_smsreader(globe);
    outside_smsreader.Read();//处理短信记录
    QStringList         Header;
    Header.append(trUtf8("错误标记"));
    Header.append(trUtf8("日期"));
    Header.append(trUtf8("时间"));
    Header.append(trUtf8("已完成事项"));
    Header.append(trUtf8("已完成事项备注"));
    Header.append(trUtf8("待办事项"));
    Header.append(trUtf8("待办事项备注"));
    table->setHorizontalHeaderLabels(Header);
    table->setColumnWidth(0,0);
    table->setColumnWidth(1,90);
    table->setColumnWidth(2,50);

    qDebug()<<"Time = "<<clock()-Time;//此步花费9639ms
    Time    =   clock();
    QFile   _TaskFile(globe->RecPos);
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
     qDebug()<<"Time = "<<clock()-Time;
     Time    =   clock();//此步花费5499ms
     MainRecord =    new RecordGetAndPost(globe);
     qDebug()<<"Time = "<<clock()-Time;
     Time    =   clock();
     MainRecord->readXmlRecord();//一次就够
     qDebug()<<"Time = "<<clock()-Time;
     Time    =   clock();
     RecordShow();

}

ReadRecord::~ReadRecord()
{
    delete ui;
}


void ReadRecord::RecordShow()
{
    SmsReader   outside_smsreader(globe);
    outside_smsreader.Read();//处理短信记录
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
    table->setColumnWidth(0,0);
    table->setColumnWidth(1,90);
    table->setColumnWidth(2,50);


    QFile   _TaskFile(globe->RecPos);
    _TaskFile.open(QIODevice::ReadOnly);
     QDomDocument    doc;
     doc.setContent(&_TaskFile);
     QDomElement                     root;
     QDomNode                        Record;
     root       =doc.firstChildElement("root");
     Record     =root.firstChildElement("Record");
     Time    =   clock();
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
     qDebug()<<"Time = "<<clock()-Time;
     Time    =   clock();//此步花费2603ms
     //ui->AddToRecordDB->show();
}



void ReadRecord::on_AddToRecordDB_clicked()
{
    if(test)
    {
    qDebug()<<"Time = "<<clock()-Time;
    Time    =   clock();//此步花费5433ms//无文件//当有打开的文件时花费44818ms//考虑换用std：：string与std：：Set
    MainRecord->RecordAdd();
    qDebug()<<"Time = "<<clock()-Time;
    Time    =   clock();//在有打开文件时此步话费10828ms
    MainRecord->writeXmlRecord();
    qDebug()<<"Time = "<<clock()-Time;
    Time    =   clock();//此步花费7875ms//在有打开文件时此步花费21730ms
    //ui->ShowRecordDB->show();
    }
    test=false;
}
void ReadRecord::on_ShowRecordDB_clicked()
{

    MainRecordDB *a =new MainRecordDB(globe);
    a->show();
    this->hide();
}



void ReadRecord::on_pushButton_2_clicked()
{
    system("explorer http://hi.baidu.com/yaojiang375");
}

void ReadRecord::on_pushButton_clicked()
{
    system("explorer https://github.com/yaojiang375/TimeRecord5th");
}
