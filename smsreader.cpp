#include "smsreader.h"



SmsReader::SmsReader(globeset &globe)
{
    QFile csvRead(globe.SmsPos);
    qDebug()<<"csv:"<<csvRead.isOpen();//qdebug
    csvRead.open(QIODevice::ReadOnly);
    QTextStream textRead(&csvRead);

    Reader      = textRead.readAll();
    LastPos     = Reader.indexOf("sms,submit,");
}

bool SmsReader::Read(globeset &globe)
{
    GTDxml        RecXml;

    while(LastPos != -1)
    {
        LastPos = Reader.indexOf(",,",LastPos);
        LastPos = Reader.indexOf(",,",LastPos+1);
        _Date.setYMD(Reader.mid(LastPos+2,4).toInt(),Reader.mid(LastPos+7,2).toInt(),Reader.mid(LastPos+10,2).toInt());//"sms,submit,,	10086,,2013. 6.25  9:44,109,。？做答案"
        _Time.setHMS(Reader.mid(LastPos+13,2).toInt(),Reader.mid(LastPos+16,2).toInt(),0,0);

        NextPos = Reader.indexOf("sms,",LastPos+19);//一旦有人把sms写进信息体里面怎么办？
        Buffer  = Reader.mid(LastPos+19+4,NextPos-LastPos-23);
        qDebug()<<"Date= "+_Date.toString("yyyy-MM-dd");
        qDebug()<<"Buffer = "+Buffer;//Debug
        /******************************************************/
        if(Buffer[0]==globe.RecFlag[0])//globe待改成指针模式
        {
            Buffer.remove(0,1);
            while(Buffer[0]==globe.AddFlag[0])
            {
                CommaLastRecord=CommaNextRecord=CommaLastRemember=CommaNextRemember="";
                CommaContent="";//清空缓存
                Buffer.remove(0,1);
                if(Buffer.indexOf(globe.AddEndFlag)!=-1)
                {
                    CommaContent = Buffer.mid(0,Buffer.indexOf(globe.AddEndFlag));
                    if(CountMidThing(CommaContent,globe)==1)//保证，与！间只有一条记录
                    {
                        Buffer.remove(0,Buffer.indexOf(globe.AddEndFlag)+1);
                        if(CommaContent.length()<5)
                        {
                           //报错，throw一个错误
                        }
                        _CommaTime.setHMS(CommaContent.mid(0,2).toInt(),CommaContent.mid(3,2).toInt(),0);
                        if(_CommaTime > _Time)
                        {
                            _CommaDate = _Date.addDays(-1);// 返回数字之后的日期，返回之前的日期用负数
                        }
                        else
                        {
                            _CommaDate = _Date;
                        }
                        CommaContent.remove(0,5);
                        ContentDeal(CommaContent,CommaLastRecord,CommaNextRecord,CommaLastRemember,CommaNextRemember,globe);
                        RecXml.DomWrite(_CommaDate.toString("yyyy-MM-dd"),_CommaTime.toString("hh:mm"),CommaLastRecord,CommaLastRemember,CommaNextRecord,CommaNextRemember,FALSE);
                    }
                }
                else
                {
                    globe.ReadWrong=TRUE;
                }

            }
            if(CountMidThing(Buffer,globe)!=1)
            {
                globe.ReadWrong=TRUE;
            }
                ContentDeal(Buffer,LastRecord,NextRecord,LastRemember,NextRemember,globe);
                RecXml.DomWrite(_Date.toString("yyyy-MM-dd"),_Time.toString("hh:mm"),LastRecord,LastRemember,NextRecord,NextRemember,globe.ReadWrong);
                globe.ReadWrong=FALSE;
        }


        /******************************************************/

        LastPos = Reader.indexOf("sms,submit,",NextPos);
        LastRecord=NextRecord=LastRemember=NextRemember="";//归零；


    }
    RecXml.XmlSave("c:/RecXml.xml");//读取后的Sms记录（此时尚未转换，转换后也叫这个名字）
    return TRUE;
}

void SmsReader::ContentDeal(QString &Content, QString &LastRecord, QString &NextRecord, QString &LastRemember, QString &NextRemember,globeset &globe)
{

    QString     Buffer;
    Buffer  =   Content.mid(0,Content.indexOf(globe.MidThing));
    if(Buffer.indexOf(globe.LeftRem)!=-1&&Buffer.indexOf(globe.ReghtRem)!=-1)//若左右标记同时存在，则认为中间内容为备注内容。
    {
        LastRemember = Buffer.mid(Buffer.indexOf(globe.LeftRem)+1,Buffer.indexOf(globe.ReghtRem)-Buffer.indexOf(globe.LeftRem)-1);
        Buffer.remove(Buffer.indexOf(globe.LeftRem),Buffer.indexOf(globe.ReghtRem)-Buffer.indexOf(globe.LeftRem)+1);
    }
    LastRecord = Buffer;
    Buffer  =Content.mid(Content.indexOf(globe.MidThing)+1,-1);
    if(Buffer.indexOf(globe.LeftRem)!=-1&&Buffer.indexOf(globe.ReghtRem)!=-1)//若左右标记同时存在，则认为中间内容为备注内容。
    {
        NextRemember = Buffer.mid(Buffer.indexOf(globe.LeftRem)+1,Buffer.indexOf(globe.ReghtRem)-Buffer.indexOf(globe.LeftRem)-1);
        Buffer.remove(Buffer.indexOf(globe.LeftRem),Buffer.indexOf(globe.ReghtRem)-Buffer.indexOf(globe.LeftRem)+1);
    }
    NextRecord = Buffer;
    return;
}

int SmsReader::CountMidThing(QString Content,globeset &globe)
{

        int Pos=0;
        int Count=0;
        while(Content.indexOf(globe.MidThing,Pos)!=-1)
        {
            qDebug()<<Content;
            qDebug()<<Content.indexOf(globe.MidThing,Pos);
            Pos=Content.indexOf(globe.MidThing,Pos)+1;
            Count++;
        }
        return Count;

}


