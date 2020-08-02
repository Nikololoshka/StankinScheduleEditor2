#ifndef PAIR_H
#define PAIR_H

#include <QtWidgets>
#include <vector>

#include "time_.h"
#include "type.h"
#include "subgroup.h"
#include "date.h"


class Pair
{
public:
    static Pair fromJson(const QJsonObject &value);

    Pair(const QString &title, const QString &lecturer, const QString &classroom,
         const Type &type, const Subgroup &subgroup, const Time_ &time);
    Pair(const Pair &pair);
    Pair(Pair &&pair);

    QJsonObject toJson() const;

    Pair& operator=(const Pair &pair);
    bool operator==(const Pair &pair) const;

    QString title() const;
    void setTitle(const QString &title);

    QString lecturer() const;
    void setLecturer(const QString &lecturer);

    QString classroom() const;
    void setClassroom(const QString &classroom);

    Type type() const;
    void setType(const Type &type);

    Subgroup subgroup() const;
    void setSubgroup(const Subgroup &subgroup);

    Time_ time() const;
    void setTime(const Time_ &time);

    Date date() const;
    void setDate(const Date &date);

    std::unique_ptr<Pair> copy() const;

    void addDate(const std::unique_ptr<DateItem> &item);
    void removeDate(const std::unique_ptr<DateItem> &item);

    bool before(const Pair &pair) const;
    bool contains(const std::unique_ptr<DateItem> &item) const;

    DayOfWeek dayOfWeek() const;
    bool intersect(const Pair &pair) const;
    bool separate(const Pair &pair) const;

    QString toString() const;

private:
    QString title_;
    QString lecturer_;
    QString classroom_;
    Type type_;
    Subgroup subgroup_;
    Time_ time_;
    Date date_;
};

#endif // PAIR_H
