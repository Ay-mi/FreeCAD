// inclusion of the generated files (generated out of MbDFEMLink.xml)
#include "MbDAssemblyLinkPy.h"
#include "MbDAssemblyLinkPy.cpp"

using namespace MbDFEM;

// returns a string which represents the object e.g. when printed in python
std::string MbDAssemblyLinkPy::representation() const
{
    return {"<MbDFEM link>"};
}

PyObject* MbDAssemblyLinkPy::getCustomAttributes(const char* /*attr*/) const
{
    return nullptr;
}

int MbDAssemblyLinkPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}

Py::List MbDAssemblyLinkPy::getJoints() const
{
    Py::List ret;
    std::vector<App::DocumentObject*> list = getMbDAssemblyLinkPtr()->getJoints();

    for (auto It : list) {
        ret.append(Py::Object(It->getPyObject(), true));
    }

    return ret;
}
