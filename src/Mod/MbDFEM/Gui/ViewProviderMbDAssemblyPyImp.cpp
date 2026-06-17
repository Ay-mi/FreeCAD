#include <Base/Interpreter.h>
#include <Base/PlacementPy.h>
#include <Base/GeometryPyCXX.h>
#include <App/DocumentObjectPy.h>

// inclusion of the generated files (generated out of ViewProviderMbDAssemblyPy.xml)
#include "ViewProviderMbDAssemblyPy.h"
#include "ViewProviderMbDAssemblyPy.cpp"


using namespace Gui;

// returns a string which represents the object e.g. when printed in python
std::string ViewProviderMbDAssemblyPy::representation() const
{
    std::stringstream str;
    str << "<MbDFEM View provider object at " << getViewProviderMbDAssemblyPtr() << ">";

    return str.str();
}

PyObject* ViewProviderMbDAssemblyPy::isInEditMode(PyObject* args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return nullptr;
    }

    Py::Boolean val(getViewProviderMbDAssemblyPtr()->isInEditMode());
    return Py::new_reference_to(val);
}

Py::Boolean ViewProviderMbDAssemblyPy::getEnableMovement() const
{
    return {getViewProviderMbDAssemblyPtr()->getEnableMovement()};
}

void ViewProviderMbDAssemblyPy::setEnableMovement(Py::Boolean arg)
{
    getViewProviderMbDAssemblyPtr()->setEnableMovement(arg);
}

Py::Boolean ViewProviderMbDAssemblyPy::getMoveOnlyPreselected() const
{
    return {getViewProviderMbDAssemblyPtr()->getMoveOnlyPreselected()};
}

void ViewProviderMbDAssemblyPy::setMoveOnlyPreselected(Py::Boolean arg)
{
    getViewProviderMbDAssemblyPtr()->setMoveOnlyPreselected(arg);
}

Py::Boolean ViewProviderMbDAssemblyPy::getMoveInCommand() const
{
    return {getViewProviderMbDAssemblyPtr()->getMoveInCommand()};
}

void ViewProviderMbDAssemblyPy::setMoveInCommand(Py::Boolean arg)
{
    getViewProviderMbDAssemblyPtr()->setMoveInCommand(arg);
}

Py::Boolean ViewProviderMbDAssemblyPy::getDraggerVisibility() const
{
    return {getViewProviderMbDAssemblyPtr()->getDraggerVisibility()};
}

void ViewProviderMbDAssemblyPy::setDraggerVisibility(Py::Boolean arg)
{
    getViewProviderMbDAssemblyPtr()->setDraggerVisibility(arg);
}

PyObject* ViewProviderMbDAssemblyPy::getDragger(PyObject* args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return nullptr;
    }
    Gui::SoTransformDragger* asmDragger = getViewProviderMbDAssemblyPtr()->getDragger();

    return Base::Interpreter().createSWIGPointerObj("pivy.coin", "SoDragger *", asmDragger, 0);
}


Py::Object ViewProviderMbDAssemblyPy::getDraggerPlacement() const
{
    return Py::Placement(getViewProviderMbDAssemblyPtr()->getDraggerPlacement());
}

void ViewProviderMbDAssemblyPy::setDraggerPlacement(Py::Object arg)
{
    PyObject* p = arg.ptr();
    if (PyObject_TypeCheck(p, &(Base::PlacementPy::Type))) {
        Base::Placement* trf = static_cast<Base::PlacementPy*>(p)->getPlacementPtr();
        getViewProviderMbDAssemblyPtr()->setDraggerPlacement(*trf);
    }
    else {
        std::string error = std::string("type must be 'Placement', not ");
        error += p->ob_type->tp_name;
        throw Py::TypeError(error);
    }
}

PyObject* ViewProviderMbDAssemblyPy::getCustomAttributes(const char* /*attr*/) const
{
    return nullptr;
}

int ViewProviderMbDAssemblyPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}

PyObject* ViewProviderMbDAssemblyPy::isolateComponents(PyObject* args)
{
    PyObject* pyList = nullptr;
    int modeInt = 0;
    if (!PyArg_ParseTuple(args, "Oi", &pyList, &modeInt)) {
        return nullptr;
    }

    if (!PySequence_Check(pyList)) {
        PyErr_SetString(PyExc_TypeError, "First argument must be a sequence of DocumentObjects");
        return nullptr;
    }

    if (modeInt < 0 || modeInt > 2) {
        PyErr_SetString(PyExc_ValueError, "Mode must be an integer between 0 and 2");
        return nullptr;
    }

    std::set<App::DocumentObject*> partsSet;
    Py_ssize_t size = PySequence_Size(pyList);
    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject* item = PySequence_GetItem(pyList, i);
        if (item && PyObject_TypeCheck(item, &(App::DocumentObjectPy::Type))) {
            auto* pyObj = static_cast<App::DocumentObjectPy*>(item);
            App::DocumentObject* docObj = pyObj->getDocumentObjectPtr();
            if (docObj) {
                partsSet.insert(docObj);
            }
        }
        Py_XDECREF(item);
    }

    auto mode = static_cast<ViewProviderMbDAssembly::IsolateMode>(modeInt);
    getViewProviderMbDAssemblyPtr()->isolateComponents(partsSet, mode);

    Py_DECREF(Py_None);
    return Py_None;
}

PyObject* ViewProviderMbDAssemblyPy::clearIsolate(PyObject* args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return nullptr;
    }

    getViewProviderMbDAssemblyPtr()->clearIsolate();
    return Py::new_reference_to(Py::None());
}
