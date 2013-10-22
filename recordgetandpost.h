#ifndef RECORDGETANDPOST_H
#define RECORDGETANDPOST_H
#include "globe.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QDateTime>

#include <QtAlgorithms>

#include <QtXml>
#define RECORDLENGTH  100000

/*******************
 *总记录类，负责储存全部的历史纪录
 *待扩展
 *
 *******************/


typedef struct
{
    QString Date;
    QString Time;
    QString LastThing;
    QString LastRem;
    QString NextThing;
    QString NextRem;
    int     intDate;
    int     intTime;
    void    Debugprintf(void)
    {
        qDebug()<<"Date="<<Date;
        qDebug()<<"intDate="<<intDate;
        qDebug()<<"Time="<<Time;
        qDebug()<<"intTime="<<intTime;
        qDebug()<<"LastThing="<<LastThing;
        qDebug()<<"LastRem="<<LastRem;
        qDebug()<<"NextThing="<<NextThing;
        qDebug()<<"NextRem="<<NextRem;
    }
}_xmlRecordRead;



typedef struct apc//指向自身的指针
{
   QString Date;
   QString Time;
   int     intDate;//标准时间1970年1月1日
   int     intTime;//标准时间00：00
   int     Minute;
   QString Thing;
   QString ThingRem;

   bool operator < (const struct apc b)const//不清楚为什么会成功
   {
       return this->intTime < b.intTime;
   }
   void operator =(_xmlRecordRead a)
   {
       Date     = a.Date;
       Time     = a.Time;
       intDate  = a.intDate;
       intTime  = a.intTime;
       Thing    = a.NextThing;
       ThingRem = a.NextRem;
       return;
   }
   void operator =(struct apc   a)
    {
       Date     = a.Date;
       Time     = a.Time;
       intDate  = a.intDate;
       intTime  = a.intTime;
       Thing    = a.Thing;
       ThingRem = a.ThingRem;
       Minute   = a.Minute;
    }

   void clear(void)
   {
       Date.clear();
       Time.clear();
       intDate=intTime=Minute=0;
       Thing.clear();
       ThingRem.clear();
       return;

   }
   friend bool operator ==(struct apc a,struct apc b)//为方便比较所设的友元函数
{
    if(b.Date     == a.Date&&
    b.Time     == a.Time&&
    b.intDate  == a.intDate&&
    b.intTime  == a.intTime&&
    b.Thing    == a.Thing&&
    b.ThingRem == a.ThingRem&&
    b.Minute   == a.Minute)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

}_RecordType;



typedef QVector<_RecordType> _ReciveRecord;


typedef struct GaE
{
public:
    bool            CompleteFlag;//是否记录完成，记录完成为TRUE，默认为FALSE
    bool            NeedToFigure;//记录是否需要计算，默认为TRUE；
    bool            SortedFlag;//是否已排序，默认为未排序FALSE
    _ReciveRecord   Record;

    GaE()
    {
        CompleteFlag =FALSE;
        NeedToFigure =TRUE;
        SortedFlag   =FALSE;
    }
    bool            CheckDuplicate(_RecordType a);//存在重复返回TRUE，不存在返回FLASE
    bool            append(_RecordType a);//添加成功返回TURE，添加失败返回FALSE


    int            Sort1(int begin,int end)
    {
        //手工实现快排
        _ReciveRecord::iterator    a = Record.begin()+begin;
        _RecordType                buf=*a;
        int i=begin+1;//初始位置
        int k=0;//缓存变量
        int z=end;//结束位置
        while(i!=z)
        {
        while(buf.intTime >= (Record.begin()+i)->intTime)
        {
            *(a+k) = *(Record.begin()+i);
            k=i-begin;
            i++;
        }

        while(buf.intTime < (Record.begin()+z)->intTime)
        {
            z--;
        }
        *(a+k) = *(Record.begin()+z);
        k=z-begin;
        *(a+k) = *(Record.begin()+i);
        k=i-begin;
        i++;
        }
        *(a+k) = buf;
        i--;
        return i;
    }
    int      qSort1(int begin,int end)
    {
        if(begin == end)
        {
            return 0;
        }
        else
        {
            qSort1(Sort1(begin,end),end);
            qSort1(begin,Sort1(begin,end)-1);
            return 1;
        }
    }

    int             Sort(void)
    {
        qSort(Record);
        return 0;
    }

    bool            update(_RecordType a);

}  EndRecordType;


class RecordGetAndPost
{
public:
    RecordGetAndPost();//记录总类，负责读取，返回，储存记录
    void RecordReadFromFile(globeset globe);
    void RecordAdd(globeset globe);
    void RecordSave(globeset globe);
    QList<QString>           ErrorList;
private:
    _ReciveRecord   DayRecord;
    _RecordType     ReadBuf;//读取文件记录时的缓存，目前只在add函数中调用过，使用前需先调用clear方法
    EndRecordType   *SortByDate[RECORDLENGTH];
    _xmlRecordRead  xmlFileRecordReader;//读取登记后的记录内容
    QVector<QString>  WroongRecord;//当出现重复记录时将年-月-日&&&时：分放入vector中，并抛弃之
    int             newRecord;//最新纪录所在日期（距离标准日期的天数）
    QDate           BufDate;
    QTime           BufTime;

};

#endif // RECORDGETANDPOST_H
