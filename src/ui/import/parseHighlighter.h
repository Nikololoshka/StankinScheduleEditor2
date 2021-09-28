#ifndef PARSEHIGHLIGHTER_H
#define PARSEHIGHLIGHTER_H

#include "stdafx.h"

/**
 * @brief Класс для подсвечивания пары в редакторе.
*/
class ParseHighlighter : QSyntaxHighlighter
{
public:
    ParseHighlighter(QTextDocument *parent);

protected:
    void highlightBlock(const QString &text) override;

private:
    //! список с форматами пары
    QVector<QTextCharFormat> formats_;
};

#endif // PARSEHIGHLIGHTER_H
