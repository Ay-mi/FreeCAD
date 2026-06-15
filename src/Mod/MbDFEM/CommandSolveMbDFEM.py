import os
import FreeCAD as App

from PySide.QtCore import QT_TRANSLATE_NOOP

if App.GuiUp:
    import FreeCADGui as Gui
    from PySide import QtCore, QtGui, QtWidgets

import UtilsMbDFEM
import MbDFEM_rc

__title__ = "MbDFEM Command to Solve MbDFEM"
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
