#ifndef CONFUSEDIALOG_H
#define CONFUSEDIALOG_H

#include "stdafx.h"
#include "parseWorkerManager.h"

namespace Ui {
class ConfuseDialog;
}

/**
 * @brief The ConfuseStatus enum
 */
enum ConfuseStatus {
    Wait,
    Work,
    Solved
};

class ConfuseDialog : public QDialog
{
    // Q_OBJECT

public:

    ConfuseDialog(const QSharedPointer<ParseWorkerManager> &workerManager, QWidget *parent = nullptr);
    ~ConfuseDialog();

    void start(const ConfuseInfo &info);
    QString solve();

    ConfuseStatus status() const;
    ConfuseInfo info() const;

public slots:
    void onOkButtonClicked();
    void onRetryButtonClicked();
    void onContextButtonClicked();
    void onZoomInClicked();
    void onZoomOutClicked();

    void onTransitionButtonClicked();
    void onSetsButtonClicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

private:
    Ui::ConfuseDialog *ui;

    double scaleFactor_;
    QLabel *previewImage_;
    QScrollArea *scrollArea_;

    QSharedPointer<ParseWorkerManager> workerManager_;

    ConfuseInfo info_;
    ConfuseStatus status_;
};

#endif // CONFUSEDIALOG_H
