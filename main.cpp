#include "readrecord.h"
#include <QApplication>
#include <globe.h>
#include "readrecord.h"

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
    ReadRecord t(&globe);
    t.show();
    return a.exec();
}
