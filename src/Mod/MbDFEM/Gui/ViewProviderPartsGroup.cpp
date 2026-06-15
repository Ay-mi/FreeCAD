#include <App/Document.h>
#include <App/DocumentObject.h>
#include <Gui/Application.h>
#include <Gui/BitmapFactory.h>

#include "ViewProviderPartsGroup.h"


using namespace MbDFEMGui;

PROPERTY_SOURCE(MbDFEMGui::ViewProviderPartsGroup, Gui::ViewProviderDocumentObjectGroup)

ViewProviderPartsGroup::ViewProviderPartsGroup()
{}

ViewProviderPartsGroup::~ViewProviderPartsGroup() = default;

QIcon ViewProviderPartsGroup::getIcon() const
{
    //return Gui::BitmapFactory().pixmap("MbDFEM_PartsGroup.svg");
    
    return Gui::BitmapFactory().pixmap("MbDFEM_JointGroup.svg"); //using this svg for now

}
