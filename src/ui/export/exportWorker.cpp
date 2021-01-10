#include "exportWorker.h"

#include "exporter.h"
#include "weeklyExporter.h"


ExportWorker::ExportWorker(const QSharedPointer<ExportWorkerManager> &manager)
    : QRunnable(),
      manager_(manager)
{
}

void ExportWorker::run()
{
    WeeklyExporter exporter;
    try {
        while (true) {
            const QString path = manager_->nextSchedulePath();
            qInfo() << "Export: " + path;

            if (path.isEmpty()) {
                break;
            }

            QPrinter printer(QPrinter::HighResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setPageOrientation(QPageLayout::Landscape);
            printer.setOutputFileName(path.chopped(4) + "pdf");

            exporter.setSchedule(path);
            exporter.runExport(printer);

            if (manager_->stopExport()) {
                break;
            }
            manager_->addCompleteCount();
        }
    } catch (std::exception &e) {
        qCritical() << e.what();
        manager_->addWorkDone();
    }
}
