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

DayOfWeek DateUtils::of(int number)
{
    switch (number) {
    case 0: return DayOfWeek::Monday;
    case 1: return DayOfWeek::Tuesday;
    case 2: return DayOfWeek::Wednesday;
    case 3: return DayOfWeek::Thursday;
    case 4: return DayOfWeek::Friday;
    case 5: return DayOfWeek::Saturday;
    }
    throw std::invalid_argument(("Number day of week unsupported: "
                                 + QString::number(number)).toStdString());
}

QString DateUtils::toString(DayOfWeek dayOfWeek)
{
    return dayOfWeekList()[static_cast<int>(dayOfWeek)];
}

QVector<DayOfWeek> DateUtils::list()
{
    return {
        DayOfWeek::Monday,
        DayOfWeek::Tuesday,
        DayOfWeek::Wednesday,
        DayOfWeek::Thursday,
        DayOfWeek::Friday,
        DayOfWeek::Saturday
    };
}
