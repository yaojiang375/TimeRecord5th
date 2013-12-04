#ifndef FORM_H
#define FORM_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include "globe.h"
#include "content_check.h"
#include <QWidget>
#include <QList>
#include "readrecord.h"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QMessageBox>

#include <time.h>

#define  READLENGTH 9999
/*******************************************
 *私有变量version为版本号
 *修改后会把新纪录直接写入源文件中，而且没有撤销功能，感觉比较不妥
 *
 ******************************************/

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT
    
public:
    explicit Form(globeset *globek, QWidget *parent = 0);
    ~Form();


private slots:
    void on_pushButton_clicked();

    void on_SaveRepair_clicked();

    void on_next_clicked();

    void on_pushButton_2_clicked();

    void finishedSlot(QNetworkReply* Reply);//检测更新
    void on_Check_clicked();

private:

    Ui::Form *ui;

    globeset            *globe;
    QList<int>          needfix;//记录需要修改的位置
    QString             readFile;
    QVector<QString>    ReadString;
    QString             judgebuf;
    QList<QString>          wrongstring;

    clock_t                    Time;//消耗时间测试
    QString version;
    QString UpdateVersion;
    QString UpdateAdress;
    QNetworkAccessManager   *manager;
};

#endif // FORM_H
