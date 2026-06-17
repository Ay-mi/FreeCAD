#pragma once

#include <Mod/MbDFEM/MbDFEMGlobal.h>
#include <App/DocumentObjectGroup.h>


namespace MbDFEM
{

class MbDFEMExport PartGroup: public App::DocumentObjectGroup
{
    PROPERTY_HEADER_WITH_OVERRIDE(MbDFEM::PartGroup);

public:
    PartGroup();
    ~PartGroup() override;

    PyObject* getPyObject() override; //definition in cpp file

    //called by CommandCreateMbDAssembly
    const char* getViewProviderName() const override
    {
        return "MbDFEMGui::ViewProviderPartGroup"; //goes to ViewProviderPartGroup, gets icon
    }

    std::vector<App::DocumentObject*> getParts(); //definition in cpp file
};


}  // namespace MbDFEM
