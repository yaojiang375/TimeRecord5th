#include "readrecord.h"
#include <QApplication>
#include <globe.h>
#include "sorttherecordnamelikedir.h"

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
    SortTheRecordNameLikeDir tk;
    tk.show();
    return a.exec();
}
