#include "tesseractWrapper.h"

TesseractWrapper::TesseractWrapper(const QString &program)
{
    tesseract_ = std::make_unique<tesseract::TessBaseAPI>();
    tesseract_->Init(nullptr, "rus");
}

TesseractWrapper::~TesseractWrapper()
{
    tesseract_->End();
}

QString TesseractWrapper::imageToString(const QString &imagePath) const
{
    auto image = std::unique_ptr<Pix>(pixRead(imagePath.toStdString().c_str()));
    tesseract_->SetImage(image.get());
    return tesseract_->GetUTF8Text();

//    QStringList arguments = {
//        imagePath,
//        "stdout",
//        "-l",
//        "rus"
//    };

//    qDebug() << "Start:" << imagePath;

//    tesseract_->start(program_, arguments, QIODevice::ReadOnly);
//    tesseract_->waitForStarted();
//    tesseract_->waitForFinished();

//    qDebug() << "End:" << imagePath;

//    if (tesseract_->exitStatus() != QProcess::NormalExit) {
//        throw std::logic_error(("Tesseract error: " + imagePath +
//                                "\n"
//                                "Status: " + QString::number(tesseract_->exitStatus()) +
//                                "\n" +
//                                "Error: " + tesseract_->errorString()).toStdString());
//    }



//    return tesseract_->readAllStandardOutput();
}

QString TesseractWrapper::checkTesseract() const
{
    return "";
//    QStringList arguments = {
//        "--help"
//    };

//    QString command = "$" + program_ + " " + arguments.join(' ');
//    try {
//        tesseract_->start(program_, arguments, QIODevice::ReadOnly);
//        tesseract_->waitForStarted();
//        tesseract_->waitForFinished();

//        if (tesseract_->exitStatus() == QProcess::NormalExit) {
//            return "";
//        }

//    } catch (std::exception &e) {
//        return command + "\nException: " + e.what();
//    }

//    return command + "\n" +
//           "Code: " + QString::number(tesseract_->exitCode()) +
//           "\n" +
//           "Error: " + tesseract_->errorString();
}
