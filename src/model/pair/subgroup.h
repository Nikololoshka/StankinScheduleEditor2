#ifndef SUBGROUP_H
#define SUBGROUP_H

#include <QtWidgets>


class Subgroup
{
public:
    static Subgroup fromString(const QString &value);
    static QVector<Subgroup> list();

    static Subgroup getA();
    static Subgroup getB();
    static Subgroup getCommon();

    bool isShow() const;
    bool separate(const Subgroup &subgroup) const;

    QString tag() const;
    QString text() const;

    bool operator==(const Subgroup &subgroup) const;
    bool operator!=(const Subgroup &subgroup) const;

private:
    Subgroup(QString tag, QString text);

private:
    QString tag_;
    QString text_;
};

#endif // SUBGROUP_H
