#include <App/Application.h>
#include <App/Document.h>
#include <App/FeaturePythonPyImp.h>
#include <App/PropertyPythonObject.h>
#include <Base/Console.h>
#include <Base/Tools.h>

#include "PartGroup.h"
#include "PartGroupPy.h"

using namespace MbDFEM;


PROPERTY_SOURCE(MbDFEM::PartGroup, App::DocumentObjectGroup)

PartGroup::PartGroup()
{}

PartGroup::~PartGroup() = default;

/*
- overriding virtual method getPyObject() defined originally in App::DocumentObject
- inheriting from DocumentObjectGroup allows for getPyObject() to be overridden
- returns the Python wrapper for PartGroup obj so it can be used from Python scripts
*/
PyObject* PartGroup::getPyObject()
{
    if (PythonObject.is(Py::_None())) {
        PythonObject = Py::Object(new PartGroupPy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}

/* 
- getParts() returns all valid child objects contained in the PartGroup
- uses getObjects() to get child objects, then iterates through them, filters out null pointers
- getObjects() is inherited from DocumentObjectGroup 
- if you leave getPyObject() generic (aka without overriding above), Python won't be able to access getParts() esp this version
they're connected through PartGroupPy (generated using PartGroup.pyi):
PyObject* PartGroupPy::getParts(PyObject* args)  // Python-callable
{return this->getPartsGroupPtr()->getParts();   // calls the C++ method}
*/
std::vector<App::DocumentObject*> PartGroup::getParts()
{
    std::vector<App::DocumentObject*> parts = {};

    for (auto obj : getObjects()) {
        if (obj && obj->isDerivedFrom(App::DocumentObject::getClassTypeId())) {
            parts.push_back(obj);
        }
    }

    return parts;
}
