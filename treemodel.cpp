/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

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
    return rootItem->columnCount();
}
//! [2]

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
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
    if (!index.isValid())
        return Qt::ItemIsEnabled|Qt::ItemIsDropEnabled;

    return QAbstractItemModel::flags(index)|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled| Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//! [3]

//! [4]
TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}
//! [4]

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

//! [5]
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{

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
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    position=getItem(parent)->childCount();//parent指的是将要作为父节点的表项，直接将数据插入到项目最后即可，省心
    TreeItem *parentItem = getItem(parent);
    bool success;
    QVector<QVariant> Data;
    Data<<" "<<" ";

    beginInsertRows(parent, position, position + rows - 1);
    while(rows>0)
    {
    success &= parentItem->insertChildren(position,new TreeItem(Data,parentItem));
    rows--;
    }
    endInsertRows();

    return success;
}

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
//! [7]

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
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
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    /**************************/
    parentItem=getItem(parent);
    qDebug()<<"the parent item = "<<parentItem->returnitemData();
    qDebug()<<"position = "<<position<<"\nrow = "<<rows;
    for(int i=position;i<rows+position;i++)
    {
        TreeItem* child_parent_needtoinsert=parentItem->child(i);
        qDebug()<<"child_parent_needtoinsert is "<<child_parent_needtoinsert->returnitemData();
        if(child_parent_needtoinsert->childCount()>0)
        {
            qDebug()<<"The item ready to delete whitch has child = "<<child_parent_needtoinsert->returnitemData();
            qDebug()<<"parent item  = "<<parentItem->returnitemData();
            TreeItem *tk = new TreeItem(child_parent_needtoinsert->returnitemData(),child_parent_needtoinsert->returnchildItem(),child_parent_needtoinsert->parent());
            Buf_Parent_Point[Buf_cout]=tk;
            Buf_cout++;
        }
    }
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
    return Qt::MoveAction;
}

QStringList TreeModel::mimeTypes() const
{
    QStringList types;
    types<<"application/vnd.text.list";
    return types;//&&待修改
}

QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray  encodeData;
    QDataStream stream(&encodeData,QIODevice::WriteOnly);
    foreach( QModelIndex index,indexes)
    {
        qDebug()<<getItem(index)->returnitemData();
    }
    int k=1;
    k++;


    for(int i=0;i<(indexes.size()/2);i++)
    {
        if(indexes[i].isValid())
        {
            QString text;
            text=((TreeItem*)indexes[i].internalPointer())->returnitemData().at(0).toString();
            stream<<text;
            qDebug()<<"index["<<i<<"] = "<<text;
            text=((TreeItem*)indexes[i].internalPointer())->returnitemData().at(1).toString();
            qDebug()<<"index["<<i<<"] = "<<text;
            stream<<text;//此处颇应改进&&
        }
    }
    mimeData->setData("application/vnd.text.list",encodeData);
    return mimeData;
}

bool TreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action==Qt::IgnoreAction)
        return true;
    if(!data->hasFormat("application/vnd.text.list"))
        return false;
    /***
     *if(column>0)
     *return false;
     ***/
    /**********
     *
     *删除行前先将子树部分转移出来插入
     ********************/

    /*************************
     *
     *end
     *
     ************************/
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
    QStringList newItems;
    int rows=0;
    while(!stream.atEnd())
    {
        QString text;
        stream>>text;
        newItems<<text;
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
    insertRows(beginrow,rows/2,parent);

    QVector<QVariant> var;
    int num=0;
    while(num<(newItems.size()))
    {
        QModelIndex idx = index(beginrow,2,parent);
        var<<newItems[num]<<newItems[num+1];
        qDebug()<<newItems[num+1];
        qDebug()<<"all newitem = "<<newItems;
        num+=2;
        qDebug()<<"std var = "<<var;
        setData(idx,var);
        qDebug()<<"set end";
        var.clear();
        beginrow++;
    }
    while(Buf_cout>=0)
    {
        TreeItem *Supply_Origin=Buf_Parent_Point[Buf_cout];
        TreeItem *Supply_Parent=Supply_Origin->parent();
        /*
        beginInsertRows(Supply_Parent,Supply_Parent->childCount(),parentItem->childCount()+child_parent_needtoinsert->childCount()-1);

        for(int z=0;z< child_parent_needtoinsert->childCount();z++)
        {
            qDebug()<<"child item = "<<child_parent_needtoinsert->child(z)->returnitemData();
            parentItem->insertChildren(parentItem->childCount(),child_parent_needtoinsert->child(z));
            child_parent_needtoinsert->child(z)->setparent(parentItem);
            index(parentItem->childCount()-1,2,parent);
        }
        endInsertRows();
        */
        Buf_cout--;
    }
    return true;
}

QVector<QVariant> TreeModel::returnItemData( QModelIndex index)
{
    TreeItem *a=getItem(index);
    return a->returnitemData();
}



//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
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
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

TreeItem * TreeModel::setupModelData( QDomElement &Node, TreeItem *parent)
{
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
