#ifndef PARSEFILEEXCEPTION_H
#define PARSEFILEEXCEPTION_H

#include "stdafx.h"

/**
 * @brief ����� ������ ��� �������� ���� �� ����������.
*/
class ParseFileException : public QException
{
public:
    /**
     * @brief ����������� ������.
     * @param type ��� ������.
     * @param maybe ��������� �������.
     * @param confuse ������� �������������.
    */
    ParseFileException(const QString &type,
                       const QString &maybe,
                       const QString &confuse);

    const char *what() const noexcept override;

    /**
     * @brief ���������� ������������� ������ � ���� ������.
    */
    QString toString() const;

    /**
     * @brief ���������� ��� ������.
    */
    QString type() const;
    /**
     * @brief ���������� ��������� �������.
    */
    QString maybe() const;
    /** 
     * @brief ���������� ������� ������.
    */
    QString confuse() const;

private:
    //! ��� ������
    QString type_;
    //! ��������� �������
    QString maybe_;
    //! ������� �������������
    QString confuse_;
};

#endif // PARSEFILEEXCEPTION_H
