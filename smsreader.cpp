#include "smsreader.h"



SmsReader::SmsReader(globeset *globeread)
{
    globe       =       globeread;
    QFile csvRead(globe->SmsPos);
    qDebug()<<"csv:"<<csvRead.isOpen();//qdebug
    csvRead.open(QIODevice::ReadOnly);
    QTextStream textRead(&csvRead);

    Reader      = textRead.readAll();
    LastPos     = Reader.indexOf("sms,submit,");
}

bool SmsReader::Read()
{
    Time=clock();
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
        if(Buffer[0]==globe->RecFlag[0])//globe待改成指针模式
        {
            Buffer.remove(0,1);
            while(Buffer[0]==globe->AddFlag[0])
            {
                CommaContent="";//清空缓存
                Buffer.remove(0,1);
                    CommaContent = Buffer.mid(0,Buffer.indexOf(globe->AddEndFlag));
                    Buffer.remove(0,Buffer.indexOf(globe->AddEndFlag)+1);
                    if(CommaContent.length()<5)
                    {
                        exit(1);//直接强制退出，当然，肯定不会有这种情况出现的
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
                    ContentDeal(CommaContent,_Date.toString("yyyy-MM-dd"),_Time.toString("hh:mm"),globe);
            }
            ContentDeal(Buffer,_Date.toString("yyyy-MM-dd"),_Time.toString("hh:mm"),globe);
        }
        /******************************************************/
        LastPos = Reader.indexOf("sms,submit,",NextPos);
    }
    qDebug()<<"Time = "<<clock()-Time;//10061ms
    Time=clock();
    writeXmlRecord();
    qDebug()<<"Time = "<<clock()-Time;//12878ms
    return TRUE;
}

void SmsReader::writeXmlRecord()
{
    QFile           Xml_txt(globe->RecPos);//转换
    Xml_txt.open(QIODevice::ReadWrite);
    Xml_txt.resize(0);
    QTextStream     Xml_Text(&Xml_txt);
    QDomDocument    XML_Doc;
    QDomProcessingInstruction   Instruction;
    QDomElement     root;

    QDomElement     Record;
    QDomElement     Date;
    QDomElement     Time;
    QDomElement     Body;
    QDomElement     LastThing;
    QDomElement     LastThingRemember;
    QDomElement     NextThing;
    QDomElement     NextThingRemember;
    QDomText        Text;
    Instruction     =       XML_Doc.createProcessingInstruction("xml","version=\"1.0\" encoding = \"UTF-8\"");
    XML_Doc.appendChild(Instruction);
    root            =       XML_Doc.createElement("root");
    int VectorSize=RecordFromCsv.size();
    for(int i =0;i<VectorSize;i++)
    {
        Record              =       XML_Doc.createElement("Record");
        Date                =       XML_Doc.createElement("Date");
        Time                =       XML_Doc.createElement("Time");
        Body                =       XML_Doc.createElement("Body");
        LastThing           =       XML_Doc.createElement("LastThing");
        LastThingRemember   =       XML_Doc.createElement("LastThingRemember");
        NextThingRemember   =       XML_Doc.createElement("NextThingRemember");
        NextThing           =       XML_Doc.createElement("NextThing");

        RecordFromCsv[i]->Debug();
        Text                =       XML_Doc.createTextNode(RecordFromCsv[i]->Date);
        Date.appendChild(Text);
        Record.appendChild(Date);

        Text                =       XML_Doc.createTextNode(RecordFromCsv[i]->Time);
        Time.appendChild(Text);
        Record.appendChild(Time);

        Text                =       XML_Doc.createTextNode(RecordFromCsv[i]->LastThing);
        LastThing.appendChild(Text);
        Body.appendChild(LastThing);

        Text                =       XML_Doc.createTextNode(RecordFromCsv[i]->LastThingRem);
        LastThingRemember.appendChild(Text);
        Body.appendChild(LastThingRemember);

        Text                =       XML_Doc.createTextNode(RecordFromCsv[i]->NextThing);
        NextThing.appendChild(Text);
        Body.appendChild(NextThing);

        Text                =       XML_Doc.createTextNode(RecordFromCsv[i]->NextThingRem);
        NextThingRemember.appendChild(Text);
        Body.appendChild(NextThingRemember);

        Record.appendChild(Body);

        root.appendChild(Record);
    }
    XML_Doc.appendChild(root);
    XML_Doc.save(Xml_Text,4);
    Xml_txt.close();
    return;
}

void SmsReader::ContentDeal(QString &Content,QString Date,QString Time,globeset *globe)
{

    QString     Buffer;
    BaseRecord* NewRecord=new BaseRecord;
    NewRecord->Date=Date;
    NewRecord->Time=Time;
    Buffer  =   Content.mid(0,Content.indexOf(globe->MidThing));
    if(Buffer.indexOf(globe->LeftRem)!=-1&&Buffer.indexOf(globe->ReghtRem)!=-1)//若左右标记同时存在，则认为中间内容为备注内容。
    {
        NewRecord->LastThingRem = Buffer.mid(Buffer.indexOf(globe->LeftRem)+1,Buffer.indexOf(globe->ReghtRem)-Buffer.indexOf(globe->LeftRem)-1);
        Buffer.remove(Buffer.indexOf(globe->LeftRem),Buffer.indexOf(globe->ReghtRem)-Buffer.indexOf(globe->LeftRem)+1);
    }
    NewRecord->LastThing = Buffer;
    Buffer  =Content.mid(Content.indexOf(globe->MidThing)+1,-1);
    if(Buffer.indexOf(globe->LeftRem)!=-1&&Buffer.indexOf(globe->ReghtRem)!=-1)//若左右标记同时存在，则认为中间内容为备注内容。
    {
        NewRecord->NextThingRem= Buffer.mid(Buffer.indexOf(globe->LeftRem)+1,Buffer.indexOf(globe->ReghtRem)-Buffer.indexOf(globe->LeftRem)-1);
        Buffer.remove(Buffer.indexOf(globe->LeftRem),Buffer.indexOf(globe->ReghtRem)-Buffer.indexOf(globe->LeftRem)+1);
    }
    NewRecord->NextThing = Buffer;
    RecordFromCsv.append(NewRecord);
    return;
}
