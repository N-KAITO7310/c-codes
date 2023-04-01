#include <maya/MPxContext.h>
#include <maya/MSelectionList.h>


class JointCreateContext : public MPxContext
{
public:
    JointCreateContext();
    virtual ~JointCreateContext() override;

    virtual MStatus helpStateHasChanged(MEvent& event) override;

    virtual void toolOnSetup(MEvent& event) override;
    virtual void toolOffCleanup() override;

    virtual MStatus doRelease(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;

    virtual void completeAction() override;
    virtual void deleteAction() override;
    virtual void abortAction() override;


private:
    MStatus UpdateHelpString();

    void UpdateState();
    void ResetState();

    int mState;

    MSelectionList mContextSelection;
};
