#include "ui/stankinScheduleEditor2.h"

#include <QApplication>
#include  <mutex>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/app"));

    StankinScheduleEditor2 w;
    w.show();

    cv::Mat img(cv::Mat::zeros(300, 300, CV_8U));
    imshow("window", img);

    return a.exec();
}
