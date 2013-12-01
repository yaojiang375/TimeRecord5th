#include "form.h"
#include "ui_form.h"
#include <QFileDialog>

Form::Form(globeset *globek, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    globe=globek;
    ui->next->hide();
    this->setWindowTitle(trUtf8("时间管理器5th"));
    //检测更新
    version="0.7181";
    manager = new QNetworkAccessManager(this);
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(finishedSlot(QNetworkReply*)));
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

        globe->SmsPos=QFileDialog::getOpenFileName(this,trUtf8("打开短信记录文件"),trUtf8("."),trUtf8("豌豆荚导出的短信记录 (*.csv);;所有文件 (*.*)"));
        qDebug()<<"SmsPos="<<globe->SmsPos;




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
    ui->tableWidget->setRowCount(needfix.length()*4);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0,300);
    ui->tableWidget->setColumnWidth(1,500);
    while(i<needfix.length())
    {
        k=needfix[i];
        z=4*i-3+4;
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
        ui->tableWidget->setItem(z+2,0,new QTableWidgetItem());
        ui->tableWidget->setItem(z+2,1,new QTableWidgetItem());
        i++;
    }
    if(needfix.length()==0)
    {
        ui->next->show();
        ui->pushButton->hide();
    }
    a.close();
}

void Form::on_SaveRepair_clicked()
{

    qDebug()<<"ReadString.length()"<<needfix.length();
    int i=0;
    int z=0;
    while(i<needfix.length())
    {
        z=4*i+4;
        ReadString[needfix[i]]=   ui->tableWidget->item(z-3,1)->text();
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

void Form::on_pushButton_2_clicked()
{
    UpdateAdress="http://timerecord-timerecord.stor.sinaapp.com/TimeRecordVersion_UpdateAdress.txt";
    QUrl url(UpdateAdress);
    manager->get(QNetworkRequest(url));
}

void Form::finishedSlot(QNetworkReply *Reply)
{
    int i;
    i=1;
    if(Reply->error()==QNetworkReply::NoError)
    {
        QByteArray bytes =Reply->readAll();
        QString    Read  =QString::fromUtf8(bytes);
        qDebug()<<Read;
        if(Read.size()<5)
        {
            return;
        }
        UpdateVersion    =Read.split("_")[0];
        UpdateAdress     =Read.split("_")[1];
        if(version!=UpdateVersion)
        {
            if(QMessageBox::question(NULL,trUtf8("发现新版本"),trUtf8("检测到新版本，是否更新？"),QMessageBox::Yes,QMessageBox::No))
            {
                UpdateVersion="explorer "+UpdateAdress;
                qDebug()<<UpdateVersion;
                system(UpdateVersion.toStdString().c_str());
            }
        }
        else
        {
            QMessageBox::question(NULL,trUtf8("检测完毕"),trUtf8("已经是最新版本了~\n谢谢支持"),QMessageBox::Ok);
        }
    }
}

void Form::on_Check_clicked()
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
    ui->tableWidget->setRowCount(needfix.length()*4);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0,300);
    ui->tableWidget->setColumnWidth(1,500);
    while(i<needfix.length())
    {
        k=needfix[i];
        z=4*i-3+4;
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
        ui->tableWidget->setItem(z+2,0,new QTableWidgetItem());
        ui->tableWidget->setItem(z+2,1,new QTableWidgetItem());
        i++;
    }
    if(needfix.length()==0)
    {
    ui->next->show();
    }
    a.close();
}
