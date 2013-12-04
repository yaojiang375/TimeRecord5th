#ifndef READRECORD_H
#define READRECORD_H
#include "globe.h"
#include "smsreader.h"
#include <QWidget>
#include "recordgetandpost.h"
#include "mainrecorddb.h"
#include <time.h>
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
    void on_AddToRecordDB_clicked();

    void on_ShowRecordDB_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    void insertTable();
    void RecordShow();
    clock_t Time;
    Ui::ReadRecord *ui;
    bool             test;//测试，用于避免多次点击加入总记录
    RecordGetAndPost *MainRecord;
    globeset *globe;
};

#endif // READRECORD_H
