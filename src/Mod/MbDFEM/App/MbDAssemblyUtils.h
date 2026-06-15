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


#pragma once

#include <GeomAbs_CurveType.hxx>
#include <GeomAbs_SurfaceType.hxx>

#include <Mod/MbDFEM/MbDFEMGlobal.h>

#include <App/FeaturePython.h>
#include <App/Part.h>

namespace App
{
class DocumentObject;
}  // namespace App

namespace Base
{
class Placement;
}  // namespace Base

namespace MbDFEM
{

// This enum has to be the same as the one in JointObject.py
enum class JointType
{
    Fixed,
    Revolute,
    Cylindrical,
    Slider,
    Ball,
    Distance,
    Parallel,
    Perpendicular,
    Angle,
    RackPinion,
    Screw,
    Gears,
    Belt,
};

enum class GeometryType
{
    Point = 0,

    // Edges
    Line = 1,
    Curve = 2,
    Circle = 3,

    // Faces
    Place = 4,
    Cylinder = 5,
    Sphere = 6,
    Cone = 7,
    Torus = 8,
};

enum class DistanceType
{
    PointPoint,

    LineLine,
    LineCircle,
    CircleCircle,

    PlanePlane,
    PlaneCylinder,
    PlaneSphere,
    PlaneCone,
    PlaneTorus,
    CylinderCylinder,
    CylinderSphere,
    CylinderCone,
    CylinderTorus,
    ConeCone,
    ConeTorus,
    ConeSphere,
    TorusTorus,
    TorusSphere,
    SphereSphere,

    PointPlane,
    PointCylinder,
    PointSphere,
    PointCone,
    PointTorus,

    LinePlane,
    LineCylinder,
    LineSphere,
    LineCone,
    LineTorus,

    CurvePlane,
    CurveCylinder,
    CurveSphere,
    CurveCone,
    CurveTorus,

    PointLine,
    PointCurve,

    Other,
};

class MbDAssembly;
class JointGroup;

MbDFEMExport void swapJCS(const App::DocumentObject* joint);

MbDFEMExport bool isEdgeType(
    const App::DocumentObject* obj,
    const std::string& elName,
    const GeomAbs_CurveType type
);
MbDFEMExport bool isFaceType(
    const App::DocumentObject* obj,
    const std::string& elName,
    const GeomAbs_SurfaceType type
);
MbDFEMExport double getFaceRadius(const App::DocumentObject* obj, const std::string& elName);
MbDFEMExport double getEdgeRadius(const App::DocumentObject* obj, const std::string& elName);

MbDFEMExport DistanceType getDistanceType(App::DocumentObject* joint);
MbDFEMExport JointGroup* getJointGroup(const App::Part* part);

MbDFEMExport std::vector<App::DocumentObject*> getMbDFEMComponents(const MbDAssembly* MbDFEM);

// getters to get from properties
MbDFEMExport void setJointActivated(const App::DocumentObject* joint, bool val);
MbDFEMExport bool getJointActivated(const App::DocumentObject* joint);
MbDFEMExport double getJointAngle(const App::DocumentObject* joint);
MbDFEMExport double getJointDistance(const App::DocumentObject* joint);
MbDFEMExport double getJointDistance2(const App::DocumentObject* joint);
MbDFEMExport JointType getJointType(const App::DocumentObject* joint);
MbDFEMExport std::string getElementFromProp(const App::DocumentObject* obj, const char* propName);
MbDFEMExport std::string getElementTypeFromProp(const App::DocumentObject* obj, const char* propName);
MbDFEMExport App::DocumentObject* getObjFromProp(
    const App::DocumentObject* joint,
    const char* propName
);
MbDFEMExport App::DocumentObject* getObjFromRef(App::DocumentObject* obj, const std::string& sub);
MbDFEMExport App::DocumentObject* getObjFromRef(const App::PropertyXLinkSub* prop);
MbDFEMExport App::DocumentObject* getObjFromJointRef(
    const App::DocumentObject* joint,
    const char* propName
);
MbDFEMExport App::DocumentObject* getLinkedObjFromRef(
    const App::DocumentObject* joint,
    const char* propName
);
// Get the moving part from a selection, which has the full path.
MbDFEMExport App::DocumentObject* getMovingPartFromSel(
    const MbDAssembly* MbDAssembly,
    App::DocumentObject* obj,
    const std::string& sub
);
MbDFEMExport App::DocumentObject* getMovingPartFromRef(const App::PropertyXLinkSub* prop);
MbDFEMExport App::DocumentObject* getMovingPartFromRef(App::DocumentObject* joint, const char* pName);
MbDFEMExport std::vector<std::string> getSubAsList(const App::PropertyXLinkSub* prop);
MbDFEMExport std::vector<std::string> getSubAsList(
    const App::DocumentObject* joint,
    const char* propName
);
MbDFEMExport void syncPlacements(App::DocumentObject* src, App::DocumentObject* to);
MbDFEMExport double getJointCurrentValue(App::DocumentObject* joint, bool isAngle);

}  // namespace MbDFEM
