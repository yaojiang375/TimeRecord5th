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
#define  READLENGTH 9999
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

    void on_pushButton_2_clicked();

    void on_next_clicked();

private:

    Ui::Form *ui;

    globeset            *globe;
    QList<int>          needfix;//记录需要修改的位置
    QString             readFile;
    QList<QString>      ReadString;
    QString             judgebuf;
    QList<QString>          wrongstring;

};

#endif // FORM_H
