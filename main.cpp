#include "readrecord.h"
#include <QApplication>
#include <globe.h>
#include "form.h"

/*****************
 *测试
 *
 ****************/
#include "mainrecorddb.h"
globeset globe;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    globe.read();
    /*Form b(&globe);
    b.show();
    */
    MainRecordDB b(&globe);
    b.show();
    return a.exec();
}
