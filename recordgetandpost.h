#ifndef RECORDGETANDPOST_H
#define RECORDGETANDPOST_H
#include "globe.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QtXml>
#include <QSet>
#include <QVector>
#include <QDateTime>

#include <QtAlgorithms>


#define RECORDLENGTH  100000

/*******************
 *总记录类，负责储存全部的历史纪录
 *待扩展
 *
 *******************/

/**************************************/

class RecordItem
{
public:
    RecordItem();
    int        Serial_Number;//Record序号
    QString    Date;
    int        intDate;
    QString    Time;
    int        intTime;
    int        Minute;
    QString    Thing;
    QString    ThingRem;
    QString ReturnDiv(void)
    {
        return  Date+Time;//汉字不靠谱。。。同样的数据能读出来不一样的结果。。。改用时间加日期作为主码
    }
};
class DayItem
{
public:
    int        DayNumber;//标准日期
    QVector<RecordItem*> Children;
};
class XMLRoot
{
public:
    QVector<DayItem*>   DaySerial;
    DayItem* findDayItem(int begin,int end,int intDate);
};

/****************************************************/
class RecordGetAndPost
{
public:
    RecordGetAndPost();//记录总类，负责读取，返回，储存记录
    void RecordReadFromFile(globeset globe);//读取RecordGetAndPost.Xml
    void RecordAdd(globeset globe);//读取Rec.Xml（简单处理后的短信记录），与总库记录相比较，若为新加记录，则添加入总库中，同时将新添记录记录至./ini/GetAndPost.xml内
    void RecordSave(globeset globe);//保存总库记录
private:
    QSet<QString>   CheckDuplicate;
    XMLRoot         XmlRoot;


};
bool RecordLessThan(RecordItem *first, RecordItem *next);//能否正常调用？
bool DayLessThan(DayItem *first, DayItem *next);

#endif // RECORDGETANDPOST_H
