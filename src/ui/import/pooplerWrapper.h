#ifndef POOPLERWRAPPER_H
#define POOPLERWRAPPER_H

#include "stdafx.h"

/**
 * @brief Класс для работы с poopler (pdftoppm).
*/
class PooplerWrapper
{
public:
    /**
     * @brief Конструктор класса.
     * @param program путь к внешней программе.
     * @param dpi значение DPI для конвертации.
    */
    PooplerWrapper(const QString &program, int dpi);
    ~PooplerWrapper();

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
    //! путь к внешней программе
    QString program_;
    //! процесс poopler'a
    QProcess* poopler_;
    //! значение DPI для конвертации
    int dpi_;
};

#endif // POOPLERWRAPPER_H
