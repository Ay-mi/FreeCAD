#include <App/Application.h>
#include <App/Document.h>
#include <App/FeaturePythonPyImp.h>
#include <App/PropertyPythonObject.h>
#include <Base/Console.h>
#include <Base/Tools.h>

#include "PartsGroup.h"
#include "PartsGroupPy.h"

using namespace MbDFEM;


PROPERTY_SOURCE(MbDFEM::PartsGroup, App::DocumentObjectGroup)

PartsGroup::PartsGroup()
{}

PartsGroup::~PartsGroup() = default;

PyObject* PartsGroup::getPyObject()
{
    if (PythonObject.is(Py::_None())) {
        PythonObject = Py::Object(new PartsGroupPy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}

std::vector<App::DocumentObject*> PartsGroup::getParts()
{
    std::vector<App::DocumentObject*> parts = {};

    for (auto obj : getObjects()) {
        if (obj && obj->isDerivedFrom(App::DocumentObject::getClassTypeId())) {
            parts.push_back(obj);
        }
    }

    return parts;
}
