#include "stdafx.h"
#include "parseHighlighter.h"

#include "parseWorker.h"

ParseHighlighter::ParseHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    const QVector<Qt::GlobalColor> colors = {
        Qt::darkGray, Qt::blue, Qt::green,
        Qt::darkYellow, Qt::darkMagenta, Qt::darkCyan, Qt::red
    };

    for (const auto &color : colors) {
        QTextCharFormat format;
        format.setForeground(color);
        formats_.append(std::move(format));
    }
}


void ParseHighlighter::highlightBlock(const QString &text)
{
    QRegularExpression re(ParseWorker::patternCommon());
    auto it = re.globalMatch(text);
    while (it.hasNext()) {
        auto match = it.next();
        for (int i = 0; i < formats_.size(); ++i) {
            if (!match.captured(i).isEmpty() && i != 3) {
                setFormat(match.capturedStart(i), match.capturedLength(i), formats_[i]);
            }
        }
    }
}
