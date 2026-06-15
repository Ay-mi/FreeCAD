#pragma once

#include <Mod/MbDFEM/MbDFEMGlobal.h>
#include <Gui/ViewProviderDocumentObjectGroup.h>


namespace MbDFEMGui
{

class MbDFEMGuiExport ViewProviderPartsGroup: public Gui::ViewProviderDocumentObjectGroup
{
    PROPERTY_HEADER_WITH_OVERRIDE(MbDFEMGui::ViewProviderPartsGroup);

public:
    ViewProviderPartsGroup();
    ~ViewProviderPartsGroup() override;

    QIcon getIcon() const override;

    bool canDragObjects() const override
    {
        return false;
    }
    bool canDropObjects() const override
    {
        return false;
    }
    bool canDragAndDropObject(App::DocumentObject*) const override
    {
        return false;
    }
    bool canDelete(App::DocumentObject*) const override
    {
        return false;
    }
    bool onDelete(const std::vector<std::string>&) override
    {
        return false;
    }
};

}  // namespace MbDFEMGui
