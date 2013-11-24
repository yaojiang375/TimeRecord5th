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
    explicit SortTheRecordNameLikeDir(QVector<QString> &t,QWidget *parent = 0);
    ~SortTheRecordNameLikeDir();
    
private slots:
    //void on_treeWidget_customContextMenuRequested(const QPoint &pos);

    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_treeWidgetItem_delete();
    void on_treeWidgetItem_insert();
    void on_pushButton_clicked();

private:
    Ui::SortTheRecordNameLikeDir *ui;
    QList<QTreeWidgetItem> ReturnItemList_XML(QDomElement &Node, QTreeWidgetItem *parent);
    QTreeWidgetItem *Menueitem;
    void FreeItem(QTreeWidgetItem* item);
    int  MenuCount;
    QDomElement   WidgetToDom(QTreeWidgetItem* item, QDomDocument &root);
};

#endif // SORTTHERECORDNAMELIKEDIR_H
