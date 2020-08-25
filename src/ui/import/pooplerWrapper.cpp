#include "pooplerWrapper.h"

PooplerWrapper::PooplerWrapper(const QString &program, int dpi)
    : program_(program),
      dpi_(dpi)
{
    poopler_ = new QProcess();
}

PooplerWrapper::~PooplerWrapper()
{
    delete poopler_;
}

QString PooplerWrapper::print(const QString pdfPath) const
{
    QString imagePath = pdfPath.left(pdfPath.size() - QFileInfo(pdfPath).suffix().size() - 1) + "-debug";
    QStringList arguments = {
        "-r",
        QString::number(dpi_),
        "-jpeg",
        "-singlefile",
        pdfPath,
        imagePath
    };

    poopler_->start(program_, arguments, QIODevice::ReadOnly);
    poopler_->waitForStarted();
    poopler_->waitForFinished();

    if (poopler_->exitStatus() != QProcess::NormalExit) {
        throw std::logic_error(("Poopler error: " + pdfPath +
                                "\n" +
                                "Status: " + QString::number(poopler_->exitStatus()) +
                                "\n" +
                                "Error: " + poopler_->errorString()).toStdString());
    }

    return imagePath + ".jpg";
}

QString PooplerWrapper::checkPoopler() const
{
    QStringList arguments = {
        "--help"
    };

    QString command = "$" + program_ + " " + arguments.join(' ');
    try {
        poopler_->start(program_, arguments, QIODevice::ReadOnly);
        poopler_->waitForStarted();
        poopler_->waitForFinished();

        if (poopler_->exitStatus() == QProcess::NormalExit) {
            return "";
        }

    } catch (std::exception &e) {
        return command + "\nException: " + e.what();
    }

    return command + "\n" +
           "Code: " + QString::number(poopler_->exitStatus()) +
           "\n" +
           "Error: " + poopler_->errorString();
}
