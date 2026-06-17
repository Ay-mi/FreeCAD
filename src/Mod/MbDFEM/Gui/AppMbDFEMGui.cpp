#include <Base/Console.h>
#include <Base/Interpreter.h>
#include <Base/PyObjectBase.h>

#include "Commands.h"
#include "ViewProviderMbDAssembly.h"
#include "ViewProviderMbDAssemblyLink.h"
#include "ViewProviderMbDPart.h"
#include "ViewProviderBom.h"
#include "ViewProviderPartGroup.h"
#include "ViewProviderBomGroup.h"
#include "ViewProviderJointGroup.h"
#include "ViewProviderViewGroup.h"
#include "ViewProviderSimulationGroup.h"

namespace MbDFEMGui
{
extern PyObject* initModule();
}

/* Python entry */
PyMOD_INIT_FUNC(MbDFEMGui)
{
    // load dependent module
    try {
        Base::Interpreter().runString("import SpreadsheetGui");
    }
    catch (const Base::Exception& e) {
        PyErr_SetString(PyExc_ImportError, e.what());
        PyMOD_Return(nullptr);
    }

    PyObject* mod = MbDFEMGui::initModule();
    Base::Console().log("Loading MbDFEMGui module... done\n");

    MbDFEMGui::CreateMbDFEMCommands();

    // NOTE: To finish the initialization of our own type objects we must
    // call PyType_Ready, otherwise we run into a segmentation fault, later on.
    // This function is responsible for adding inherited slots from a type's base class.

    MbDFEMGui::ViewProviderMbDAssembly::init();
    MbDFEMGui::ViewProviderMbDAssemblyLink::init();
    MbDFEMGui::ViewProviderMbDPart::init();
    MbDFEMGui::ViewProviderBom::init();
    MbDFEMGui::ViewProviderPartGroup::init(); //registers ViewProviderPartGroup in FreeCAD global type system
    MbDFEMGui::ViewProviderBomGroup::init();
    MbDFEMGui::ViewProviderJointGroup::init();
    MbDFEMGui::ViewProviderViewGroup::init();
    MbDFEMGui::ViewProviderSimulationGroup::init();

    PyMOD_Return(mod);
}
