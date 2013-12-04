#ifndef SMSREADER_H
#define SMSREADER_H
#include "globe.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QtXml>
#include <ctime>
class BaseRecord
{
public :
    QString    Date;
    QString    Time;
    QString    LastThing;
    QString    LastThingRem;
    QString    NextThing;
    QString    NextThingRem;
    void Debug()
    {
        qDebug()<<Date;
        qDebug()<<Time;
        qDebug()<<LastThing;
        qDebug()<<LastThingRem;
        qDebug()<<NextThing;
        qDebug()<<NextThingRem;
        return;
    }
};

class SmsReader
{
public:
    SmsReader(globeset *globe);
    bool Read();

private:


    QDate       _Date,_CommaDate;
    QTime       _Time,_CommaTime;

    QString     Reader;

    QString     Buffer;
    QString     CommaContent;
    QString     ReadItLatter;//待办记事
    int         DayDate,TimeDate;
    int         LastPos,NextPos;//

    void writeXmlRecord();
    void ContentDeal(QString &Content, QString Date, QString Time, globeset *globe);
    QVector<BaseRecord*>    RecordFromCsv;
    globeset*   globe;
    clock_t     Time;
};

#endif // SMSREADER_H
