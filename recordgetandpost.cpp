#include "recordgetandpost.h"

#define   RECORDSERIALNUMBER  (RecordSerialNumber:LastRecord?NextRecord)  //转换
#define   _RECORDSERIALNUMBER (RecordSerialNumber:NextRecord?LastRecord) //逆转换
RecordGetAndPost::RecordGetAndPost()
{

}

void RecordGetAndPost::RecordReadFromFile(globeset globe)
{
    QFile   XmlReader(globe.RecordGetAndPost);//读取原纪录
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
            XmlDay->Children.append(XmlRecord);
            qDebug()<<"CheckDuplicate = "<<XmlRecord->ReturnDiv();
            CheckDuplicate.insert(XmlRecord->ReturnDiv());//测重标记
        }
        qSort(XmlDay->Children.begin(),XmlDay->Children.end(),RecordLessThan);
        Day=Day.nextSiblingElement("Day");
        XmlRoot.DaySerial.append(XmlDay);
    }
    qSort(XmlRoot.DaySerial.begin(),XmlRoot.DaySerial.end(),DayLessThan);
    XmlReader.close();
    /*******************************/
    return;
}

void RecordGetAndPost::RecordAdd(globeset globe)
{
    /*****************************************
     *函数逻辑
     *RecordAdd函数逻辑
     *  • 首先，读取所有『日期+时间+事项名』记录至Set，利用Set检测重复。
     *  • 其次，读取一条新纪录
     *      • 检测Set中是否已存在『日期+时间+事项名』
     *          • 存在
     *               • 跳过
     *           • 不存在
     *              • 抽取日期，检测记录中是否存在对应日期
     *                  • 存在日期K【接续先前记录】
     *                      • 逆序遍历日期K中的所有Record值（假定已按降序排
     *                        列），以第一个intTime小于ThisRecord。intTime的值
     *                        作为LastRecord，ThisRecord插入到LastRecord之后
     *                  • 若无，ThisRecord插入到首位
     *                      • 更新LastRecord的minute值//默认为-1
     *                      • 检测记录中的LastThing是否为空
     *                          • 为空
     *                              •LastRecord.Thing不需要更新
     *                          • 不为空
     *                              • 更新LastRecord.Thing及
     *                                LastRecord.ThingRem
     *            • 不存在【该记录为在新日期上的记录】
     *              • 新建日期记录，更新其intData值（假定为K）
     *                  • 将ThisRecord加入到新纪录中
     *                      • 查找记录【K-1】是否存在
     *                          • 存在
     *                              • 对Day[K-1]的所有Record记录进行排序
     *                              • 取Last（）作为LastRecord
     *                              • 更新LastRecord
     *                           • 不存在
     *                               • 全新记录
     *                                   • 不必检测LastThing
     ****************************************/
    QFile   XmlReader("./ini/RecXml.xml");//读取初步转换后的短信记录
    XmlReader.open(QIODevice::ReadOnly);
    QDomDocument    XML_Doc;
    XML_Doc.setContent(&XmlReader);
    QDomElement     Record;
    QDomElement     Node;
    bool     RecordSerialNumber=0;//标记先后顺序，便于计算间隔时间//存疑
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
        ThisRecord->Thing            =      Node.firstChildElement("NextThing").text().trimmed();
        //qDebug()<<"ThisRecord div = "<<ThisRecord->ReturnDiv();
        if(CheckDuplicate.contains(ThisRecord->ReturnDiv()))
        {
            Record                       =      Record.nextSiblingElement("Record");
            continue;
        }
        ThisRecord->ThingRem         =      Node.firstChildElement("NextThingRemember").text();
        ThisRecord->intDate          =      globe.STLDate.daysTo(QDate::fromString(ThisRecord->Date,"yyyy-MM-dd"));
        ThisRecord->intTime          =      globe.STLTime.secsTo(QTime::fromString(ThisRecord->Time,"hh:mm"))/60;
        LastDay                      =      NewDay;
        NewDay                       =      XmlRoot.findDayItem(0,XmlRoot.DaySerial.count()-1,ThisRecord->intDate);
        if(LastDay!=NewDay&&LastDay!=NULL)//更新日期时对前一日期进行排序
        {
            qSort(LastDay->Children.begin(),LastDay->Children.end(),RecordLessThan);
        }
        if(NewDay!=NULL)
        {
            Node                     =      Node.firstChildElement("LastThing");
                int     i=NewDay->Children.count()-1;
                for(;NewDay->Children[i]->intTime>ThisRecord->intTime&&i>=0;i--)//原初：第一个大于iniTime的值的前一个位置//修改：第一个小于iniTime的位置//影响速度，可以直接用last进行处理
                {
                    qDebug()<<NewDay->Children.count();//Only test;
                }
                if(i>=0)
                {
                LastRecord=NewDay->Children[i];
                LastRecord->Minute    =ThisRecord->intTime-LastRecord->intTime;
                if(Node.text().size()!=0)
                {
                    LastRecord->Thing      =Node.text();
                    LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text();
                }
                NewDay->Children.insert(i+1,ThisRecord);
                }
                else
                {
                    NewDay->Children.insert(0,ThisRecord);//出现这种情况一般都是故障了
                }

        }
        else
        {
            NewDay                   =       new DayItem;
            NewDay->DayNumber        =       ThisRecord->intDate;
            NewDay->Children.append(ThisRecord);
            XmlRoot.DaySerial.append(NewDay);
            qSort(XmlRoot.DaySerial.begin(),XmlRoot.DaySerial.end(),DayLessThan);//添加完新日期之后进行排序，可以进一步改进，改成插入模式,至少能再快两个数量级
            LastDay                  =       XmlRoot.findDayItem(0,XmlRoot.DaySerial.count()-1,NewDay->DayNumber-1);
            if(LastDay!=NULL)
            {
                //qSort(LastDay->Children.begin(),LastDay->Children.end(),RecordLessThan);//对前一日记录进行排序//是否必要？//必要，因为前一日期是否有序为未知事件
                LastRecord=LastDay->Children.last();
                LastRecord->Minute   =       ThisRecord->intTime+24*60-LastRecord->intTime;
                Node                     =       Node.firstChildElement("LastThing");
                if(Node.text().size()!=0)
                {
                    LastRecord->Thing      =Node.text();
                    LastRecord->ThingRem   =Node.nextSiblingElement("LastThingRemember").text();
                }
            }
            else
            {
                Record                       =      Record.nextSiblingElement("Record");
                continue;
            }
        }
        Record                       =      Record.nextSiblingElement("Record");
    }
    XmlReader.close();

}

void RecordGetAndPost::RecordSave(globeset globe)
{
    QFile           Xml_txt(globe.RecordGetAndPost);//转换
    Xml_txt.open(QIODevice::ReadWrite);
    Xml_txt.resize(0);
    QTextStream     Xml_Text(&Xml_txt);
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
    Xml_txt.close();
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
    this->Serial_Number=0;
    this->Thing="";
    this->ThingRem="";
    this->Time="";
}
