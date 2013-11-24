#include "sorttherecordnamelikedir.h"
#include "ui_sorttherecordnamelikedir.h"
#include <QMenu>
#include <QAction>
#include "mainrecorddb.h"
SortTheRecordNameLikeDir::SortTheRecordNameLikeDir(QVector<QString> &t, QWidget *parent) :QWidget(parent),

    ui(new Ui::SortTheRecordNameLikeDir)
{

    ui->setupUi(this);
    //ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);//设置右键菜单
    MenuCount=0;
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(on_treeWidget_customContextMenuRequested(const QPoint&)));
    QFile txt("C:/test.xml");
    txt.open(QIODevice::ReadOnly);
    QDomDocument read;
    read.setContent(&txt);
    ui->treeWidget->setColumnWidth(0,500);
    ui->treeWidget->setColumnWidth(1,50);
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
        RootItem->setFlags(RootItem->flags()|Qt::ItemIsEditable);
        ui->treeWidget->addTopLevelItem(RootItem);
        Data=Data.nextSiblingElement("Node");
    }
    QStringList comment;
    if(t.size()>0)
    {
        comment<<trUtf8("未分类")<<"";
        QTreeWidgetItem *RootItem=new QTreeWidgetItem(comment);
        QVector<QString>::iterator VecRead=t.begin();
        while(VecRead!=t.end())
        {
            comment.clear();
            comment<<VecRead->simplified()<<"";
            QTreeWidgetItem *BufItem=new QTreeWidgetItem(comment);
            BufItem->setFlags(BufItem->flags()|Qt::ItemIsEditable);
            RootItem->addChild(BufItem);
            VecRead++;
        }
        ui->treeWidget->addTopLevelItem(RootItem);
    }


    /*********************
     *\读取XML
     ********************/
    txt.close();
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
        child->setFlags(child->flags()|Qt::ItemIsEditable);
        ChildNode=Node.firstChildElement("Node");
        if(!ChildNode.isNull())
        {
            ReturnItemList_XML(ChildNode,child);
            qDebug()<<"child = "<<ChildNode.firstChildElement("Quick_Num").text();
        }
        List_xml.append(*child);
        return  List_xml;
}

void SortTheRecordNameLikeDir::FreeItem(QTreeWidgetItem *item)
{
    if(item->childCount()==0)
    {
        delete item;
    }
    else
    {
        for(int i=0;i<item->childCount();i++)
        {
            FreeItem(item->child(i));
        }
        delete item;
    }
    return;
}

QDomElement SortTheRecordNameLikeDir::WidgetToDom(QTreeWidgetItem *item,QDomDocument &root)
{
    QDomElement RootDom;
    RootDom=root.createElement("Node");
    for(int i=0;i<item->childCount();i++)
    {
        RootDom.appendChild(WidgetToDom(item->child(i),root));
    }
    QDomElement Sort_ID,Quick_Num;
    Quick_Num=root.createElement("Quick_Num");
    Sort_ID  =root.createElement("Sort_ID");
    QDomText    text;
    if(item->childCount()==0)
    {
        text=root.createTextNode("");
    }
    else
    {
        text=root.createTextNode(item->text(1));
    }
    Quick_Num.appendChild(text);
    text=root.createTextNode(item->text(0));
    Sort_ID.appendChild(text);
    RootDom.appendChild(Sort_ID);
    RootDom.appendChild(Quick_Num);
    return RootDom;
}

void SortTheRecordNameLikeDir::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
     Menueitem = const_cast<QTreeWidgetItem*> (ui->treeWidget->itemAt(pos));
     //不知道为什么点一下右键会发出两个信号
     MenuCount++;
     qDebug()<<"Show Menu"<<MenuCount;
     if(!(MenuCount%2))
     {return;}
            if (Menueitem) {
                // Note: We must map the point to global from the viewport to
                // account for the header.
                QMenu menu;
                QAction *xml_insert=menu.addAction(trUtf8("插入"));
                QAction *xml_delete=menu.addAction(trUtf8("删除"));
                connect(xml_delete, SIGNAL(triggered()), this, SLOT(on_treeWidgetItem_delete()));
                connect(xml_insert, SIGNAL(triggered()), this, SLOT(on_treeWidgetItem_insert()));
                menu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
            }
}

void SortTheRecordNameLikeDir::on_treeWidgetItem_delete()
{

    if(Menueitem->childCount()==0)
    {
        if(Menueitem->parent()==NULL)
        {
            for(int i=0;i<ui->treeWidget->topLevelItemCount();i++)
            {
                if(ui->treeWidget->topLevelItem(i)==Menueitem)
                {
                    ui->treeWidget->takeTopLevelItem(i);
                    break;
                }
            }

        }
        else
        {
            delete Menueitem;
        }
    }
    else
    {
        if(Menueitem->parent()==NULL)
        {
            for(int i=0;i<ui->treeWidget->topLevelItemCount();i++)
            {
                if(ui->treeWidget->topLevelItem(i)==Menueitem)
                {
                    ui->treeWidget->takeTopLevelItem(i);
                    break;
                }
            }
        }
        else
        {
            FreeItem(Menueitem);
        }
    }
    if(ui->treeWidget->topLevelItemCount()==0)
    {
        QStringList      Buf;
        Buf<<trUtf8("啊哦，全被删掉了")<<"Except Me~";
        qDebug()<<Menueitem->text(1);
        QTreeWidgetItem* iteminsert =new QTreeWidgetItem(Buf);
        iteminsert->setFlags(iteminsert->flags()|Qt::ItemIsEditable);
        ui->treeWidget->addTopLevelItem(iteminsert);
    }
    qDebug()<<"hello world delete";
}

void SortTheRecordNameLikeDir::on_treeWidgetItem_insert()
{

    QStringList      Buf;
    Buf<<trUtf8("我是新建项")<<" ";
    qDebug()<<Menueitem->text(1);
    QTreeWidgetItem* iteminsert =new QTreeWidgetItem(Menueitem,Buf);
    iteminsert->setFlags(iteminsert->flags()|Qt::ItemIsEditable);
    Menueitem->insertChild(Menueitem->childCount(),iteminsert);

}

void SortTheRecordNameLikeDir::on_pushButton_clicked()
{
    QFile txt("C:/test.xml");
    txt.open(QIODevice::WriteOnly);
    txt.resize(0);
    QTextStream text(&txt);
    QDomDocument Xml_File;
    QDomProcessingInstruction       File_InsTruction;
    QDomElement                     File_root;
    File_InsTruction=Xml_File.createProcessingInstruction("xml","version=\"1.0\" encoding = \"UTF-8\"");
    Xml_File.appendChild(File_InsTruction);
    File_root=Xml_File.createElement("root");
    File_root.setAttribute("DefineByself","1");
    for(int i =0;i<ui->treeWidget->topLevelItemCount();i++)
    {
        qDebug()<<ui->treeWidget->topLevelItem(i)->text(0);
        QString read=ui->treeWidget->topLevelItem(i)->text(0);
        if((ui->treeWidget->topLevelItem(i)->text(0).compare(trUtf8("未分类"))))//汉语字符要注意utf转换
        {
            File_root.appendChild(WidgetToDom(ui->treeWidget->topLevelItem(i),Xml_File));
        }
    }
    Xml_File.appendChild(File_root);
    Xml_File.save(text,4);
    txt.close();
    this->close();
}

