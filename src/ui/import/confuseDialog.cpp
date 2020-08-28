#include "confuseDialog.h"
#include "ui_confuseDialog.h"

#include "parseHighlighter.h"

ConfuseDialog::ConfuseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfuseDialog),
    scaleFactor_(1),
    previewImage_(new QLabel()),
    scrollArea_(new QScrollArea())
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

    previewImage_->setPixmap(QPixmap(info.imagePath));
    previewImage_->resize(previewImage_->pixmap()->size() / 12);
    scaleFactor_ = 1.0;

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

void ConfuseDialog::onZoomInClicked()
{
    scaleImage(1.25);
}

void ConfuseDialog::onZoomOutClicked()
{
    scaleImage(0.75);
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
    previewImage_->resize(scaleFactor_ * previewImage_->pixmap()->size());

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
