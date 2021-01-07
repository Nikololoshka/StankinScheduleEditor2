#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "stdafx.h"
#include "exportWorkerManager.h"


enum class PrintType
{
    PDF,
    Printer
};


namespace Ui {
class exportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    ~ExportDialog() override;

    const static int PATH_ROLE = Qt::UserRole + 1;

public slots:
    void onAddFilesClicked();
    void onAddFolderClicked();
    void onExportButtonClicked();
    void onStartDateChanged(const QDate &newDate);
    void onEndDateChanged(const QDate &newDate);

private:
    void initMainSetting();
    void initPrintSetting();
    void initDateSetting();
    void initSubgroupSetting();

    void updateScheduleList(const QStringList &paths);
    void updateCurrentScheduleCount(int count) const;
    void updateDateCount() const;

    QIcon createColorIcon(const QColor &color);
    QStringList recursiveFind(QString rootDir) const;

private:
    Ui::exportDialog *ui;

    QThreadPool workerPool_;
    QSharedPointer<ExportWorkerManager> workManager_;
};

#endif // EXPORTDIALOG_H
