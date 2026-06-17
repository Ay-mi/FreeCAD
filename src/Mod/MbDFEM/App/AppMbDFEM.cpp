#include <Base/Console.h>
#include <Base/Interpreter.h>
#include <Base/PyObjectBase.h>

#include "MbDAssembly.h"
#include "MbDAssemblyLink.h"
#include "MbDPart.h"
#include "PartGroup.h"
#include "BomObject.h"
#include "BomGroup.h"
#include "JointGroup.h"
#include "ViewGroup.h"
#include "SimulationGroup.h"


namespace MbDFEM
{
extern PyObject* initModule();
}

/* Python entry */
PyMOD_INIT_FUNC(MbDFEMApp)
{
    // load dependent module
    try {
        Base::Interpreter().runString("import Part");
        Base::Interpreter().runString("import Spreadsheet");
    }
    catch (const Base::Exception& e) {
        PyErr_SetString(PyExc_ImportError, e.what());
        PyMOD_Return(nullptr);
    }

    PyObject* mod = MbDFEM::initModule();
    Base::Console().log("Loading MbDFEM module... done\n");


    // NOTE: To finish the initialization of our own type objects we must
    // call PyType_Ready, otherwise we run into a segmentation fault, later on.
    // This function is responsible for adding inherited slots from a type's base class.

    MbDFEM::MbDAssembly ::init();
    MbDFEM::MbDAssemblyLink ::init();
    MbDFEM::MbDPart::init();
    MbDFEM::BomObject ::init();

    MbDFEM::PartGroup::init();
    MbDFEM::BomGroup ::init();
    MbDFEM::JointGroup ::init();
    MbDFEM::ViewGroup ::init();
    MbDFEM::SimulationGroup ::init();

    PyMOD_Return(mod);
}
