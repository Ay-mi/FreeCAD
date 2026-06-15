// inclusion of the generated files (generated out of MbDPart.xml)
#include "MbDPartPy.h"
#include "MbDPartPy.cpp"

using namespace MbDFEM;

std::string MbDPartPy::representation() const
{
    return {"<MbDPart>"};
}

PyObject* MbDPartPy::getCustomAttributes(const char* /*attr*/) const
{
    return nullptr;
}

int MbDPartPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}
