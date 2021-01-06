#include "stankinScheduleEditor2.h"
#include "ui_stankinScheduleEditor2.h"

#include "editor/pairSelectorDialog.h"
#include "import/importDialog.h"
#include "import/transitionDialog.h"
#include "import/setsDialog.h"
#include "import/parseWorkerManager.h"
#include "export/exportDialog.h"


StankinScheduleEditor2::StankinScheduleEditor2(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::StankinScheduleEditor2),
      isModified(false)
{
    ui->setupUi(this);

    schedule_ = QSharedPointer<Schedule>::create();

    initStatusBar();
    initTable();

    connect(ui->actionCreate, &QAction::triggered,
            this, &StankinScheduleEditor2::onNewFileClicked);
    connect(ui->actionOpen, &QAction::triggered,
            this, &StankinScheduleEditor2::onOpenFileClicked);
    connect(ui->actionSave, &QAction::triggered,
            this, &StankinScheduleEditor2::onSaveFileClicked);
    connect(ui->actionSaveAs, &QAction::triggered,
            this, &StankinScheduleEditor2::onSaveAsFileClicked);
    connect(ui->actionImport, &QAction::triggered,
            this, &StankinScheduleEditor2::onImportClicked);
    connect(ui->actionExport, &QAction::triggered,
            this, &StankinScheduleEditor2::onExportClicked);
    connect(ui->actionSets, &QAction::triggered,
            this, &StankinScheduleEditor2::onSetsButtonClicked);
    connect(ui->actionTransition, &QAction::triggered,
            this, &StankinScheduleEditor2::onTransitionButtonClicked);
}

StankinScheduleEditor2::~StankinScheduleEditor2()
{
    delete ui;
}

void StankinScheduleEditor2::onNewFileClicked()
{
    if (isModified) {
        auto answer = QMessageBox::warning(this,
                                           "Расписание было изменено",
                                           "Вы хотите сохранить внесенные изменения?\n"
                                           "Ваши изменения будут потеряны, если вы не сохраните их",
                                           QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                           QMessageBox::Save);
        switch (answer) {
            case QMessageBox::Save: {
                onSaveFileClicked();
                break;
            }
            case QMessageBox::Cancel: {
                return;
            }
            default: {

            }
        }
    }

    schedule_ = QSharedPointer<Schedule>::create();
    file_ = std::optional<QFileInfo>();
    isModified = false;
    updateTable();
    setWindowTitle(qApp->applicationName());
}

void StankinScheduleEditor2::onOpenFileClicked()
{
    auto path = QFileDialog::getOpenFileName(this,
                                             "Открытие расписание",
                                             ".",
                                             "Json file (*.json)");
    if (path.isEmpty()) {
        return;
    }

    try {
        loadSchedule(path);
    } catch (std::exception &e) {
        QMessageBox::critical(this,
                              "Ошибка загрузки расписания",
                              e.what());
        return;
    }

    file_ = std::make_optional<QFileInfo>(path);
    setWindowTitle(qApp->applicationName() + " [" + file_->fileName() + "]");
}

void StankinScheduleEditor2::onSaveFileClicked()
{
    if (!file_.has_value()) {
        onSaveAsFileClicked();
    }

    QFile file(file_->absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly)) {
        // TODO: Open error
    }

    auto json = schedule_->toJson();
    QJsonDocument doc(json);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

void StankinScheduleEditor2::onSaveAsFileClicked()
{
    auto path = QFileDialog::getSaveFileName(this,
                                             "Сохранить расписание",
                                             ".",
                                             "Json file (*.json)");
}

void StankinScheduleEditor2::onImportClicked()
{
    auto import = new ImportDialog(this);
    import->show();
}

void StankinScheduleEditor2::onExportClicked()
{
    auto export_ = new ExportDialog(this);
    export_->show();
}

void StankinScheduleEditor2::onTransitionButtonClicked()
{
    auto workerManager = QSharedPointer<ParseWorkerManager>::create();
    auto dialog = new TransitionDialog(workerManager, this);
    dialog->show();
}

void StankinScheduleEditor2::onSetsButtonClicked()
{
    auto workerManager = QSharedPointer<ParseWorkerManager>::create(0);
    auto dialog = new SetsDialog(workerManager, this);
    dialog->show();
}

void StankinScheduleEditor2::onTableCellDoubleClicked(int row, int column)
{
    auto index = schedule_->transform(row, column);
    auto selector = new PairSelectorDialog(schedule_, index, this);
    selector->exec();
    updateTable();
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

    connect(ui->table, &QTableWidget::cellDoubleClicked,
            this, &StankinScheduleEditor2::onTableCellDoubleClicked);

    ui->table->setTextElideMode(Qt::ElideNone);
    ui->table->setStyleSheet(
        "QTableWidget::item { "
        "   selection-background-color: #deeff5;"
        "   selection-color: #000000;"
        "}"
    );

    resizeTable();
}

void StankinScheduleEditor2::loadSchedule(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "File not open!";
        throw std::logic_error(("Не удалось открыть файл! " + file.errorString()).toStdString());
    }

    auto json = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(json);
    schedule_ = QSharedPointer<Schedule>::create(doc);
    updateTable();
    isModified = false;
}

void StankinScheduleEditor2::updateTable()
{
    ui->table->clearSpans();
    ui->table->clearContents();

    ui->table->setRowCount(schedule_->row());
    headerView_->setIndexses(schedule_->indexes());

    for (int i = 0; i < ui->table->rowCount(); ++i) {
        for (int j = 0; j < ui->table->columnCount(); ++j) {
            auto cell = schedule_->pairsTextByIndex(i, j);
            QTableWidgetItem *item = new QTableWidgetItem(cell.text);
            item->setTextAlignment(Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap);
            ui->table->setItem(i, j, item);

            if (cell.isSpanValid()) {
                ui->table->setSpan(i, j, cell.rowSpan, cell.columnSpan);
            }

            if (cell.columnSpan > 1) {
                j += cell.columnSpan - 1;
            }
        }
    }

    isModified = true;
    resizeTable();
}

void StankinScheduleEditor2::resizeTable()
{
    const int REAL_ROW_SIZE = (ui->table->height() - ui->table->horizontalHeader()->height()) / 6;

    auto indexes = schedule_->indexes();

    int rowPadding = 0;
    for (int i = 0; i < indexes.size(); ++i) {
        const int rowSize = REAL_ROW_SIZE / indexes[i];
        const int columnSize = ui->table->columnWidth(0);

        for (int j = 0; j < indexes[i]; ++j) {
            const int index = rowPadding + j;
            ui->table->verticalHeader()->resizeSection(index, rowSize);

            for (int k = 0; k < ui->table->columnCount(); ++k) {
                auto item = ui->table->item(index, k);
                if (item != nullptr) {
                    int columnSpanCount = ui->table->columnSpan(index, k);
                    int rowSpanCount = ui->table->rowSpan(index, k);

                    if (!item->text().isEmpty()) {
                        auto font = fontForText(item->text(),
                                                columnSize * columnSpanCount,
                                                rowSize * rowSpanCount - 5);
                        item->setFont(font);
                    }
                }
            }
        }
        rowPadding += indexes[i];
    }

    ui->table->repaint();
}

void StankinScheduleEditor2::updateStatusBarCoords(int row, int column)
{
    auto index = schedule_->transform(row, column);    
    labelCoords_->setText(index.toString());

    for (auto& p:  schedule_->pairsByIndex(index)) {
         qDebug() << p.toString();
    }
}

QFont StankinScheduleEditor2::fontForText(const QString &text, int width, int height,
                                          unsigned int flags)
{
    QFont font = qApp->font();

    for (int i = 12; i > 3; --i) {
        font.setPixelSize(i);
        auto rect = QFontMetrics(font)
                        .boundingRect(0, 0, width, height, static_cast<int>(flags), text);
        if (rect.width() < width && rect.height() < height) {
            font.setPixelSize(i);
            break;
        }
    }

    return font;
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
