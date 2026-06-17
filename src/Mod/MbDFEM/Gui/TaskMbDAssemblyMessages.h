#pragma once

#include <Gui/TaskView/TaskSolverMessages.h>


namespace MbDFEMGui
{

class ViewProviderMbDAssembly;

class TaskMbDAssemblyMessages: public Gui::TaskSolverMessages
{
    Q_OBJECT

public:
    explicit TaskMbDAssemblyMessages(ViewProviderMbDAssembly* vp);
    ~TaskMbDAssemblyMessages() override;

private:
    void onLabelStatusLinkClicked(const QString&) override;

    void updateToolTip(const QString& link) override;

protected:
    ViewProviderMbDAssembly* vp;
};

}  // namespace MbDFEMGui
