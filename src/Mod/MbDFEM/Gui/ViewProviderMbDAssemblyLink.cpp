#include <QAction>
#include <QMenu>
#include <vector>
#include <sstream>
#include <iostream>


#include <App/Link.h>
#include <App/Document.h>
#include <App/DocumentObject.h>
#include <App/Part.h>

#include <Gui/Action.h>
#include <Gui/ActionFunction.h>
#include <Gui/Application.h>
#include <Gui/BitmapFactory.h>
#include <Gui/CommandT.h>
#include <Gui/MainWindow.h>

#include <Mod/MbDFEM/App/MbDAssembly.h>
#include <Mod/MbDFEM/App/MbDAssemblyLink.h>

#include "ViewProviderMbDFEM.h"
#include "ViewProviderMbDAssemblyLink.h"


using namespace MbDFEM;
using namespace MbDFEMGui;


PROPERTY_SOURCE(MbDFEMGui::ViewProviderMbDAssemblyLink, Gui::ViewProviderPart)

ViewProviderMbDAssemblyLink::ViewProviderMbDAssemblyLink()
{}

ViewProviderMbDAssemblyLink::~ViewProviderMbDAssemblyLink() = default;

QIcon ViewProviderMbDAssemblyLink::getIcon() const
{
    auto* linkObj = dynamic_cast<MbDFEM::MbDAssemblyLink*>(getObject());
    if (linkObj->isRigid()) {
        return Gui::BitmapFactory().pixmap("MbDFEM_MbDFEMLinkRigid.svg");
    }
    else {
        return Gui::BitmapFactory().pixmap("MbDFEM_MbDFEMLink.svg");
    }
}

bool ViewProviderMbDAssemblyLink::setEdit(int mode)
{
    auto* linkObj = dynamic_cast<MbDFEM::MbDAssemblyLink*>(getObject());

    if (!linkObj->isRigid() && mode == (int)ViewProvider::Transform) {
        Base::Console().userTranslatedNotification("Flexible sub-assemblies cannot be transformed.");
        return true;
    }

    return ViewProviderPart::setEdit(mode);
}

bool ViewProviderMbDAssemblyLink::doubleClicked()
{
    auto* link = freecad_cast<MbDAssemblyLink*>(getObject());
    if (!link) {
        return true;
    }
    auto* MbDFEM = link->getLinkedMbDFEM();
    if (!MbDFEM) {
        return true;
    }

    auto* vpa = freecad_cast<ViewProviderMbDFEM*>(
        Gui::Application::Instance->getViewProvider(MbDFEM)
    );
    if (!vpa) {
        return true;
    }

    auto doc = MbDFEM->getDocument();
    auto guiDoc = vpa->getDocument();
    if (!doc || !guiDoc) {
        return true;
    }

    Gui::MDIView* mdi = guiDoc->getActiveView();

    // Ensure the linked MbDFEM document is fully loaded and has a view
    if (doc->testStatus(App::Document::PartialDoc) || !mdi) {
        Gui::Application::Instance->reopen(doc);

        // reopening invalidates the pointer.
        auto* MbDFEM = link->getLinkedMbDFEM();
        if (!MbDFEM) {
            return true;
        }

        vpa = freecad_cast<ViewProviderMbDFEM*>(
            Gui::Application::Instance->getViewProvider(MbDFEM)
        );
        if (!vpa) {
            return true;
        }
    }

    return vpa->doubleClicked();
}

bool ViewProviderMbDAssemblyLink::onDelete(const std::vector<std::string>& subNames)
{
    Q_UNUSED(subNames)

    Gui::Command::doCommand(
        Gui::Command::Doc,
        "App.getDocument(\"%s\").getObject(\"%s\").removeObjectsFromDocument()",
        getObject()->getDocument()->getName(),
        getObject()->getNameInDocument()
    );

    // getObject()->purgeTouched();

    return ViewProviderPart::onDelete(subNames);
}

void ViewProviderMbDAssemblyLink::setupContextMenu(QMenu* menu, QObject* receiver, const char* member)
{
    auto func = new Gui::ActionFunction(menu);
    QAction* act;
    auto* linkObj = dynamic_cast<MbDFEM::MbDAssemblyLink*>(getObject());
    if (linkObj->isRigid()) {
        act = menu->addAction(QObject::tr("Turn flexible"));
        act->setToolTip(
            QObject::tr("Your sub-MbDFEM is currently rigid. This will make it flexible instead.")
        );
    }
    else {
        act = menu->addAction(QObject::tr("Turn rigid"));
        act->setToolTip(
            QObject::tr("Your sub-MbDFEM is currently flexible. This will make it rigid instead.")
        );
    }

    func->trigger(act, [this]() {
        auto* linkObj = dynamic_cast<MbDFEM::MbDAssemblyLink*>(getObject());
        getDocument()->openCommand(QT_TRANSLATE_NOOP("Command", "Toggle Rigid"));
        Gui::cmdAppObjectArgs(
            linkObj,
            "Rigid = %s",
            linkObj->Rigid.getValue() ? "False" : "True"
        );

        getDocument()->commitCommand();
        Gui::Selection().clearSelection();
    });

    Gui::CommandManager& mgr = Gui::Application::Instance->commandManager();
    Gui::Command* cmd = mgr.getCommandByName("MbDFEM_LinkSelectLinked");
    if (cmd && cmd->getAction()) {
        QAction* action = cmd->getAction()->action();
        if (action) {
            menu->addAction(action);
        }
    }

    Q_UNUSED(receiver)
    Q_UNUSED(member)
}
