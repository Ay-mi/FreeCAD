// inclusion of the generated files (generated out of MbDAssembly.xml)
#include "MbDAssemblyPy.h"
#include "MbDAssemblyPy.cpp"

using namespace MbDFEM;

// returns a string which represents the object e.g. when printed in python
std::string MbDAssemblyPy::representation() const
{
    return {"<MbDFEM object>"};
}

PyObject* MbDAssemblyPy::getCustomAttributes(const char* /*attr*/) const
{
    return nullptr;
}

int MbDAssemblyPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}

PyObject* MbDAssemblyPy::solve(PyObject* args) const
{
    PyObject* enableUndoPy;
    bool enableUndo;

    if (!PyArg_ParseTuple(args, "O!", &PyBool_Type, &enableUndoPy)) {
        PyErr_Clear();
        if (!PyArg_ParseTuple(args, "")) {
            return nullptr;
        }
        else {
            enableUndo = false;
        }
    }
    else {
        enableUndo = Base::asBoolean(enableUndoPy);
    }

    int ret = this->getMbDAssemblyPtr()->solve(enableUndo);
    return Py_BuildValue("i", ret);
}

PyObject* MbDAssemblyPy::generateSimulation(PyObject* args) const
{
    PyObject* pyobj;

    if (!PyArg_ParseTuple(args, "O", &pyobj)) {
        return nullptr;
    }
    auto* obj = static_cast<App::DocumentObjectPy*>(pyobj)->getDocumentObjectPtr();
    int ret = this->getMbDAssemblyPtr()->generateSimulation(obj);
    return Py_BuildValue("i", ret);
}

PyObject* MbDAssemblyPy::ensureIdentityPlacements(PyObject* args) const
{
    if (!PyArg_ParseTuple(args, "")) {
        return nullptr;
    }
    this->getMbDAssemblyPtr()->ensureIdentityPlacements();
    Py_Return;
}

PyObject* MbDAssemblyPy::updateForFrame(PyObject* args) const
{
    unsigned long index {};

    if (!PyArg_ParseTuple(args, "k", &index)) {
        throw Py::RuntimeError("updateForFrame requires an integer index");
    }
    PY_TRY
    {
        this->getMbDAssemblyPtr()->updateForFrame(index);
    }
    PY_CATCH;

    Py_Return;
}

PyObject* MbDAssemblyPy::numberOfFrames(PyObject* args) const
{
    if (!PyArg_ParseTuple(args, "")) {
        return nullptr;
    }
    size_t ret = this->getMbDAssemblyPtr()->numberOfFrames();
    return Py_BuildValue("k", ret);
}

PyObject* MbDAssemblyPy::updateSolveStatus(PyObject* args) const
{
    if (!PyArg_ParseTuple(args, "")) {
        return nullptr;
    }

    this->getMbDAssemblyPtr()->updateSolveStatus();
    Py_Return;
}

PyObject* MbDAssemblyPy::undoSolve(PyObject* args) const
{
    if (!PyArg_ParseTuple(args, "")) {
        return nullptr;
    }
    this->getMbDAssemblyPtr()->undoSolve();
    Py_Return;
}

PyObject* MbDAssemblyPy::clearUndo(PyObject* args) const
{
    if (!PyArg_ParseTuple(args, "")) {
        return nullptr;
    }
    this->getMbDAssemblyPtr()->clearUndo();
    Py_Return;
}

PyObject* MbDAssemblyPy::isPartConnected(PyObject* args) const
{
    PyObject* pyobj;

    if (!PyArg_ParseTuple(args, "O!", &(App::DocumentObjectPy::Type), &pyobj)) {
        return nullptr;
    }
    auto* obj = static_cast<App::DocumentObjectPy*>(pyobj)->getDocumentObjectPtr();
    bool ok = this->getMbDAssemblyPtr()->isPartConnected(obj);
    return Py_BuildValue("O", (ok ? Py_True : Py_False));
}

PyObject* MbDAssemblyPy::isPartGrounded(PyObject* args) const
{
    PyObject* pyobj;

    if (!PyArg_ParseTuple(args, "O!", &(App::DocumentObjectPy::Type), &pyobj)) {
        return nullptr;
    }
    auto* obj = static_cast<App::DocumentObjectPy*>(pyobj)->getDocumentObjectPtr();
    bool ok = this->getMbDAssemblyPtr()->isPartGrounded(obj);
    return Py_BuildValue("O", (ok ? Py_True : Py_False));
}

PyObject* MbDAssemblyPy::isJointConnectingPartToGround(PyObject* args) const
{
    PyObject* pyobj;
    char* pname;

    if (!PyArg_ParseTuple(args, "O!s", &(App::DocumentObjectPy::Type), &pyobj, &pname)) {
        return nullptr;
    }
    auto* obj = static_cast<App::DocumentObjectPy*>(pyobj)->getDocumentObjectPtr();
    bool ok = this->getMbDAssemblyPtr()->isJointConnectingPartToGround(obj, pname);
    return Py_BuildValue("O", (ok ? Py_True : Py_False));
}

PyObject* MbDAssemblyPy::exportAsASMT(PyObject* args) const
{
    char* utf8Name;
    if (!PyArg_ParseTuple(args, "et", "utf-8", &utf8Name)) {
        return nullptr;
    }

    std::string fileName = utf8Name;
    PyMem_Free(utf8Name);

    if (fileName.empty()) {
        PyErr_SetString(PyExc_ValueError, "Passed string is empty");
        return nullptr;
    }

    this->getMbDAssemblyPtr()->exportAsASMT(fileName);

    Py_Return;
}

Py::List MbDAssemblyPy::getJoints() const
{
    Py::List ret;
    std::vector<App::DocumentObject*> list = getMbDAssemblyPtr()->getJoints();

    for (auto It : list) {
        ret.append(Py::Object(It->getPyObject(), true));
    }

    return ret;
}

PyObject* MbDAssemblyPy::getDownstreamParts(PyObject* args) const
{
    PyObject* pyPart;
    PyObject* pyJoint;

    // Parse the two arguments: a part object and a joint object
    if (!PyArg_ParseTuple(
            args,
            "O!O!",
            &(App::DocumentObjectPy::Type),
            &pyPart,
            &(App::DocumentObjectPy::Type),
            &pyJoint
        )) {
        return nullptr;
    }

    auto* part = static_cast<App::DocumentObjectPy*>(pyPart)->getDocumentObjectPtr();
    auto* joint = static_cast<App::DocumentObjectPy*>(pyJoint)->getDocumentObjectPtr();

    // Call the C++ method
    std::vector<MbDFEM::ObjRef> downstreamParts
        = this->getMbDAssemblyPtr()->getDownstreamParts(part, joint);

    // Convert the result into a Python list of DocumentObjects
    Py::List ret;
    for (const auto& objRef : downstreamParts) {
        if (objRef.obj) {
            ret.append(Py::Object(objRef.obj->getPyObject(), true));
        }
    }

    return Py::new_reference_to(ret);
}
