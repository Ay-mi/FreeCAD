// SPDX-License-Identifier: LGPL-2.1-or-later
/****************************************************************************
 *                                                                          *
 *   Copyright (c) 2024 Ondsel <development@ondsel.com>                     *
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

#include <App/Application.h>
#include <App/Document.h>
#include <App/FeaturePythonPyImp.h>
#include <App/PropertyPythonObject.h>
#include <Base/Console.h>
#include <Base/Placement.h>

#include "MbDPart.h"
#include "MbDPartPy.h"

using namespace MbDFEM;


PROPERTY_SOURCE(MbDFEM::MbDPart, App::GeoFeature)

MbDPart::MbDPart()
{
    ADD_PROPERTY_TYPE(
        cadPart,
        (nullptr),
        "General",
        (App::PropertyType)(App::Prop_None),
        "The CAD body or part whose motion this MbDPart tracks."
    );
}

MbDPart::~MbDPart() = default;

PyObject* MbDPart::getPyObject()
{
    if (PythonObject.is(Py::_None())) {
        PythonObject = Py::Object(new MbDPartPy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}

App::DocumentObjectExecReturn* MbDPart::execute()
{
    auto* obj = cadPart.getValue();
    if (!obj) {
        return nullptr;
    }

    auto* placementProperty =
        dynamic_cast<App::PropertyPlacement*>(obj->getPropertyByName("Placement"));
    if (placementProperty) {
        placementProperty->setValue(Placement.getValue());
    }

    return App::GeoFeature::execute();
}

App::DocumentObject* MbDPart::getCadPart() const
{
    return cadPart.getValue();
}
