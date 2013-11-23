#ifndef MAINRECORDDB_H
#define MAINRECORDDB_H
#include "globe.h"
#include "recordgetandpost.h"
#include <QWidget>
#include <QTableView>
#include <QList>

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QtXml>
/*********************
 *负责显示数据库内容
 *是否需要把内容设置为可修改？
 ********************/
namespace Ui {
class MainRecordDB;
}

class MainRecordDB : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainRecordDB(globeset *globek, QWidget *parent = 0);
    ~MainRecordDB();
    
private slots:
    void on_ShowButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainRecordDB *ui;
    globeset *globe;
    void Sort_Map(QDomElement root,QMap<QString,QString> &Map_ItemToSortString,QMap<int,QString>  &Map_intToSortString,QString Dir="");
    QMap<QString,QString> Map_ItemToSortString;
    QMap<int,QString>     Map_intToSortString;
    QSet<QString>         Set_NeedSort;
    void                  FlushMap();
};

#endif // MAINRECORDDB_H
