#include "ui/stankinScheduleEditor2.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/app"));

    StankinScheduleEditor2 w;
    w.show();

    return a.exec();
}
