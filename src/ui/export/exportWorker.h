#ifndef EXPORTWORKER_H
#define EXPORTWORKER_H

#include "stdafx.h"

#include "exporter.h"
#include "exportWorkerManager.h"


class ExportWorker : public QRunnable
{
public:
    ExportWorker(const QSharedPointer<ExportWorkerManager> &manager);
    void run() override;

private:
    QSharedPointer<ExportWorkerManager> manager_;
};

#endif // EXPORTWORKER_H
