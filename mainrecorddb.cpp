#include "mainrecorddb.h"
#include "ui_mainrecorddb.h"

#include "sorttherecordnamelikedir.h"
#include <QStandardItemModel>
#include <QFileDialog>//导出为xml
MainRecordDB::MainRecordDB(globeset *globek,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainRecordDB)
{
    ui->setupUi(this);
    globe = globek;
    ui->treeWidget->setColumnWidth(0,100);//日期
    ui->treeWidget->setColumnWidth(1,50);//时间
    ui->treeWidget->setColumnWidth(2,100);//事项名
    ui->treeWidget->setColumnWidth(3,80);//持续时长
    ui->treeWidget->setColumnWidth(4,100);//事件备注
    ui->treeWidget->setColumnWidth(5,50);//分类代号
    QStringList     BufHeaderList;
    BufHeaderList<<trUtf8("日期")<<trUtf8("时间")<<trUtf8("事项名")<<trUtf8("持续时长")<<trUtf8("事件备注")<<trUtf8("分类");
    ui->treeWidget->setHeaderItem(new QTreeWidgetItem(BufHeaderList));
    RecordShow();
}

MainRecordDB::~MainRecordDB()
{
    delete ui;
}

void MainRecordDB::on_ShowButton_clicked()
{
    RecordShow();
}

void MainRecordDB::on_pushButton_clicked()
{
    QFile testFileExist ;

    if(!testFileExist.exists("./ini/test.xml"))
    {
        QFile txt("./ini/test.xml");
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
    QFile txt("./ini/test.xml");
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





void MainRecordDB::on_ConvertToExcel_clicked()
{
    /***********************
     *Excel日期转换函数
     *=IF(ISTEXT(D1),TIMEVALUE(D1),"")
     *
     *************************/
    QString FileSave=QFileDialog::getSaveFileName(this,trUtf8("导出为Excel文件"),".",trUtf8("Excel文件 (*.xls);;所有文件(*.*)"));
    QFile   Txt(FileSave);
    Txt.open(QIODevice::ReadWrite);
    Txt.resize(0);
    QTextStream Text(&Txt);
    QDomDocument    Save;
    QDomProcessingInstruction   Instruction;
    QDomElement                 Row;//将来可升级为OPEN XML格式
    QDomElement                 WorkBook;
    QDomElement                 Styles;//重要，无此部分无法打开// 更正，这应该是样式表模块，可以删掉
        QDomElement                 Style;
        QDomElement                 Alignment;
        QDomElement                 Font;
        QDomElement                 NumberFormat;
    QDomElement                 WorkSheet;
    QDomElement                 Table;
    QDomElement                 Column;
    QDomElement                 Cell;
    QDomElement                 Date;
    QDomText                    Dom_text;
    Instruction     =Save.createProcessingInstruction("xml","version=\"1.0\" encoding = \"UTF-8\"");
    Save.appendChild(Instruction);
    Instruction     =Save.createProcessingInstruction("mso-application","progid=\"Excel.Sheet\"");
    Save.appendChild(Instruction);
    QTreeWidgetItem         *Item;
    WorkBook        =Save.createElement("Workbook");
        WorkBook.setAttribute("xmlns","urn:schemas-microsoft-com:office:spreadsheet");
        WorkBook.setAttribute("xmlns:o","urn:schemas-microsoft-com:office:office");
        WorkBook.setAttribute("xmlns:x","urn:schemas-microsoft-com:office:excel");
    int                     Hour=0;//用于生产标准时间
    int                     Minute=0;//生成标准时间
    QString                 Hour_Minute;
    QString                 Set_Num;

    WorkSheet       =Save.createElement("Worksheet");
    WorkSheet.setAttribute("ss:Name",trUtf8("时间统计表"));
    Table           =Save.createElement("Table");
{
    Column          =Save.createElement("Column");
    Column.setAttribute("ss:AutoFitWidth","0");
    Table.appendChild(Column);
    Column          =Save.createElement("Column");
    Column.setAttribute( "ss:AutoFitWidth","0");
    Table.appendChild(Column);
    Column          =Save.createElement("Column");
    Column.setAttribute("ss:AutoFitWidth","0");
    Table.appendChild(Column);
    Column          =Save.createElement("Column");
    Column.setAttribute("ss:AutoFitWidth","0");
    Table.appendChild(Column);
    Column          =Save.createElement("Column");
    Column.setAttribute("ss:AutoFitWidth","0");
    Table.appendChild(Column);
    Row             =Save.createElement("Row");
{
        Cell=Save.createElement("Cell");
        Date=Save.createElement("Data");
        Date.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("日期"));
        Date.appendChild(Dom_text);
        Cell.appendChild(Date);
        Row.appendChild(Cell);

        Cell=Save.createElement("Cell");
        Date=Save.createElement("Data");
        Date.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("时间"));
        Date.appendChild(Dom_text);
        Cell.appendChild(Date);
        Row.appendChild(Cell);

        Cell=Save.createElement("Cell");
        Date=Save.createElement("Data");
        Date.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("事项名"));
        Date.appendChild(Dom_text);
        Cell.appendChild(Date);
        Row.appendChild(Cell);

        Cell=Save.createElement("Cell");
        Date=Save.createElement("Data");
        Date.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("持续时长"));
        Date.appendChild(Dom_text);
        Cell.appendChild(Date);
        Row.appendChild(Cell);

        Cell=Save.createElement("Cell");
        Date=Save.createElement("Data");
        Date.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("事件备注"));
        Date.appendChild(Dom_text);
        Cell.appendChild(Date);
        Row.appendChild(Cell);

        Cell=Save.createElement("Cell");
        Date=Save.createElement("Data");
        Date.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("分类"));
        Date.appendChild(Dom_text);
        Cell.appendChild(Date);
        Row.appendChild(Cell);
}
    Table.appendChild(Row);
    //标题列
}
    for(int i=0;i<ui->treeWidget->topLevelItemCount();i++)
    {

        Row =Save.createElement("Row");
        Item=ui->treeWidget->topLevelItem(i);
        Cell=Save.createElement("Cell");
        Date=Save.createElement("Data");
        Date.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(Item->text(0));
        Date.appendChild(Dom_text);
        Cell.appendChild(Date);
        Row.appendChild(Cell);
        Table.appendChild(Row);
        for(int k=0;k<ui->treeWidget->topLevelItem(i)->childCount();k++)
        {
            Row =Save.createElement("Row");
            Item=ui->treeWidget->topLevelItem(i)->child(k);
            for(int z=0;z<Item->columnCount();z++)
            {
                Cell=Save.createElement("Cell");
                Date=Save.createElement("Data");
                if(z==3)
                {
                    Date.setAttribute("ss:Type","String");
                    Minute=atoi(Item->text(z).toStdString().c_str());
                    Hour=Minute/60;
                    Minute=Minute%60;
                    if(Hour>9)
                    {
                        Hour_Minute+=Set_Num.setNum(Hour)+":";
                    }
                    else
                    {
                        Hour_Minute+="0";
                        Hour_Minute+=Set_Num.setNum(Hour)+":";
                    }
                    if(Minute>9)
                    {
                        Hour_Minute+=Set_Num.setNum(Minute);
                    }
                    else
                    {
                        if(Minute==-1)
                        {
                           Minute=0;
                        }

                        Hour_Minute+="0";
                        Hour_Minute+=Set_Num.setNum(Minute);

                    }
                    qDebug()<<Hour_Minute;
                    Dom_text=Save.createTextNode(Hour_Minute);
                    Hour_Minute.clear();
                }
                else
                {
                   Date.setAttribute("ss:Type","String");
                   Dom_text=Save.createTextNode(Item->text(z));
                }


                Date.appendChild(Dom_text);
                Cell.appendChild(Date);
                Row.appendChild(Cell);
            }
            Table.appendChild(Row);
        }

    }

    WorkSheet.appendChild(Table);
    WorkBook.appendChild(WorkSheet);
    Save.appendChild(WorkBook);
    Save.save(Text,4);



}

void MainRecordDB::RecordShow()
{
    FlushMap();
    /**************************************************
     * <xml大杀器，高能慎入>
     *************************************************/
    ui->treeWidget->clear();
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
        QTreeWidgetItem *      ItemList;
        QStringList    TopRowList;
        Record =       Day.firstChildElement("Record");
        VarDate=       Record.firstChildElement("Date");
        TopRowList<<VarDate.text()<<" "<<" "<<" "<<" ";
        ItemList=(new QTreeWidgetItem(TopRowList));
        while(!Record.isNull())
        {
            QStringList    RowList;
            RowList<<" ";
            VarDate=       Record.firstChildElement("Time");
            RowList<<VarDate.text();
            VarDate=       Record.firstChildElement("Thing");
            RowList<<VarDate.text().simplified();
            VarDate=       Record.firstChildElement("Minute");
            RowList<<VarDate.text();
            VarDate=       Record.firstChildElement("ThingRem");
            RowList<<VarDate.text().simplified();
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
            RowList<<Sort_ID;
            ItemList->addChild(new QTreeWidgetItem(RowList));
            Record   =     Record.nextSiblingElement("Record");
        }
            RowCount++;
            Day      =     Day.nextSiblingElement("Day");
            ui->treeWidget->addTopLevelItem(ItemList);
    }
    /**************************************************
     * </xml大杀器，高能慎入>
     *************************************************/
}
