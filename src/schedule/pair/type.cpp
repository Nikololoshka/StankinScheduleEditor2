#include "type.h"

Type Type::fromString(const QString &value)
{
    if (value == "Lecture") {
        return getLecture();
    }
    if (value == "Seminar") {
        return getSeminar();
    }
    if (value == "Laboratory") {
        return getLaboratory();
    }
    throw std::invalid_argument("Can't parse type: " + value.toStdString());
}

Type Type::getLecture()
{
    return Type("Lecture", "Лекция");
}

Type Type::getSeminar()
{
    return Type("Seminar", "Семинар");
}

Type Type::getLaboratory()
{
    return Type("Laboratory", "Лабораторная работа");
}

QString Type::text() const
{
    return text_;
}

QString Type::tag() const
{
    return tag_;
}


Type::Type(QString tag, QString text)
    : tag_(tag), text_(text)
{

}
