#pragma once

#include <Mod/MbDFEM/MbDFEMGlobal.h>
#include <App/DocumentObjectGroup.h>


namespace MbDFEM
{

class MbDFEMExport PartsGroup: public App::DocumentObjectGroup
{
    PROPERTY_HEADER_WITH_OVERRIDE(MbDFEM::PartsGroup);

public:
    PartsGroup();
    ~PartsGroup() override;

    PyObject* getPyObject() override;

    const char* getViewProviderName() const override
    {
        return "MbDFEMGui::ViewProviderPartsGroup";
    }

    std::vector<App::DocumentObject*> getParts();
};


}  // namespace MbDFEM
