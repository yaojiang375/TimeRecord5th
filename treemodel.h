/**************************
 *
 *由示例文件中引入，负责显示树形结构
 *
 *model部分
 *
 *************************/
#ifndef TREEMODEL_H
#define TREEMODEL_H
#define BUF_LENGTH 10000
#include <QDebug>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QtXml/qdom.h>
#include <QMimeData>
class TreeItem;

//! [0]
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeModel( QDomDocument &data,
              QObject *parent = 0);
    ~TreeModel();
//! [0] //! [1]

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
//! [1]

//! [2]
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVector<QVariant> &value,
                 int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVector<QVariant> &value, int role = Qt::EditRole);

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());
/***********************
 *拖放支持
 ***********************/
    Qt::DropActions supportedDropActions() const;
    QStringList     mimeTypes() const;//&&存疑，可能需要更换
    QMimeData       *mimeData(const QModelIndexList &indexes) const;
    bool            dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QVector<QVariant> returnItemData(QModelIndex index);
/***********************
*\拖放支持
***********************/
private:
    TreeItem *setupModelData( QDomElement &Node, TreeItem *parent);
    TreeItem *getItem(const QModelIndex &index) const;

    TreeItem *rootItem;
    /*******************
     *拖放支持
     *******************/
    TreeItem* Buf_Parent_Point[BUF_LENGTH];
    int       Buf_cout;
};
//! [2]

#endif
