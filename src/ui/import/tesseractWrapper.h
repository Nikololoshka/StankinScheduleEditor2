#ifndef TESSERACTWRAPPER_H
#define TESSERACTWRAPPER_H

#include "stdafx.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

/**
 * @brief Класс для работы с Tesseract.
*/
class TesseractWrapper
{
public:
    /**
     * @brief Конструктор по умолчанию.
    */
    TesseractWrapper();
    ~TesseractWrapper();

    /**
     * @brief Распознает текст с изображения.
     * @param imagePath путь до изображения.
     * @return текст с изображения.
    */
    QString imageToString(const QString &imagePath) const;
    /**
     * @brief Проверяет доступность tesseract'a.
     * @return если есть доступ к tesseract, то возвращаеться пустая строка. 
     *         Иначе текст с описанием ошибки.
    */
    QString checkTesseract() const;

private:
    //! объект для доступа к TessractAPI
    std::unique_ptr<tesseract::TessBaseAPI> tesseract_;
    //! успешна ли инициализация
    bool initSuccess_;
};

#endif // TESSERACTWRAPPER_H
