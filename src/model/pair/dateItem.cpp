#include "stdafx.h"
#include "dateItem.h"

#include "dateRange.h"
#include "dateSingle.h"

std::unique_ptr<DateItem> DateItem::fromJson(const QJsonObject& item)
{
    auto frequency = Frequency::fromString(item["frequency"].toString());
    if (frequency == Frequency::getOnce()) {
        return std::make_unique<DateSingle>(item);
    }

    return std::make_unique<DateRange>(item, frequency);
}

DateItem::DateItem(const Frequency& frequency)
    : frequency_(frequency)
{
}

Frequency DateItem::frequency() const
{
    return frequency_;
}

QString DateItem::FULL_FORMAT = "yyyy.MM.dd";

QString DateItem::PRINT_FORMAT = "dd.MM";
