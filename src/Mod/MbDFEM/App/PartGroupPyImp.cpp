// inclusion of the generated files (generated out of PartGroup.xml)
#include "PartGroupPy.h"
#include "PartGroupPy.cpp"

using namespace MbDFEM;

// returns a string which represents the object e.g. when printed in python
std::string PartGroupPy::representation() const
{
    return {"<Parts Group>"};
}

PyObject* PartGroupPy::getCustomAttributes(const char* /*attr*/) const
{
    return nullptr;
}

int PartGroupPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}
