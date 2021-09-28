#ifndef PARSEFILEEXCEPTION_H
#define PARSEFILEEXCEPTION_H

#include "stdafx.h"
#include "parseWorkerManager.h"

/**
 * @brief Класс ошибки при парсинге пары из расписания.
*/
class ParseFileException : public QException
{
public:
    /**
     * @brief Конструктор ошибки.
     * @param type тип ошибки.
     * @param maybe возможное решение.
     * @param confuse причина возникновения.
    */
    ParseFileException(const ConfuseType &type,
                       const QString &maybe,
                       const QString &confuse);

    const char *what() const noexcept override;

    /**
     * @brief Возвращает представление ошибки в виде текста.
    */
    QString toString() const;

    /**
     * @brief Возвращает тип ошибки.
    */
    ConfuseType type() const;

    /**
     * @brief Возвращает возможное решение.
    */
    QString maybe() const;
    /** 
     * @brief Возвращает причину ошибки.
    */
    QString confuse() const;

private:
    //! тип ошибки
    ConfuseType type_;
    //! возможное решение
    QString maybe_;
    //! причина возникновения
    QString confuse_;
};

#endif // PARSEFILEEXCEPTION_H
