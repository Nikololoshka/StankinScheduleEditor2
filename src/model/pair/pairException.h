#ifndef PAIREXCEPTION_H
#define PAIREXCEPTION_H

#include "stdafx.h"

class InvalidAddDateException : public QException
{
public:
    InvalidAddDateException(const QString &msg);
    const char *what() const noexcept override;

private:
    QString msg_;
};

#endif // PAIREXCEPTION_H
