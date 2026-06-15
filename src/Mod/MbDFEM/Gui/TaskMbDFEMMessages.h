#pragma once

#include <Gui/TaskView/TaskSolverMessages.h>


namespace MbDFEMGui
{

class ViewProviderMbDFEM;

class TaskMbDFEMMessages: public Gui::TaskSolverMessages
{
    Q_OBJECT

public:
    explicit TaskMbDFEMMessages(ViewProviderMbDFEM* vp);
    ~TaskMbDFEMMessages() override;

private:
    void onLabelStatusLinkClicked(const QString&) override;

    void updateToolTip(const QString& link) override;

protected:
    ViewProviderMbDFEM* vp;
};

}  // namespace MbDFEMGui
