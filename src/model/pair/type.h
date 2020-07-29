#ifndef TYPE_H
#define TYPE_H

#include <QtWidgets>


class Type
{
public:
    static Type fromString(const QString &value);
    static QVector<Type> list();

    static Type getLecture();
    static Type getSeminar();
    static Type getLaboratory();

    bool operator==(const Type &type) const;

    QString tag() const;
    QString text() const;

private:
    Type(QString tag, QString text);

private:
    QString tag_;
    QString text_;
};

#endif // TYPE_H
