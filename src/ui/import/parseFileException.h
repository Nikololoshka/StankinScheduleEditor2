#ifndef PARSEFILEEXCEPTION_H
#define PARSEFILEEXCEPTION_H

#include "stdafx.h"

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
    ParseFileException(const QString &type,
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
    QString type() const;
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
    QString type_;
    //! возможное решение
    QString maybe_;
    //! причина возникновения
    QString confuse_;
};

#endif // PARSEFILEEXCEPTION_H
