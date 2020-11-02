#ifndef DAYOFWEEK_H
#define DAYOFWEEK_H

#include "stdafx.h"

/**
 * @brief Перечисление с днями недели в расписании.
 */
enum class DayOfWeek {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

namespace DateUtils {
    /**
     * @brief Возвращает день недели из даты.
     * @param date дата.
     * @throw std::invalid_argument если не поддерживаемый день недели.
     */
    DayOfWeek of(const QDate& date);
    /**
     * @brief Возвращает день недели из номер дня в недели
     * @param number номер для в недели (от 0 по 5).
     * @throw std::invalid_argument если не поддерживаемый день недели.
     */
    DayOfWeek of(int number);
    /**
     * @brief Возвращает список возможных дней недели.
     */
    QVector<DayOfWeek> list();
    /**
     * @brief Возвращает список полных строк (понидельник, вторник и т.д.) дней недели.
     */
    QStringList dayOfWeekList();
    /**
     * @brief Возвращает список сокращенных строк (ПН, ВТ и т.д.) дней недели.
     */
    QStringList dayOfWeekMiniList();
    /**
     * @brief Возвращает полную строку, соответсвующую дню недели
     */
    QString toString(DayOfWeek dayOfWeek);
}

#endif // DAYOFWEEK_H
