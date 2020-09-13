#include "subgroup.h"

Subgroup Subgroup::fromString(const QString& value)
{
    if (value == "A") {
        return getA();
    }
    if (value == "B") {
        return getB();
    }
    if (value == "Common") {
        return getCommon();
    }
    throw std::invalid_argument("Can't parse subgroup: " + value.toStdString());
}

std::vector<Subgroup> Subgroup::list()
{
    return {
        Subgroup::getCommon(), Subgroup::getA(), Subgroup::getB()
    };
}

Subgroup Subgroup::getA()
{
    return Subgroup("A", "(А)");
}

Subgroup Subgroup::getB()
{
    return Subgroup("B", "(Б)");
}

Subgroup Subgroup::getCommon()
{
    return Subgroup("Common", "");
}

bool Subgroup::isShow() const
{
    return !text_.isEmpty();
}

bool Subgroup::separate(const Subgroup& subgroup) const
{
    const Subgroup common = getCommon();
    return *this != subgroup && *this != common && subgroup != common;
}

QString Subgroup::tag() const
{
    return tag_;
}

QString Subgroup::text() const
{
    return text_;
}

bool Subgroup::operator==(const Subgroup& subgroup) const
{
    return tag_ == subgroup.tag_;
}

bool Subgroup::operator!=(const Subgroup& subgroup) const
{
    return !(*this == subgroup);
}

Subgroup::Subgroup(QString tag, QString text)
    : tag_(tag)
    , text_(text)
{
}
