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

#include "Commands.h"
#include "ViewProviderMbDFEM.h"
#include "ViewProviderMbDAssemblyLink.h"
#include "ViewProviderMbDPart.h"
#include "ViewProviderBom.h"
#include "ViewProviderPartsGroup.h"
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

    MbDFEMGui::ViewProviderMbDFEM::init();
    MbDFEMGui::ViewProviderMbDAssemblyLink::init();
    MbDFEMGui::ViewProviderMbDPart::init();
    MbDFEMGui::ViewProviderBom::init();
    MbDFEMGui::ViewProviderPartsGroup::init();
    MbDFEMGui::ViewProviderBomGroup::init();
    MbDFEMGui::ViewProviderJointGroup::init();
    MbDFEMGui::ViewProviderViewGroup::init();
    MbDFEMGui::ViewProviderSimulationGroup::init();

    PyMOD_Return(mod);
}
