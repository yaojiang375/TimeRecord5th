#ifndef SMSREADER_H
#define SMSREADER_H
#include "globe.h"
#include "GTDxml.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>
#include <QVector>



class SmsReader
{
public:
    SmsReader(globeset &globe);
    bool Read(globeset &globe);

private:


    QDate       _Date,_CommaDate;
    QTime       _Time,_CommaTime;

    QString     Reader;

    QString     Buffer;
    QString          LastRecord,     NextRecord,     LastRemember,     NextRemember,
                CommaLastRecord,CommaNextRecord,CommaLastRemember,CommaNextRemember;
    QString     CommaContent;
    QString     ReadItLatter;//待办记事
    int         DayDate,TimeDate;
    int         LastPos,NextPos;//

    void ContentDeal(QString &Content,QString &LastRecord,QString &NextRecord,QString &LastRemember,QString &NextRemember,globeset &globe);
    int  CountMidThing(QString  Content,globeset &globe);//如果数据中有不等于一个的事件分隔符的话，则存在错误


};

#endif // SMSREADER_H
