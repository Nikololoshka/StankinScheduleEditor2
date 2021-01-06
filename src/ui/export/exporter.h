#ifndef EXPORTER_H
#define EXPORTER_H

#include "stdafx.h"
#include "schedule.h"
#include <QPrinter>


enum class ExportType {
    Weekly,
    Full,
};

/**
 * @brief The Exporter class
 */
class Exporter {
public:
    /**
     * @brief Exporter
     */
    Exporter();

    /**
     * @brief runExport
     * @param printer
     */
    void runExport(QPrinter &printer) const;

private:
    void runExport(QPrinter &printer, QPainter &painter) const;

};


#endif // EXPORTER_H
