#include "parseWorkerManager.h"

ParseWorkerManager::ParseWorkerManager(int workerCount)
    : workerCount_(workerCount)
{
    readData("titles.txt", titles_);
    readData("lecturers.txt", lecturers_);
    readData("classrooms.txt", classrooms_);
}

QString ParseWorkerManager::nextSchedule()
{
    QMutexLocker locker(&mutex_);
    if (!schedules_.isEmpty()) {
        return schedules_.takeFirst();
    }
    return "";
}

bool ParseWorkerManager::hasTitle(const QString &title) const
{
    return titles_.contains(title);
}

bool ParseWorkerManager::hasLecturer(const QString &lecturer) const
{
    return lecturers_.contains(lecturer);
}

bool ParseWorkerManager::hasClassroom(const QString &classroom) const
{
    return classrooms_.contains(classroom);
}

QVector<QString> ParseWorkerManager::progressTexts() const
{
    auto map = progressText_;
    QVector<QString> texts;
    for (int i = 0; i < workerCount_; ++i) {
        texts.append(map[i]);
    }
    return texts;
}

QVector<int> ParseWorkerManager::progressValues() const
{
    auto map = progressValue_;
    QVector<int> values;
    for (int i = 0; i < workerCount_; ++i) {
        values.append(map[i]);
    }
    return values;
}

QVector<WorkerStatus> ParseWorkerManager::workerStatuses() const
{
    auto map = workerStatuses_;
    QVector<WorkerStatus> statuses;
    for (int i = 0; i < workerCount_; ++i) {
        statuses.append(map[i]);
    }
    return statuses;
}

void ParseWorkerManager::setProgressText(int id, const QString &scheduleName)
{
    progressText_[id] = scheduleName + " (%p%)";
}

void ParseWorkerManager::setProgressValue(int id, int value)
{
    progressValue_[id] = value;
}

void ParseWorkerManager::setWorkerStatus(int id, WorkerStatus status)
{
    workerStatuses_[id] = status;
}

void ParseWorkerManager::setSchedules(const QStringList &schedules)
{
    schedules_ = schedules;
}

QString ParseWorkerManager::pooplerPath() const
{
    return pooplerPath_;
}

void ParseWorkerManager::setPooplerPath(const QString &pooplerPath)
{
    pooplerPath_ = pooplerPath;
}

QString ParseWorkerManager::tesseractPath() const
{
    return tesseractPath_;
}

void ParseWorkerManager::setTesseractPath(const QString &tesseractPath)
{
    tesseractPath_ = tesseractPath;
}

int ParseWorkerManager::dpi() const
{
    return dpi_;
}

void ParseWorkerManager::setDpi(int dpi)
{
    dpi_ = dpi;
}

void ParseWorkerManager::readData(const QString &filePath, QSet<QString> &data)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        auto line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            data << line;
        }
    }
}
