#pragma once

#include <Mod/MbDFEM/MbDFEMGlobal.h>

#include <App/GeoFeature.h>
#include <App/PropertyLinks.h>


namespace MbDFEM
{

class MbDFEMExport MbDPart: public App::GeoFeature
{
    PROPERTY_HEADER_WITH_OVERRIDE(MbDFEM::MbDPart);

public:
    MbDPart();
    ~MbDPart() override;

    PyObject* getPyObject() override;

    const char* getViewProviderName() const override
    {
        return "MbDFEMGui::ViewProviderMbDPart";
    }

    App::DocumentObjectExecReturn* execute() override;

    App::DocumentObject* getCadPart() const;

    App::PropertyXLink cadPart;
};


}  // namespace MbDFEM
