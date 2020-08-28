#ifndef CONFUSEDIALOG_H
#define CONFUSEDIALOG_H

#include <QtWidgets>

namespace Ui {
class ConfuseDialog;
}

struct ConfuseInfo {
    int id;
    int cellNumber;
    QString type;
    QString maybe;
    QString confuse;
    QString context;
    QString imagePath;
};

enum ConfuseStatus {
    Wait,
    Work,
    Solved
};

class ConfuseDialog : public QDialog
{
    Q_OBJECT

public:

    ConfuseDialog(QWidget *parent = nullptr);
    ~ConfuseDialog();

    void start(const ConfuseInfo &info);
    QString solve();

    ConfuseStatus status() const;
    ConfuseInfo info() const;

public slots:
    void onOkButtonClicked();
    void onZoomInClicked();
    void onZoomOutClicked();

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

    ConfuseInfo info_;
    ConfuseStatus status_;
};

#endif // CONFUSEDIALOG_H
