#ifndef TESSERACTWRAPPER_H
#define TESSERACTWRAPPER_H

#include "stdafx.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

/**
 * @brief ����� ��� ������ � Tesseract.
*/
class TesseractWrapper
{
public:
    /**
     * @brief ����������� �� ���������.
    */
    TesseractWrapper();
    ~TesseractWrapper();

    /**
     * @brief ���������� ����� � �����������.
     * @param imagePath ���� �� �����������.
     * @return ����� � �����������.
    */
    QString imageToString(const QString &imagePath) const;
    /**
     * @brief ��������� ����������� tesseract'a.
     * @return ���� ���� ������ � tesseract, �� ������������� ������ ������. 
     *         ����� ����� � ��������� ������.
    */
    QString checkTesseract() const;

private:
    //! ������ ��� ������� � TessractAPI
    std::unique_ptr<tesseract::TessBaseAPI> tesseract_;
    //! ������� �� �������������
    bool initSuccess_;
};

#endif // TESSERACTWRAPPER_H
