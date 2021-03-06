#include "importDialog.h"
#include "ui_importDialog.h"

#include "parseFileException.h"
#include "setsDialog.h"
#include "transitionDialog.h"

ImportDialog::ImportDialog(QWidget *parent) :
      QDialog(parent),
      ui(new Ui::ImportDialog)
{
    ui->setupUi(this);

    int maxWorkers = QThreadPool::globalInstance()->maxThreadCount();
    workerPool_.setMaxThreadCount(maxWorkers);
    workerManager_ = QSharedPointer<ParseWorkerManager>::create(maxWorkers);

    int columns = 2;
    int rows = maxWorkers / columns;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            // нужен progress bar
            if (i * columns + j < maxWorkers) {
                auto progress = new QProgressBar(this);
                progress->setRange(0, 100);
                progress->setTextVisible(true);

                workerProgressBars_.append(progress);
                ui->workerLayout->addWidget(progress, i, j);
            }
        }
    }

    ui->yearSpinBox->setValue(QDate::currentDate().year());

    workerTimer_ = new QTimer(this);
    confuseDialog_ = new ConfuseDialog(workerManager_, this);

    QAction *actionAddFiles = new QAction("Файлы", ui->addButton);
    ui->addButton->addAction(actionAddFiles);

    QAction *actionAddFolder = new QAction("Папку", ui->addButton);
    ui->addButton->addAction(actionAddFolder);

    connect(workerTimer_, &QTimer::timeout,
            this, &ImportDialog::workerLoop);
    connect(actionAddFiles, &QAction::triggered,
            this, &ImportDialog::onAddFilesClicked);
    connect(actionAddFolder, &QAction::triggered,
            this, &ImportDialog::onAddFolderClicked);
    connect(ui->importButton, &QPushButton::clicked,
            this, &ImportDialog::onImportButtonClicked);
    connect(ui->stopButton, &QPushButton::clicked,
            this, &ImportDialog::onStopButtonClicked);
    connect(ui->setsButton, &QPushButton::clicked,
            this, &ImportDialog::onSetsButtonClicked);
    connect(ui->transitionButton, &QPushButton::clicked,
            this, &ImportDialog::onTransitionButtonClicked);
}

ImportDialog::~ImportDialog()
{
    delete ui;
}

void ImportDialog::onImportButtonClicked()
{
    auto files = scheduleFiles();
    if (files.isEmpty()) {
        QMessageBox::information(this,
                                 "Список пуст",
                                 "Нет файлов расписаний для работы");
        return;
    }

    workerManager_->setSchedules(files);
    workerManager_->setYear(ui->yearSpinBox->value());
    workerManager_->setDpi(ui->dpiSpinBox->value());

    if (!isWorkPooplerAndTesseract()) {
        return;
    }

    if (workers_.isEmpty()) {
        for (int i = 0; i < workerProgressBars_.size(); ++i) {
            QScopedPointer<ParseWorker> worker(new ParseWorker(workerManager_, i));
            workerPool_.tryStart(worker.data());
            workers_.append(worker.take());
        }
    } else {
        for (auto &worker : workers_) {
            workerPool_.tryStart(worker);
        }
    }

    enableUI(false);

    ui->mainProgressBar->setRange(0, files.size());
    ui->mainProgressBar->setFormat("%v/%m");

    workerTimer_->start(500);
}

void ImportDialog::onStopButtonClicked()
{
    workerPool_.clear();
    enableUI(true);
}

void ImportDialog::onAddFilesClicked()
{
    auto paths = QFileDialog::getOpenFileNames(this,
                                               "Выбор файлов расписания",
                                               QDir::homePath(),
                                               "PDF file (*.pdf) ;; All files (*.*)");
    if (paths.isEmpty()) {
        return;
    }

    updateFilesList(paths);
}

void ImportDialog::onAddFolderClicked()
{
    auto path = QFileDialog::getExistingDirectory(this,
                                                  "Выбор папки с расписаниями",
                                                  QDir::homePath());
    if (path.isEmpty()) {
        return;
    }

    auto paths = recursiveFind(path);
    updateFilesList(paths);
}

void ImportDialog::onTransitionButtonClicked()
{
    auto dialog = new TransitionDialog(workerManager_, this);
    dialog->show();
}

void ImportDialog::onSetsButtonClicked()
{
    auto dialog = new SetsDialog(workerManager_, this);
    dialog->show();
}

bool ImportDialog::isWorkPooplerAndTesseract()
{
    auto messagePoopler = PopplerWrapper(0, workerManager_->dpi())
            .checkPoopler();
    if (!messagePoopler.isEmpty()) {
        QMessageBox::critical(this, "Ошибка Poopler", messagePoopler);
        return false;
    }

    auto messageTesseract = TesseractWrapper()
            .checkTesseract();
    if (!messageTesseract.isEmpty()) {
        QMessageBox::critical(this, "Ошибка Tesseract", messageTesseract);
        return false;
    }

    return true;
}

QStringList ImportDialog::recursiveFind(QString rootDir) const
{
    QStringList paths;
    QDirIterator it(rootDir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        auto path = it.next();
        if (path.endsWith(".pdf")) {
            paths << path;
        }
    }
    return paths;
}

QStringList ImportDialog::scheduleFiles() const
{
    QStringList files;
    for (int i = 0; i < ui->filesList->count(); ++i) {
        files << ui->filesList->item(i)->data(PATH_ROLE).toString();
    }
    return files;
}

void ImportDialog::updateFilesList(const QStringList &paths)
{
    ui->filesList->clear();
    QFileIconProvider iconProvider;
    for (auto &path : paths) {
        QFileInfo file(path);
        auto item = new QListWidgetItem(ui->filesList);
        item->setText(file.baseName());
        item->setData(PATH_ROLE, path);
        item->setToolTip(path);
        item->setIcon(iconProvider.icon(file));
    }

    ui->filesLabel->setText("Список расписаний "
                            "(" + QString::number(ui->filesList->count()) + ")");
}

void ImportDialog::enableUI(bool enable)
{
    ui->addButton->setEnabled(enable);
    ui->yearSpinBox->setEnabled(enable);
    ui->dpiSpinBox->setEnabled(enable);
    ui->importButton->setEnabled(enable);
    ui->cancelButton->setEnabled(enable);
}

void ImportDialog::workerLoop()
{
    // была решена задача
    if (confuseDialog_->status() == ConfuseStatus::Solved) {
        int id = confuseDialog_->info().id;
        workerManager_->setSolve(id, confuseDialog_->solve());
    }

    // получение актуальной информации.
    auto texts = workerManager_->progressTexts();
    auto values = workerManager_->progressValues();
    auto statuses = workerManager_->workerStatuses();
    auto info = workerManager_->confuseInfo();
    auto progress = workerManager_->currentProgress();

    int workComplite = 0;
    // обход потоков
    for (int i = 0; i < workerProgressBars_.size(); ++i) {
        workerProgressBars_[i]->setFormat(texts[i]);
        workerProgressBars_[i]->setValue(values[i]);

        if (statuses[i] == WorkerStatus::Complete) {
            workerProgressBars_[i]->setFormat("Завершено");
            ++workComplite;
        } else if (statuses[i] == WorkerStatus::Confused &&
                   confuseDialog_->status() == ConfuseStatus::Wait) {
            confuseDialog_->start(info[i]);
        }
    }

    ui->mainProgressBar->setValue(progress.current + (workComplite - 4));

    const auto cmp = [](const WorkerStatus &status) {
        return status != WorkerStatus::Complete || status != WorkerStatus::Error;
    };

    if (std::any_of(statuses.begin(), statuses.end(), cmp)) {
        workerTimer_->start(500);
    } else {
        enableUI(true);
        ui->mainProgressBar->setFormat("Завершено");
    }
}
