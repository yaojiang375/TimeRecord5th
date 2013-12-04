#include "recordgetandpost.h"
#include <QObject>

#define   RECORDSERIALNUMBER  (RecordSerialNumber:LastRecord?NextRecord)  //转换
#define   _RECORDSERIALNUMBER (RecordSerialNumber:NextRecord?LastRecord) //逆转换
RecordGetAndPost::RecordGetAndPost(globeset *readglobe)
{
        globe   =   readglobe;
}

void RecordGetAndPost::readXmlRecord()
{
    QFile   XmlReader(globe->RecordGetAndPostPos);//读取原纪录
    XmlReader.open(QIODevice::ReadOnly);
    QDomDocument    XML_Doc;
    XML_Doc.setContent(&XmlReader);
    QDomElement     Day;
    DayItem*        XmlDay;
    QDomElement     Record;
    QDomElement     Node;//获取Record的各项属性
    RecordItem*     XmlRecord;
    Day=XML_Doc.firstChildElement("root");
    Day=Day.firstChildElement("Day");
    /*******************************/
    //开始读取
    while(!Day.isNull())
    {
        XmlDay      =   new DayItem;
        XmlDay->DayNumber=Day.attribute("DayNumber").toInt();
        Record      =   Day.firstChildElement("Record");
        while(!Record.isNull())
        {
            XmlRecord                   =   new    RecordItem;
            Node                        =   Record.firstChildElement("Date");//Record为什么要有日期？冗余啊这是
            XmlRecord->Date             =   Node.text();
            XmlRecord->intDate          =   Node.attribute("intDate").toInt();//这里Date写错了，待会儿改过来
            Node                        =   Record.firstChildElement("Time");
            XmlRecord->Time             =   Node.text();
            XmlRecord->intTime          =   Node.attribute("intTime").toInt();
            Node                        =   Record.firstChildElement("Minute");
            XmlRecord->Minute           =   Node.text().toInt();
            Node                        =   Record.firstChildElement("Thing");
            XmlRecord->Thing            =   Node.text();
            Node                        =   Record.firstChildElement("ThingRem");
            XmlRecord->ThingRem         =   Node.text();
            Record                      =   Record.nextSiblingElement("Record");
            XmlRecord->Div              =   XmlRecord->intDate*10000+XmlRecord->intTime;
            XmlDay->Children.append(XmlRecord);
            qDebug()<<"CheckDuplicate = "<<XmlRecord->Div;
            CheckDuplicate.insert(XmlRecord->Div);//测重标记
        }
        //qSort(XmlDay->Children.begin(),XmlDay->Children.end(),RecordLessThan);//假定存储前已按顺序排好
        Day=Day.nextSiblingElement("Day");
        XmlRoot.DaySerial.append(XmlDay);
    }
    qSort(XmlRoot.DaySerial.begin(),XmlRoot.DaySerial.end(),DayLessThan);
    XmlReader.close();
    /*******************************/
    return;
}

void RecordGetAndPost::RecordAdd()//待添加的记录
{
    /*****************************************
     *函数逻辑
     *RecordAdd函数逻辑
     *  • 首先，读取所有『日期+时间+事项名』记录至Set，利用Set检测重复。
     *  • 其次，读取一条新纪录
     *      • 检测Set中是否已存在『日期+时间』
     *          • 存在
     *               • 跳过
     *           • 不存在//新记录
     *             • 抽取日期，检测记录中是否存在对应日期
     *                  • 存在日期K【接续先前记录】
     *                      • 逆序遍历日期K中的所有Record值（假定已按降序排
     *                        列），以第一个intTime小于ThisRecord。intTime的值
     *                        作为LastRecord，ThisRecord插入到LastRecord之后//此时日期内有序
     *                      • 若无，ThisRecord插入到首位（LastRecord在【K=1天】的记录内，查找，找到更新LastRecord与自身，找不到只更新自身）//需补充完整
     *                          • 更新LastRecord的minute值//默认为-1
     *                          • 检测记录中的LastThing是否为空
     *                              • 为空
     *                                  •LastRecord.Thing不需要更新
     *                              • 不为空
     *                                  • 更新LastRecord.Thing及
     *                                    LastRecord.ThingRem
     *                  • 不存在【该记录为在新日期上的记录】
     *                      • 新建日期记录，更新其intData值（假定为K）
     *                      • 将ThisRecord加入到新纪录中
     *                          • 查找记录【K-1】是否存在
     *                              • 存在
     *                                  • 对Day[K-1]的所有Record记录进行排序
     *                                  • 取Last（）作为LastRecord
     *                                  • 更新LastRecord//将前条记录的NextThing及其备注作为LastRecord的备注存留
     *                              • 不存在
     *                                  • 全新记录
     *                                      • 不必检测LastThing
     ****************************************/
    QFile   XmlReader(globe->RecPos);//读取初步转换后的短信记录
    XmlReader.open(QIODevice::ReadOnly);
    QDomDocument    XML_Doc;
    XML_Doc.setContent(&XmlReader);
    QDomElement     Record;
    QDomElement     Node;
    bool            RecordSerialNumber=0;//标记先后顺序，便于计算间隔时间//存疑
    Record          =   XML_Doc.firstChildElement("root");
    Record          =   Record.firstChildElement("Record");
    RecordItem      *LRecord=NULL;
    RecordItem      *NRecord=NULL;
    RecordItem      *ThisRecord=NULL;
    RecordItem      *LastRecord=NULL;
    DayItem         *NewDay =NULL;
    DayItem         *LastDay=NULL;//新添加的日期记录
    while(!Record.isNull())
    {
        RecordSerialNumber=!RecordSerialNumber;
        if(RecordSerialNumber)
        {
            LRecord  =new RecordItem;
            ThisRecord  =LRecord;
            LastRecord=NRecord;
        }
        else
        {
            NRecord=new RecordItem;
            ThisRecord  =NRecord;
            LastRecord=LRecord;
        }

        Node                         =      Record.firstChildElement("Date");
        ThisRecord->Date             =      Node.text();
        Node                         =      Record.firstChildElement("Time");
        ThisRecord->Time             =      Node.text();
        Node                         =      Record.firstChildElement("Body");
        ThisRecord->intDate          =      globe->STLDate.daysTo(QDate::fromString(ThisRecord->Date,"yyyy-MM-dd"));
        ThisRecord->intTime          =      globe->STLTime.secsTo(QTime::fromString(ThisRecord->Time,"hh:mm"))/60;
        ThisRecord->Div              =      ThisRecord->intDate*10000   +   ThisRecord->intTime;
        qDebug()<<"ThisRecord div = "<<ThisRecord->ReturnDiv();
        if(CheckDuplicate.contains(ThisRecord->Div))
        {
            Record                       =      Record.nextSiblingElement("Record");
            continue;
        }
        ThisRecord->Thing            =      Node.firstChildElement("NextThing").text().trimmed();
        ThisRecord->ThingRem         =      Node.firstChildElement("NextThingRemember").text();
        LastDay                      =      NewDay;
        NewDay                       =      XmlRoot.findDayItem(0,XmlRoot.DaySerial.count()-1,ThisRecord->intDate);
        if(LastDay!=NewDay&&LastDay!=NULL)//更新日期时对前一日期进行排序//&&不必要，应设法取消
        {
            qSort(LastDay->Children.begin(),LastDay->Children.end(),RecordLessThan);
        }
        if(NewDay!=NULL)
        {
            Node                     =      Node.firstChildElement("LastThing");
            int     i=NewDay->Children.count();
            for(;i>0&&NewDay->Children[i-1]->intTime>ThisRecord->intTime;i--)//原初：第一个大于iniTime的值的前一个位置//修改：第一个小于iniTime的位置//影响速度，可以直接用last进行处理//i>=0必须要放到最前面，要不直接数组越界
            {
                qDebug()<<"DayNumber ="<<i;//检测效率
            }
            if(i>0)
            {
                i--;
                LastRecord=NewDay->Children[i];
                LastRecord->Minute    =ThisRecord->intTime-LastRecord->intTime;
                if(Node.text().size()!=0)
                {
                    if(LastRecord->ThingRem.length())
                    {
                        LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text()+QObject::trUtf8("被覆盖的记录 ： ")+LastRecord->Thing+QObject::trUtf8("记录备注 ： ")+LastRecord->ThingRem;
                    }
                    else
                    {
                        if(LastRecord->Thing.length())
                        {
                            LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text()+QObject::trUtf8("被覆盖的记录 ： ")+LastRecord->Thing;
                        }
                        else
                        {
                            LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text();
                        }

                    }
                    LastRecord->Thing      =Node.text();
                }
                NewDay->Children.insert(i+1,ThisRecord);
            }
            else
            {
                NewDay->Children.insert(0,ThisRecord);//出现这种情况一般都是故障了
                ThisRecord->Minute=NewDay->Children[1]->intTime-ThisRecord->intTime;
                LastDay                  =       XmlRoot.findDayItem(0,XmlRoot.DaySerial.count()-1,NewDay->DayNumber-1);
                if(LastDay==NULL)
                {
                    Record                       =      Record.nextSiblingElement("Record");
                    continue;
                }
                else
                {
                    LastRecord=LastDay->Children.last();
                    LastRecord->Minute           =      ThisRecord->intTime+24*60-LastRecord->intTime;
                    Node                     =       Node.firstChildElement("LastThing");
                    if(Node.text().size()!=0)
                    {
                        if(LastRecord->ThingRem.length())
                        {
                            LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text()+QObject::trUtf8("被覆盖的记录 ： ")+LastRecord->Thing+QObject::trUtf8("记录备注 ： ")+LastRecord->ThingRem;
                        }
                        else
                        {
                            if(LastRecord->Thing.length())
                            {
                                LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text()+QObject::trUtf8("被覆盖的记录 ： ")+LastRecord->Thing;
                            }
                            else
                            {
                                LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text();
                            }

                        }
                        LastRecord->Thing      =Node.text();
                    }
                }

            }

        }
        else
        {
            NewDay                   =       new DayItem;
            NewDay->DayNumber        =       ThisRecord->intDate;
            NewDay->Children.append(ThisRecord);
            int i   =   XmlRoot.DaySerial.size();
            for(;i>0&&XmlRoot.DaySerial[i-1]->DayNumber>NewDay->DayNumber;i--)//第一个小于DayNumber的位置
            {}
            XmlRoot.DaySerial.insert(i,NewDay);
            //qSort(XmlRoot.DaySerial.begin(),XmlRoot.DaySerial.end(),DayLessThan);//添加完新日期之后进行排序，可以进一步改进，改成插入模式,至少能再快两个数量级
            LastDay                  =       XmlRoot.findDayItem(0,XmlRoot.DaySerial.count()-1,NewDay->DayNumber-1);
            if(LastDay!=NULL)
            {
                //qSort(LastDay->Children.begin(),LastDay->Children.end(),RecordLessThan);//对前一日记录进行排序//是否必要？//必要，因为前一日期是否有序为未知事件
                LastRecord=LastDay->Children.last();
                LastRecord->Minute   =       ThisRecord->intTime+24*60-LastRecord->intTime;
                Node                     =       Node.firstChildElement("LastThing");
                if(Node.text().size()!=0)
                {
                    if(LastRecord->ThingRem.length())
                    {
                        LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text()+QObject::trUtf8("被覆盖的记录 ： ")+LastRecord->Thing+QObject::trUtf8("记录备注 ： ")+LastRecord->ThingRem;
                    }
                    else
                    {
                        if(LastRecord->Thing.length())
                        {
                            LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text()+QObject::trUtf8("被覆盖的记录 ： ")+LastRecord->Thing;
                        }
                        else
                        {
                            LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text();
                        }

                    }
                    LastRecord->Thing      =Node.text();
                }
            }
            else
            {
                Record                       =      Record.nextSiblingElement("Record");
                continue;
            }
        }
        NewAddRecord.append(LastRecord);
        Record                       =      Record.nextSiblingElement("Record");
    }
    qSort(XmlRoot.DaySerial.begin(),XmlRoot.DaySerial.end(),DayLessThan);//临末，安全起见，排个序
    XmlReader.close();
}

void RecordGetAndPost::writeXmlRecord()
{
    QFile           Xml_Maintxt(globe->RecordGetAndPostPos);//转换
    Xml_Maintxt.open(QIODevice::ReadWrite);
    Xml_Maintxt.resize(0);
    QTextStream     Xml_Text(&Xml_Maintxt);
    QDomDocument    XML_Doc;
    QDomProcessingInstruction   Instruction;
    QDomElement     root;
    QDomElement     Day;
    DayItem         *Day_Item;
    int             DayCount;
    QDomElement     Record;
    RecordItem      *Record_Item;
    int             RecordCount;
    QDomElement     Node;
    QDomText        Text;
    Instruction     =       XML_Doc.createProcessingInstruction("xml","version=\"1.0\" encoding = \"UTF-8\"");
    XML_Doc.appendChild(Instruction);
    root            =       XML_Doc.createElement("root");
    for(DayCount=0;DayCount<XmlRoot.DaySerial.count();DayCount++)
    {
        Day_Item    =       XmlRoot.DaySerial[DayCount];
        Day         =       XML_Doc.createElement("Day");
        Day.setAttribute("DayNumber",Day_Item->Children.first()->intDate);
        for(RecordCount=0;RecordCount<Day_Item->Children.count();RecordCount++)
        {
            Record_Item =   Day_Item->Children[RecordCount];
            Record      =   XML_Doc.createElement("Record");
            Node        =   XML_Doc.createElement("Date");
            Node.setAttribute("intDate",Record_Item->intDate);
            Text        =   XML_Doc.createTextNode(Record_Item->Date);
            Node.appendChild(Text);
            Record.appendChild(Node);

            Node        =   XML_Doc.createElement("Time");
            Node.setAttribute("intTime",Record_Item->intTime);
            Text        =   XML_Doc.createTextNode(Record_Item->Time);
            Node.appendChild(Text);
            Record.appendChild(Node);

            Node        =   XML_Doc.createElement("Minute");
            QString         Minute_Buf;
            Minute_Buf.setNum(Record_Item->Minute);
            Text        =   XML_Doc.createTextNode(Minute_Buf);//&&
            Node.appendChild(Text);
            Record.appendChild(Node);

            Node        =   XML_Doc.createElement("Thing");
            Text        =   XML_Doc.createTextNode(Record_Item->Thing);
            Node.appendChild(Text);
            Record.appendChild(Node);

            Node        =   XML_Doc.createElement("ThingRem");
            Text        =   XML_Doc.createTextNode(Record_Item->ThingRem);
            Node.appendChild(Text);
            Record.appendChild(Node);
            Day.appendChild(Record);
        }
        root.appendChild(Day);
    }
    XML_Doc.appendChild(root);
    XML_Doc.save(Xml_Text,4);
    Xml_Maintxt.close();
    /****************主记录*******************************/
    /****************新加记录*****************************/
    QFile           Xml_NewAddtxt(globe->NewAddRecordPos);//转换
    Xml_NewAddtxt.open(QIODevice::ReadWrite);
    Xml_NewAddtxt.resize(0);
    QTextStream     Xml_NewAddText(&Xml_NewAddtxt);
    QDomDocument    XML_NewAddDoc;
    Instruction     =   XML_NewAddDoc.createProcessingInstruction("xml","version=\"1.0\" encoding = \"UTF-8\"");
    XML_NewAddDoc.appendChild(Instruction);
    root            =   XML_NewAddDoc.createElement("root");
    RecordItem      *Record_buf;
    for(int i=0;i<NewAddRecord.size();i++)
    {
        Record_buf  =   NewAddRecord[i];
        Record      =   XML_NewAddDoc.createElement("Record");
        Node        =   XML_NewAddDoc.createElement("Date");
        Node.setAttribute("intDate",Record_buf->intDate);
        Text        =   XML_NewAddDoc.createTextNode(Record_buf->Date);
        Node.appendChild(Text);
        Record.appendChild(Node);

        Node        =   XML_NewAddDoc.createElement("Time");
        Node.setAttribute("intTime",Record_buf->intTime);
        Text        =   XML_NewAddDoc.createTextNode(Record_buf->Time);
        Node.appendChild(Text);
        Record.appendChild(Node);

        Node        =   XML_NewAddDoc.createElement("Minute");
        QString         Minute_Buf;
        Minute_Buf.setNum(Record_buf->Minute);
        Text        =   XML_NewAddDoc.createTextNode(Minute_Buf);//&&
        Node.appendChild(Text);
        Record.appendChild(Node);

        Node        =   XML_NewAddDoc.createElement("Thing");
        Text        =   XML_NewAddDoc.createTextNode(Record_buf->Thing);
        Node.appendChild(Text);
        Record.appendChild(Node);

        Node        =   XML_NewAddDoc.createElement("ThingRem");
        Text        =   XML_NewAddDoc.createTextNode(Record_buf->ThingRem);
        Node.appendChild(Text);
        Record.appendChild(Node);
        root.appendChild(Record);
    }
    XML_NewAddDoc.appendChild(root);
    XML_NewAddDoc.save(Xml_NewAddText,4);
    return;
}

bool RecordLessThan(RecordItem* first, RecordItem* next)
{
    return first->intTime < next->intTime;
}

bool DayLessThan(DayItem * first, DayItem * next)
{
    return first->DayNumber < next->DayNumber;
}





DayItem *XMLRoot::findDayItem(int begin, int end, int intDate)
{
    //假定是从小到大排列
    if(end<begin)
    {
        //初次运行
        return NULL;
    }

    if(DaySerial[((begin+end)/2)]->DayNumber==intDate)
    {
        return DaySerial[((begin+end)/2)];
    }
    if((end-begin)<=3)
    {
        while(begin<=end)
        {
            if(DaySerial[begin]->DayNumber==intDate)
            {
                return DaySerial[begin];
            }
            begin++;
        }
        return NULL;
    }
    if(DaySerial[((begin+end)/2)]->DayNumber>intDate)
    {
        return findDayItem(begin,((begin+end)/2)-1,intDate);//
    }
    else
    {
        return findDayItem(((begin+end)/2)+1,end,intDate);
    }
}


RecordItem::RecordItem()//初始化
{
    this->Date="";
    this->intDate=0;
    this->intTime=0;
    this->Minute=-1;
    this->Thing="";
    this->ThingRem="";
    this->Time="";
}
