// SPDX-License-Identifier: LGPL-2.1-or-later
/****************************************************************************
 *                                                                          *
 *   Copyright (c) 2023 Ondsel <development@ondsel.com>                     *
 *                                                                          *
 *   This file is part of FreeCAD.                                          *
 *                                                                          *
 *   FreeCAD is free software: you can redistribute it and/or modify it     *
 *   under the terms of the GNU Lesser General Public License as            *
 *   published by the Free Software Foundation, either version 2.1 of the   *
 *   License, or (at your option) any later version.                        *
 *                                                                          *
 *   FreeCAD is distributed in the hope that it will be useful, but         *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of             *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU       *
 *   Lesser General Public License for more details.                        *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with FreeCAD. If not, see                                *
 *   <https://www.gnu.org/licenses/>.                                       *
 *                                                                          *
 ***************************************************************************/


#include <Base/Console.h>
#include <Base/Interpreter.h>
#include <Base/PyObjectBase.h>

#include "MbDAssembly.h"
#include "MbDAssemblyLink.h"
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
    MbDFEM::BomObject ::init();

    MbDFEM::BomGroup ::init();
    MbDFEM::JointGroup ::init();
    MbDFEM::ViewGroup ::init();
    MbDFEM::SimulationGroup ::init();

    PyMOD_Return(mod);
}
