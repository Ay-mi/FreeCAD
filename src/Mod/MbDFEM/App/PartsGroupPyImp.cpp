// inclusion of the generated files (generated out of PartsGroup.xml)
#include "PartsGroupPy.h"
#include "PartsGroupPy.cpp"

using namespace MbDFEM;

// returns a string which represents the object e.g. when printed in python
std::string PartsGroupPy::representation() const
{
    return {"<Parts Group>"};
}

PyObject* PartsGroupPy::getCustomAttributes(const char* /*attr*/) const
{
    return nullptr;
}

int PartsGroupPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}
