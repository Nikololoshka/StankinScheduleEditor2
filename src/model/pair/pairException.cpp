#include "pairException.h"

InvalidAddDateException::InvalidAddDateException(const QString &msg)
    : QException(),
      msg_(msg)
{
}


const char *InvalidAddDateException::what() const noexcept
{
    return msg_.toStdString().c_str();
}
