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
    DayOfWeek of(int number);
    QVector<DayOfWeek> list();
    QStringList dayOfWeekList();
    QStringList dayOfWeekMiniList();
    QString toString(DayOfWeek dayOfWeek);
}


#endif // DAYOFWEEK_H
