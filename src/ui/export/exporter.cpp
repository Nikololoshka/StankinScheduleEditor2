#include "exporter.h"

Exporter::Exporter()
{

}

void Exporter::runExport(QPrinter &printer) const
{
    QPainter painter;
    if (!painter.begin(&printer)) {
        throw std::logic_error("Не удалось запустить печать на переданное устройство");
    }

    runExport(printer, painter);
    painter.end();
}

void Exporter::runExport(QPrinter &printer, QPainter &painter) const
{
    int width = printer.width();
    int height = printer.height();

    printer.pageLayout().fullRect();
}
