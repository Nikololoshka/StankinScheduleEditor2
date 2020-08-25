#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QtWidgets>
#include "parseWorker.h"
#include "parseWorkerManager.h"


namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = nullptr);
    ~ImportDialog();

    const static int PATH_ROLE = Qt::UserRole + 1;

public slots:
    void onImportButtonClicked();
    void onStopButtonClicked();

    void onAddFilesClicked();
    void onAddFolderClicked();

private:
    bool isWorkPooplerAndTesseract();
    QStringList recursiveFind(QString rootDir) const;
    QStringList scheduleFiles() const;
    void updateFilesList(const QStringList &paths);
    void enableUI(bool enable);

    void workerLoop();

private:
    Ui::ImportDialog *ui;

    QTimer *workerTimer_;

    QThreadPool workerPool_;
    QVector<ParseWorker*> workers_;
    QVector<QProgressBar*> workerProgressBars_;
    QSharedPointer<ParseWorkerManager> workerManager_;
};

#endif // IMPORTDIALOG_H
