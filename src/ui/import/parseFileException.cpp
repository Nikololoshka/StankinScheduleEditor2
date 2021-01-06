#include "parseFileException.h"

ParseFileException::ParseFileException(const QString &type,
                                       const QString &maybe,
                                       const QString &confuse)
    : QException(),
      type_(type),
      maybe_(maybe),
      confuse_(confuse)
{
}


const char *ParseFileException::what() const noexcept
{
    return toString().toStdString().c_str();
}

QString ParseFileException::toString() const
{
    return "Тип ошибки: " + type_ + "\n\n" +
           "Возможный решение: " + maybe_ + "\n\n" +
           "Причина: " + confuse_;

}

QString ParseFileException::type() const
{
    return type_;
}

QString ParseFileException::maybe() const
{
    return maybe_;
}

QString ParseFileException::confuse() const
{
    return confuse_;
}
