#include "mainrecorddb.h"
#include "ui_mainrecorddb.h"

#include "sorttherecordnamelikedir.h"
#include <QStandardItemModel>
MainRecordDB::MainRecordDB(globeset *globek,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainRecordDB)
{
    ui->setupUi(this);
    globe = globek;
    ui->treeView->setColumnWidth(0,100);//日期
    ui->treeView->setColumnWidth(1,50);//时间
    ui->treeView->setColumnWidth(2,100);//事项名
    ui->treeView->setColumnWidth(3,80);//持续时长
    ui->treeView->setColumnWidth(4,100);//事件备注
    ui->treeView->setColumnWidth(5,50);//分类代号
    FlushMap();
}

MainRecordDB::~MainRecordDB()
{
    delete ui;
}

void MainRecordDB::on_ShowButton_clicked()
{
    FlushMap();
    QStandardItemModel    *Model = new QStandardItemModel(0,6);
    Model->setHeaderData(0, Qt::Horizontal, trUtf8("日期"));
    Model->setHeaderData(1, Qt::Horizontal, trUtf8("时间"));
    Model->setHeaderData(2, Qt::Horizontal, trUtf8("事项名"));
    Model->setHeaderData(3, Qt::Horizontal, trUtf8("持续时长"));
    Model->setHeaderData(4, Qt::Horizontal, trUtf8("事件备注"));
    Model->setHeaderData(5, Qt::Horizontal, trUtf8("分类"));
    /**************************************************
     * <测试>
     *************************************************/


    ui->treeView->setDragEnabled(true);
    ui->treeView->setDropIndicatorShown(true);
    ui->treeView->viewport()->setAcceptDrops(true);
    ui->treeView->setDragDropMode(QAbstractItemView::InternalMove);


    /**************************************************
     * </测试>
     *************************************************/



    /**************************************************
     * <xml大杀器，高能慎入>
     *************************************************/

    QFile wojiuluanqiminglezhadi(globe->RecordGetAndPost);
    wojiuluanqiminglezhadi.open(QIODevice::ReadOnly);
    QDomDocument    doc;
    doc.setContent(&wojiuluanqiminglezhadi);
    QDomElement     Day,Record,VarDate;
    Day    =        doc.firstChildElement("root");
    Day    =        Day.firstChildElement("Day");//对于为什么会有Day=0的情况心有疑问，但急着上厕所，就不先追究了&&
    Day    =        Day.nextSiblingElement("Day");
    int             RowCount=0;//记录行数
    QSize           RowSize;//利用QSize设定行高，应该有其他方法，第二版改进&&
    RowSize.setHeight(20);
    while(!Day.isNull())
    {
        QList<QStandardItem *>      ItemList;
        Record =       Day.firstChildElement("Record");
        VarDate=       Record.firstChildElement("Date");
        ItemList.append(new QStandardItem(VarDate.text()));
        qDebug()<<VarDate.text();
        while(!Record.isNull())
        {
            QList<QStandardItem *> ItemChildList;
            ItemChildList.append(new QStandardItem(" "));
            VarDate=       Record.firstChildElement("Time");
            ItemChildList.append(new QStandardItem(VarDate.text()));
            VarDate=       Record.firstChildElement("Thing");
            ItemChildList.append(new QStandardItem(VarDate.text().simplified()));
            VarDate=       Record.firstChildElement("Minute");
            ItemChildList.append(new QStandardItem(VarDate.text()));
            VarDate=       Record.firstChildElement("ThingRem");
            ItemChildList.append(new QStandardItem(VarDate.text()));
            QString        Sort_ID=Map_ItemToSortString.value(Record.firstChildElement("Thing").text().simplified());
            if(Sort_ID.size()==0)
            {
                Sort_ID=trUtf8("待分类");
                Vector_NeedSort.append((Record.firstChildElement("Thing").text().simplified()));
            }
            if(Sort_ID.split("\\").at(0).compare(trUtf8("自定义"))==0)
            {
                Sort_ID.clear();
            }
            ItemChildList.append(new QStandardItem(Sort_ID));
            ItemChildList.at(0)->setSizeHint(RowSize);//设定行高
            ItemChildList.at(1)->setSizeHint(RowSize);
            ItemChildList.at(2)->setSizeHint(RowSize);
            ItemChildList.at(3)->setSizeHint(RowSize);
            ItemChildList.at(4)->setSizeHint(RowSize);

            ItemList.at(0)->appendRow(ItemChildList);
            Record   =     Record.nextSiblingElement("Record");
        }
            RowCount++;
            Day      =     Day.nextSiblingElement("Day");
            Model->appendRow(ItemList);
    }

    ui->treeView->setModel(Model);
    /**************************************************
     * </xml大杀器，高能慎入>
     *************************************************/
}

void MainRecordDB::on_pushButton_clicked()
{
    QFile testFileExist ;

    if(!testFileExist.exists("C:/test.xml"))
    {
        QFile txt("C:/test.xml");
        txt.open(QIODevice::ReadWrite);
        QTextStream text(&txt);
        QDomDocument read;
        read.setContent(&txt);
        QDomElement                 root;
        QDomProcessingInstruction   _InsTruction;
        _InsTruction=read.createProcessingInstruction("xml","version=\"1.0\" encoding = \"UTF-8\"");
        read.appendChild(_InsTruction);
        root=read.createElement("root");
        root.setAttribute("DefineByself","1");
        QDomElement DefineBySelf;
        DefineBySelf=read.createElement("Node");
        QDomElement Quick_Num,Sort_ID;
        QDomText    Domtext;
        Sort_ID=read.createElement(trUtf8("Sort_ID"));
        Quick_Num=read.createElement("Quick_Num");
        Domtext=read.createTextNode(trUtf8("自定义"));
        Sort_ID.appendChild(Domtext);
        Domtext=read.createTextNode("65536");
        Quick_Num.appendChild(Domtext);
        DefineBySelf.appendChild(Sort_ID);
        DefineBySelf.appendChild(Quick_Num);
        root.appendChild(DefineBySelf);
        read.appendChild(root);
        read.save(text,4);
        txt.close();
    }
    qSort(Vector_NeedSort.begin(),Vector_NeedSort.end());//对Vector去重
    QVector<QString> Buf_Vec(Vector_NeedSort);
    Vector_NeedSort.clear();
    for(int i=0;i<Buf_Vec.size();i++)
    {
        if(i<(Buf_Vec.size()-1))
        {
            if(Buf_Vec[i]==Buf_Vec[i+1])
            {
                continue;
            }
        }
        Vector_NeedSort.append(Buf_Vec[i]);
    }
    SortTheRecordNameLikeDir   *kz= new SortTheRecordNameLikeDir(Vector_NeedSort,0);
    kz->show();

    //this->hide();
}

void MainRecordDB::Sort_Map(QDomElement root,QMap<QString,QString> &Map_ItemToSortString,QMap<int,QString>  &Map_intToSortString,QString Dir)//事项名——分类名\分类号——分类名
{
    QDomElement Node;
    Node = root.firstChildElement("Node");
    QString text;
    if(Node.isNull())
    {
        //叶节点

        text=root.firstChildElement("Sort_ID").text();
        Map_ItemToSortString.insert(text,Dir);
        return;
    }
    else
    {
        //根节点
        text=root.firstChildElement("Sort_ID").text();
        if(Dir.size()==0)
        {
            Dir+=text;
        }
        else
        {
            Dir+="\\"+text;
        }
        text=root.firstChildElement("Quick_Num").text();
        if(text.size()>0)
        {
            Map_intToSortString.insert(text.toInt(),Dir);
        }
        while (!Node.isNull())
        {
            Sort_Map(Node,Map_ItemToSortString,Map_intToSortString,Dir);
            Node=Node.nextSiblingElement("Node");
        }
    }
}

void MainRecordDB::FlushMap()
{
    Map_intToSortString.clear();
    Map_ItemToSortString.clear();
    QFile txt("C:/test.xml");
    txt.open(QIODevice::ReadOnly);
    QDomDocument read;
    read.setContent(&txt);
    QDomElement Node=read.firstChildElement("root").firstChildElement("Node");
    while(!Node.isNull())
    {
        Sort_Map(Node,Map_ItemToSortString,Map_intToSortString);
        Node=Node.nextSiblingElement("Node");
    }
    txt.close();
    return;
}


