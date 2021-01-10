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

    QFont font() const;
    void setFont(const QFont &font);

    bool showDate() const;
    void setShowDate(bool showDate);

    QDate startDate() const;
    void setStartDate(const QDate &startDate);

    QDate endDate() const;
    void setEndDate(const QDate &endDate);

    QColor colorSubgroupA() const;
    void setColorSubgroupA(const QColor &colorSubgroupA);

    QColor colorSubgroupB() const;
    void setColorSubgroupB(const QColor &colorSubgroupB);

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
    QFont font_;
    bool showDate_;

    QDate startDate_;
    QDate endDate_;
    QColor colorSubgroupA_;
    QColor colorSubgroupB_;

    std::mutex mutex_;
};

#endif // EXPORTWORKERMANAGER_H
