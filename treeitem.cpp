
/*
    treeitem.cpp

    A container for items of data supplied by the simple tree model.
*/

#include <QStringList>
#include "QDebug"
#include "treeitem.h"

//! [0]
TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
{
    parentItem = parent;
    itemData = data;
    Buf_Parentitem=NULL;
}

TreeItem::TreeItem(QVector<QVariant> data, QList<TreeItem *> child, TreeItem *parent)
{
    this->childItems = child;
    this->itemData   = data;
    this->parentItem = parent;
    Buf_Parentitem=NULL;
}







//! [0]

//! [1]
TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}
//! [1]

//! [2]
TreeItem *TreeItem::child(int number)
{
    return childItems.value(number);
}
//! [2]

//! [3]
int TreeItem::childCount() const
{
    return childItems.count();
}
//! [3]

//! [4]
int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
//! [4]

//! [5]
int TreeItem::columnCount() const
{
    return itemData.count();
}
//! [5]

//! [6]
QVariant TreeItem::data(int column) const
{
    return itemData.value(column);
}

QVector<QVariant> TreeItem::returnitemData()
{
    return itemData;
}

QList<TreeItem *> TreeItem::returnchildItem()
{
    return childItems;
}
//! [6]

//! [7]
bool TreeItem::insertChildren(int position = 0, TreeItem *item=0)
{
    qDebug()<<"Postion = "<<position;
    qDebug()<<"size    = "<<childItems.size();
    if(position>childItems.size())
    {
        position=childItems.size();//若超出范围，则在其最下边开始插入
    }

    if (position < 0 || position > childItems.size())
    {
        return false;
    }
        childItems.insert(position, item);


    return true;
}
//! [7]

//! [8]
bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (TreeItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}
//! [8]

//! [9]
TreeItem *TreeItem::parent()
{
    if(Buf_Parentitem!=NULL)
    {
        TreeItem* buf=parentItem;
        parentItem=Buf_Parentitem;
        Buf_Parentitem=NULL;
        qDebug()<<"Parent has been changed";
        return buf;
    }
    else
    {
        return parentItem;
    }

}
//! [9]

//! [10]
bool TreeItem::removeChildren(int position, int count)
{
    qDebug()<<"childItems.size"<<childItems.size();
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        qDebug()<<"this item has been remove , his data is "<<childItems.takeAt(position)->returnitemData();
    }
    return true;
}
//! [10]

bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (TreeItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

//! [11]
bool TreeItem::setData(int column, const QVector<QVariant> &value)
{
    qDebug()<<"itemdata.size="<<itemData.size();
    qDebug()<<"Value.size = "<<value.size();
    if (column < 0 || column > itemData.size())
        return false;
    for(int i=0;i<column;i++)
    {
        itemData[i]=(value.at(i));
    }
    qDebug()<<"end of Value.size = "<<value.size();
    qDebug()<<"end of setData and the data = "<<itemData;
    return true;
}

bool TreeItem::setparent(TreeItem *newparent)
{
    Buf_Parentitem=newparent;
    return true;
}

void TreeItem::TreeItem_copy(TreeItem item)
{
    this->childItems = item.returnchildItem();
    this->itemData   = item.returnitemData();
    this->parentItem = item.parent();
    return;
}
