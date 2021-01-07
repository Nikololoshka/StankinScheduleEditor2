#include "exportWorker.h"

ExportWorker::ExportWorker(const QSharedPointer<ExportWorkerManager> &manager)
    : QRunnable(),
      manager_(manager)
{
}

void ExportWorker::run()
{
    qDebug() << "start";
    while (true) {
        QString path = manager_->nextSchedulePath();
        qDebug() << path;

        if (path.isEmpty()) {
            break;
        }

        QThread::sleep(1);
        manager_->addCompliteCount();
    }
}
