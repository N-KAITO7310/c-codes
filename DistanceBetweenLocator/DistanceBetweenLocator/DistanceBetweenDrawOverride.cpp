#include "DistanceBetweenDrawOverride.h"

#include <algorithm>

#include <maya/MFnDependencyNode.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPlug.h>
#include <maya/MPointArray.h>
#include <maya/MRampAttribute.h>

#include "DistanceBetweenUserData.h"


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
DistanceBetweenDrawOverride::~DistanceBetweenDrawOverride()
{
}

MHWRender::DrawAPI DistanceBetweenDrawOverride::supportedDrawAPIs() const
{
    return(MHWRender::kAllDevices);
}

bool DistanceBetweenDrawOverride::hasUIDrawables() const
{
    return(true);
}

bool DistanceBetweenDrawOverride::refineSelectionPath(const MSelectionInfo& selectInfo, const MRenderItem& hitItem, MDagPath& path, MObject& geomComponents, MSelectionMask& objectMask)
{
    return(false);
}

MUserData* DistanceBetweenDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData)
{
    DistanceBetweenUserData* data = dynamic_cast<DistanceBetweenUserData*>(oldData);
    if(!oldData)
    {
        data = new DistanceBetweenUserData(false);
    }

    MFnDependencyNode nodeFn(objPath.node());

    data->point1 = MPoint(
        nodeFn.findPlug("point1X", false).asDouble(),
        nodeFn.findPlug("point1Y", false).asDouble(),
        nodeFn.findPlug("point1Z", false).asDouble()
    );

    data->point2 = MPoint(
        nodeFn.findPlug("point2X", false).asDouble(),
        nodeFn.findPlug("point2Y", false).asDouble(),
        nodeFn.findPlug("point2Z", false).asDouble()
    );

    data->distance = nodeFn.findPlug("distance", false).asDouble();

	double minDistance = nodeFn.findPlug("minDistance", false).asDouble();
	double maxDistance = nodeFn.findPlug("maxDistance", false).asDouble();

    double rampPosition = 0.0;
    if (minDistance < maxDistance)
    {
        rampPosition = (data->distance - minDistance) / (maxDistance - minDistance);
        rampPosition = std::min(std::max(0.0, rampPosition), 1.0);
    }

    MPlug colorRampPlug = nodeFn.findPlug("colorRamp", false);
    MRampAttribute colorRampAttribute(colorRampPlug);

    colorRampAttribute.getColorAtPosition(static_cast<float>(rampPosition), data->color);

    return(data);
}

void DistanceBetweenDrawOverride::addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* userData)
{
    const DistanceBetweenUserData* data = dynamic_cast<const DistanceBetweenUserData*>(userData);
    if(!data)
    {
        return;
    }

    drawManager.beginDrawable();

    drawManager.setFontSize(14);
    drawManager.setFontWeight(100);
    drawManager.setColor(data->color);

    MPoint position = MPoint((MVector(data->point1) + MVector(data->point2) / 2.0));
    MString text = MString() + (static_cast<int>(data->distance * 100) * 0.01f);

    drawManager.text(position, text, MUIDrawManager::kCenter);
    drawManager.line(data->point1, data->point2);

    drawManager.endDrawable();
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
MHWRender::MPxDrawOverride* DistanceBetweenDrawOverride::Creator(const MObject& obj)
{
    return(new DistanceBetweenDrawOverride(obj));
}


//-----------------------------------------------------------------------------
// PRIVATE METHODS
//-----------------------------------------------------------------------------
DistanceBetweenDrawOverride::DistanceBetweenDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, nullptr, true)
{
}
