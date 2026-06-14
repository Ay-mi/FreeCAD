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

import UtilsMbDFEM
import MbDFEM_rc

__title__ = "MbDFEM Command to Solve MbDFEM"
__author__ = "Ondsel"
__url__ = "https://www.freecad.org"


class CommandSolveMbDFEM:
    def __init__(self):
        pass

    def GetResources(self):

        return {
            "Pixmap": "MbDFEM_SolveMbDFEM",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_SolveMbDFEM", "Solve MbDFEM"),
            "Accel": "Z",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_SolveMbDFEM",
                "Solves the currently active MbDFEM.",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return UtilsMbDFEM.isMbDFEMCommandActive()

    def Activated(self):
        MbDFEM = UtilsMbDFEM.activeMbDFEM()
        if not MbDFEM:
            return

        App.setActiveTransaction("Solve MbDFEM")
        MbDFEM.recompute(True)
        Gui.ActiveDocument.commitCommand()


if App.GuiUp:
    Gui.addCommand("MbDFEM_SolveMbDFEM", CommandSolveMbDFEM())
