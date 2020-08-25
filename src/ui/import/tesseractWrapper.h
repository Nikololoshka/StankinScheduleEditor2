#ifndef TESSERACTWRAPPER_H
#define TESSERACTWRAPPER_H

#include <QtWidgets>


class TesseractWrapper
{
public:
    TesseractWrapper(const QString &program);
    ~TesseractWrapper();

    QString imageToString(const QString &imagePath) const;
    QString checkTesseract() const;

private:
    QString program_;
    QProcess *tesseract_;
};

#endif // TESSERACTWRAPPER_H
