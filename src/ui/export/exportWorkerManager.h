#ifndef EXPORTWORKERMANAGER_H
#define EXPORTWORKERMANAGER_H

#include "stdafx.h"
#include <QProgressDialog>


class ExportWorkerManager : public QObject
{
    Q_OBJECT

public:
    ExportWorkerManager();

    QString nextSchedulePath();
    void setSchedules(QStringList &schedules);

    int currentCount() const;
    int totalCount() const;

    void addCompleteCount();
    bool isDone() const;

    bool stopExport() const;
    void setStopExport(bool stopExport);

signals:
    void changedProgress(int progress);
    void workComplete();

private:
    bool stopExport_;
    int currentCount_;
    int totalCount_;

    QStringList schedules_;

    std::mutex mutex_;
};

#endif // EXPORTWORKERMANAGER_H
