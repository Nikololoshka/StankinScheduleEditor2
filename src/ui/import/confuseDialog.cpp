#include "confuseDialog.h"
#include "ui_confuseDialog.h"

#include <QGuiApplication>

#include "parseHighlighter.h"
#include "transitionDialog.h"
#include "setsDialog.h"

ConfuseDialog::ConfuseDialog(const QSharedPointer<ParseWorkerManager> &workerManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfuseDialog),
    scaleFactor_(1),
    previewImage_(new QLabel()),
    scrollArea_(new QScrollArea()),
    workerManager_(workerManager)
{
    ui->setupUi(this);
    resize(qApp->primaryScreen()->size() * 3 / 5);

    previewImage_->setBackgroundRole(QPalette::Base);
    previewImage_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    previewImage_->setScaledContents(true);

    scrollArea_->setBackgroundRole(QPalette::Dark);
    scrollArea_->setWidget(previewImage_);
    ui->verticalLayout_2->insertWidget(1, scrollArea_);

    new ParseHighlighter(ui->solveEdit->document());
    status_ = ConfuseStatus::Wait;

    connect(ui->zoomInButton, &QPushButton::clicked,
            this, &ConfuseDialog::onZoomInClicked);
    connect(ui->zoomOutButton, &QPushButton::clicked,
            this, &ConfuseDialog::onZoomOutClicked);
    connect(ui->okButton, &QPushButton::clicked,
            this, &ConfuseDialog::onOkButtonClicked);
    connect(ui->retryButton, &QPushButton::clicked,
            this, &ConfuseDialog::onRetryButtonClicked);
    connect(ui->contextButton, &QPushButton::clicked,
            this, &ConfuseDialog::onContextButtonClicked);
    connect(ui->setsButton, &QPushButton::clicked,
            this, &ConfuseDialog::onSetsButtonClicked);
    connect(ui->transitionButton, &QPushButton::clicked,
            this, &ConfuseDialog::onTransitionButtonClicked);
}

ConfuseDialog::~ConfuseDialog()
{
    delete ui;
}

void ConfuseDialog::start(const ConfuseInfo &info)
{
    ui->confuseEdit->setPlainText(
                "Поток: " + QString::number(info.id) + "\n" +
                "Ячейка: " + QString::number(info.cellNumber) + "\n\n" +
                "Тип: " + info.type + "\n" +
                "Возможное решение: " + info.maybe + "\n\n" +
                "Причина: " + info.confuse + "\n\n"
                "Контекст: " + info.context);

    QString context = info.context;
    if (!info.maybe.isEmpty() && info.maybe != "null") {
        context.replace(info.confuse, info.maybe);
    }
    ui->solveEdit->setPlainText(context);

    ui->retryButton->setToolTip(info.context);

    previewImage_->setPixmap(QPixmap(info.imagePath));
    auto img = previewImage_->pixmap(Qt::ReturnByValue);
    previewImage_->resize(img.size() / 12);
    scaleFactor_ = 1.0;

    onZoomInClicked();
    onZoomOutClicked();
    onZoomOutClicked();
    onZoomOutClicked();
    onZoomOutClicked();
    onZoomOutClicked();

    info_ = info;
    status_ = ConfuseStatus::Work;
    show();
}

QString ConfuseDialog::solve()
{
    status_ = ConfuseStatus::Wait;
    return ui->solveEdit->toPlainText();
}

ConfuseStatus ConfuseDialog::status() const
{
    return status_;
}

ConfuseInfo ConfuseDialog::info() const
{
    return info_;
}

void ConfuseDialog::onOkButtonClicked()
{
    status_ = ConfuseStatus::Solved;
    close();
}

void ConfuseDialog::onRetryButtonClicked()
{
    ui->solveEdit->setPlainText(info_.context);
    onOkButtonClicked();
}

void ConfuseDialog::onContextButtonClicked()
{
    ui->solveEdit->setPlainText(info_.context);
}

void ConfuseDialog::onZoomInClicked()
{
    scaleImage(1.25);
}

void ConfuseDialog::onZoomOutClicked()
{
    scaleImage(0.75);
}

void ConfuseDialog::onTransitionButtonClicked()
{
    auto dialog = new TransitionDialog(workerManager_, this);
    dialog->show();
}

void ConfuseDialog::onSetsButtonClicked()
{
    auto dialog = new SetsDialog(workerManager_, this);
    dialog->show();
}

void ConfuseDialog::closeEvent(QCloseEvent *event)
{
    if (status_ != ConfuseStatus::Solved) {
        status_ = ConfuseStatus::Wait;
    }

    QDialog::closeEvent(event);
}

void ConfuseDialog::scaleImage(double factor)
{
    scaleFactor_ *= factor;
    auto img = previewImage_->pixmap(Qt::ReturnByValue);
    previewImage_->resize(scaleFactor_ * img.size());

    adjustScrollBar(scrollArea_->verticalScrollBar(), factor);
    adjustScrollBar(scrollArea_->horizontalScrollBar(), factor);

    ui->zoomInButton->setEnabled(scaleFactor_ < 5.0);
    ui->zoomOutButton->setEnabled(scaleFactor_ > 0.333);
}

void ConfuseDialog::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(static_cast<int>(factor * scrollBar->value()
                                         + ((factor - 1) * scrollBar->pageStep() / 2)));
}
