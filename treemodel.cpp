#include <QtGui>

#include "treeitem.h"
#include "treemodel.h"

//! [0]
TreeModel::TreeModel( QDomDocument &data,
                      QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> header;
    header<<"Sort_ID"<<"quickNum";
    rootItem = new TreeItem(header);
    QDomElement Data=data.firstChildElement("root").firstChildElement("Node");
    //qDebug()<<Data.firstChildElement("Sort_ID").text();
    setupModelData(Data,rootItem);


    Buf_cout=0;
    memset(Buf_Parent_Point,BUF_LENGTH,sizeof(TreeItem*));
    return;
}
//! [0]

//! [1]
TreeModel::~TreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    qDebug()<<"columnCount has been use";
    return rootItem->columnCount();
}
//! [2]

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    qDebug()<<"data has been use";
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

//! [3]
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    qDebug()<<"flag has been use";
    if (!index.isValid())
        return Qt::ItemIsEnabled|Qt::ItemIsDropEnabled;

    return QAbstractItemModel::flags(index)|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled| Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//! [3]

//! [4]
TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    qDebug()<<"getItem has been use";
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
        {
            qDebug()<<"Debug by getitem,this item data = "<<item->returnitemData();
            return item;
        }
    }
    return rootItem;
}
//! [4]

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    qDebug()<<"headerData has been use";
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

//! [5]
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    qDebug()<<"index has been use";

    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
//! [5]
    qDebug()<<"parent.column = "<<parent.column();

//! [6]
    TreeItem *parentItem = getItem(parent);
    qDebug()<<"parent.data   = "<<parentItem->returnitemData();
    qDebug()<<"targer row = "<<row;
    qDebug()<<"parent.childcount() = "<<parentItem->childCount();
    TreeItem *childItem = parentItem->child(row);
    if (childItem)
    {
        qDebug()<<"debug by index childitem Point ="<<childItem;
        qDebug()<<"\ndata ="<<childItem->returnitemData();
        return createIndex(row, column, childItem);

    }
    else
    {
           return QModelIndex();
    }
}
//! [6]

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;
    qDebug()<<"inserColumns has been use";
    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    qDebug()<<"insertRows has been use";
    position=getItem(parent)->childCount();//parent指的是将要作为父节点的表项，直接将数据插入到项目最后即可，省心
    bool success=true;
    beginInsertRows(parent, position, position + rows - 1);
    endInsertRows();
    return success;
}

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    qDebug()<<"parent has been use";
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);

    qDebug()<<"item data = "<<childItem->returnitemData();
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem||parentItem==NULL)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
//! [7]

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    qDebug()<<"removeColumns has been use";
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    qDebug()<<"removeRows has been use";
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    /**************************/
    parentItem=getItem(parent);
    qDebug()<<"the parent item = "<<parentItem->returnitemData();
    qDebug()<<"position = "<<position<<"\nrow = "<<rows;
    qDebug()<<"before remove parent.row = "<<parentItem->childCount();
     /**************************/
    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    qDebug()<<"parent.row = "<<parentItem->childCount();
    return success;
}

Qt::DropActions TreeModel::supportedDropActions() const
{
    qDebug()<<"supportedDropActions has been use";
    return Qt::MoveAction;
}

QStringList TreeModel::mimeTypes() const
{
    qDebug()<<"mimeTypes has been use";
    QStringList types;
    types<<"application/vnd.text.list";
    return types;//&&待修改
}

QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
{
    qDebug()<<"mimeData has been use";
    QMimeData *mimeData = new QMimeData();
    QByteArray  encodeData;
    QDataStream stream(&encodeData,QIODevice::WriteOnly);
    foreach( QModelIndex index,indexes)
    {
        qDebug()<<getItem(index)->returnitemData();
    }
    for(int i=0;i<(indexes.size());i+=2)
    {
        if(indexes[i].isValid())
        {
            stream<<((qint32)indexes[i].internalPointer());
        }
    }
    mimeData->setData("application/vnd.text.list",encodeData);
    return mimeData;
}

bool TreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    qDebug()<<"dropMimeData has been use";
    if (action==Qt::IgnoreAction)
        return true;
    if(!data->hasFormat("application/vnd.text.list"))
        return false;
    int beginrow;
    if(row!=-1)
    {
        beginrow=row;
    }
    else
    {
        if(parent.isValid())
        {
            beginrow=parent.row();
        }
        else
        {
            beginrow=0;
        }
    }
    QByteArray encodeData=data->data("application/vnd.text.list");
    QDataStream stream(&encodeData,QIODevice::ReadOnly);
    int rows=0;
    while(!stream.atEnd())
    {
        qint32 buf;
        stream>>buf;
        Buf_Parent_Point[rows]=(TreeItem*)buf;
        rows++;
    }
    /******************************
     *test
     *
     *****************************/
    TreeItem *test=getItem(parent);
    qDebug()<<"target Data Value is "<<test->returnitemData();
    qDebug()<<"parent.column = "<<parent.column()<<" and  parent.isValid() = "<<parent.isValid();
    if(parent.isValid()&& parent.column() != 0)
    {
        return false;
    }
    /******************************
     *test
     *
     *****************************/
    beginrow=getItem(parent)->childCount();//beginrow用于记录初始插入的位置
    insertRows(beginrow,rows,parent);

    for(int i=0;i<rows;i++)
    {
        Buf_Parent_Point[i]->setparent(getItem(parent));
        getItem(parent)->insertChildren(beginrow+i,Buf_Parent_Point[i]);
        if(Buf_Parent_Point[i]->childCount()>0)
        {
            insertRows(0,Buf_Parent_Point[i]->childCount(),index(beginrow+i,2,parent));
        }
    }
    return true;
}

QVector<QVariant> TreeModel::returnItemData( QModelIndex index)
{
    qDebug()<<"returnItemData has been use";
    TreeItem *a=getItem(index);
    return a->returnitemData();
}



//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
    qDebug()<<"rowCount has been use";
    TreeItem *parentItem = getItem(parent);//&&
    if(parentItem!=NULL)
    {
    return parentItem->childCount();
    }
    else
    {
        return 0;
    }
}
//! [8]

bool TreeModel::setData(const QModelIndex &index, const QVector<QVariant> &value,
                        int role)
{
    qDebug()<<"setData has been use";
    qDebug()<<"begin setdata";
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    qDebug()<<"index.column = "<<index.column();
    qDebug()<<"setData = "<<value;
    int index_column=index.column();
    if(index_column==-1)
    {
        index_column=2;//&&要疯了b
    }
    bool result = item->setData(index_column, value);
    if (result)
        emit dataChanged(index, index);

    return result;

}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVector<QVariant> &value, int role)
{
    qDebug()<<"setHeaderData has been use";
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;

}

TreeItem * TreeModel::setupModelData( QDomElement &Node, TreeItem *parent)
{
    qDebug()<<"setupModelData has been use";
    QDomElement    ChildNode=Node.nextSiblingElement("Node");
    if(!ChildNode.isNull())
    {
        setupModelData(ChildNode,parent);
        qDebug()<<"Sibling = "<<ChildNode.firstChildElement("Quick_Num").text();
    }

    QVector<QVariant> comment;
    comment<<Node.firstChildElement("Sort_ID").text()<<Node.firstChildElement("Quick_Num").text();
    TreeItem *child=new TreeItem(comment,parent);
    parent->insertChildren(0,child);
    ChildNode=Node.firstChildElement("Node");
    if(!ChildNode.isNull())
    {
        setupModelData(ChildNode,child);
        qDebug()<<"child = "<<ChildNode.firstChildElement("Quick_Num").text();
    }
    return  child;

}
