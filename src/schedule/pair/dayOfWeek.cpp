#include "dayOfWeek.h"

DayOfWeek DateUtils::of(const QDate &date)
{
    switch (date.dayOfWeek()) {
        case Qt::DayOfWeek::Monday: return DayOfWeek::Monday;
        case Qt::DayOfWeek::Tuesday: return DayOfWeek::Tuesday;
        case Qt::DayOfWeek::Wednesday: return DayOfWeek::Wednesday;
        case Qt::DayOfWeek::Thursday: return DayOfWeek::Thursday;
        case Qt::DayOfWeek::Friday: return DayOfWeek::Friday;
        case Qt::DayOfWeek::Saturday: return DayOfWeek::Saturday;
    }
    throw std::invalid_argument("Date with unsupported day of week: "
                                + date.toString().toStdString());
}


QStringList DateUtils::dayOfWeekList()
{
    return {
        "Понедельник",
        "Вторник",
        "Среда",
        "Четверг",
        "Пятница",
        "Суббота"
    };
}

QStringList DateUtils::dayOfWeekMiniList()
{
    return {
        "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"
    };
}
