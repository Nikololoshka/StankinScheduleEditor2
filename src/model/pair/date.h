#ifndef DATE_H
#define DATE_H

#include <QtWidgets>
#include <vector>
#include "dateItem.h"

class Date
{
public:
    Date();
    Date(const Date &date);
    Date(Date &&date);

    Date& operator=(const Date &date);
    const std::unique_ptr<DateItem>& operator[](int index) const;
    bool operator==(const Date &date) const;

    QJsonArray toJson() const;

    std::unique_ptr<DateItem> at(int index) const;

    void addDate(const std::unique_ptr<DateItem> &item);
    void removeDate(const std::unique_ptr<DateItem> &item);

    int size() const;
    bool isEmpty() const;
    void changeDate(const std::unique_ptr<DateItem> &oldDate,
                    const std::unique_ptr<DateItem> &newDate);
    void possibleChange(const std::unique_ptr<DateItem> &oldDate,
                        const std::unique_ptr<DateItem> &newDate) const;

    bool contains(const std::unique_ptr<DateItem> &item) const;
    bool contains(const Date &date) const;

    bool before(const Date &date) const;

    DayOfWeek dayOfWeek() const;
    QString toString() const;

private:
     std::vector<std::unique_ptr<DateItem>> dates_;
};

#endif // DATE_H
