#ifndef PARSEHIGHLIGHTER_H
#define PARSEHIGHLIGHTER_H

#include "stdafx.h"


class ParseHighlighter : QSyntaxHighlighter
{
public:
    ParseHighlighter(QTextDocument *parent);

protected:
    void highlightBlock(const QString &text) override;

private:
    QVector<QTextCharFormat> formats_;
};

#endif // PARSEHIGHLIGHTER_H
