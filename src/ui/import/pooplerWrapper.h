#ifndef POOPLERWRAPPER_H
#define POOPLERWRAPPER_H

#include "stdafx.h"

/**
 * @brief ����� ��� ������ � poopler (pdftoppm).
*/
class PooplerWrapper
{
public:
    /**
     * @brief ����������� ������.
     * @param program ���� � ������� ���������.
     * @param dpi �������� DPI ��� �����������.
    */
    PooplerWrapper(const QString &program, int dpi);
    ~PooplerWrapper();

    /**
     * @brief ������������ PDF ���� � JPEG ����
     * @param pdfPath ���� � PDF �����.
     * @return ���� � JPEG �����.
    */
    QString print(const QString pdfPath) const;
    /**
     * @brief ��������� ����������� poopler'a.
     * @return ���� ���� ������ � poopler, �� ������������� ������ ������. 
     *         ����� ����� � ��������� ������.
    */
    QString checkPoopler() const;

public:
    //! ���� � ������� ���������
    QString program_;
    //! ������� poopler'a
    QProcess* poopler_;
    //! �������� DPI ��� �����������
    int dpi_;
};

#endif // POOPLERWRAPPER_H
