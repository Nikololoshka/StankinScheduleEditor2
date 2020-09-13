#ifndef DATERANGE_H
#define DATERANGE_H

#include <QtWidgets>

#include "dateItem.h"

/**
 * Единождная дата пары.
 */
class DateSingle;

/**
 * @brief Дата с диапазоном.
 */
class DateRange : public DateItem {

public:
    /**
     * @brief Конструктор создания даты с диапозоном.
     * @param start начало даты.
     * @param end конец даты.
     * @param frequency периодичносьт даты.
     */
    DateRange(const QDate& start, const QDate& end, const Frequency& frequency);
    /**
     * @brief Конструктор даты из json объекта.
     * @param item json объект
     * @param frequency периодичность даты.
     */
    DateRange(const QJsonObject& item, const Frequency& frequency);
    /**
     * @brief Конструкор копирования
     */
    DateRange(const DateRange& date);

    /**
     * @brief Возвращает дату начала диапазона.
     */
    QDate start() const;
    /**
     * @brief Возвращает дату конца диапазона.
     */
    QDate end() const;

    QJsonObject toJson() const override;
    DayOfWeek dayOfWeek() const override;
    bool contains(const DateItem* item) const override;
    bool before(const DateItem* item) const override;
    std::unique_ptr<DateItem> copy() const override;

    QString toString() const override;

    /**
     * @brief Возвращает количество дат в диапозоне.
     */
    qint64 count() const;
    /**
     * @brief Возвращает дату в диапозоне по индексу
     */
    QDate at(const int index) const;
    QDate operator[](const int index) const;

private:
    void init(const QDate& start, const QDate& end, const Frequency& frequency);

private:
    //! дата начала
    QDate start_;
    //! дата конца
    QDate end_;

    friend DateSingle;
};

#endif // DATERANGE_H
