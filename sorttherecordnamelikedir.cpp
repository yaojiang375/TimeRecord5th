#include "sorttherecordnamelikedir.h"
#include "ui_sorttherecordnamelikedir.h"
#include "treeitem.h"
#include "treemodel.h"
#include <QDebug>
#include <QFile>
SortTheRecordNameLikeDir::SortTheRecordNameLikeDir(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SortTheRecordNameLikeDir)
{
    ui->setupUi(this);
    QFile txt("C:/test.xml");
    txt.open(QIODevice::ReadOnly);
    QDomDocument read;
    read.setContent(&txt);
    qDebug()<<txt.readAll();
    qDebug()<<"read is null ="<<read.isNull();
    qDebug()<<read.firstChildElement("root").text();
    TreeModel   *model = new TreeModel(read);

    ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->treeView->setDragEnabled(true);
    ui->treeView->setAcceptDrops(true);
    ui->treeView->setDropIndicatorShown(true);
    ui->treeView->setModel(model);
    //ui->treeView->show();
}

SortTheRecordNameLikeDir::~SortTheRecordNameLikeDir()
{
    delete ui;
}
