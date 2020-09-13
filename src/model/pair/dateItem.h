#ifndef DATEITEM_H
#define DATEITEM_H

#include <QtWidgets>

#include "dayOfWeek.h"
#include "frequency.h"

/**
 * @brief Элемент даты в датах пары.
 */
class DateItem {

public:
    /**
     * @brief Возвращает дату из json объекта.
     */
    static std::unique_ptr<DateItem> fromJson(const QJsonObject& item);

    /**
     * @brief Конструктор элемента даты от периодичности.
     */
    DateItem(const Frequency& frequency);
    virtual ~DateItem() = default;

    /**
     * @brief Возвращает элемент даты в виде json объекта.
     */
    virtual QJsonObject toJson() const = 0;
    /**
     * @brief Возвращает строку, соответсвующую дате.
     */
    virtual QString toString() const = 0;
    /**
     * @brief Возвращает день недели даты.
     */
    virtual DayOfWeek dayOfWeek() const = 0;
    /**
     * @brief Определяет, пересекаются ли элементы даты.
     * @param item другая дата.
     */
    virtual bool contains(const DateItem* item) const = 0;
    /**
     * @brief Определяет, идет ли по порядку дата раньше другой.
     * @param item другая дата.
     */
    virtual bool before(const DateItem* item) const = 0;
    /**
     * @brief Возвращает копию элемента даты.
     */
    virtual std::unique_ptr<DateItem> copy() const = 0;
    /**
     * @brief Возвращает периодиность даты.
     */
    Frequency frequency() const;

public:
    /**
     * @brief Формат даты сохраняемой в json.
     */
    static QString FULL_FORMAT;
    /**
     * @brief Формат даты для отображения в расписании.
     */
    static QString PRINT_FORMAT;

protected:
    //! периодичность
    Frequency frequency_;
};

#endif // DATEITEM_H
