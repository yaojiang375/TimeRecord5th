#ifndef GLOBE_H
#define GLOBE_H
#include <QTextStream>
#include <QFile>
#include <QDateTime>

#include <QDebug>

class globeset
{
public:

    /*****Flag********/
    QString RILFlag;
    QString RecFlag;
    QString AddFlag;
    QString AddEndFlag;
    QString ReghtRem;
    QString LeftRem;
    QString MidThing;
    QDate   STLDate;//标准时间，1970-01-01_00：00
    QTime   STLTime;//标准时间00：00

    /*****Pos*********/
    QString SmsPos;
    QString iniPos;
    QString RecordGetAndPostPos;
    QString RecPos;//初次转换后短信文件位置
    QString NewAddRecordPos;//新加入记录
    QString	SortIDTreePos;  //分类记录
    globeset()
    {
        SmsPos      = "./ini/1.csv";
        RILFlag     ="！";
        RecFlag     ="。";
        AddFlag     ="，";
        AddEndFlag  ="！";
        ReghtRem    ="]";
        LeftRem     ="[";
        MidThing    ="？";
        iniPos      ="./ini/1.ini";//Qdebug
        STLDate.setDate(2010,01,01);
        STLTime.setHMS(0,0,0);
        RecordGetAndPostPos ="./ini/RecordGetAndPost.xml";
        NewAddRecordPos     ="./ini/NewAddRecordPos.xml";
        SortIDTreePos       ="./ini/SortIDTree.xml";
        RecPos              ="./ini/RecPos.xml";
    }
    void read()
    {
        QFile ini(iniPos);
        qDebug()<<ini.isOpen();//Qdebug;
        QChar key;
        ini.open(QIODevice::ReadOnly);
        QTextStream iniRead(&ini);

        iniRead>>key;
        RecFlag=key;
        iniRead>>MidThing;
        iniRead>>RILFlag;
        iniRead>>AddFlag;
        iniRead>>AddEndFlag;
        
        return;
    }
    void Show()
    {
        qDebug()<<"SmsPos="     <<SmsPos    <<'\n'
                <<"RILFlag="    <<RILFlag   <<'\n'
                <<"RecFlag="    <<RecFlag   <<'\n'
                <<"AddFlag="    <<AddFlag   <<'\n'
                <<"AddEndFlag=" <<AddEndFlag<<'\n'
                <<"ReghtRem="   <<ReghtRem  <<'\n'
                <<"LeftRem="    <<LeftRem   <<'\n'
                <<"MidThing="   <<MidThing  <<'\n'
                <<"iniPos="     <<iniPos    <<'\n';
              /*  <<"=" << <<'\n'
                <<"=" << <<'\n'
                <<"=" << <<'\n'  */
    }
    void operator = (globeset a)
    {
        this->AddEndFlag        =   a.AddEndFlag;
        this->AddFlag           =   a.AddFlag;
        this->LeftRem           =   a.LeftRem;
        this->ReghtRem          =   a.ReghtRem;
        this->MidThing          =   a.MidThing;
        this->RecFlag           =   a.RecFlag;
        this->RILFlag           =   a.RILFlag;

        this->STLDate           =   a.STLDate;
        this->STLTime           =   a.STLTime;

        this->SortIDTreePos     =   a.SortIDTreePos;
        this->iniPos            =   a.iniPos;
        this->NewAddRecordPos   =   a.NewAddRecordPos;
        this->RecordGetAndPostPos=  a.RecordGetAndPostPos;
        this->SmsPos            =   a.SmsPos;
        this->RecPos            =   a.RecPos ;
        return;
    }
};



#endif // GLOBE_H
