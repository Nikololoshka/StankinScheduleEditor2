#include "frequency.h"

Frequency Frequency::fromString(const QString& value)
{
    if (value == "once") {
        return getOnce();
    }
    if (value == "every") {
        return getEvery();
    }
    if (value == "throughout") {
        return getThroughout();
    }
    throw std::invalid_argument("Can't parse frequency: " + value.toStdString());
}

std::vector<Frequency> Frequency::list()
{
    return {
        getOnce(),
        getEvery(),
        getThroughout()
    };
}

Frequency Frequency::getOnce()
{
    return Frequency(1, "once", "");
}

Frequency Frequency::getEvery()
{
    return Frequency(7, "every", "к.н.");
}

Frequency Frequency::getThroughout()
{
    return Frequency(14, "throughout", "ч.н.");
}

int Frequency::delta() const
{
    return delta_;
}

Frequency::Frequency(const int delta, const QString& tag, const QString& text)
    : delta_(delta)
    , tag_(tag)
    , text_(text)
{
}

QString Frequency::tag() const
{
    return tag_;
}

QString Frequency::text() const
{
    return text_;
}

bool Frequency::operator==(const Frequency& other) const
{
    return delta_ == other.delta_
        && tag_ == other.tag_
        && text_ == other.text_;
}
