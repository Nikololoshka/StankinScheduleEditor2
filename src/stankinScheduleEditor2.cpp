#include "stankinScheduleEditor2.h"
#include "ui_stankinScheduleEditor2.h"

StankinScheduleEditor2::StankinScheduleEditor2(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::StankinScheduleEditor2)
{
    ui->setupUi(this);

    initTable();
    try {
        QString root = "D:\\Temp files\\Qt projects\\data\\ИДБ-17-09.json";
        // QDir dir(root);

        // for (auto &path : dir.entryList()) {
            QFile file(root);

            qDebug() << "File open " + file.fileName() + "...";
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "File not open!";
            }

            auto json = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(json);

            // qDebug() << QString(json);

            qDebug() << "Schedule load...";
            schedule_ = Schedule::fromJson(doc);
            qDebug() << "Schedule complite!";

            updateTable();
        //    break;
        // }

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

void StankinScheduleEditor2::initTable()
{
    headerView_.reset(new ScheduleHeaderView());
    ui->table->setVerticalHeader(headerView_.get());
    ui->table->verticalHeader()->setStretchLastSection(true);

    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->table->setHorizontalHeaderLabels(Time_::timeList());
    ui->table->setVerticalHeaderLabels(DateUtils::dayOfWeekMiniList());

    connect(ui->table, &QTableWidget::cellClicked, [this](int, int){
        ui->table->verticalHeader()->doItemsLayout();
    });

    ui->table->setStyleSheet(
        "QTableWidget::item { selection-background-color: #deeff5; } "
    );

    ui->table->selectionModel()->setCurrentIndex(
        ui->table->model()->index(0, 0), QItemSelectionModel::Select
    );

    resizeTable();
}

void StankinScheduleEditor2::updateTable()
{
    ui->table->setRowCount(schedule_.row());
    headerView_->setIndexses(schedule_.indexes());
    resizeTable();
}

void StankinScheduleEditor2::resizeTable()
{
    const int REAL_ROW_SIZE = (ui->table->height() - ui->table->horizontalHeader()->height()) / 6;

    auto indexes = schedule_.indexes();

    int rowPadding = 0;
    for (int i = 0; i < indexes.size(); ++i) {
        const int rowSize = REAL_ROW_SIZE / indexes[i];
        for (int j = 0; j < indexes[i]; ++j) {
            const int index = rowPadding + j;
            ui->table->verticalHeader()->resizeSection(index, rowSize);
        }
        rowPadding += indexes[i];
    }

    ui->table->repaint();
}



void StankinScheduleEditor2::resizeEvent(QResizeEvent *event)
{
    resizeTable();
    QMainWindow::resizeEvent(event);
}


void StankinScheduleEditor2::showEvent(QShowEvent *event)
{
    resizeTable();
    QMainWindow::showEvent(event);
}
