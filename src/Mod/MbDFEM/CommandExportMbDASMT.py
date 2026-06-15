import FreeCAD as App
import UtilsMbDFEM

from PySide.QtCore import QT_TRANSLATE_NOOP
from PySide.QtWidgets import QFileDialog

if App.GuiUp:
    import FreeCADGui as Gui


__title__ = "MbDFEM Command Create MbDFEM"
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
