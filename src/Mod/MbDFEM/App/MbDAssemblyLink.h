#pragma once

#include <unordered_map>

#include <Mod/MbDFEM/MbDFEMGlobal.h>

#include <App/FeaturePython.h>
#include <App/Part.h>
#include <App/PropertyLinks.h>


namespace MbDFEM
{
class MbDAssembly;
class JointGroup;

class MbDFEMExport MbDAssemblyLink: public App::Part
{
    PROPERTY_HEADER_WITH_OVERRIDE(MbDFEM::MbDAssemblyLink);

public:
    MbDAssemblyLink();
    ~MbDAssemblyLink() override;

    PyObject* getPyObject() override;

    /// returns the type name of the ViewProvider
    const char* getViewProviderName() const override
    {
        return "MbDFEMGui::ViewProviderMbDAssemblyLink";
    }

    App::DocumentObjectExecReturn* execute() override;

    // Returns the MbDAssembly that this MbDAssemblyLink pseudo-links to recursively.
    MbDAssembly* getLinkedMbDAssembly() const;
    // Returns the main MbDAssembly in which this link is contained.
    MbDAssembly* getParentMbDAssembly() const;

    // Overriding DocumentObject::getLinkedObject is giving bugs
    // This function returns the linked object, either an MbDAssembly or an MbDFEMLink
    App::DocumentObject* getLinkedObject2(bool recurse = true) const;

    bool isRigid() const;

    /**
     * Update all of the components and joints from the MbDFEM
     */
    void updateContents();
    void updateParentJoints();

    void synchronizeComponents();
    void synchronizeJoints();
    void handleJointReference(
        App::DocumentObject* joint,
        App::DocumentObject* lJoint,
        const char* refName
    );
    void ensureNoJointGroup();
    JointGroup* ensureJointGroup();
    std::vector<App::DocumentObject*> getJoints();

    bool allowDuplicateLabel() const override;

    bool isEmpty() const;
    int numberOfComponents() const;

    App::PropertyXLink LinkedObject;
    App::PropertyBool Rigid;

    std::unordered_map<App::DocumentObject*, App::DocumentObject*> objLinkMap;

protected:
    /// get called by the container whenever a property has been changed
    void onChanged(const App::Property* prop) override;
};


}  // namespace MbDFEM
