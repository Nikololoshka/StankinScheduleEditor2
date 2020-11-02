#ifndef POOPLERWRAPPER_H
#define POOPLERWRAPPER_H

#include "stdafx.h"

class PooplerWrapper
{
public:
    PooplerWrapper(const QString &program, int dpi);
    ~PooplerWrapper();

    QString print(const QString pdfPath) const;
    QString checkPoopler() const;

public:
    QString program_;
    QProcess* poopler_;
    int dpi_;
};

#endif // POOPLERWRAPPER_H
