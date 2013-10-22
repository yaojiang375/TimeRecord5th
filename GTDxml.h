#ifndef XML_H
#define XML_H
#include "globe.h"
#include <QtXml>
#include <QFile>
#include <QTextStream>
#include <QString>

class GTDxml
{

public:
    GTDxml();

    void DomWrite(QString _Date,QString _Time,QString _LastThing,QString _LastRem,QString _NextThing,QString _NextRem,bool  _WrongFlag);
    void XmlSave(QString xmlWrite);
    void ElementRead(QDomElement key,QString _Date,QString _Time,QString _LastThing,QString _LastRem,QString _NextThing,QString _NextRem,bool  _WrongFlag);
    void GTDxmlFileOpen(QString xmlWrite);
private:
        int ID;
        QDomDocument doc        ;
        QDomProcessingInstruction instruction;
        QDomElement root        ;
        QDomElement Record      ;
        QDomElement id          ;
        QDomElement WrongFlag   ;
        QDomElement Date        ;
        QDomElement Time        ;
        QDomElement Body        ;
        QDomElement LastThing   ;
        QDomElement LastRem     ;
        QDomElement NextThing   ;
        QDomElement NextRem     ;

        QDomText    text        ;

        QString     _Toint      ;
};

#endif // XML_H
