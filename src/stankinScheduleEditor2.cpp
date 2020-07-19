#include "stankinScheduleEditor2.h"
#include "forms/ui_stankinScheduleEditor2.h"

StankinScheduleEditor2::StankinScheduleEditor2(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::StankinScheduleEditor2)
{
    ui->setupUi(this);

    try {
        QString root = "D:\\Temp files\\Qt projects\\data";
        QDir dir(root);

        for (auto &path : dir.entryList()) {
            QFile file(root + "\\" + path);

            qDebug() << "File open " + file.fileName() + "...";
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "File not open!";
            }

            auto json = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(json);

            // qDebug() << QString(json);

            qDebug() << "Schedule load...";
            Schedule schedule = Schedule::fromJson(doc);
            qDebug() << "Schedule complite!";
        }

    } catch (std::exception &e) {
        qDebug() << e.what();
    } catch (QException &e) {
        qDebug() << e.what();
    } catch (...) {
        qDebug() << "Unknown error :(";
    }
}

StankinScheduleEditor2::~StankinScheduleEditor2()
{
    delete ui;
}

