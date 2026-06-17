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


class CommandSolveMbDAssembly:
    def __init__(self):
        pass

    def GetResources(self):

        return {
            "Pixmap": "MbDFEM_SolveMbDAssembly",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_SolveMbDAssembly", "Solve MbDAssembly"),
            "Accel": "Z",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_SolveMbDAssembly",
                "Solves the currently active MbDAssembly.",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        return UtilsMbDFEM.isMbDFEMCommandActive()

    def Activated(self):
        MbDFEM = UtilsMbDFEM.activeMbDFEM()
        if not MbDFEM:
            return

        App.setActiveTransaction("Solve MbDAssembly")
        MbDFEM.recompute(True)
        Gui.ActiveDocument.commitCommand()


if App.GuiUp:
    Gui.addCommand("MbDFEM_SolveMbDAssembly", CommandSolveMbDAssembly())
