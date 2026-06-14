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


#pragma once

#include <unordered_map>

#include <Mod/MbDFEM/MbDFEMGlobal.h>

#include <App/FeaturePython.h>
#include <App/Part.h>
#include <App/PropertyLinks.h>


namespace MbDFEM
{
class MbDAssembly;
class JointGroup;

class MbDFEMExport MbDFEMLink: public App::Part
{
    PROPERTY_HEADER_WITH_OVERRIDE(MbDFEM::MbDFEMLink);

public:
    MbDFEMLink();
    ~MbDFEMLink() override;

    PyObject* getPyObject() override;

    /// returns the type name of the ViewProvider
    const char* getViewProviderName() const override
    {
        return "MbDFEMGui::ViewProviderMbDFEMLink";
    }

    App::DocumentObjectExecReturn* execute() override;

    // The linked MbDFEM is the MbDAssembly that this MbDFEMLink pseudo-links to recursively.
    MbDAssembly* getLinkedMbDFEM() const;
    // The parent MbDFEM is the main MbDFEM in which the linked MbDFEM is contained
    MbDAssembly* getParentMbDFEM() const;

    // Overriding DocumentObject::getLinkedObject is giving bugs
    // This function returns the linked object, either an MbDAssembly or an MbDFEMLink
    App::DocumentObject* getLinkedObject2(bool recurse = true) const;

    bool isRigid() const;

    /**
     * Update all of the components and joints from the MbDFEM
     */
    void updateContents();
    void updateParentJoints();

    void synchronizeComponents();
    void synchronizeJoints();
    void handleJointReference(
        App::DocumentObject* joint,
        App::DocumentObject* lJoint,
        const char* refName
    );
    void ensureNoJointGroup();
    JointGroup* ensureJointGroup();
    std::vector<App::DocumentObject*> getJoints();

    bool allowDuplicateLabel() const override;

    bool isEmpty() const;
    int numberOfComponents() const;

    App::PropertyXLink LinkedObject;
    App::PropertyBool Rigid;

    std::unordered_map<App::DocumentObject*, App::DocumentObject*> objLinkMap;

protected:
    /// get called by the container whenever a property has been changed
    void onChanged(const App::Property* prop) override;
};


}  // namespace MbDFEM
