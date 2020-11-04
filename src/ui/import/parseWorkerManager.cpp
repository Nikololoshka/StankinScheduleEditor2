#include "stdafx.h"
#include "parseWorkerManager.h"

#include "difflib.h"

ParseWorkerManager::ParseWorkerManager(int workerCount)
    : workerCount_(workerCount)
{
    readData("sets/titles.txt", titles_);
    readData("sets/lecturers.txt", lecturers_);
    readData("sets/classrooms.txt", classrooms_);
    readData("transition/transition.txt", transitions_);
}

QString ParseWorkerManager::nextSchedule()
{
    std::unique_lock lock(mutex_);

    if (!schedules_.isEmpty()) {
        ++currentCount_;
        return schedules_.takeFirst();
    }
    return "";
}

ProgressData ParseWorkerManager::currentProgress() const
{
    return { currentCount_, totalCount_ };
}

ValidData ParseWorkerManager::hasTitle(const QString &title) const
{
    return hasValid(title, titles_);
}

ValidData ParseWorkerManager::hasLecturer(const QString &lecturer) const
{
    return hasValid(lecturer, lecturers_);
}

ValidData ParseWorkerManager::hasClassroom(const QString &classroom) const
{
    return hasValid(classroom, classrooms_);
}

const QVector<QString>& ParseWorkerManager::classrooms() const
{
    return classrooms_;
}

const QVector<QString>& ParseWorkerManager::lecturers() const
{
    return lecturers_;
}

const QVector<QString>& ParseWorkerManager::titles() const
{
    return titles_;
}

void ParseWorkerManager::updateSets(QVector<QString> &titles, QVector<QString> &lecturers, QVector<QString> &classrooms)
{
    std::unique_lock lock(sharedMutex_);

    titles_.swap(titles);
    lecturers_.swap(lecturers);
    classrooms_.swap(classrooms);

    saveData("sets/titles.txt", titles_);
    saveData("sets/lecturers.txt", lecturers_);
    saveData("sets/classrooms.txt", classrooms_);
}

const QMap<QString, QString>& ParseWorkerManager::transitions() const
{
    std::shared_lock lock(sharedMutex_);

    return transitions_;
}

void ParseWorkerManager::transitionData(QString &data) const
{
    std::shared_lock lock(sharedMutex_);
    for (auto it = transitions_.begin(); it != transitions_.end(); ++it) {
        data.replace(it.key(), it.value());
    }
}

void ParseWorkerManager::updateTransition(QMap<QString, QString> &transitions)
{
    std::unique_lock lock(sharedMutex_);

    transitions_.swap(transitions);
    saveData("transition/transition.txt", transitions_);
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

QVector<ConfuseInfo> ParseWorkerManager::confuseInfo() const
{
    auto map = confuseInfo_;
    QVector<ConfuseInfo> info;
    for (int i = 0; i < workerCount_; ++i) {
        info.append(map[i]);
    }
    return info;
}

WorkerStatus ParseWorkerManager::status(int id) const
{
    return workerStatuses_[id];
}

QString ParseWorkerManager::solve(int id)
{
    workerStatuses_[id] = WorkerStatus::Running;
    return solves_[id];
}

void ParseWorkerManager::setSolve(int id, const QString &data)
{
    solves_[id] = data;
    workerStatuses_[id] = WorkerStatus::ConfuseSolved;
}

void ParseWorkerManager::setProgressText(int id, const QString &scheduleName)
{
    progressText_[id] = scheduleName + " (%p%)";
}

void ParseWorkerManager::setProgressValue(int id, int value)
{
    progressValue_[id] = value;
}

void ParseWorkerManager::setWorkerStatus(int id, const WorkerStatus status)
{
    workerStatuses_[id] = status;
}

void ParseWorkerManager::setConfuseInfo(int id, const ConfuseInfo &info)
{
    confuseInfo_[id] = info;
    workerStatuses_[id] = WorkerStatus::Confused;
}

void ParseWorkerManager::setSchedules(const QStringList &schedules)
{
    schedules_ = schedules;
    currentCount_ = 0;
    totalCount_ = schedules_.size();
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

void ParseWorkerManager::readData(const QString &filePath, QVector<QString> &data) const
{
    QFile file(qApp->applicationDirPath() + "/" + filePath);    
    if (!file.exists()) {
        QFileInfo info(file);
        QDir().mkdir(info.absolutePath());
        QFile::copy(":/" + filePath, qApp->applicationDirPath() + "/" + filePath);
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        auto line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            data << line;
        }
    }
}

void ParseWorkerManager::readData(const QString &filePath, QMap<QString, QString> &data) const
{
    QFile file(qApp->applicationDirPath() + "/" + filePath);
    if (!file.exists()) {
        QFileInfo info(file);
        QDir().mkdir(info.absolutePath());
        QFile::copy(":/" + filePath, qApp->applicationDirPath() + "/" + filePath);
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        auto line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            auto rule = line.split("->");
            auto source = rule[0].trimmed();
            auto translate = rule[1].trimmed();
            data[source] = translate;
        }
    }
}

void ParseWorkerManager::saveData(const QString &filePath, const QVector<QString> &data) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    for (const auto& value : data) {
        out << value << Qt::endl;
    }
}

void ParseWorkerManager::saveData(const QString &filePath, const QMap<QString, QString> &data) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    for (auto it = data.begin(); it != data.end(); ++it) {
        out << it.key() << " -> " << it.value() << Qt::endl;
    }
}

double ParseWorkerManager::compare(const QString &first, const QString &second) const
{
    return difflib::SequenceMatcher<std::string>(first.toStdString(), second.toStdString()).ratio();
}

ValidData ParseWorkerManager::hasValid(const QString &value, const QVector<QString> &values) const
{
    std::shared_lock lock(sharedMutex_);

    QString maybe = "";
    double score = 0.0;

    for (auto& v : values) {
        if (v == value) {
            return { value, true };
        }
        double newScore = compare(v, value);
        if (newScore > score) {
            score = newScore;
            maybe = v;
        }
    }

    if (score < 0.95) {
        return { maybe, false };
    }

    return  { maybe, true };
}

