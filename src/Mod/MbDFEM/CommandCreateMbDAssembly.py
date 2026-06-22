import FreeCAD as App

from PySide.QtCore import QT_TRANSLATE_NOOP

if App.GuiUp:
    import FreeCADGui as Gui
    from PySide import QtCore, QtGui, QtWidgets

import UtilsMbDFEM
import MbDFEMPreferences as Preferences

translate = App.Qt.translate

__title__ = "MbDFEM Command Create MbDAssembly"

class CommandCreateMbDAssembly:
    def __init__(self):
        pass

    def GetResources(self):
        return {
            #"Pixmap": "GeoMbDFEM",
            "Pixmap": "MbDFEMWorkbench", #replace later if needed
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_CreateMbDAssembly", "New MbDAssembly"),
            "Accel": "A",
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_CreateMbDAssembly",
                "Creates an MbDAssembly in the current document, or in the current active MbDAssembly (if any). Limit of one root MbDAssembly per file.",
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        if Gui.Control.activeDialog():
            return False

        if Preferences.preferences().GetBool("EnforceOneMbDFEMRule", True):
            activeMbFEMAssm = UtilsMbDFEM.activeMbFEMAssm()

            if UtilsMbDFEM.isThereOneRootMbDFEM() and not activeMbFEMAssm:
                return False

        return App.ActiveDocument is not None

    def Activated(self):
        Gui.ActiveDocument.openCommand("New MbDAssembly")

        activeMbFEMAssm = UtilsMbDFEM.activeMbFEMAssm()
        Gui.addModule("UtilsMbDFEM")
        if activeMbFEMAssm:
            commands = (
                "activeMbFEMAssm = UtilsMbDFEM.activeMbFEMAssm()\n"
                'mbdFemAssembly = activeMbFEMAssm.newObject("MbDFEM::MbDAssembly", "MbDAssembly")\n'
            )
        else:
            commands = (
                'mbdFemAssembly = App.ActiveDocument.addObject("MbDFEM::MbDAssembly", "MbDAssembly")\n'
            )

        commands = commands + 'mbdFemAssembly.Type = "MbDAssembly"\n'
        commands = commands + 'mbdFemAssembly.newObject("MbDFEM::PartGroup", "Parts")\n' #creates PartGroup obj, looks up ViewProvider name
        commands = commands + 'mbdFemAssembly.newObject("MbDFEM::JointGroup", "Joints")'

        Gui.doCommand(commands)
        if not activeMbFEMAssm:
            Gui.doCommandGui("Gui.ActiveDocument.setEdit(mbdFemAssembly)")

        Gui.ActiveDocument.commitCommand()


class ActivateMbDFEMTaskPanel:
    """A basic TaskPanel to select an MbDFEM to activate."""

    def __init__(self, assemblies):
        self.assemblies = assemblies
        self.form = QtWidgets.QWidget()
        self.form.setWindowTitle(translate("MbDFEM_ActivateMbDFEM", "Activate MbDFEM"))

        layout = QtWidgets.QVBoxLayout(self.form)
        label = QtWidgets.QLabel(
            translate("MbDFEM_ActivateMbDFEM", "Select an MbDFEM to activate:")
        )
        self.combo = QtWidgets.QComboBox()

        for asm in self.assemblies:
            # Store the user-friendly Label for display, and the internal Name for activation
            self.combo.addItem(asm.Label, asm.Name)

        layout.addWidget(label)
        layout.addWidget(self.combo)

    def accept(self):
        """Called when the user clicks OK."""
        selected_name = self.combo.currentData()
        if selected_name:
            Gui.doCommand(f"Gui.ActiveDocument.setEdit('{selected_name}')")
        return True

    def reject(self):
        """Called when the user clicks Cancel or closes the panel."""
        return True


class CommandActivateMbDFEM:
    def __init__(self):
        self.task_panel = None

    def GetResources(self):
        return {
            "Pixmap": "MbDFEM_ActivateMbDFEM",
            "MenuText": QT_TRANSLATE_NOOP("MbDFEM_ActivateMbDFEM", "Activate MbDFEM"),
            "ToolTip": QT_TRANSLATE_NOOP(
                "MbDFEM_ActivateMbDFEM", "Sets an MbDFEM as the active one for editing."
            ),
            "CmdType": "ForEdit",
        }

    def IsActive(self):
        if Gui.Control.activeDialog() or App.ActiveDocument is None:
            return False

        # Command is only active if no MbDFEM is currently active
        if UtilsMbDFEM.activeMbFEMAssm() is not None:
            return False

        # And if there is at least one MbDFEM in the document to activate
        for obj in App.ActiveDocument.Objects:
            if obj.isDerivedFrom("MbDFEM::MbDAssembly"):
                return True

        return False

    def Activated(self):
        doc = App.ActiveDocument
        assemblies = [o for o in doc.Objects if o.isDerivedFrom("MbDFEM::MbDAssembly")]

        if len(assemblies) == 1:
            # If there's only one, activate it directly without showing a dialog
            Gui.doCommand(f"Gui.ActiveDocument.setEdit('{assemblies[0].Name}')")
        elif len(assemblies) > 1:
            # If there are multiple, show a task panel to let the user choose
            self.task_panel = ActivateMbDFEMTaskPanel(assemblies)
            Gui.Control.showDialog(self.task_panel)


if App.GuiUp:
    Gui.addCommand("MbDFEM_CreateMbDAssembly", CommandCreateMbDAssembly())
    Gui.addCommand("MbDFEM_ActivateMbDFEM", CommandActivateMbDFEM())
