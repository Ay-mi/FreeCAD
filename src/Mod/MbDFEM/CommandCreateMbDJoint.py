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

import os
import FreeCAD as App

from PySide.QtCore import QT_TRANSLATE_NOOP

if App.GuiUp:
    import FreeCADGui as Gui
    from PySide import QtCore, QtGui, QtWidgets

import MbDJointObject as JointObject
from MbDJointObject import TaskMbDFEMCreateJoint
import UtilsMbDFEM
import MbDFEM_rc

# translate = App.Qt.translate

__title__ = "MbDFEM Commands to Create Joints"
__author__ = "Ondsel"
__url__ = "https://www.freecad.org"


def noOtherTaskActive():
    return UtilsMbDFEM.isMbDFEMCommandActive() or JointObject.activeTask is not None


def isCreateJointActive():
    return UtilsMbDFEM.MbDFEM_has_at_least_n_parts(1) and noOtherTaskActive()


def activateJoint(index):
    if JointObject.activeTask:
        JointObject.activeTask.reject()

    Gui.addModule("MbDJointObject")  # NOLINT
    Gui.doCommand(f"panel = MbDJointObject.TaskMbDFEMCreateJoint({index})")
    Gui.doCommandGui("dialog = Gui.Control.showDialog(panel)")
    dialog = Gui.doCommandEval("dialog")
    if dialog is not None:
        dialog.setAutoCloseOnTransactionChange(True)
        dialog.setAutoCloseOnDeletedDocument(True)
        dialog.setDocumentName(App.ActiveDocument.Name)


class CommandCreateJointFixed:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointFixed",
            "MenuText": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointFixed",
                "Fixed Joint",
            ),
            "Accel": "F",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointFixed",
                "<p>1 - If an MbDFEM is active : Creates a joint statically locking two parts together, preventing any movement or rotation</p>"
                "<p>2 - If a part is active: Positions sub-parts by matching selected coordinate systems. The second part selected will move.</p>",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        if UtilsMbDFEM.activePart() is not None:
            return UtilsMbDFEM.MbDFEM_has_at_least_n_parts(2)

        return isCreateJointActive()

    def Activated(self):
        activateJoint(0)


class CommandCreateJointRevolute:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointRevolute",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointRevolute", "Revolute Joint"),
            "Accel": "R",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointRevolute",
                "Creates a revolute joint allowing rotation around a single axis between selected parts",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(1)


class CommandCreateJointCylindrical:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointCylindrical",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointCylindrical", "Cylindrical Joint"),
            "Accel": "C",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointCylindrical",
                "Creates a cylindrical joint that allows rotation around and translation along a single axis between assembled parts",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(2)


class CommandCreateJointSlider:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointSlider",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointSlider", "Slider Joint"),
            "Accel": "S",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointSlider",
                "Creates a slider joint that allows linear movement along a single axis, but restricts rotation between selected parts",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(3)


class CommandCreateJointBall:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointBall",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointBall", "Ball Joint"),
            "Accel": "B",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointBall",
                "Creates a ball joint that connects parts at a point, allowing unrestricted movement as long as the connection points remain in contact",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(4)


class CommandCreateJointDistance:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointDistance",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointDistance", "Distance Joint"),
            "Accel": "D",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointDistance",
                "<p>Creates a distance joint that fixes the distance between the selected objects</p>"
                "<p>Creates one of several different joints based on the selection. "
                "For example, a distance of 0 between a plane and a cylinder creates a tangent joint. A distance of 0 between planes will make them co-planar.</p>",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(5)


class CommandCreateJointParallel:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointParallel",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointParallel", "Parallel Joint"),
            "Accel": "N",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointParallel",
                "Creates a parallel joint that makes the Z-axis of the selected coordinate systems parallel",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(6)


class CommandCreateJointPerpendicular:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointPerpendicular",
            "MenuText": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointPerpendicular", "Perpendicular Joint"
            ),
            "Accel": "M",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointPerpendicular",
                "Creates a perpendicular joint that makes the Z-axis of the selected coordinate systems perpendicular",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(7)


class CommandCreateJointAngle:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointAngle",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointAngle", "Angle Joint"),
            "Accel": "X",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointAngle",
                "Creates an angle joint that fixes the angle between the Z-axis of the selected coordinate systems",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(8)


class CommandCreateJointRackPinion:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointRackPinion",
            "MenuText": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointRackPinion", "Rack and Pinion Joint"
            ),
            "Accel": "Q",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointRackPinion",
                "<p>Creates a rack and pinion joint that links a part with a slider joint to a part with a revolute joint</p>"
                "<p>Select the same coordinate systems as the revolute and slider joints. The pitch radius defines the movement ratio between the rack and the pinion.</p>",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(9)


class CommandCreateJointScrew:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointScrew",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointScrew", "Screw Joint"),
            "Accel": "W",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointScrew",
                "<p>Creates a screw joint that links a part with a slider joint to a part with a revolute joint</p>"
                "<p>Select the same coordinate systems as the revolute and slider joints. The pitch radius defines the movement ratio between the rotating screw and the sliding part.</p>",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(10)


class CommandCreateJointGears:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointGears",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointGears", "Gears Joint"),
            "Accel": "T",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointGears",
                "<p>Creates a gears joint that links 2 rotating gears together. They will have inverse rotation direction.</p>"
                "<p>Select the same coordinate systems as the revolute joints.</p>",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(11)


class CommandCreateJointBelt:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_CreateJointPulleys",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointBelt", "Belt Joint"),
            "Accel": "L",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointBelt",
                "<p>Creates a belt joint that links 2 rotating objects together. They will have the same rotation direction.</p>"
                "<p>Select the same coordinate systems as the revolute joints.</p>",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()

    def Activated(self):
        activateJoint(12)


class CommandGroupGearBelt:
    def GetCommands(self):
        return ("MbDFEM_CreateJointGears", "MbDFEM_CreateJointBelt")

    def GetResources(self):
        """Set icon, menu and tooltip."""
        return {
            "Pixmap": "MbDFEM_CreateJointGears",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateJointGearBelt", "Gears/Belt Joint"),
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateJointGearBelt",
                "<p>Creates a gears or belt joint that links 2 rotating gears together</p>"
                "<p>Select the same coordinate systems as the revolute joints.</p>",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return isCreateJointActive()


def createGroundedJoint(obj):
    if not UtilsMbDFEM.activeMbDFEM():
        return

    Gui.addModule("UtilsMbDFEM")
    Gui.addModule("MbDJointObject")
    commands = (
        f'obj = App.ActiveDocument.getObject("{obj.Name}")\n'
        "MbDFEM = UtilsMbDFEM.activeMbDFEM()\n"
        "joint_group = UtilsMbDFEM.getJointGroup(MbDFEM)\n"
        'ground = joint_group.newObject("App::FeaturePython", "GroundedJoint")\n'
        "MbDJointObject.GroundedJoint(ground, obj)"
    )
    Gui.doCommand(commands)
    Gui.doCommandGui("MbDJointObject.ViewProviderGroundedJoint(ground.ViewObject)")

    Gui.doCommand("UtilsMbDFEM.activeMbDFEM().Document.recompute()")
    return Gui.doCommandEval("ground")


class CommandToggleGrounded:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_ToggleGrounded",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_ToggleGrounded", "Toggle Grounded"),
            "Accel": "G",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_ToggleGrounded",
                "<p>Toggles the grounding of a part.</p>"
                "<p>Grounding a part permanently locks its position in the MbDFEM, preventing any movement or rotation.",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return (
            UtilsMbDFEM.isMbDFEMCommandActive()
            and UtilsMbDFEM.MbDFEM_has_at_least_n_parts(1)
        )

    def Activated(self):
        MbDFEM = UtilsMbDFEM.activeMbDFEM()
        if not MbDFEM:
            return

        joint_group = UtilsMbDFEM.getJointGroup(MbDFEM)

        selection = Gui.Selection.getSelectionEx("*", 0)
        if not selection:
            return

        App.ActiveDocument.openTransaction("Toggle grounded")
        for sel in selection:
            # If you select 2 solids (bodies for example) within an MbDFEM.
            # There'll be a single sel but 2 SubElementNames.
            for sub in sel.SubElementNames:
                # First check if selection is a grounded object
                resolved = sel.Object.resolveSubElement(sub)
                if resolved:
                    obj = resolved[0]
                    if hasattr(obj, "ObjectToGround"):
                        commands = (
                            "doc = App.ActiveDocument\n"
                            f'doc.removeObject("{obj.Name}")\n'
                            "doc.recompute()\n"
                        )
                        Gui.doCommand(commands)
                        continue

                moving_part, new_sub = UtilsMbDFEM.getComponentReference(
                    MbDFEM, sel.Object, sub
                )
                if not moving_part:
                    continue

                # Only objects within the MbDFEM.
                if moving_part is None:
                    continue

                # Check if part is grounded and if so delete the joint.
                ungrounded = False
                for joint in joint_group.Group:
                    if hasattr(joint, "ObjectToGround") and joint.ObjectToGround == moving_part:
                        commands = (
                            "doc = App.ActiveDocument\n"
                            f'doc.removeObject("{joint.Name}")\n'
                            "doc.recompute()\n"
                        )
                        Gui.doCommand(commands)
                        ungrounded = True
                        break
                if ungrounded:
                    continue

                # Create groundedJoint.
                createGroundedJoint(moving_part)
        App.ActiveDocument.commitTransaction()


if App.GuiUp:
    Gui.addCommand("MbDFEM_ToggleGrounded", CommandToggleGrounded())
    Gui.addCommand("MbDFEM_CreateJointFixed", CommandCreateJointFixed())
    Gui.addCommand("MbDFEM_CreateJointRevolute", CommandCreateJointRevolute())
    Gui.addCommand("MbDFEM_CreateJointCylindrical", CommandCreateJointCylindrical())
    Gui.addCommand("MbDFEM_CreateJointSlider", CommandCreateJointSlider())
    Gui.addCommand("MbDFEM_CreateJointBall", CommandCreateJointBall())
    Gui.addCommand("MbDFEM_CreateJointDistance", CommandCreateJointDistance())
    Gui.addCommand("MbDFEM_CreateJointParallel", CommandCreateJointParallel())
    Gui.addCommand("MbDFEM_CreateJointPerpendicular", CommandCreateJointPerpendicular())
    Gui.addCommand("MbDFEM_CreateJointAngle", CommandCreateJointAngle())
    Gui.addCommand("MbDFEM_CreateJointRackPinion", CommandCreateJointRackPinion())
    Gui.addCommand("MbDFEM_CreateJointScrew", CommandCreateJointScrew())
    Gui.addCommand("MbDFEM_CreateJointGears", CommandCreateJointGears())
    Gui.addCommand("MbDFEM_CreateJointBelt", CommandCreateJointBelt())
    Gui.addCommand("MbDFEM_CreateJointGearBelt", CommandGroupGearBelt())
