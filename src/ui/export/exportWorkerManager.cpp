#include "exportWorkerManager.h"

ExportWorkerManager::ExportWorkerManager(int workerCount)
    : QObject(),
      stopExport_(false),
      workerCount_(workerCount),
      currentCount_(-1),
      totalCount_(-1),
      runningWorkerCount_(0)
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
    runningWorkerCount_ = workerCount_;
    totalCount_ = schedules_.size();
    stopExport_ = false;
}

int ExportWorkerManager::currentCount() const
{
    return currentCount_;
}

int ExportWorkerManager::totalCount() const
{
    return totalCount_;
}

void ExportWorkerManager::addCompleteCount()
{
    std::unique_lock<std::mutex> lock(mutex_);
    ++currentCount_;

    emit changedProgress(currentCount_);
}

void ExportWorkerManager::addWorkDone()
{
    std::unique_lock<std::mutex> lock(mutex_);
    --runningWorkerCount_;

    if (runningWorkerCount_ <= 0) {
        emit workComplete();
    }
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

void ExportWorkerManager::onCancelWork()
{
    stopExport_ = true;
}

