#ifndef DAYOFWEEK_H
#define DAYOFWEEK_H

#include <QtWidgets>


enum class DayOfWeek {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

namespace DateUtils {
    DayOfWeek of(const QDate& date);
    QStringList dayOfWeekList();
    QStringList dayOfWeekMiniList();
}


#endif // DAYOFWEEK_H
