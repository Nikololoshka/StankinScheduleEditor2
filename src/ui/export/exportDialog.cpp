#include "exportDialog.h"
#include "ui_exportDialog.h"

#include "exportWorker.h"
#include "exporter.h"
#include <QProgressDialog>


ExportDialog::ExportDialog(QWidget *parent) :
      QDialog(parent),
      ui(new Ui::exportDialog)
{
    ui->setupUi(this);

    int maxWorkers = QThreadPool::globalInstance()->maxThreadCount();
    workerPool_.setMaxThreadCount(maxWorkers);
    workManager_ = QSharedPointer<ExportWorkerManager>::create(maxWorkers);

    initMainSetting();
    initPrintSetting();
    initDateSetting();
    initSubgroupSetting();

    QAction *filesAction = new QAction("Файлы", ui->addSchedulesButton);
    ui->addSchedulesButton->addAction(filesAction);

    QAction *folderAction = new QAction("Папку", ui->addSchedulesButton);
    ui->addSchedulesButton->addAction(folderAction);

    connect(filesAction, &QAction::triggered,
            this, &ExportDialog::onAddFilesClicked);
    connect(folderAction, &QAction::triggered,
            this, &ExportDialog::onAddFolderClicked);
    connect(ui->exportButton, &QPushButton::clicked,
            this, &ExportDialog::onExportButtonClicked);

    // test section
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFileName("test.pdf");

    Exporter exporter;
    exporter.runExport(printer);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::onAddFilesClicked()
{
    auto paths = QFileDialog::getOpenFileNames(this,
                                  "Выбор файлов расписаний",
                                  QDir::homePath(),
                                  "PDF file (*.pdf) ;; All files (*.*)");
    if (paths.isEmpty()) {
        return;
    }

    updateScheduleList(paths);
}

void ExportDialog::onAddFolderClicked()
{
    auto path = QFileDialog::getExistingDirectory(this,
                                                  "Выбор папки с расписаниями",
                                                  QDir::homePath());
    if (path.isEmpty()) {
        return;
    }

    auto paths = recursiveFind(path);
    updateScheduleList(paths);
}

void ExportDialog::onExportButtonClicked()
{

    int scheduleCount = ui->scheduleListWidget->count();
    QProgressDialog *progressDialog = new QProgressDialog("Экспорт расписаний",
                                                          "Отмена",
                                                          0,
                                                          scheduleCount,
                                                          this);
    progressDialog->setWindowTitle("Прогресс экспорта");
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setWindowFlag(Qt::WindowCloseButtonHint, false);
    progressDialog->setAutoClose(false);

    QStringList paths;
    for (int i = 0; i < scheduleCount; ++i) {
        paths << ui->scheduleListWidget->item(i)->data(PATH_ROLE).toString();
    }
    workManager_->setSchedules(paths);

    for (int i = 0; i < workerPool_.maxThreadCount(); ++i) {
        ExportWorker *worker = new ExportWorker(workManager_);
        worker->setAutoDelete(true);
        workerPool_.start(worker);
    }

    connect(workManager_.get(), &ExportWorkerManager::changedProgress,
            progressDialog, &QProgressDialog::setValue);
    connect(workManager_.get(), &ExportWorkerManager::workComplete,
            progressDialog, &QProgressDialog::close);
    connect(progressDialog, &QProgressDialog::canceled,
            workManager_.get(), &ExportWorkerManager::onCancelWork);

    enableUI(false);
    progressDialog->exec();
    enableUI(true);
}

void ExportDialog::onStartDateChanged(const QDate &newDate)
{
    if (ui->dateEnd->date() < newDate.addDays(7)) {
        ui->dateEnd->setDate(newDate.addDays(7));
    }

    ui->dateEnd->setMinimumDate(newDate);
    updateDateCount();
}

void ExportDialog::onEndDateChanged(const QDate &newDate)
{
    Q_UNUSED(newDate)
    updateDateCount();
}

void ExportDialog::initMainSetting()
{
    // тип экспорта
    const QVector<QPair<QString, ExportType>> exportTypes = {
        { "Еженедельное", ExportType::Weekly },
        { "Полное", ExportType::Full },
    };
    for (auto &type: exportTypes) {
        ui->exportTypeComboBox->addItem(type.first, static_cast<int>(type.second));
    }

    // тип печати
    const QVector<QPair<QString, PrintType>> printTypes = {
        { "PDF", PrintType::PDF },
        { "На принтер", PrintType::Printer },
    };
    for (auto &type: printTypes) {
        ui->printTypeComboBox->addItem(type.first, static_cast<int>(type.second));
    }
}

void ExportDialog::initPrintSetting()
{
    ui->encodingComboBox->addItems({
       "UTF-8", "CP1251", "Latin-1"
    });
    ui->scheduleNameComboBox->addItems({
       "Исходя из имени файла"
    });
    ui->showDateCheckBox->setChecked(true);
}

void ExportDialog::initDateSetting()
{
    QDate current = QDate::currentDate();
    if (current.month() < 6) {
        current.setDate(current.year(), 2, 1);
    } else {
        current.setDate(current.year(), 9, 1);
    }
    current = current.addDays(8 - current.dayOfWeek());

    ui->dateStart->setDate(current);
    ui->dateStart->setMinimumDate(current);

    ui->dateEnd->setDate(current.addDays(7 * 16));
    ui->dateEnd->setMinimumDate(current.addDays(7));

    updateDateCount();

    connect(ui->dateStart, &QDateEdit::dateChanged,
            this, &ExportDialog::onStartDateChanged);
    connect(ui->dateEnd, &QDateEdit::dateChanged,
            this, &ExportDialog::onEndDateChanged);
}

void ExportDialog::initSubgroupSetting()
{
    const QVector<QPair<QString, QColor>> colors = {
        { "Морская волна", QColor(0, 255, 255) },
        { "Серый", QColor(128, 128, 128) },
        { "Тёмно-синий", QColor(0, 0, 192) },
        { "Серебряный", QColor(192, 192, 192) },
        { "Чёрный", QColor(0, 0, 0) },
        { "Зелёный", QColor(0, 128, 0) },
        { "Оливковый", QColor(192, 192, 0) },
        { "Синий", QColor(0, 0, 255) },
        { "Лайм", QColor(0, 255, 0) },
        { "Пурпурный", QColor(128, 0, 128) },
        { "Белый", QColor(255, 255, 255) },
        { "Фуксия", QColor(255, 0, 255) },
        { "Тёмно-бордовый", QColor(128, 0, 0) },
        { "Красный", QColor(255, 0, 0) },
        { "Жёлтый", QColor(255, 255, 0) },
    };

    for (auto& color: colors) {
        QIcon icon = createColorIcon(color.second);
        ui->subgroupAComboBox->addItem(icon, color.first, color.second);
        ui->subgroupBComboBox->addItem(icon, color.first, color.second);
    }

    ui->subgroupAComboBox->setCurrentText("Лайм");
    ui->subgroupBComboBox->setCurrentText("Жёлтый");
}

void ExportDialog::updateScheduleList(const QStringList &paths)
{
    ui->scheduleListWidget->clear();
    QFileIconProvider iconProvider;
    for (auto &path : paths) {
        QFileInfo file(path);
        auto item = new QListWidgetItem(ui->scheduleListWidget);
        item->setText(file.baseName());
        item->setData(PATH_ROLE, path);
        item->setToolTip(path);
        item->setIcon(iconProvider.icon(file));
    }

    updateCurrentScheduleCount(paths.size());
}

void ExportDialog::updateCurrentScheduleCount(int count) const
{
    ui->schedulesLabel->setText("Выбранно расписаний: " + QString::number(count));
}

void ExportDialog::updateDateCount() const
{
    qint64 days = ui->dateStart->date().daysTo(ui->dateEnd->date());
    int weeks = static_cast<int>(days / 7);
    ui->dateSelectedLabel->setText(QString::number(weeks) + " недель");
}

QIcon ExportDialog::createColorIcon(const QColor &color)
{
    QPixmap pix(32, 32);
    pix.fill(color);
    return QIcon(pix);
}

QStringList ExportDialog::recursiveFind(QString rootDir) const
{
    QStringList paths;
    QDirIterator it(rootDir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        auto path = it.next();
        if (path.endsWith(".json")) {
            paths << path;
        }
    }
    return paths;
}

void ExportDialog::enableUI(bool enable) const
{
    ui->exportButton->setEnabled(enable);
    ui->cancelButton->setEnabled(enable);
    ui->addSchedulesButton->setEnabled(enable);
    ui->scheduleListWidget->setEnabled(enable);
    ui->mainGroupBox->setEnabled(enable);
    ui->printGroupBox->setEnabled(enable);
    ui->dateGroupBox->setEnabled(enable);
    ui->subgroupGroupBox->setEnabled(enable);
}
