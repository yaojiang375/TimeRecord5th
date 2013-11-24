#ifndef READRECORD_H
#define READRECORD_H
#include "globe.h"
#include "smsreader.h"
#include <QWidget>
#include "recordgetandpost.h"
#include "mainrecorddb.h"
namespace Ui {
class ReadRecord;
}

class ReadRecord : public QWidget
{
    Q_OBJECT
    
public:
    explicit ReadRecord(globeset *globek, QWidget *parent = 0);
    ~ReadRecord();
signals:


private slots:

    void on_ShowButton_clicked();
    void on_AddToRecordDB_clicked();

    void on_ShowRecordDB_clicked();



private:
    void insertTable();
    Ui::ReadRecord *ui;
    globeset *globe;
};

#endif // READRECORD_H
