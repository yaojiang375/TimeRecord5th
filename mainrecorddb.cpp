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
    FlushMap();
}

MainRecordDB::~MainRecordDB()
{
    delete ui;
}

void MainRecordDB::on_ShowButton_clicked()
{
    FlushMap();
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





void MainRecordDB::on_ConvertToExcel_clicked()
{
    QString FileSave=QFileDialog::getSaveFileName(this,trUtf8("导出为Excel文件"),".",trUtf8("Excel文件 (*.xls);;所有文件(*.*)"));
    QFile   Txt(FileSave);
    Txt.open(QIODevice::ReadWrite);
    Txt.resize(0);
    QTextStream Text(&Txt);
    QDomDocument    Save;
    QDomProcessingInstruction   Instruction;
    QDomElement                 Row;//将来可升级为OPEN XML格式
    QDomElement                 WorkBook;
    QDomElement                 Styles;//重要，无此部分无法打开
        QDomElement                 Style;
        QDomElement                 Alignment;
        QDomElement                 Font;
        QDomElement                 NumberFormat;
    QDomElement                 WorkSheet;
    QDomElement                 Table;
    QDomElement                 Column;
    QDomElement                 Cell;
    QDomElement                 Data;
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
    WorkBook.setAttribute("xmlns:ss","urn:schemas-microsoft-com:office:spreadsheet");
    WorkBook.setAttribute("xmlns:html","http://www.w3.org/TR/REC-html40");
    Styles          =Save.createElement("Styles");
    Style       =Save.createElement("Style");
    Style.setAttribute("ss:ID","Default");
    Style.setAttribute("ss:Name","Normal");
    Alignment   =Save.createElement("Alignment");
    Alignment.setAttribute("ss:Vertical","Center");
    Font        =Save.createElement("Font");
    Font.setAttribute("ss:FontName",trUtf8("宋体");
    Font.setAttribute("x:CharSet","134");
    Font.setAttribute("ss:Size","11");
    Font.setAttribute("ss:Color","#000000");
    Style.appendChild(Alignment);
    Style.appendChild(Font);
    Styles.appendChild(Style);
    Style       =Save.createElement("Style");
    Style.setAttribute("ss:ID","s62");
    NumberFormat=Save.createElement("NumberFormat");
    NumberFormat.setAttribute("ss:Format","Short Date");
    Style.appendChild(NumberFormat);
    Styles.appendChild(Style);

    Style       =Save.createElement("Style");
    Style.setAttribute("ss:ID","s63");
    NumberFormat=Save.createElement("NumberFormat");
    NumberFormat.setAttribute("ss:Format","h:mm;@");
    Style.appendChild(NumberFormat);
    Styles.appendChild(Style);

    Style       =Save.createElement("Style");
    Style.setAttribute("ss:ID","s64");
    NumberFormat=Save.createElement("NumberFormat");
    NumberFormat.setAttribute("ss:Format","@");
    Style.appendChild(NumberFormat);
    Styles.appendChild(Style);

    WorkBook.appendChild(Styles);
    WorkSheet       =Save.createElement("Worksheet");
    WorkSheet.setAttribute("ss:Name",trUtf8("时间统计表"));
    Table           =Save.createElement("Table");
{    Table.setAttribute("ss:ExpandedColumnCount","6");
    Table.setAttribute("ss:ExpandedRowCount","10000000");
    Table.setAttribute( "x:FullColumns","1");
    Table.setAttribute("x:FullRows","1");
    Table.setAttribute("ss:DefaultColumnWidth","54");
    Table.setAttribute("ss:DefaultRowHeight","13.5");
    Column          =Save.createElement("Column");
    Column.setAttribute("ss:StyleID","s62");
    Column.setAttribute("ss:AutoFitWidth","0");
    Column.setAttribute("ss:Width","84.75");
    Table.appendChild(Column);
    Column          =Save.createElement("Column");
    Column.setAttribute("ss:StyleID","s63");
    Column.setAttribute( "ss:AutoFitWidth","0");
    Table.appendChild(Column);
    Column          =Save.createElement("Column");
    Column.setAttribute("ss:StyleID","s64");
    Column.setAttribute("ss:AutoFitWidth","0");
    Column.setAttribute("ss:Span","3");
    Column.setAttribute("ss:Width","84.75");
    Table.appendChild(Column);
    Row             =Save.createElement("Row");
{
        Cell=Save.createElement("Cell");
        Data=Save.createElement("Data");
        Data.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("日期"));
        Data.appendChild(Dom_text);
        Cell.appendChild(Data);
        Row.appendChild(Cell);

        Cell=Save.createElement("Cell");
        Data=Save.createElement("Data");
        Data.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("时间"));
        Data.appendChild(Dom_text);
        Cell.appendChild(Data);
        Row.appendChild(Cell);

        Cell=Save.createElement("Cell");
        Data=Save.createElement("Data");
        Data.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("事项名"));
        Data.appendChild(Dom_text);
        Cell.appendChild(Data);
        Row.appendChild(Cell);

        Cell=Save.createElement("Cell");
        Data=Save.createElement("Data");
        Data.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("持续时长"));
        Data.appendChild(Dom_text);
        Cell.appendChild(Data);
        Row.appendChild(Cell);

        Cell=Save.createElement("Cell");
        Data=Save.createElement("Data");
        Data.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("事件备注"));
        Data.appendChild(Dom_text);
        Cell.appendChild(Data);
        Row.appendChild(Cell);

        Cell=Save.createElement("Cell");
        Data=Save.createElement("Data");
        Data.setAttribute("ss:Type","String");
        Dom_text=Save.createTextNode(trUtf8("分类"));
        Data.appendChild(Dom_text);
        Cell.appendChild(Data);
        Row.appendChild(Cell);
}
    Table.appendChild(Row);
    //标题列
}
    for(int i=0;i<ui->treeWidget->topLevelItemCount();i++)
    {
        Row =Save.createElement("Row");
        for(int k=0;k<ui->treeWidget->topLevelItem(i)->childCount();k++)
        {
            Item=ui->treeWidget->topLevelItem(i);
            Cell=Save.createElement("Cell");
            Data=Save.createElement("Data");
            Data.setAttribute("ss:Type","String");
            Dom_text=Save.createTextNode(Item->text(0));
            Data.appendChild(Dom_text);
            Cell.appendChild(Data);
            Row.appendChild(Cell);

            Item=ui->treeWidget->topLevelItem(i)->child(k);
            for(int z=0;z<Item->columnCount();z++)
            {
                Cell=Save.createElement("Cell");
                Data=Save.createElement("Data");
                switch (z)
                {
                case 1:
                   Data.setAttribute("ss:Type","DateTime");
                   break;
                case 3:
                    Data.setAttribute("ss:Type","Number");
                    break;
                default:
                    Data.setAttribute("ss:Type","String");
                    break;
                }
                if(z==1)
                {
                    Dom_text=Save.createTextNode("1991-03-20T"+Item->text(z)+":00.000");
                }
                else
                {
                    Dom_text=Save.createTextNode(Item->text(z));
                }
                Data.appendChild(Dom_text);
                Cell.appendChild(Data);
                Row.appendChild(Cell);
            }
        }
        Table.appendChild(Row);
    }

    WorkSheet.appendChild(Table);
    WorkBook.appendChild(WorkSheet);
    Save.appendChild(WorkBook);
    Save.save(Text,4);



}
