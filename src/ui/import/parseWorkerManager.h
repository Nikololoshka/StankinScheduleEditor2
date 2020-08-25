#ifndef PARSEWORKERMANAGER_H
#define PARSEWORKERMANAGER_H

#include <QtWidgets>

enum class WorkerStatus {
    Stay,
    Running,
    Confused,
    Complete,
    Error
};

class ParseWorkerManager
{
public:

    ParseWorkerManager(int workerCount);
    QString nextSchedule();

    bool hasTitle(const QString &title) const;
    bool hasLecturer(const QString &lecturer) const;
    bool hasClassroom(const QString &classroom) const;

    QVector<QString> progressTexts() const;
    QVector<int> progressValues() const;
    QVector<WorkerStatus> workerStatuses() const;

    void setProgressText(int id, const QString &scheduleName);
    void setProgressValue(int id, int value);
    void setWorkerStatus(int id, WorkerStatus status);

    void setSchedules(const QStringList &schedules);

    QString pooplerPath() const;
    void setPooplerPath(const QString &pooplerPath);

    QString tesseractPath() const;
    void setTesseractPath(const QString &tesseractPath);

    int dpi() const;
    void setDpi(int dpi);

private:
    void readData(const QString &filePath, QSet<QString> &data);

private:
    int workerCount_;

    QSet<QString> titles_;
    QSet<QString> lecturers_;
    QSet<QString> classrooms_;

    QMap<int, QString> progressText_;
    QMap<int, int> progressValue_;
    QMap<int, WorkerStatus> workerStatuses_;

    QStringList schedules_;
    QString pooplerPath_;
    QString tesseractPath_;
    int dpi_;

    QMutex mutex_;
};

#endif // PARSEWORKERMANAGER_H
