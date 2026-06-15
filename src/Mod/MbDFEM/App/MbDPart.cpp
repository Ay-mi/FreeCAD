#include <App/Application.h>
#include <App/Document.h>
#include <App/FeaturePythonPyImp.h>
#include <App/PropertyPythonObject.h>
#include <Base/Console.h>
#include <Base/Placement.h>

#include "MbDPart.h"
#include "MbDPartPy.h"

using namespace MbDFEM;


PROPERTY_SOURCE(MbDFEM::MbDPart, App::GeoFeature)

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
    auto* obj = cadPart.getValue();
    if (!obj) {
        return nullptr;
    }

    auto* placementProperty =
        dynamic_cast<App::PropertyPlacement*>(obj->getPropertyByName("Placement"));
    if (placementProperty) {
        placementProperty->setValue(Placement.getValue());
    }

    return App::GeoFeature::execute();
}

App::DocumentObject* MbDPart::getCadPart() const
{
    return cadPart.getValue();
}
