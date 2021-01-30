#ifndef POOPLERWRAPPER_H
#define POOPLERWRAPPER_H

#include "stdafx.h"

#include <poppler/cpp/poppler-page-renderer.h>

/**
 * @brief Класс для работы с poopler (pdftoppm).
*/
class PopplerWrapper
{
public:
    /**
     * @brief Конструктор класса.
     * @param program путь к внешней программе.
     * @param dpi значение DPI для конвертации.
    */
    PopplerWrapper(const int id, int dpi);

    /**
     * @brief Конвертирует PDF файл в JPEG файл
     * @param pdfPath путь в PDF файлу.
     * @return путь к JPEG файлу.
    */
    QString print(const QString pdfPath) const;
    /**
     * @brief Проверяет доступность poopler'a.
     * @return если есть доступ к poopler, то возвращаеться пустая строка. 
     *         Иначе текст с описанием ошибки.
    */
    QString checkPoopler() const;

public:
    //! название промежуточного изображения
    QString name_;
    //! объект для рендеринга pdf файла
    poppler::page_renderer render_;
    //! значение DPI для конвертации
    int dpi_;
};

#endif // POOPLERWRAPPER_H
