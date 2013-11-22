#include "sorttherecordnamelikedir.h"
#include "ui_sorttherecordnamelikedir.h"

SortTheRecordNameLikeDir::SortTheRecordNameLikeDir(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SortTheRecordNameLikeDir)
{
    ui->setupUi(this);
    QFile txt("C:/test.xml");
    txt.open(QIODevice::ReadOnly);
    QDomDocument read;
    read.setContent(&txt);
    ui->treeWidget->setColumnWidth(0,100);
    ui->treeWidget->setColumnWidth(1,100);
    /*********************
     *读取XML
     ********************/
    QDomElement Data=read.firstChildElement("root").firstChildElement("Node");
    while(!Data.isNull())
    {
        QStringList comment;
        comment<<Data.firstChildElement("Sort_ID").text()<<Data.firstChildElement("Quick_Num").text();
        QTreeWidgetItem *RootItem=new QTreeWidgetItem(comment);
        if(!Data.firstChildElement("Node").isNull())
        {
            QDomElement buf=Data.firstChildElement("Node");
            ReturnItemList_XML(buf,RootItem);
        }
        ui->treeWidget->addTopLevelItem(RootItem);
        Data=Data.nextSiblingElement("Node");
    }
    /*********************
     *\读取XML
     ********************/


    ui->treeWidget->show();
}

SortTheRecordNameLikeDir::~SortTheRecordNameLikeDir()
{
    delete ui;
}

QList<QTreeWidgetItem> SortTheRecordNameLikeDir::ReturnItemList_XML(QDomElement &Node, QTreeWidgetItem *parent=0)
{
    QDomElement    ChildNode=Node.nextSiblingElement("Node");
    QList<QTreeWidgetItem > List_xml;
        if(!ChildNode.isNull())
        {
            List_xml.append(ReturnItemList_XML(ChildNode,parent));
            qDebug()<<"Sibling = "<<ChildNode.firstChildElement("Quick_Num").text();
        }
        QStringList comment;
        comment<<Node.firstChildElement("Sort_ID").text()<<Node.firstChildElement("Quick_Num").text();
        QTreeWidgetItem *child=new QTreeWidgetItem(parent,comment);
        ChildNode=Node.firstChildElement("Node");
        if(!ChildNode.isNull())
        {
            ReturnItemList_XML(ChildNode,child);
            qDebug()<<"child = "<<ChildNode.firstChildElement("Quick_Num").text();
        }
        List_xml.append(*child);
        return  List_xml;
}
