#ifndef POOPLERWRAPPER_H
#define POOPLERWRAPPER_H

#include "stdafx.h"

#include <poppler/cpp/poppler-page-renderer.h>

/**
 * @brief ����� ��� ������ � poopler (pdftoppm).
*/
class PopplerWrapper
{
public:
    /**
     * @brief ����������� ������.
     * @param program ���� � ������� ���������.
     * @param dpi �������� DPI ��� �����������.
    */
    PopplerWrapper(const int id, int dpi);

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
    //! �������� �������������� �����������
    QString name_;
    //! ������ ��� ���������� pdf �����
    poppler::page_renderer render_;
    //! �������� DPI ��� �����������
    int dpi_;
};

#endif // POOPLERWRAPPER_H
