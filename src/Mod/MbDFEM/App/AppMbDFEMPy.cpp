#include <Base/Interpreter.h>
#include <Base/Tools.h>


namespace MbDFEM
{
class Module: public Py::ExtensionModule<Module>
{
public:
    Module()
        : Py::ExtensionModule<Module>("MbDFEMApp")
    {
        initialize("This module is the MbDFEM module.");  // register with Python
    }
};

PyObject* initModule()
{
    return Base::Interpreter().addModule(new Module);
}

}  // namespace MbDFEM
