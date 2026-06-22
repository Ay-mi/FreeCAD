#include <App/Document.h>
#include <App/DocumentObject.h>
#include <Gui/Application.h>
#include <Gui/BitmapFactory.h>

#include "ViewProviderMbDPart.h"


using namespace MbDFEMGui;

PROPERTY_SOURCE(MbDFEMGui::ViewProviderMbDPart, PartGui::ViewProviderPart)

ViewProviderMbDPart::ViewProviderMbDPart()
{}

ViewProviderMbDPart::~ViewProviderMbDPart() = default;

QIcon ViewProviderMbDPart::getIcon() const
{
    return Gui::BitmapFactory().pixmap("MbDFEM_MbDPart.svg");
}
