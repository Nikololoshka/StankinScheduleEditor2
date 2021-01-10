#ifndef DATESINGLE_H
#define DATESINGLE_H

#include "stdafx.h"

#include "dateItem.h"

/**
 * Класс даты с диапазоном.
 */
class DateRange;

/**
 * @brief Единождная дата пары.
 */
class DateSingle : public DateItem {

public:
    /**
     * @brief Конструктор из объекта даты.
     */
    DateSingle(const QDate& date);
    /**
     * @brief Конструктор из json объекта с датой.
     */
    DateSingle(const QJsonObject& item);
    /**
     * @brief Конструктор копирования.
     */
    DateSingle(const DateSingle& date);

    /**
     * @brief Возвращает объект даты.
     */
    QDate date() const;

    QJsonObject toJson() const override;
    DayOfWeek dayOfWeek() const override;
    bool contains(const DateItem* item) const override;
    bool intersect(const QDate &startDate, const QDate &end) const override;
    bool before(const DateItem* item) const override;
    std::unique_ptr<DateItem> copy() const override;

    QString toString() const override;

    bool operator==(const QDate& date) const;

private:
    //! дата
    QDate date_;

    friend DateRange;
};

#endif // DATESINGLE_H
