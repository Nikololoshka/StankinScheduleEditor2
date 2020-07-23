#include "stankinScheduleEditor2.h"
#include "ui_stankinScheduleEditor2.h"

StankinScheduleEditor2::StankinScheduleEditor2(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::StankinScheduleEditor2)
{
    ui->setupUi(this);

    initStatusBar();
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

void StankinScheduleEditor2::initStatusBar()
{
    labelCoords_ = new QLabel(this);
    labelCoords_->setText("");
    ui->statusBar->addPermanentWidget(labelCoords_);
}

void StankinScheduleEditor2::initTable()
{
    headerView_= new ScheduleVerticalHeader(ui->table);
    ui->table->setVerticalHeader(headerView_);
    ui->table->verticalHeader()->setStretchLastSection(true);

    ui->table->setHorizontalHeader(new ScheduleHorizontalHeader(ui->table));

    ui->table->horizontalHeader()->setMinimumHeight(30);
    ui->table->verticalHeader()->setMinimumWidth(30);

    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->table->setHorizontalHeaderLabels(Time_::timeList());
    ui->table->setVerticalHeaderLabels(DateUtils::dayOfWeekMiniList());


    connect(ui->table, &QTableWidget::itemSelectionChanged, [this](){
        auto indexes = ui->table->selectionModel()->selectedIndexes();
        if (!indexes.isEmpty()) {
            auto& index = indexes.first();
            ui->table->verticalHeader()->doItemsLayout();
            updateStatusBarCoords(index.row(), index.column());
        }
    });

    ui->table->setStyleSheet(
        "QTableWidget::item { selection-background-color: #deeff5; } "
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

void StankinScheduleEditor2::updateStatusBarCoords(int row, int column)
{
    auto index = schedule_.transform(row, column);
    labelCoords_->setText(QString("Координаты: (%1; %2; %3)")
                              .arg(index.row).arg(index.number).arg(index.innerRow));
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

