#ifndef TESSERACTWRAPPER_H
#define TESSERACTWRAPPER_H

#include <QtWidgets>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

class TesseractWrapper
{
public:
    TesseractWrapper(const QString &program);
    ~TesseractWrapper();

    QString imageToString(const QString &imagePath) const;
    QString checkTesseract() const;

private:
    std::unique_ptr<tesseract::TessBaseAPI> tesseract_;

//    QString program_;
//    QProcess *tesseract_;
};

#endif // TESSERACTWRAPPER_H
