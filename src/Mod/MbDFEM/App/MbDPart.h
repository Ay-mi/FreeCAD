#pragma once

#include <Mod/MbDFEM/MbDFEMGlobal.h>

#include <Mod/Part/App/PartFeature.h>
#include <App/PropertyLinks.h>
#include <Base/Placement.h>
#include <OndselSolver/ASMTPart.h>


namespace MbDFEM
{

class MbDFEMExport MbDPart: public Part::Feature
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

    App::PropertyXLink cadPart; //stores a cross-document reference to the original cad geometry
    //used to locate shape and copy into MbDPart's shape

    // Transient members — set during solve, not saved to disk, need to look into this
    std::shared_ptr<MbD::ASMTPart> liveASMTPart; // a pointer to the solver's ASMTPart obj for this part
    Base::Placement offsetPlc; //will store fixed offset between the solver's frame and cad geom frame
    //these two are in objectPartMap in assembly, trying to put them into MbDPart
};


}  // namespace MbDFEM
