#ifndef DATESINGLE_H
#define DATESINGLE_H

#include <QtWidgets>

#include "dateItem.h"

class DateRange;

class DateSingle : public DateItem
{
public:
    DateSingle(const QDate &date);
    DateSingle(const QJsonObject &item);
    DateSingle(const DateSingle &date);

    QDate date() const;

    QJsonObject toJson() const override;
    DayOfWeek dayOfWeek() const override;
    bool contains(const DateItem *item) const override;
    bool before(const DateItem *item) const override;
    std::unique_ptr<DateItem> copy() const override;

    QString toString() const override;

    bool operator==(const QDate &date) const;

private:
    QDate date_;

    friend DateRange;
};

#endif // DATESINGLE_H
