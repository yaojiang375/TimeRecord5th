#include "recordgetandpost.h"


RecordGetAndPost::RecordGetAndPost()
{
    _ReciveRecord   *atp1;
    memset(SortByDate,NULL,sizeof(atp1)*RECORDLENGTH);

    ErrorList.clear();
}

void RecordGetAndPost::RecordReadFromFile(globeset globe)
{
    QFile   fanzhengjiaoshenmemingziyezhiyongzheyici(globe.RecordGetAndPost);
    fanzhengjiaoshenmemingziyezhiyongzheyici.open(QIODevice::ReadOnly);
    QDomDocument doc;
    doc.setContent(&fanzhengjiaoshenmemingziyezhiyongzheyici);
    QDomElement   docElem;
    QDomNode      Node;
    QDomElement   Flag;
    QDomElement   CFlag;
    QDomElement   NFlag;
    QDomElement   SFlag;

    QDomElement   Record;
    QDomElement   Thing;
    QDomElement   ThingRem;
    QDomElement   Date;
    QDomElement   Time;
    QDomElement   Minute;

    QDomElement   Day;
    docElem       =doc.documentElement();
    Node          =docElem.firstChild();

    _ReciveRecord   *atp;
    memset(SortByDate,NULL,sizeof(atp)*RECORDLENGTH);//清空
    int i = 0;
    while(!Node.isNull())
    {
        Day    = Node.firstChildElement("Day");
        i      = Day.attribute("DayNumber").toInt();
        SortByDate[i]   = new EndRecordType;//用于标记每一日期的排序状况，存在于日期记录结尾处。话说1000行以上的软件真需要一份文档啊。。。
        Flag   = Day.firstChildElement("Flag");
        CFlag  = Flag.firstChildElement("CompleteFlag");
        NFlag  = Flag.firstChildElement("NeedToFigure");
        SFlag  = Flag.firstChildElement("SortedFlag");
        SortByDate[i]->CompleteFlag = CFlag.attribute("bool").toInt();
        SortByDate[i]->NeedToFigure = NFlag.attribute("bool").toInt();
        SortByDate[i]->SortedFlag   = SFlag.attribute("bool").toInt();
        Record = Day.firstChildElement("Record");
        while(!Record.isNull())
        {
            Date    = Record.firstChildElement("Date");
            Time    = Record.firstChildElement("Time");
            Thing   = Record.firstChildElement("Thing");
            ThingRem= Record.firstChildElement("ThingRem");

            ReadBuf.Date    =Date.text();
            ReadBuf.Time    =Time.text();
            ReadBuf.intDate =Date.attribute("intDate").toInt();
            ReadBuf.intTime =Date.attribute("intTime").toInt();
            ReadBuf.Minute  =Minute.text().toInt();
            ReadBuf.Thing   =Thing.text();
            ReadBuf.ThingRem=ThingRem.text();

            SortByDate[i]->Record.append(ReadBuf);

            Record  = Record.nextSiblingElement("Record");
        }


        Node    =   Node.nextSibling();
    }

    return;

}

void RecordGetAndPost::RecordAdd(globeset globe)
{
    QFile _xml("c:/RecXml.xml");//后期重点，需改成全局变量
    _xml.open(QIODevice::ReadOnly);
    QDomDocument doc;
    QDomElement  xml_Body;


    /********************************************************************/
        //这一段代码负责将已完成记录写入c：/GetAndPost.xml文件中，以供其他程序使用
        QFile   GetAndPost(globe.GetAndPostPos);
        GetAndPost.open(QIODevice::ReadWrite);
        QTextStream    jzyzy(&GetAndPost);

        QDomDocument                    Other_Save;
        QDomProcessingInstruction       Other_InsTruction;

        QDomElement                     Other_root;
        QDomElement                     Other_Record;
        QDomElement                     Other_Date;
        QDomElement                     Other_Time;
        QDomElement                     Other_Minute;
        QDomElement                     Other_Thing;
        QDomElement                     Other_ThingRem;
        QDomText                        Other_Text;

        Other_InsTruction = Other_Save.createProcessingInstruction("xml","version=\"1.0\" encoding = \"UTF-8\"");
        Other_Save.appendChild(Other_InsTruction);
        Other_root        = Other_Save.createElement("Other_root");

    /********************************************************************/






    QDomElement  xml_Record;
    doc.setContent(&_xml);
    xml_Record  =doc.firstChildElement("root");
    xml_Record  =xml_Record.firstChildElement("Record");

    ReadBuf.clear();
    ErrorList.clear();
    while(!xml_Record.isNull())
    {
        xmlFileRecordReader.Date = xml_Record.firstChildElement("Date").text();
        xmlFileRecordReader.Time = xml_Record.firstChildElement("Time").text();
        xml_Body                 = xml_Record.firstChildElement("Body");
        xmlFileRecordReader.LastThing = xml_Body.firstChildElement("LastThing").text();
        xmlFileRecordReader.LastRem = xml_Body.firstChildElement("LastthingRemember").text();
        xmlFileRecordReader.NextThing = xml_Body.firstChildElement("NextThing").text();
        xmlFileRecordReader.NextRem = xml_Body.firstChildElement("NextThingRemember").text();

        BufDate=QDate::fromString(xmlFileRecordReader.Date,"yyyy-MM-dd");
        BufTime=QTime::fromString(xmlFileRecordReader.Time,"hh:mm");

        xmlFileRecordReader.intDate =globe.STLDate.daysTo(BufDate);
        xmlFileRecordReader.intTime =globe.STLTime.msecsTo(BufTime)/60000;//只记录分钟数就够了

        xmlFileRecordReader.Debugprintf();
/*        qDebug()<<"globe.Date="<<globe.STLDate.toString("yyyy-MM-dd");
        qDebug()<<"BufDate="<<BufDate.toString("yyyy-MM-dd");
        qDebug()<<"Date"<<xmlFileRecordReader.Date;//debug
        qDebug()<<"intDate"<<xmlFileRecordReader.intDate ;*/
        if(SortByDate[xmlFileRecordReader.intDate]==NULL)//没有记录
        {
            if(SortByDate[xmlFileRecordReader.intDate-1]!=NULL)//当intDate值为0时怎么办？
            {
                qDebug()<<"intDate"<<xmlFileRecordReader.intDate ;//Debug
                qDebug()<<"SortByDate[xmlFileRecordReader.intDate-1]="<<SortByDate[xmlFileRecordReader.intDate-1];//Debug
                {
                 if(xmlFileRecordReader.LastThing.isEmpty())
                 {}
                 else
                 {
                     (SortByDate[xmlFileRecordReader.intDate-1]->Record.end()-1)->Thing     = xmlFileRecordReader.LastThing;
                 }
                 if(xmlFileRecordReader.LastRem.isEmpty())
                 {}
                 else
                 {
                     SortByDate[xmlFileRecordReader.intDate-1]->Record.last().ThingRem  =     (SortByDate[xmlFileRecordReader.intDate-1]->Record.last()).ThingRem + "&__&" +xmlFileRecordReader.LastRem;
                 }
                }//针对Last的检测,将Last写回

                qDebug()<<"SortByDate[xmlFileRecordReader.intDate-1]->Record.first().Date"<<SortByDate[xmlFileRecordReader.intDate-1]->Record.first().Date;//Debug;


                SortByDate[xmlFileRecordReader.intDate] = new EndRecordType;
                ReadBuf               = xmlFileRecordReader;
                SortByDate[xmlFileRecordReader.intDate]->append(ReadBuf);//此处可以优化，只写入数据一次，不修改FigureFlag
                SortByDate[xmlFileRecordReader.intDate-1]->CompleteFlag = TRUE; //关闭写入入口

               if(SortByDate[xmlFileRecordReader.intDate-1]->SortedFlag)
               {}
               else
               {
                   SortByDate[xmlFileRecordReader.intDate-1]->Sort();
                   //SortByDate[xmlFileRecordReader.intDate-1]->qSort(0,SortByDate[xmlFileRecordReader.intDate-1]->Record.count());//排序
                   SortByDate[xmlFileRecordReader.intDate-1]->SortedFlag = TRUE;//排序
               }
               if(SortByDate[xmlFileRecordReader.intDate-1]->NeedToFigure)
               {
                   //计算分钟数
                   _ReciveRecord::iterator first = SortByDate[xmlFileRecordReader.intDate-1]->Record.begin();
                   while((first+1) !=SortByDate[xmlFileRecordReader.intDate-1]->Record.end())
                   {
                    first->Minute = (first+1)->intTime-first->intTime;
                    first++;
                   }
                   first->Minute = 1440 -first->intTime + SortByDate[xmlFileRecordReader.intDate]->Record.begin()->intTime;
                   /********************************************************************/
                       //这一段代码负责将新添加的记录写入c:/GetAndPost.xml文件中，以供其他程序使用

                   first        =   SortByDate[xmlFileRecordReader.intDate-1]->Record.begin();
int test=0;
                   while(first!=SortByDate[xmlFileRecordReader.intDate-1]->Record.end())
                   {
                        Other_Record =   Other_Save.createElement("Other_Record");
                        Other_Date   =   Other_Save.createElement("Other_Date");
                        Other_Time   =   Other_Save.createElement("Other_Time");
                        Other_Minute =   Other_Save.createElement("Other_Minute");
                        Other_Thing  =   Other_Save.createElement("Other_Thing");
                        Other_ThingRem=  Other_Save.createElement("Other_ThingRem");
test++;
qDebug()<<test;
                        Other_Record.setAttribute("intDate",first->intDate);
                        Other_Date.setAttribute("intDate",first->intDate);
                        Other_Time.setAttribute("intTime",first->intTime);
                        Other_Minute.setAttribute("Minute",first->Minute);

                        Other_Text      =Other_Save.createTextNode(first->Date);
                        Other_Date.appendChild(Other_Text);
                        Other_Text      =Other_Save.createTextNode(first->Time);
                        Other_Time.appendChild(Other_Text);
                        Other_Text      =Other_Save.createTextNode(first->Thing);
                        Other_Thing.appendChild(Other_Text);
                        Other_Text      =Other_Save.createTextNode(first->ThingRem);
                        Other_ThingRem.appendChild(Other_Text);

                        Other_Record.appendChild(Other_Date);
                        Other_Record.appendChild(Other_Time);
                        Other_Record.appendChild(Other_Minute);
                        Other_Record.appendChild(Other_Thing);
                        Other_Record.appendChild(Other_ThingRem);

                        Other_root.appendChild(Other_Record);
                        first++;
                   }
                   /********************************************************************/


               }
               else
               {}


            }
            else//全新
            {
                SortByDate[xmlFileRecordReader.intDate] = new EndRecordType;
                ReadBuf               = xmlFileRecordReader;
                SortByDate[xmlFileRecordReader.intDate]->append(ReadBuf);//此处可以优化，只写入数据一次，不修改FigureFlag
            }

        }
        else
        {
            ReadBuf               = xmlFileRecordReader;
            if(SortByDate[xmlFileRecordReader.intDate]->CheckDuplicate(ReadBuf))
            {
                ErrorList.append(xmlFileRecordReader.Date + xmlFileRecordReader.Time);
                //怎么返回错误？抛异常？
            }
            else
            {
                if(!xmlFileRecordReader.LastThing.isEmpty())
                {
                    SortByDate[xmlFileRecordReader.intDate]->Record.last().Thing = xmlFileRecordReader.LastThing;
                }
                if(!xmlFileRecordReader.LastRem.isEmpty())
                {
                    SortByDate[xmlFileRecordReader.intDate]->Record.last().ThingRem=xmlFileRecordReader.LastRem;
                }//last处理

                SortByDate[xmlFileRecordReader.intDate]->append(ReadBuf);

            }
        }

        xml_Record      =   xml_Record.nextSiblingElement("Record");
    }

    /********************************************************************/
        //这一段代码负责将新添加的记录写入c:/GetAndPost.xml文件中，以供其他程序使用

    Other_Save.appendChild(Other_root);

    Other_Save.save(jzyzy,4);
    /********************************************************************/
    return;

}



void RecordGetAndPost::RecordSave(globeset globe)
{
    QFile _xml(globe.RecordGetAndPost);
    _xml.open(QIODevice::ReadWrite);
    QTextStream _XMLreader(&_xml);

    QDomDocument doc;
    QDomProcessingInstruction instruction;
    QDomElement root            ;

    QDomElement xml_Flag        ;
    QDomElement xml_CompleteFlag        ;
    QDomElement xml_NeedToFigureFlag        ;
    QDomElement xml_SortedFlag        ;

    QDomElement xml_Day         ;

    QDomElement xml_Record      ;
      QDomElement xml_Date        ;
      QDomElement xml_Time        ;
      QDomElement xml_minute      ;
      QDomElement xml_Thing       ;
      QDomElement xml_ThingRem    ;

    QDomText    xml_Text        ;

    int i = 0;
    for(;i<RECORDLENGTH&&SortByDate[i]==NULL;i++)
    {

    }

    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding = \"UTF-8\"");
    doc.appendChild(instruction);
    root        = doc.createElement("root");
    root.setAttribute("RecordBegin",i);
    for(;i<RECORDLENGTH;i++)//以空间换计算，此处可以进一步优化,检测NULL
    {
        if(SortByDate[i]!=NULL)
        {
        xml_Day =doc.createElement("Day");
        xml_Day.setAttribute("DayNumber",i);
        int z =0;//记录每天的事件数，从0开始

        xml_Flag            =doc.createElement("Flag");
        xml_CompleteFlag    =doc.createElement("CompleteFlag");
        xml_CompleteFlag.setAttribute("bool",SortByDate[i]->CompleteFlag);
        xml_NeedToFigureFlag=doc.createElement("NeedToFigure");
        xml_NeedToFigureFlag.setAttribute("bool",SortByDate[i]->NeedToFigure);
        xml_SortedFlag=doc.createElement("SortedFlag");
        xml_SortedFlag.setAttribute("bool",SortByDate[i]->SortedFlag);

        xml_Flag.appendChild(xml_CompleteFlag);
        xml_Flag.appendChild(xml_NeedToFigureFlag);
        xml_Flag.appendChild(xml_SortedFlag);

        while(!SortByDate[i]->Record.isEmpty())
        {
            xml_Record  = doc.createElement("Record");
            xml_Date    = doc.createElement("Date");
            xml_Date.setAttribute("intDate",SortByDate[i]->Record.first().intDate);
            xml_Time    = doc.createElement("Time");
            xml_Time.setAttribute("intTime",SortByDate[i]->Record.first().intTime);
            xml_minute  = doc.createElement("Minute");
            xml_Thing   = doc.createElement("Thing");
            xml_ThingRem= doc.createElement("ThingRem");



            z++;
            xml_Record.setAttribute("RecordNum",z);

            QString fromint;
            xml_Text = doc.createTextNode(SortByDate[i]->Record.first().Date);
            xml_Date.appendChild( xml_Text);
            xml_Text = doc.createTextNode(SortByDate[i]->Record.first().Time);
            xml_Time.appendChild( xml_Text);
            xml_Text = doc.createTextNode(fromint.setNum(SortByDate[i]->Record.first().Minute));
            xml_minute.appendChild( xml_Text);
            xml_Text = doc.createTextNode(SortByDate[i]->Record.first().Thing);
            xml_Thing.appendChild( xml_Text);
            xml_Text = doc.createTextNode(SortByDate[i]->Record.first().ThingRem);
            xml_ThingRem.appendChild( xml_Text);

            xml_Record.appendChild(xml_Date);
            xml_Record.appendChild(xml_Time);
            xml_Record.appendChild(xml_minute);
            xml_Record.appendChild(xml_Thing);
            xml_Record.appendChild(xml_ThingRem);

            xml_Day.appendChild(xml_Record);

            SortByDate[i]->Record.pop_front();
        }
        xml_Day.appendChild(xml_Flag);
        root.appendChild(xml_Day);
        }
    }
    doc.appendChild(root);
    doc.save(_XMLreader,4);
}


bool GaE::CheckDuplicate(_RecordType a)
{
    QVector<_RecordType>::iterator first = Record.begin();
    QVector<_RecordType>::iterator end   = Record.end()  ;
    while(first != end)
    {
        if(first->intTime == a.intTime)
        {
            return TRUE;
        }
        else
        {
            first++;
        }
    }
    return FALSE;
}

bool GaE::append(_RecordType a)
{
     Record.append(a);
     NeedToFigure=TRUE;
     return TRUE;
}

bool GaE::update(_RecordType a)//更新指定时间的内容
{
    _ReciveRecord::iterator t = Record.begin();
    for(;t->intTime!=a.intTime&&t!=Record.end();t++)
    {}
        if(t==Record.end())
        {
            return FALSE;
        }
        else
        {
            a.Minute = t->Minute;
            *t = a;
            return TRUE;
        }
}
