#ifndef PAIR_H
#define PAIR_H

#include <QtWidgets>
#include <vector>

#include "time_.h"
#include "type.h"
#include "subgroup.h"
#include "dateSingle.h"
#include "dateRange.h"


class Pair
{
public:
    static Pair fromJson(const QJsonObject &value);

    Pair(const QString &title, const QString &lecturer, const QString &classroom,
         const Type &type, const Subgroup &subgroup, const Time_ &time);
    Pair(const Pair &pair);

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

    void addDate(const std::unique_ptr<DateItem> &item);
    void removeDate(DateItem *const item);
    bool contains(const DateItem *item) const;

    QString toString() const;

private:
    QString datesToString() const;

private:
    QString title_;
    QString lecturer_;
    QString classroom_;

    Type type_;
    Subgroup subgroup_;

    Time_ time_;

    std::vector<std::unique_ptr<DateItem>> dates_;
};

#endif // PAIR_H
