#include "JointCreateContext.h"

#include <maya/MFnTransform.h>
#include <maya/MGlobal.h>
#include <maya/MUIDrawManager.h>
#include <maya/MVector.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MString TITLE = "Joint Create Context";

static MString HELP_TEXT[] = {
    MString("Select first joint location"),
    MString("Select second joint location"),
    MString("Select final joint location"),
    MString("Press Enter to complete")
};


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
JointCreateContext::JointCreateContext() :
    MPxContext(),
    mState(0)
{
    setTitleString(TITLE);
    // setImage("D:/Sandbox/context_image.png", MPxContext::kImage1);
}

JointCreateContext::~JointCreateContext()
{
}

MStatus JointCreateContext::helpStateHasChanged(MEvent& event)
{
    return(UpdateHelpString());
}

void JointCreateContext::toolOnSetup(MEvent& event)
{
    MGlobal::selectCommand(MSelectionList());
    ResetState();
}

void JointCreateContext::toolOffCleanup()
{
    ResetState();
}

MStatus JointCreateContext::doRelease(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
    if(mState >= 0 && mState < 3)
    {
        short x, y;
        event.getPosition(x, y);
        MGlobal::selectFromScreen(x, y, MGlobal::kReplaceList);

        MSelectionList activeSelection;
        MGlobal::getActiveSelectionList(activeSelection);

        if(activeSelection.length() == 1)
        {
            mContextSelection.merge(activeSelection);
        }

        MGlobal::setActiveSelectionList(mContextSelection, MGlobal::kReplaceList);

        UpdateState();
    }

    return(MS::kSuccess);
}

void JointCreateContext::completeAction()
{
    const int selectionCount = mContextSelection.length();
    if(selectionCount == 3)
    {
        MGlobal::setActiveSelectionList(MSelectionList());

        MObject obj;
        MFnTransform transformFn;
        for(int i = 0; i < selectionCount; i++)
        {
            mContextSelection.getDependNode(i, obj);
            transformFn.setObject(obj);

            MVector position = transformFn.getTranslation(MSpace::kTransform);
            MGlobal::executeCommand(MString("joint -position ") + position.x + " " + position.y + " " + position.z, false, true);

            MGlobal::executeCommand(MString("delete ") + transformFn.name(), false, true);
        }

        MGlobal::selectCommand(MSelectionList(), MGlobal::kReplaceList);

        ResetState();
    }
    else
    {
        MGlobal::displayError("Three objects must be selected");
    }
}

void JointCreateContext::deleteAction()
{
    const int selectionCount = mContextSelection.length();
    if(selectionCount > 0)
    {
        mContextSelection.remove(selectionCount - 1);

        MGlobal::setActiveSelectionList(mContextSelection, MGlobal::kReplaceList);

        UpdateState();
    }
}

void JointCreateContext::abortAction()
{
    ResetState();
}


//-----------------------------------------------------------------------------
// PRIVATE METHODS
//-----------------------------------------------------------------------------
MStatus JointCreateContext::UpdateHelpString()
{
    return(setHelpString(HELP_TEXT[mState]));
}

void JointCreateContext::UpdateState()
{
    mState = mContextSelection.length();

    UpdateHelpString();
}

void JointCreateContext::ResetState()
{
    MGlobal::setActiveSelectionList(MSelectionList());

    mContextSelection.clear();
    UpdateState();
}
