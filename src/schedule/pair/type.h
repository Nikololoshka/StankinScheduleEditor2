#ifndef TYPE_H
#define TYPE_H

#include <QtWidgets>


class Type
{
public:
    static Type fromString(const QString &value);

    static Type getLecture();
    static Type getSeminar();
    static Type getLaboratory();

    QString tag() const;
    QString text() const;

private:
    Type(QString tag, QString text);

private:
    QString tag_;
    QString text_;
};

#endif // TYPE_H
