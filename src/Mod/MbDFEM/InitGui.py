# SPDX-License-Identifier: LGPL-2.1-or-later
# /**************************************************************************
#                                                                           *
#    Copyright (c) 2023 Ondsel <development@ondsel.com>                     *
#                                                                           *
#    This file is part of FreeCAD.                                          *
#                                                                           *
#    FreeCAD is free software: you can redistribute it and/or modify it     *
#    under the terms of the GNU Lesser General Public License as            *
#    published by the Free Software Foundation, either version 2.1 of the   *
#    License, or (at your option) any later version.                        *
#                                                                           *
#    FreeCAD is distributed in the hope that it will be useful, but         *
#    WITHOUT ANY WARRANTY; without even the implied warranty of             *
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU       *
#    Lesser General Public License for more details.                        *
#                                                                           *
#    You should have received a copy of the GNU Lesser General Public       *
#    License along with FreeCAD. If not, see                                *
#    <https://www.gnu.org/licenses/>.                                       *
#                                                                           *
# **************************************************************************/

import MbDFEM_rc


class MbDFEMCommandGroup:
    def __init__(self, cmdlist, menu, tooltip=None):
        self.cmdlist = cmdlist
        self.menu = menu
        if tooltip is None:
            self.tooltip = menu
        else:
            self.tooltip = tooltip

    def GetCommands(self):
        return tuple(self.cmdlist)

    def GetResources(self):
        return {"MenuText": self.menu, "ToolTip": self.tooltip}

    def IsActive(self):
        if FreeCAD.ActiveDocument is not None:
            return True
        return False


class MbDFEMWorkbench(Workbench):
    "MbDFEM workbench"

    def __init__(self):
        self.__class__.Icon = (
            FreeCAD.getResourceDir() + "Mod/MbDFEM/Resources/icons/MbDFEMWorkbench.svg"
        )
        self.__class__.MenuText = "MbDFEM"
        self.__class__.ToolTip = "MbDFEM workbench"

    def Initialize(self):
        global MbDFEMCommandGroup

        translate = FreeCAD.Qt.translate

        # load the builtin modules
        import MbDFEMGui
        from PySide import QtCore, QtGui
        from PySide.QtCore import QT_TRANSLATE_NOOP
        import CommandCreateMbDAssembly, CommandInsertMbDLink, CommandInsertMbDNewPart, CommandCreateMbDJoint, CommandSolveMbDAssembly, CommandExportMbDASMT, CommandCreateMbDView, CommandCreateMbDSimulation, CommandCreateMbDBom
        import MbDFEMPreferences

        FreeCADGui.addLanguagePath(":/translations")
        FreeCADGui.addIconPath(":/icons")

        FreeCADGui.addPreferencePage(
            MbDFEMPreferences.PreferencesPage, QT_TRANSLATE_NOOP("QObject", "MbDFEM")
        )

        # build commands list
        cmdList = [
            "MbDFEM_CreateMbDAssembly",
            "MbDFEM_Insert",
            "MbDFEM_SolveMbDAssembly",
            "MbDFEM_CreateView",
            "MbDFEM_CreateSimulation",
            "MbDFEM_CreateBom",
        ]

        cmdListMenuOnly = [
            "MbDFEM_LinkSelectLinked",
            "MbDFEM_ExportASMT",
            "MbDFEM_SelectJointsOfComponent",
        ]

        cmdListJoints = [
            "MbDFEM_ToggleGrounded",
            "Separator",
            "MbDFEM_CreateJointFixed",
            "MbDFEM_CreateJointRevolute",
            "MbDFEM_CreateJointCylindrical",
            "MbDFEM_CreateJointSlider",
            "MbDFEM_CreateJointBall",
            "Separator",
            "MbDFEM_CreateJointDistance",
            "MbDFEM_CreateJointParallel",
            "MbDFEM_CreateJointPerpendicular",
            "MbDFEM_CreateJointAngle",
            "Separator",
            "MbDFEM_CreateJointRackPinion",
            "MbDFEM_CreateJointScrew",
            "MbDFEM_CreateJointGearBelt",
        ]

        self.appendToolbar(QT_TRANSLATE_NOOP("Workbench", "MbDFEM"), cmdList)
        self.appendToolbar(QT_TRANSLATE_NOOP("Workbench", "MbDFEM Joints"), cmdListJoints)

        self.appendMenu(
            [QT_TRANSLATE_NOOP("Workbench", "&MbDFEM")],
            cmdList + cmdListMenuOnly + ["Separator"] + cmdListJoints,
        )

    def Activated(self):
        # update the translation engine
        FreeCADGui.updateLocale()

        # Add task watchers to provide contextual tools in the task panel
        self.setWatchers()

    def Deactivated(self):
        FreeCADGui.Control.clearTaskWatcher()

    def ContextMenu(self, recipient):
        import UtilsMbDFEM

        MbDFEM = UtilsMbDFEM.activeMbDFEM()
        if MbDFEM is None:
            return

        selection = Gui.Selection.getSelectionEx("*", 0)
        if not selection:
            return

        for sel in selection:
            for sub_name in sel.SubElementNames:
                comp, new_sub = UtilsMbDFEM.getComponentReference(MbDFEM, sel.Object, sub_name)
                if comp:
                    self.appendContextMenu("", ["MbDFEM_SelectJointsOfComponent"])
                    return

    def setWatchers(self):
        import UtilsMbDFEM

        translate = FreeCAD.Qt.translate

        class MbDFEMCreateWatcher:
            """Shows 'Create MbDFEM' when no MbDFEM exists in the document."""

            def __init__(self):
                self.commands = ["MbDFEM_CreateMbDFEM"]
                self.title = translate("MbDFEM", "Create")

            def shouldShow(self):
                doc = FreeCAD.ActiveDocument

                if hasattr(doc, "RootObjects"):
                    for obj in doc.RootObjects:
                        if obj.isDerivedFrom("MbDFEM::MbDAssembly"):
                            return False
                return True

        class MbDFEMActivateWatcher:
            """Shows 'Activate MbDFEM' when an MbDFEM exists but is not active."""

            def __init__(self):
                self.commands = ["MbDFEM_ActivateMbDFEM"]
                self.title = translate("MbDFEM", "Activate")

            def shouldShow(self):
                doc = FreeCAD.ActiveDocument

                has_MbDFEM = False
                if hasattr(doc, "RootObjects"):
                    for obj in doc.RootObjects:
                        if obj.isDerivedFrom("MbDFEM::MbDAssembly"):
                            has_MbDFEM = True
                            break

                MbDFEM = UtilsMbDFEM.activeMbDFEM()

                return has_MbDFEM and (MbDFEM is None or MbDFEM.Document != doc)

        class MbDFEMBaseWatcher:
            """Base class for watchers that require an active MbDFEM."""

            def __init__(self):
                self.MbDFEM = None

            def shouldShow(self):
                doc = FreeCAD.ActiveDocument

                self.MbDFEM = UtilsMbDFEM.activeMbDFEM()
                return self.MbDFEM is not None and self.MbDFEM.Document == doc

        class MbDFEMInsertWatcher(MbDFEMBaseWatcher):
            """Shows 'Insert Component' when an MbDFEM is active."""

            def __init__(self):
                super().__init__()
                self.commands = ["MbDFEM_Insert"]
                self.title = translate("MbDFEM", "Insert")

            def shouldShow(self):
                return super().shouldShow()

        class MbDFEMGroundWatcher(MbDFEMBaseWatcher):
            """Shows 'Ground' when the active MbDFEM has no grounded parts."""

            def __init__(self):
                super().__init__()
                self.commands = ["MbDFEM_ToggleGrounded"]
                self.title = translate("MbDFEM", "Grounding")

            def shouldShow(self):
                if not super().shouldShow():
                    return False
                return (
                    UtilsMbDFEM.MbDFEM_has_at_least_n_parts(1)
                    and not UtilsMbDFEM.isMbDAssemblyGrounded()
                )

        class MbDFEMJointsWatcher(MbDFEMBaseWatcher):
            """Shows Joint, View, and BOM tools when there are enough parts."""

            def __init__(self):
                super().__init__()
                self.commands = [
                    "MbDFEM_CreateJointFixed",
                    "MbDFEM_CreateJointRevolute",
                    "MbDFEM_CreateJointCylindrical",
                    "MbDFEM_CreateJointSlider",
                    "MbDFEM_CreateJointBall",
                    "Separator",
                    "MbDFEM_CreateJointDistance",
                    "MbDFEM_CreateJointParallel",
                    "MbDFEM_CreateJointPerpendicular",
                    "MbDFEM_CreateJointAngle",
                ]
                self.title = translate("MbDFEM", "Constraints")

            def shouldShow(self):
                if not super().shouldShow():
                    return False
                return UtilsMbDFEM.MbDFEM_has_at_least_n_parts(2)

        class MbDFEMToolsWatcher(MbDFEMBaseWatcher):
            """Shows Joint, View, and BOM tools when there are enough parts."""

            def __init__(self):
                super().__init__()
                self.commands = [
                    "MbDFEM_CreateView",
                    "MbDFEM_CreateBom",
                ]
                self.title = translate("MbDFEM", "Tools")

            def shouldShow(self):
                if not super().shouldShow():
                    return False
                return UtilsMbDFEM.MbDFEM_has_at_least_n_parts(1)

        class MbDFEMSimulationWatcher(MbDFEMBaseWatcher):
            """Shows 'Create Simulation' when specific motional joints exist."""

            def __init__(self):
                super().__init__()
                self.commands = ["MbDFEM_CreateSimulation"]
                self.title = translate("MbDFEM", "Simulation")

            def shouldShow(self):
                if not super().shouldShow():
                    return False

                joint_types = ["Revolute", "Slider", "Cylindrical"]
                joints = UtilsMbDFEM.getJointsOfType(self.MbDFEM, joint_types)
                return len(joints) > 0

        watchers = [
            MbDFEMCreateWatcher(),
            MbDFEMActivateWatcher(),
            MbDFEMInsertWatcher(),
            MbDFEMGroundWatcher(),
            MbDFEMJointsWatcher(),
            MbDFEMToolsWatcher(),
            MbDFEMSimulationWatcher(),
        ]
        FreeCADGui.Control.addTaskWatcher(watchers)


Gui.addWorkbench(MbDFEMWorkbench())
