#include "tesseractWrapper.h"

TesseractWrapper::TesseractWrapper(const QString &program)
    : program_(program)
{
    tesseract_ = new QProcess();
}

TesseractWrapper::~TesseractWrapper()
{
    delete tesseract_;
}

QString TesseractWrapper::imageToString(const QString &imagePath) const
{
    QStringList arguments = {
        imagePath,
        "stdout",
        "-l",
        "rus"
    };

    tesseract_->start(program_, arguments, QIODevice::ReadOnly);
    tesseract_->waitForStarted();
    tesseract_->waitForFinished();

    if (tesseract_->exitStatus() != QProcess::NormalExit) {
        throw std::logic_error(("Tesseract error: " + imagePath +
                                "\n"
                                "Status: " + QString::number(tesseract_->exitStatus()) +
                                "\n" +
                                "Error: " + tesseract_->errorString()).toStdString());
    }



    return tesseract_->readAllStandardOutput();
}

QString TesseractWrapper::checkTesseract() const
{
    QStringList arguments = {
        "--help"
    };

    QString command = "$" + program_ + " " + arguments.join(' ');
    try {
        tesseract_->start(program_, arguments, QIODevice::ReadOnly);
        tesseract_->waitForStarted();
        tesseract_->waitForFinished();

        if (tesseract_->exitStatus() == QProcess::NormalExit) {
            return "";
        }

    } catch (std::exception &e) {
        return command + "\nException: " + e.what();
    }

    return command + "\n" +
           "Code: " + QString::number(tesseract_->exitCode()) +
           "\n" +
           "Error: " + tesseract_->errorString();
}
