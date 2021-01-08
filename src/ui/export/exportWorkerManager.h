#ifndef EXPORTWORKERMANAGER_H
#define EXPORTWORKERMANAGER_H

#include "stdafx.h"
#include <QProgressDialog>


class ExportWorkerManager : public QObject
{
    Q_OBJECT

public:
    explicit ExportWorkerManager(int workerCount);

    QString nextSchedulePath();
    void setSchedules(QStringList &schedules);

    int currentCount() const;
    int totalCount() const;

    void addCompleteCount();
    void addWorkDone();
    bool isDone() const;

    bool stopExport() const;
    void setStopExport(bool stopExport);

public slots:
    void onCancelWork();

signals:
    void changedProgress(int progress);
    void workComplete();

private:
    bool stopExport_;
    int workerCount_;
    int currentCount_;
    int totalCount_;
    int runningWorkerCount_;

    QStringList schedules_;

    std::mutex mutex_;
};

#endif // EXPORTWORKERMANAGER_H
