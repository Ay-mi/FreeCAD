#include <Gui/Application.h>
#include <Gui/BitmapFactory.h>
#include <Gui/Command.h>
// #include <Mod/MbDFEM/App/MbDAssembly.h>

#include "TaskMbDAssemblyMessages.h"
#include "ViewProviderMbDAssembly.h"

using namespace MbDFEMGui;
using namespace Gui::TaskView;
namespace sp = std::placeholders;

TaskMbDAssemblyMessages::TaskMbDAssemblyMessages(ViewProviderMbDAssembly* vp)
    : TaskSolverMessages(Gui::BitmapFactory().pixmap("GeoMbDFEM"), tr("Solver messages"))
    , vp(vp)
{
    // NOLINTBEGIN
    connectionSetUp = vp->signalSetUp.connect(
        std::bind(&TaskMbDAssemblyMessages::slotSetUp, this, sp::_1, sp::_2, sp::_3, sp::_4)
    );
    // NOLINTEND
}

TaskMbDAssemblyMessages::~TaskMbDAssemblyMessages()
{
    connectionSetUp.disconnect();
}

void TaskMbDAssemblyMessages::updateToolTip(const QString& link)
{
    if (link == QStringLiteral("#conflicting")) {
        setLinkTooltip(tr("Selects these conflicting joints"));
    }
    else if (link == QStringLiteral("#redundant")) {
        setLinkTooltip(tr("Selects these redundant joints"));
    }
    else if (link == QStringLiteral("#dofs")) {
        setLinkTooltip(
            tr("The MbDFEM has unconstrained components giving rise to those "
               "Degrees Of Freedom.\nSelects these unconstrained components.\nNote: Currently "
               "this selects only unconnected parts, not constrained parts that still have free "
               "DoF.")
        );
    }
    else if (link == QStringLiteral("#malformed")) {
        setLinkTooltip(tr("Selects these malformed joints"));
    }
}

void TaskMbDAssemblyMessages::onLabelStatusLinkClicked(const QString& str)
{
    if (str == QStringLiteral("#conflicting")) {
        Gui::Application::Instance->commandManager().runCommandByName(
            "MbDFEM_SelectConflictingConstraints"
        );
    }
    else if (str == QStringLiteral("#redundant")) {
        Gui::Application::Instance->commandManager().runCommandByName(
            "MbDFEM_SelectRedundantConstraints"
        );
    }
    else if (str == QStringLiteral("#dofs")) {
        Gui::Application::Instance->commandManager().runCommandByName(
            "MbDFEM_SelectComponentsWithDoFs"
        );
    }
    else if (str == QStringLiteral("#malformed")) {
        Gui::Application::Instance->commandManager().runCommandByName(
            "MbDFEM_SelectMalformedConstraints"
        );
    }
}

#include "moc_TaskMbDAssemblyMessages.cpp"
