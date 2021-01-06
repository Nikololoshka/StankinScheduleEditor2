#include "ui/stankinScheduleEditor2.h"


int main(int argc, char *argv[])
{
    #if defined(WIN32) || defined(WIN64)
    setlocale(LC_ALL, "Russian");
    #endif

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/app"));

    StankinScheduleEditor2 w;
    w.show();

    // cv::Mat img(cv::Mat::zeros(300, 300, CV_8U));
    // imshow("window", img);

    return a.exec();
}
