#include "SwitchableShapeNode.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnStringData.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7FA);
static const MString TYPE_NAME = "switchableShapeNode";

static const MString DRAW_DB_CLASSIFICATION = "drawdb/geometry/switchableShapeNode";
static const MString DRAW_REGISTRATION_ID = "SwitchableShapeNode";

//-----------------------------------------------------------------------------
// STATIC VARIABLES
//-----------------------------------------------------------------------------
MObject SwitchableShapeNode::shapeIndexObj;
MObject SwitchableShapeNode::fontSizeObj;
MObject SwitchableShapeNode::fontWeightObj;
MObject SwitchableShapeNode::radiusObj;

//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
SwitchableShapeNode::SwitchableShapeNode() :
	MPxLocatorNode()
{
}

SwitchableShapeNode::~SwitchableShapeNode()
{
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* SwitchableShapeNode::Creator()
{
	return(new SwitchableShapeNode());
}

MStatus SwitchableShapeNode::Initialize()
{
	MFnNumericAttribute numericAttr;
	shapeIndexObj = numericAttr.create("shapeIndex", "si", MFnNumericData::kInt, 0);
	numericAttr.setMin(0);
	numericAttr.setMax(8);

	radiusObj = numericAttr.create("radius", "rad", MFnNumericData::kInt, 1);
	numericAttr.setMin(0);
	numericAttr.setMax(100);

	fontSizeObj = numericAttr.create("fontSize", "fs", MFnNumericData::kInt, 14);
	numericAttr.setMin(1);
	numericAttr.setMax(100);

	fontWeightObj = numericAttr.create("fontWeight", "fw", MFnNumericData::kInt, 100);
	numericAttr.setMin(1);
	numericAttr.setMax(100);

	addAttribute(shapeIndexObj);
	addAttribute(radiusObj);
	addAttribute(fontSizeObj);
	addAttribute(fontWeightObj);

	return(MS::kSuccess);
}

MTypeId SwitchableShapeNode::GetTypeId()
{
	return(TYPE_ID);
}

MString SwitchableShapeNode::GetTypeName()
{
	return(TYPE_NAME);
}

MString SwitchableShapeNode::GetDrawDbClassification()
{
	return(DRAW_DB_CLASSIFICATION);
}

MString SwitchableShapeNode::GetDrawRegistrationId()
{
	return(DRAW_REGISTRATION_ID);
}
