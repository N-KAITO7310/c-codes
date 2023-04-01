#include "SoftIkNode.h"

#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>

//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7A1);
static const MString TYPE_NAME = "softIkNode";


//-----------------------------------------------------------------------------
// STATIC VARIABLES
//-----------------------------------------------------------------------------
MObject SoftIkNode::point1Attr;
MObject SoftIkNode::point2Attr;
MObject SoftIkNode::ctrlPointAttr;
MObject SoftIkNode::softAttr;
MObject SoftIkNode::outTranslateAttr;

//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
SoftIkNode::SoftIkNode() :
	MPxNode()
{
}

SoftIkNode::~SoftIkNode()
{
}

MStatus SoftIkNode::compute(const MPlug& plug, MDataBlock& data)
{
	if (plug == outTranslateAttr)
	{
		// get input data
		MPoint point1 = MPoint(data.inputValue(point1Attr).asFloatVector());
		MPoint point2 = MPoint(data.inputValue(point2Attr).asFloatVector());
		MVector ctrlVec = MVector(data.inputValue(ctrlPointAttr).asFloatVector());
		float softP = data.inputValue(softAttr).asFloat();

		MVector chainVec = point2 - point1;

		float chainLen = chainVec.length();
		float distance = ctrlVec.length();

		float softDist = distance;

		float chainLenSoftPSub = chainLen - softP;

		if (distance > chainLenSoftPSub) {
			if (softP > 0) {
				softDist = chainLen - softP * exp(-(distance - (chainLen - softP)) / softP);
			}
			else {
				softDist = chainLen;
			}
		}

		ctrlVec.y = softDist;
		
		data.outputValue(outTranslateAttr).set3Float(ctrlVec.x, ctrlVec.y, ctrlVec.z);
		
		data.setClean(plug);
		return(MS::kSuccess);
	}
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* SoftIkNode::Creator()
{
	return(new SoftIkNode());
}

MStatus SoftIkNode::Initialize()
{
	MFnNumericAttribute numericAttr;

	point1Attr = numericAttr.createPoint("point1", "p1");
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	point2Attr = numericAttr.createPoint("point2", "p2");
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	ctrlPointAttr = numericAttr.createPoint("controllerPoint", "cp");
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	softAttr = numericAttr.create("soft", "sf", MFnNumericData::kFloat, 0.2);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	outTranslateAttr = numericAttr.createPoint("outTranslate", "ot");
	numericAttr.setWritable(false);
	numericAttr.setStorable(false);

	addAttribute(point1Attr);
	addAttribute(point2Attr);
	addAttribute(ctrlPointAttr);
	addAttribute(softAttr);

	addAttribute(outTranslateAttr);

	attributeAffects(point1Attr, outTranslateAttr);
	attributeAffects(point2Attr, outTranslateAttr);
	attributeAffects(ctrlPointAttr, outTranslateAttr);
	attributeAffects(softAttr, outTranslateAttr);

	return(MS::kSuccess);
}

MTypeId SoftIkNode::GetTypeId()
{
	return(TYPE_ID);
}

MString SoftIkNode::GetTypeName()
{
	return(TYPE_NAME);
}
