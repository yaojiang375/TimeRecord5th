#ifndef SORTTHERECORDNAMELIKEDIR_H
#define SORTTHERECORDNAMELIKEDIR_H

#include <QWidget>
#include <QtXml>
#include <QDebug>
#include <QFile>
#include <QTreeWidget>
namespace Ui {
class SortTheRecordNameLikeDir;
}

class SortTheRecordNameLikeDir : public QWidget
{
    Q_OBJECT
    
public:
    explicit SortTheRecordNameLikeDir(QWidget *parent = 0);
    ~SortTheRecordNameLikeDir();
    
private:
    Ui::SortTheRecordNameLikeDir *ui;
    QList<QTreeWidgetItem> ReturnItemList_XML(QDomElement &Node, QTreeWidgetItem *parent);

};

#endif // SORTTHERECORDNAMELIKEDIR_H
