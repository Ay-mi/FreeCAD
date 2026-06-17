#pragma once

#include <QCoreApplication>

#include <Mod/MbDFEM/MbDFEMGlobal.h>

#include <Gui/ViewProviderPart.h>


namespace MbDFEMGui
{

class MbDFEMGuiExport ViewProviderMbDAssemblyLink: public Gui::ViewProviderPart
{
    Q_DECLARE_TR_FUNCTIONS(MbDFEMGui::ViewProviderMbDAssemblyLink)
    PROPERTY_HEADER_WITH_OVERRIDE(MbDFEMGui::ViewProviderMbDAssemblyLink);

public:
    ViewProviderMbDAssemblyLink();
    ~ViewProviderMbDAssemblyLink() override;

    /// deliver the icon shown in the tree view. Override from ViewProvider.h
    QIcon getIcon() const override;

    bool setEdit(int ModNum) override;

    bool doubleClicked() override;

    // When the MbDFEM link is deleted, we delete all its content as well.
    bool onDelete(const std::vector<std::string>& subNames) override;

    // Prevent deletion of the link MbDFEM's content.
    bool canDelete(App::DocumentObject*) const override
    {
        return false;
    };

    // Prevent drag/drop of objects within the MbDFEM link.
    bool canDragObjects() const override
    {
        return false;
    };
    bool canDropObjects() const override
    {
        return false;
    };
    bool canDragAndDropObject(App::DocumentObject*) const override
    {
        return false;
    };

    void setupContextMenu(QMenu*, QObject*, const char*) override;
};

}  // namespace MbDFEMGui
