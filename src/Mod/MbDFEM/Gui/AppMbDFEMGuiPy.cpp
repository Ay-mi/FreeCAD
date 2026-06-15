#include <Base/Interpreter.h>


namespace MbDFEMGui
{
class Module: public Py::ExtensionModule<Module>
{
public:
    Module()
        : Py::ExtensionModule<Module>("MbDFEMGui")
    {
        initialize("This module is the MbDFEM module.");  // register with Python
    }
};

PyObject* initModule()
{
    return Base::Interpreter().addModule(new Module);
}

}  // namespace MbDFEMGui
