#include "popplerWrapper.h"

#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-image.h>

PopplerWrapper::PopplerWrapper(const int id, int dpi)
    : name_(QString::number(id) + "-poppler.jpg"),
      dpi_(dpi)
{
    render_.set_render_hint(poppler::page_renderer::render_hint::text_antialiasing);
    render_.set_render_hint(poppler::page_renderer::render_hint::antialiasing);
    render_.set_render_hint(poppler::page_renderer::render_hint::text_hinting);
}

QString PopplerWrapper::print(const QString pdfPath) const
{
    QFileInfo pdfFileInfo(pdfPath);
    QString tempPath = pdfFileInfo.absolutePath() + "/" + name_;

    std::unique_ptr<poppler::document> doc(poppler::document::load_from_file(pdfPath.toStdString()));
    if (doc && !doc->is_locked()) {
        poppler::page* page = doc->create_page(0);
        poppler::image image = render_.render_page(page, dpi_, dpi_);
        image.save(tempPath.toStdString(), "jpg", dpi_);

    } else {
        throw std::logic_error(("Poopler error: " + pdfPath +
                                "\n" +
                                "Error: doc is null or doc is locked").toStdString());
    }

    return tempPath;
}

QString PopplerWrapper::checkPoopler() const
{
    return "";
}
