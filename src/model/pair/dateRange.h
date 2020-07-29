#ifndef DATERANGE_H
#define DATERANGE_H

#include <QtWidgets>

#include "dateItem.h"

class DateSingle;

class DateRange : public DateItem
{
public:
    DateRange(const QDate &start, const QDate &end, const Frequency &frequency);
    DateRange(const QJsonObject &item, const Frequency &frequency);
    DateRange(const DateRange &date);

    QDate start() const;
    QDate end() const;

    QJsonObject toJson() const override;
    DayOfWeek dayOfWeek() const override;
    bool contains(const DateItem *item) const override;
    bool before(const DateItem *item) const override;
    std::unique_ptr<DateItem> copy() const override;

    QString toString() const override;

    qint64 count() const;
    QDate at(const int index) const;
    QDate operator[](const int index) const;

private:
    void init(const QDate &start, const QDate &end, const Frequency &frequency);

private:
    QDate start_;
    QDate end_;

    friend DateSingle;
};

#endif // DATERANGE_H
