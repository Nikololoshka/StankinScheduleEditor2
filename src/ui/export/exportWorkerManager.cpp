#include "exportWorkerManager.h"

ExportWorkerManager::ExportWorkerManager()
    : QObject(),
      stopExport_(false),
      currentCount_(-1),
      totalCount_(-1)
{
}

QString ExportWorkerManager::nextSchedulePath()
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (!schedules_.isEmpty()) {
        return schedules_.takeFirst();
    }
    return "";
}

void ExportWorkerManager::setSchedules(QStringList &schedules)
{
    schedules_.swap(schedules);
    currentCount_ = 0;
    totalCount_ = schedules_.size();
}

int ExportWorkerManager::currentCount() const
{
    return currentCount_;
}

int ExportWorkerManager::totalCount() const
{
    return totalCount_;
}

void ExportWorkerManager::addCompliteCount()
{
    std::unique_lock<std::mutex> lock(mutex_);
    ++currentCount_;

    emit changedProgress(currentCount_);
}

bool ExportWorkerManager::isDone() const
{
    return currentCount_ == totalCount_;
}

bool ExportWorkerManager::stopExport() const
{
    return stopExport_;
}

void ExportWorkerManager::setStopExport(bool stopExport)
{
    stopExport_ = stopExport;
}

