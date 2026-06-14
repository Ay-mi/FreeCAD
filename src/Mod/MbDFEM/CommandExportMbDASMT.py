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

import FreeCAD as App
import UtilsMbDFEM

from PySide.QtCore import QT_TRANSLATE_NOOP
from PySide.QtWidgets import QFileDialog

if App.GuiUp:
    import FreeCADGui as Gui


__title__ = "MbDFEM Command Create MbDFEM"
__author__ = "Ondsel"
__url__ = "https://www.freecad.org"


class CommandExportASMT:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_ExportASMT",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_ExportASMT", "Export ASMT File"),
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_ExportASMT",
                "Export currently active MbDFEM as a ASMT file.",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return UtilsMbDFEM.isMbDFEMCommandActive()

    def Activated(self):
        document = App.ActiveDocument
        if not document:
            return

        MbDFEM = UtilsMbDFEM.activeMbDFEM()
        if not MbDFEM:
            return

        # Prompt the user for a file location and name
        defaultFileName = document.Name + ".asmt"
        filePath, _ = QFileDialog.getSaveFileName(
            None,
            "Save ASMT File",
            defaultFileName,
            "ASMT Files (*.asmt);;All Files (*)",
        )

        if filePath:
            Gui.addModule("UtilsMbDFEM")
            Gui.doCommand("MbDFEM = UtilsMbDFEM.activeMbDFEM()")
            Gui.doCommand(f'MbDFEM.exportAsASMT("{filePath}")')


if App.GuiUp:
    Gui.addCommand("MbDFEM_ExportASMT", CommandExportASMT())
