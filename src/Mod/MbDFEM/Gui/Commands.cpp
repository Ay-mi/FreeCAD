// SPDX - License - Identifier: LGPL - 2.1 - or -later
/****************************************************************************
 *                                                                          *
 *   Copyright (c) 2025 Pierre-Louis Boyer                                  *
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

#include <vector>

#include <App/Document.h>
#include <App/DocumentObject.h>
#include <Gui/Application.h>
#include <Gui/CommandT.h>
#include <Gui/Document.h>
#include <Gui/MainWindow.h>
#include <Gui/Selection/Selection.h>
#include <Gui/Tree.h>

#include <Mod/MbDFEM/App/MbDFEMLink.h>
#include <Mod/MbDFEM/App/MbDAssembly.h>
#include <Mod/MbDFEM/App/MbDFEMUtils.h>

#include "Commands.h"
#include "ViewProviderMbDFEM.h"


using namespace MbDFEM;
using namespace MbDFEMGui;

// Helper function to get the active MbDAssembly in edit mode
static MbDAssembly* getActiveMbDFEM()
{
    Gui::Document* doc = Gui::Application::Instance->activeDocument();
    if (!doc) {
        return nullptr;
    }

    auto* vp = doc->getInEdit();
    if (auto* MbDFEMVP = freecad_cast<ViewProviderMbDFEM*>(vp)) {
        return MbDFEMVP->getObject<MbDAssembly>();
    }

    return nullptr;
}

void selectObjects(const std::vector<App::DocumentObject*>& objectsToSelect)
{
    if (objectsToSelect.empty()) {
        return;
    }

    Gui::Selection().clearSelection();
    for (App::DocumentObject* obj : objectsToSelect) {
        Gui::Selection().addSelection(obj->getDocument()->getName(), obj->getNameInDocument());
    }
}

void selectObjectsByName(MbDAssembly* MbDFEM, const std::vector<std::string>& names)
{
    if (!MbDFEM || names.empty()) {
        return;
    }

    std::vector<App::DocumentObject*> objectsToSelect;
    App::Document* doc = MbDFEM->getDocument();

    for (const auto& name : names) {
        if (auto* obj = doc->getObject(name.c_str())) {
            objectsToSelect.push_back(obj);
        }
    }

    selectObjects(objectsToSelect);
}

// ================================================================================
// Go to Linked MbDFEM
// ================================================================================

DEF_STD_CMD_A(CmdMbDFEMLinkSelectLinked)

CmdMbDFEMLinkSelectLinked::CmdMbDFEMLinkSelectLinked()
    : Command("MbDFEM_LinkSelectLinked")
{
    sGroup = QT_TR_NOOP("MbDFEM");
    sMenuText = QT_TR_NOOP("Go to Linked MbDFEM");
    sToolTipText = QT_TR_NOOP("Selects the linked MbDFEM and switches to its original document");
    sWhatsThis = "MbDFEM_LinkSelectLinked";
    sStatusTip = sToolTipText;
    eType = AlterSelection;
    sPixmap = "LinkSelect";
    sAccel = "S, G";
}

void CmdMbDFEMLinkSelectLinked::activated(int iMsg)
{
    Q_UNUSED(iMsg);

    std::vector<Gui::SelectionObject> selection = Gui::Selection().getSelectionEx();
    if (selection.size() != 1) {
        return;
    }

    auto* asmLink = dynamic_cast<MbDFEM::MbDFEMLink*>(selection[0].getObject());

    if (!asmLink) {
        return;
    }

    // Get the linked object (usually an MbDAssembly in another doc)
    App::DocumentObject* linkedObj = asmLink->getLinkedMbDFEM();
    if (!linkedObj) {
        return;
    }

    Gui::Selection().clearSelection();
    Gui::Selection().addSelection(linkedObj->getDocument()->getName(), linkedObj->getNameInDocument());

    // Switch view/tab
    Gui::Document* guiDoc = Gui::Application::Instance->getDocument(linkedObj->getDocument());
    if (guiDoc) {
        // Try to activate the view containing the object
        Gui::ViewProvider* vp = guiDoc->getViewProvider(linkedObj);
        if (auto vpDoc = dynamic_cast<Gui::ViewProviderDocumentObject*>(vp)) {
            guiDoc->setActiveView(vpDoc);
        }
    }
}

bool CmdMbDFEMLinkSelectLinked::isActive()
{
    std::vector<Gui::SelectionObject> selection = Gui::Selection().getSelectionEx();
    return (
        selection.size() == 1 && selection[0].getObject()
        && selection[0].getObject()->isDerivedFrom(MbDFEM::MbDFEMLink::getClassTypeId())
    );
}


// ================================================================================
// Select Conflicting Constraints
// ================================================================================

DEF_STD_CMD_A(CmdMbDFEMSelectConflictingConstraints)

CmdMbDFEMSelectConflictingConstraints::CmdMbDFEMSelectConflictingConstraints()
    : Command("MbDFEM_SelectConflictingConstraints")
{
    sGroup = QT_TR_NOOP("MbDFEM");
    sMenuText = QT_TR_NOOP("Select Conflicting Constraints");
    sToolTipText = QT_TR_NOOP("Selects conflicting joints in the active MbDFEM");
    sWhatsThis = "MbDFEM_SelectConflictingConstraints";
    sStatusTip = sToolTipText;
    eType = ForEdit;
}

void CmdMbDFEMSelectConflictingConstraints::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    MbDAssembly* MbDFEM = getActiveMbDFEM();
    if (!MbDFEM) {
        return;
    }

    // NOTE: The solver currently reports conflicting constraints as redundant.
    // This uses the redundant list until the solver provides a separate conflicting list.
    selectObjectsByName(MbDFEM, MbDFEM->getLastRedundant());
}

bool CmdMbDFEMSelectConflictingConstraints::isActive()
{
    return getActiveMbDFEM() != nullptr;
}

// ================================================================================
// Select Redundant Constraints
// ================================================================================

DEF_STD_CMD_A(CmdMbDFEMSelectRedundantConstraints)

CmdMbDFEMSelectRedundantConstraints::CmdMbDFEMSelectRedundantConstraints()
    : Command("MbDFEM_SelectRedundantConstraints")
{
    sGroup = QT_TR_NOOP("MbDFEM");
    sMenuText = QT_TR_NOOP("Select Redundant Constraints");
    sToolTipText = QT_TR_NOOP("Selects redundant joints in the active MbDFEM");
    sWhatsThis = "MbDFEM_SelectRedundantConstraints";
    sStatusTip = sToolTipText;
    eType = ForEdit;
}

void CmdMbDFEMSelectRedundantConstraints::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    MbDAssembly* MbDFEM = getActiveMbDFEM();
    if (!MbDFEM) {
        return;
    }

    selectObjectsByName(MbDFEM, MbDFEM->getLastRedundant());
}

bool CmdMbDFEMSelectRedundantConstraints::isActive()
{
    return getActiveMbDFEM() != nullptr;
}

// ================================================================================
// Select Malformed Constraints
// ================================================================================

DEF_STD_CMD_A(CmdMbDFEMSelectMalformedConstraints)

CmdMbDFEMSelectMalformedConstraints::CmdMbDFEMSelectMalformedConstraints()
    : Command("MbDFEM_SelectMalformedConstraints")
{
    sGroup = QT_TR_NOOP("MbDFEM");
    sMenuText = QT_TR_NOOP("Select Malformed Constraints");
    sToolTipText = QT_TR_NOOP("Selects malformed joints in the active MbDFEM");
    sWhatsThis = "MbDFEM_SelectMalformedConstraints";
    sStatusTip = sToolTipText;
    eType = ForEdit;
}

void CmdMbDFEMSelectMalformedConstraints::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    MbDAssembly* MbDFEM = getActiveMbDFEM();
    if (!MbDFEM) {
        return;
    }

    selectObjectsByName(MbDFEM, MbDFEM->getLastMalformed());
}

bool CmdMbDFEMSelectMalformedConstraints::isActive()
{
    return getActiveMbDFEM() != nullptr;
}


// ================================================================================
// Select Components with Degrees of Freedom
// ================================================================================

DEF_STD_CMD_A(CmdMbDFEMSelectComponentsWithDoFs)

CmdMbDFEMSelectComponentsWithDoFs::CmdMbDFEMSelectComponentsWithDoFs()
    : Command("MbDFEM_SelectComponentsWithDoFs")
{
    sGroup = QT_TR_NOOP("MbDFEM");
    sMenuText = QT_TR_NOOP("Select Components With DoFs");
    sToolTipText = QT_TR_NOOP("Selects unconstrained components in the active MbDFEM");
    sWhatsThis = "MbDFEM_SelectComponentsWithDoFs";
    sStatusTip = sToolTipText;
    eType = ForEdit;
}

void CmdMbDFEMSelectComponentsWithDoFs::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    MbDAssembly* MbDFEM = getActiveMbDFEM();
    if (!MbDFEM) {
        return;
    }

    std::vector<App::DocumentObject*> objectsToSelect;
    std::vector<App::DocumentObject*> allParts = getMbDFEMComponents(MbDFEM);

    // Iterate through all collected parts and check their connectivity
    for (App::DocumentObject* part : allParts) {
        if (!MbDFEM->isPartConnected(part)) {
            objectsToSelect.push_back(part);
        }
    }

    selectObjects(objectsToSelect);
}

bool CmdMbDFEMSelectComponentsWithDoFs::isActive()
{
    return getActiveMbDFEM() != nullptr;
}

// ================================================================================
// Select Joints of Component
// ================================================================================

DEF_STD_CMD_A(CmdMbDFEMSelectJointsOfComponent)

CmdMbDFEMSelectJointsOfComponent::CmdMbDFEMSelectJointsOfComponent()
    : Command("MbDFEM_SelectJointsOfComponent")
{
    sGroup = QT_TR_NOOP("MbDFEM");
    sMenuText = QT_TR_NOOP("Select Component Joints");
    sToolTipText = QT_TR_NOOP("Selects all joints referencing the selected component");
    sWhatsThis = "MbDFEM_SelectJointsOfComponent";
    sStatusTip = sToolTipText;
    sPixmap = "MbDFEM_SelectJointsOfComponent";
    eType = ForEdit;
}

void CmdMbDFEMSelectJointsOfComponent::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    MbDAssembly* MbDFEM = getActiveMbDFEM();
    if (!MbDFEM) {
        return;
    }

    auto selection = Gui::Selection().getSelectionEx(
        "",
        App::DocumentObject::getClassTypeId(),
        Gui::ResolveMode::NoResolve
    );
    if (selection.empty()) {
        return;
    }

    std::set<App::DocumentObject*> components;

    for (auto& sel : selection) {
        const std::vector<std::string> subs = sel.getSubNames();
        std::string sub = subs.empty() ? "" : subs.front();

        if (App::DocumentObject* comp = getMovingPartFromSel(MbDFEM, sel.getObject(), sub)) {
            components.insert(comp);
        }
    }

    if (components.empty()) {
        return;
    }

    std::vector<App::DocumentObject*> jointsToSelect;
    for (auto* comp : components) {
        std::vector<App::DocumentObject*> partJoints = MbDFEM->getJointsOfPart(comp);
        jointsToSelect.insert(jointsToSelect.end(), partJoints.begin(), partJoints.end());
    }

    selectObjects(jointsToSelect);
}

bool CmdMbDFEMSelectJointsOfComponent::isActive()
{
    return getActiveMbDFEM() != nullptr && !Gui::Selection().getSelection().empty();
}


// ================================================================================
// Command Creation
// ================================================================================

void MbDFEMGui::CreateMbDFEMCommands()
{
    Gui::CommandManager& rcCmdMgr = Gui::Application::Instance->commandManager();

    rcCmdMgr.addCommand(new CmdMbDFEMLinkSelectLinked());
    rcCmdMgr.addCommand(new CmdMbDFEMSelectConflictingConstraints());
    rcCmdMgr.addCommand(new CmdMbDFEMSelectRedundantConstraints());
    rcCmdMgr.addCommand(new CmdMbDFEMSelectMalformedConstraints());
    rcCmdMgr.addCommand(new CmdMbDFEMSelectComponentsWithDoFs());
    rcCmdMgr.addCommand(new CmdMbDFEMSelectJointsOfComponent());
}
