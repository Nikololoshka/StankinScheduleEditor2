#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QtWidgets>


class Frequency
{
public:
    static Frequency fromString(const QString &value);
    static QVector<Frequency> list();

    static Frequency getOnce();
    static Frequency getEvery();
    static Frequency getThroughout();

    int delta() const;
    QString tag() const;
    QString text() const;

    bool operator==(const Frequency &other) const;

private:
    Frequency(const int delta, const QString tag, const QString text);

private:
    int delta_;
    QString tag_;
    QString text_;
};

#endif // FREQUENCY_H
