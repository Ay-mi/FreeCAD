#include <App/Document.h>
#include <App/DocumentObject.h>
#include <Gui/Application.h>
#include <Gui/BitmapFactory.h>

#include "ViewProviderPartGroup.h"


using namespace MbDFEMGui;

PROPERTY_SOURCE(MbDFEMGui::ViewProviderPartGroup, Gui::ViewProviderDocumentObjectGroup)

ViewProviderPartGroup::ViewProviderPartGroup()
{}

ViewProviderPartGroup::~ViewProviderPartGroup() = default;

QIcon ViewProviderPartGroup::getIcon() const
{
    //return Gui::BitmapFactory().pixmap("MbDFEM_PartGroup.svg");
    
    return Gui::BitmapFactory().pixmap("MbDFEM_JointGroup.svg"); //using this svg for now

}
