#pragma once

#include <Mod/MbDFEM/MbDFEMGlobal.h>
#include <Gui/ViewProviderDocumentObject.h>


namespace MbDFEMGui
{

class MbDFEMGuiExport ViewProviderMbDPart: public Gui::ViewProviderDocumentObject
{
    PROPERTY_HEADER_WITH_OVERRIDE(MbDFEMGui::ViewProviderMbDPart);

public:
    ViewProviderMbDPart();
    ~ViewProviderMbDPart() override;

    QIcon getIcon() const override;
};

}  // namespace MbDFEMGui
