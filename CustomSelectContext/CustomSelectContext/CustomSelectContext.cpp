#include "CustomSelectContext.h"

#include <maya/MColor.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MPoint.h>
#include <maya/MSelectionList.h>
#include <maya/MUIDrawManager.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MString TITLE = "Custom Select Context";

static const MString HELP_TEXT = "Ctrl to select only meshes. Ctrl+Shift to select only lights.";


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
CustomSelectContext::CustomSelectContext() :
    MPxContext(),
    mViewportStartX(0),
    mViewportStartY(0),
    mViewportEndX(0),
    mViewportEndY(0),
    mLightsOnly(false),
    mMeshesOnly(false)
{
    setTitleString(TITLE);
    setImage("D:/Sandbox/context_image.png", MPxContext::kImage1);
}

CustomSelectContext::~CustomSelectContext()
{
}

MStatus CustomSelectContext::helpStateHasChanged(MEvent& event)
{
    return(setHelpString(HELP_TEXT));
}

MStatus CustomSelectContext::doPress(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
    event.getPosition(mViewportStartX, mViewportStartY);

    mLightsOnly = false;
    mMeshesOnly = false;

    if(event.isModifierControl())
    {
        if(event.isModifierShift())
        {
            mMeshesOnly = true;
        }
        else
        {
            mLightsOnly = true;
        }
    }

    return(MS::kSuccess);
}

MStatus CustomSelectContext::doRelease(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
    event.getPosition(mViewportEndX, mViewportEndY);

    MSelectionList initialSelection;
    MGlobal::getActiveSelectionList(initialSelection);

    MGlobal::selectFromScreen(mViewportStartX, mViewportStartY, mViewportEndX, mViewportEndY, MGlobal::kReplaceList);

    MSelectionList selection;
    MGlobal::getActiveSelectionList(selection);

    MObject obj;
    MObject shape;

    if(mLightsOnly || mMeshesOnly)
    {
        for(int i = selection.length() - 1; i >= 0; i--)
        {
            selection.getDependNode(i, obj);
            shape = MFnDagNode(obj).child(0);

            if(mLightsOnly && !shape.hasFn(MFn::kLight))
            {
                selection.remove(i);
            }
            else if(mMeshesOnly && !shape.hasFn(MFn::kMesh))
            {
                selection.remove(i);
            }
        }
    }

    MGlobal::setActiveSelectionList(initialSelection, MGlobal::kReplaceList);
    MGlobal::selectCommand(selection, MGlobal::kReplaceList);

    return(MS::kSuccess);
}

MStatus CustomSelectContext::doDrag(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
    event.getPosition(mViewportEndX, mViewportEndY);

    DrawSelectionRectangle(
        drawMgr,
        mViewportStartX, mViewportStartY,
        mViewportEndX, mViewportStartY,
        mViewportEndX, mViewportEndY,
        mViewportStartX, mViewportEndY
    );

    return(MS::kSuccess);
}


//-----------------------------------------------------------------------------
// PRIVATE METHODS
//-----------------------------------------------------------------------------
void CustomSelectContext::DrawSelectionRectangle(MHWRender::MUIDrawManager& drawMgr, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
    drawMgr.beginDrawable();

    drawMgr.setLineWidth(1.0f);
    drawMgr.setColor(MColor(1.0, 0.0, 0.0));

    drawMgr.line2d(MPoint(x0, y0), MPoint(x1, y1));
    drawMgr.line2d(MPoint(x1, y1), MPoint(x2, y2));
    drawMgr.line2d(MPoint(x2, y2), MPoint(x3, y3));
    drawMgr.line2d(MPoint(x3, y3), MPoint(x0, y0));

    drawMgr.endDrawable();
}
