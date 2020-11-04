#include "stdafx.h"
#include "tesseractWrapper.h"

TesseractWrapper::TesseractWrapper(const QString &program)
{
    tesseract_ = std::make_unique<tesseract::TessBaseAPI>();
    initSuccess_ = tesseract_->Init(nullptr, "rus");
}

TesseractWrapper::~TesseractWrapper()
{
    tesseract_->End();
}

QString TesseractWrapper::imageToString(const QString &imagePath) const
{
    auto image = std::unique_ptr<Pix, std::function<void (Pix*)>>(
        pixRead(imagePath.toStdString().c_str()),
        [](Pix* p) { pixDestroy(&p); }
    );
    tesseract_->SetImage(image.get());
    return tesseract_->GetUTF8Text();
}

QString TesseractWrapper::checkTesseract() const
{
    return initSuccess_ ? "" : "Could not initialize tesseract";
}
