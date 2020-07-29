#ifndef DATEITEM_H
#define DATEITEM_H

#include <QtWidgets>

#include "frequency.h"
#include "dayOfWeek.h"


class DateItem
{
public:
    static std::unique_ptr<DateItem> fromJson(const QJsonObject &item);

    DateItem(const Frequency &frequency);
    virtual ~DateItem() = default;

    virtual QJsonObject toJson() const = 0;
    virtual QString toString() const = 0;
    virtual DayOfWeek dayOfWeek() const = 0;
    virtual bool contains(const DateItem* item) const = 0;
    virtual bool before(const DateItem* item) const = 0;
    virtual std::unique_ptr<DateItem> copy() const = 0;
    Frequency frequency() const;

public:
    static QString FULL_FORMAT;
    static QString PRINT_FORMAT;

protected:
    Frequency frequency_;
};

#endif // DATEITEM_H
