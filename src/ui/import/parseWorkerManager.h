#ifndef PARSEWORKERMANAGER_H
#define PARSEWORKERMANAGER_H

#include <QtWidgets>
#include <shared_mutex>

enum class WorkerStatus {
    Stay,
    Running,
    Confused,
    ConfuseSolved,
    Complete,
    Error
};

struct ValidData {
    QString data;
    bool valid;
};

struct ConfuseInfo {
    int id;
    int cellNumber;
    QString type;
    QString maybe;
    QString confuse;
    QString context;
    QString imagePath;
};

struct ProgressData {
    int current;
    int total;
};

class ParseWorkerManager
{
public:

    ParseWorkerManager(int workerCount = 0);

    QString nextSchedule();
    ProgressData currentProgress() const;

    ValidData hasTitle(const QString &title) const;
    ValidData hasLecturer(const QString &lecturer) const;
    ValidData hasClassroom(const QString &classroom) const;

    const QVector<QString>& titles() const;
    const QVector<QString>& lecturers() const;
    const QVector<QString>& classrooms() const;

    void updateSets(QVector<QString> &titles,
                    QVector<QString> &lecturers,
                    QVector<QString> &classrooms);

    const QMap<QString, QString>& transitions() const;
    void transitionData(QString &data) const;
    void updateTransition(QMap<QString, QString> &transitions);

    QVector<QString> progressTexts() const;
    QVector<int> progressValues() const;
    QVector<WorkerStatus> workerStatuses() const;
    QVector<ConfuseInfo> confuseInfo() const;

    WorkerStatus status(int id) const;

    QString solve(int id);
    void setSolve(int id, const QString &data);

    void setProgressText(int id, const QString &scheduleName);
    void setProgressValue(int id, int value);
    void setWorkerStatus(int id, const WorkerStatus status);
    void setConfuseInfo(int id, const ConfuseInfo &info);

    void setSchedules(const QStringList &schedules);

    QString pooplerPath() const;
    void setPooplerPath(const QString &pooplerPath);

    QString tesseractPath() const;
    void setTesseractPath(const QString &tesseractPath);

    int dpi() const;
    void setDpi(int dpi);

private:
    void readData(const QString &filePath, QVector<QString> &data) const;
    void readData(const QString &filePath, QMap<QString, QString> &data) const;

    void saveData(const QString &filePath, const QVector<QString> &data) const;
    void saveData(const QString &filePath, const QMap<QString, QString> &data) const;

    double compare(const QString &first, const QString &second) const;
    ValidData hasValid(const QString &value, const QVector<QString> &values) const;

private:
    int workerCount_;

    QVector<QString> titles_;
    QVector<QString> lecturers_;
    QVector<QString> classrooms_;

    QMap<QString, QString> transitions_;

    QMap<int, QString> progressText_;
    QMap<int, int> progressValue_;
    QMap<int, WorkerStatus> workerStatuses_;
    QMap<int, ConfuseInfo> confuseInfo_;

    QMap<int, QString> solves_;

    QStringList schedules_;
    int currentCount_;
    int totalCount_;

    QString pooplerPath_;
    QString tesseractPath_;
    int dpi_;

    mutable std::mutex mutex_;
    mutable std::shared_mutex sharedMutex_;
};

#endif // PARSEWORKERMANAGER_H
