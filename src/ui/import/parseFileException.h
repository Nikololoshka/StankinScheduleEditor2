#ifndef PARSEFILEEXCEPTION_H
#define PARSEFILEEXCEPTION_H

#include <QtWidgets>


class ParseFileException : public QException
{
public:
    ParseFileException(const QString &type,
                       const QString &maybe,
                       const QString &confuse);

    const char *what() const noexcept override;
    QString toString() const;

    QString type() const;
    QString maybe() const;
    QString confuse() const;

private:
    QString type_;
    QString maybe_;
    QString confuse_;
};

#endif // PARSEFILEEXCEPTION_H
