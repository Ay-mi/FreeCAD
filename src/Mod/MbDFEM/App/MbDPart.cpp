#include <App/Application.h>
#include <App/Document.h>
#include <App/FeaturePythonPyImp.h>
#include <App/PropertyPythonObject.h>
#include <Base/Console.h>
#include <Base/Placement.h>
#include <Mod/Part/App/PartFeature.h>

#include "MbDPart.h"
#include "MbDPartPy.h"

using namespace MbDFEM;

// registers MbDPart with FreeCAD's type system, makes doc.addObject("MbDFEM::MbDPart", ..) work from Python 
// inheriting Feature gives MbDPart Shape and Placement
PROPERTY_SOURCE(MbDFEM::MbDPart, Part::Feature)

MbDPart::MbDPart()
{
    ADD_PROPERTY_TYPE(
        cadPart,
        (nullptr),
        "General",
        (App::PropertyType)(App::Prop_None),
        "The CAD body or part whose motion this MbDPart tracks."
    );
}

MbDPart::~MbDPart() = default;

PyObject* MbDPart::getPyObject()
{
    if (PythonObject.is(Py::_None())) {
        PythonObject = Py::Object(new MbDPartPy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}

App::DocumentObjectExecReturn* MbDPart::execute()
{
    auto* obj = cadPart.getValue(); //gets obj cadPart is pointing to
    if (!obj) {
        return Part::Feature::execute();
    }

    // Copy shape from cadPart so MbDPart has geometry for display and selection, no shape no 3d shape in freecad
    // MbDPart::Placement (set by the solver) positions the shape in the assembly.
    //getPropertyByName() used to from Shape propetry of original part
    if (auto* shapeProp = dynamic_cast<Part::PropertyPartShape*>(obj->getPropertyByName("Shape"))) {
        Shape.setValue(shapeProp->getShape()); //shape copied into MbD::Shape using setValue()
    }

    return Part::Feature::execute();
}

App::DocumentObject* MbDPart::getCadPart() const
{
    return cadPart.getValue();
}
