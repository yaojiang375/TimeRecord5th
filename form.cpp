#include "form.h"
#include "ui_form.h"

#include <iostream>
Form::Form(globeset *globek, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    globe=globek;
    ui->next->hide();
}

Form::~Form()
{
    delete ui;
}


void Form::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    ReadString.clear();
    needfix.clear();
    wrongstring.clear();//清空缓存
    ui->tableWidget->show();
    QFile a(globe->SmsPos);
    a.open(QIODevice::ReadWrite);
    QTextStream b(&a);
    readFile=b.readAll();
    Content_Check   check;
    int midRecord=0;//分割记录


    while((midRecord=readFile.indexOf("sms",midRecord))!=-1)
    {
        qDebug()<<"readFile="<<readFile.mid(midRecord,readFile.indexOf("sms",midRecord+1)-midRecord);
        judgebuf=readFile.mid(midRecord,readFile.indexOf("sms",midRecord+1)-midRecord);//临时使用，方便写if语句

        if(judgebuf.indexOf("sms,submit")!=-1&&judgebuf[judgebuf.indexOf("109,")+4]==globe->RecFlag[0])
        {
            ReadString.append(readFile.mid(midRecord,readFile.indexOf("sms",midRecord+1)-midRecord));

            judgebuf    =ReadString.last().mid(ReadString.last().indexOf("109,")+4);

            if(judgebuf[0]==globe->RecFlag[0])
            {
                if(check.check(*globe,judgebuf)!=0)
                {
                    qDebug()<<"judgebuf="<<judgebuf;
                    switch(check.check(*globe,judgebuf))
                    {
                    case 0   :wrongstring.append(trUtf8("检测通过"));break;
                    case 1   :wrongstring.append(trUtf8("多余的左备注符号"));break;
                    case 2   :wrongstring.append(trUtf8("未检测到『？』"));break;
                    case 3   :wrongstring.append(trUtf8("检测到多于一个『？』"));break;
                    case 4   :wrongstring.append(trUtf8("在补充标识符后未检测到补充事件结束标识符『！』"));break;
                    case 5   :wrongstring.append(trUtf8("补充标识符后没有对应的时间。时间标准格式：12.35"));break;
                    case 6   :wrongstring.append(trUtf8("补充事项后没有正文内容"));break;
                    }
                    needfix.append(ReadString.length()-1);//QLIST的值比实际位置大一，需减去
                    qDebug()<<"needfix="<<needfix;
                }
            }
        }
        midRecord++;

    }

    int i=0;
    int k=0;
    int z=0;
    ui->tableWidget->setRowCount(needfix.length()*3);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0,300);
    ui->tableWidget->setColumnWidth(1,500);
    while(i<needfix.length())
    {
        k=needfix[i];
        z=3*i-2+3;
        if((k-1)>0)
        {
        ui->tableWidget->setItem(z-1,0,new QTableWidgetItem(trUtf8("前一项纪录")));
        ui->tableWidget->setItem(z-1,1,new QTableWidgetItem(ReadString[k-1]));
        }
        else
        {
            ui->tableWidget->setItem(z-1,0,new QTableWidgetItem(trUtf8("前一项纪录")));
            ui->tableWidget->setItem(z-1,1,new QTableWidgetItem(trUtf8("无")));
        }
        ui->tableWidget->setItem(z,0,new QTableWidgetItem(wrongstring[i]));
        ui->tableWidget->setItem(z,1,new QTableWidgetItem(ReadString[k]));
        if((k+1)<ReadString.length())
        {
        ui->tableWidget->setItem(z+1,0,new QTableWidgetItem(trUtf8("后一项纪录")));
        ui->tableWidget->setItem(z+1,1,new QTableWidgetItem(ReadString[k+1]));
        }
        else
        {
            ui->tableWidget->setItem(z+1,0,new QTableWidgetItem(trUtf8("后一项纪录")));
            ui->tableWidget->setItem(z+1,1,new QTableWidgetItem(trUtf8("无")));
        }
        i++;
    }
    if(needfix.length()==0)
    {
    ui->next->show();
    }
    a.close();
}

void Form::on_pushButton_2_clicked()
{

    qDebug()<<"ReadString.length()"<<needfix.length();
    int i=0;
    int z=0;
    while(i<needfix.length())
    {
        z=3*i+3;
        ReadString[needfix[i]]=   ui->tableWidget->item(z-2,1)->text();
        i++;
    }
    QFile c(globe->SmsPos);
    c.open(QIODevice::WriteOnly);
    QTextStream     d(&c);
    i=0;

    while(i<ReadString.length())
    {
        d<<ReadString[i];
        i++;
    }
    c.close();
}

void Form::on_next_clicked()
{
    ReadRecord  *a = new ReadRecord(globe);
    a->show();
    this->hide();
}
