#include "SwitchableShapeDrawOverride.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPlug.h>
#include <maya/MPointArray.h>
#include <maya/MFnTransform.h>

#include "SwitchableShapeUserData.h"

//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
SwitchableShapeDrawOverride::~SwitchableShapeDrawOverride()
{
}

MHWRender::DrawAPI SwitchableShapeDrawOverride::supportedDrawAPIs() const
{
	return(MHWRender::kAllDevices);
}

bool SwitchableShapeDrawOverride::hasUIDrawables() const
{
	return(true);
}

MUserData* SwitchableShapeDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData)
{
	SwitchableShapeUserData* data = dynamic_cast<SwitchableShapeUserData*>(oldData);
	if (!data)
	{
		data = new SwitchableShapeUserData(false);
	}

	MObject locatorObj = objPath.node();
	MFnDependencyNode nodeFn(locatorObj);

	data->shapeIndex = nodeFn.findPlug("shapeIndex", false).asInt();
	data->radius = nodeFn.findPlug("radius", false).asDouble();
	data->fontSize = nodeFn.findPlug("fontSize", false).asInt();
	data->fontWeight = nodeFn.findPlug("fontWeight", false).asInt();
	data->displayString = nodeFn.findPlug("notes", false).asString();

	MHWRender::DisplayStatus displayStatus = MGeometryUtilities::displayStatus(objPath);
	if (displayStatus == MHWRender::kDormant)
	{
		data->wireframeColor = MColor(0.0f, 0.1f, 0.0f);
	}
	else
	{
		data->wireframeColor = MGeometryUtilities::wireframeColor(objPath);
	}

	return(data);
}

void SwitchableShapeDrawOverride::addUIDrawables(const MDagPath& objPath, MUIDrawManager& drawManager, const MFrameContext& frameContext, const MUserData* data)
{
	const SwitchableShapeUserData* userData = dynamic_cast<const SwitchableShapeUserData*>(data);
	if (!userData)
	{
		return;
	}

	drawManager.beginDrawable();

	drawManager.setColor(userData->wireframeColor);
	drawManager.setFontSize(userData->fontSize);
	drawManager.setFontWeight(userData->fontWeight);

	MFnTransform objPathTransFn(objPath);
	MVector stringPosVec = objPathTransFn.getTranslation(MSpace::kWorld);
	MPoint stringPos(stringPosVec);
	drawManager.text(stringPos, userData->displayString, MUIDrawManager::kCenter);

	int shapeIndex = userData->shapeIndex;
	MPointArray pointArray;
	double radius = userData->radius;
	double height = radius * 2;
	MPoint center(0.0, 0.0, 0.0);
	MVector upVec(0.0, 1.0, 0.0);

	switch (shapeIndex)
	{
	case 0:
		// circle
		drawManager.circle(center, upVec, radius, false);
		break;
	case 1:
		// rectangle
		drawManager.rect(center, MVector(0.0, 0.0, 1.0), upVec, radius, radius, false);
		break;
	case 2:
		// triangle
		pointArray.append(MPoint(-radius, 0.0, -radius));
		pointArray.append(MPoint(0.0, 0.0, radius));
		pointArray.append(MPoint(radius, 0.0, -radius));
		pointArray.append(MPoint(-radius, 0.0, -radius));

		drawManager.lineStrip(pointArray, false);
		break;
	case 3:
		// arc
		drawManager.arc(center, MVector(0.0, 0.0, radius), MVector(0.0, 0.0, -radius), upVec, radius / 2, false);
		break;
	case 4:
		// cone
		drawManager.cone(center, MVector(1.0, 0.0, 0.0), radius, height*2, false);
		break;
	case 5:
		// cylinder
		drawManager.cylinder(center, upVec, radius, height, 6, false);
		break;
	case 6:
		// capsule
		drawManager.capsule(center, upVec, radius, height, 8, 6, false);
		break;
	case 7:
		// box
		drawManager.box(center, upVec, MVector(1.0, 0.0, 0.0), radius, radius, radius, false);
		break;
	case 8:
		drawManager.sphere(center, radius, false);
		break;
	default:
		drawManager.circle(center, upVec, radius, false);
		break;
	}

	drawManager.endDrawable();
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
MHWRender::MPxDrawOverride* SwitchableShapeDrawOverride::Creator(const MObject& obj)
{
	return(new SwitchableShapeDrawOverride(obj));
}


//-----------------------------------------------------------------------------
// PRIVATE METHODS
//-----------------------------------------------------------------------------
SwitchableShapeDrawOverride::SwitchableShapeDrawOverride(const MObject& obj) :
	MHWRender::MPxDrawOverride(obj, nullptr, true)
{
}
