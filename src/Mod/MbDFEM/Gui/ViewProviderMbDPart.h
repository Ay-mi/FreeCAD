#pragma once

#include <Mod/MbDFEM/MbDFEMGlobal.h>
#include <Mod/Part/Gui/ViewProvider.h>


namespace MbDFEMGui
{

class MbDFEMGuiExport ViewProviderMbDPart: public PartGui::ViewProviderPart
{
    PROPERTY_HEADER_WITH_OVERRIDE(MbDFEMGui::ViewProviderMbDPart);

public:
    ViewProviderMbDPart();
    ~ViewProviderMbDPart() override;

    QIcon getIcon() const override;
};

}  // namespace MbDFEMGui
