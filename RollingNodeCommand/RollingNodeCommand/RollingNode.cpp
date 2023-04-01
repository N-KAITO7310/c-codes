#include "RollingNode.h"

#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnCompoundAttribute.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7F9);
static const MString TYPE_NAME = "rollingNode";


//-----------------------------------------------------------------------------
// STATIC VARIABLES
//-----------------------------------------------------------------------------
MObject RollingNode::positionObj;
MObject RollingNode::radiusObj;
MObject RollingNode::rotateOrderObj;

MObject RollingNode::rotationObj;
MObject RollingNode::rotationXObj;
MObject RollingNode::rotationYObj;
MObject RollingNode::rotationZObj;

//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
RollingNode::RollingNode() :
	MPxNode()
{
}

RollingNode::~RollingNode()
{
}

MStatus RollingNode::compute(const MPlug& plug, MDataBlock& data)
{
	if (plug == rotationXObj || plug == rotationYObj || plug == rotationZObj)
	{
		MPoint position = MPoint(data.inputValue(positionObj).asFloatVector());
		double radius = data.inputValue(radiusObj).asDouble();

		MEulerRotation rotation = MEulerRotation();

		MVector vector = MVector();
		if(count > 0)
		{
			vector = position - lastPos;
			quat = quat * MQuaternion(vector.length() / radius, upVector ^ vector);

			lastPos = position;

			MEulerRotation tempRot = quat.asEulerRotation();
			int axisIndex = data.inputValue(rotateOrderObj).asInt();
			switch (axisIndex)
			{
			case 0:
				rotation = MEulerRotation(tempRot.x, tempRot.y, tempRot.z, MEulerRotation::kXYZ);
				break;
			case 1:
				rotation = MEulerRotation(tempRot.x, tempRot.y, tempRot.z, MEulerRotation::kYZX);
				break;
			case 2:
				rotation = MEulerRotation(tempRot.x, tempRot.y, tempRot.z, MEulerRotation::kZXY);
				break;
			case 3:
				rotation = MEulerRotation(tempRot.x, tempRot.y, tempRot.z, MEulerRotation::kXZY);
				break;
			case 4:
				rotation = MEulerRotation(tempRot.x, tempRot.y, tempRot.z, MEulerRotation::kYXZ);
				break;
			case 5:
				rotation = MEulerRotation(tempRot.x, tempRot.y, tempRot.z, MEulerRotation::kZYX);
				break;
			default:
				MGlobal::displayError("Error rotateAxis");
				return(MS::kFailure);
			}

			MDataHandle rotationXDataHandle = data.outputValue(rotationXObj);
			rotationXDataHandle.setDouble(rotation.x);
			MDataHandle rotationYDataHandle = data.outputValue(rotationYObj);
			rotationYDataHandle.setDouble(rotation.y);
			MDataHandle rotationZDataHandle = data.outputValue(rotationZObj);
			rotationZDataHandle.setDouble(rotation.z);
		}
		
		data.setClean(plug);
		count += 1;
	}

	return(MS::kSuccess);
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* RollingNode::Creator()
{
	return(new RollingNode());
}

MStatus RollingNode::Initialize()
{
	MFnNumericAttribute numericAttr;

	positionObj = numericAttr.createPoint("position", "pos");
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	radiusObj = numericAttr.create("radius", "rad", MFnNumericData::kDouble, 0.0);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	rotateOrderObj = numericAttr.create("rotateOrder", "ro", MFnNumericData::kInt, 0);
	numericAttr.setKeyable(false);
	numericAttr.setReadable(true);
	numericAttr.setMax(5);
	numericAttr.setMin(0);

	MFnUnitAttribute unitAttr;
	MFnCompoundAttribute compoundAttr;
	rotationXObj = unitAttr.create("rotationX", "rx", MFnUnitAttribute::kAngle, 0.0);
	unitAttr.setWritable(false);
	unitAttr.setStorable(false);
	rotationYObj = unitAttr.create("rotationY", "ry", MFnUnitAttribute::kAngle, 0.0);
	unitAttr.setWritable(false);
	unitAttr.setStorable(false);
	rotationZObj = unitAttr.create("rotationZ", "rz", MFnUnitAttribute::kAngle, 0.0);
	unitAttr.setWritable(false);
	unitAttr.setStorable(false);
	rotationObj = compoundAttr.create("rotation", "rot");
	compoundAttr.setWritable(false);
	compoundAttr.setStorable(false);
	compoundAttr.addChild(rotationXObj);
	compoundAttr.addChild(rotationYObj);
	compoundAttr.addChild(rotationZObj);

	addAttribute(positionObj);
	addAttribute(radiusObj);
	addAttribute(rotateOrderObj);

	addAttribute(rotationXObj);
	addAttribute(rotationYObj);
	addAttribute(rotationZObj);
	addAttribute(rotationObj);

	attributeAffects(positionObj, rotationXObj);
	attributeAffects(radiusObj, rotationXObj);
	attributeAffects(rotateOrderObj, rotationXObj);

	attributeAffects(positionObj, rotationYObj);
	attributeAffects(radiusObj, rotationYObj);
	attributeAffects(rotateOrderObj, rotationYObj);

	attributeAffects(positionObj, rotationZObj);
	attributeAffects(radiusObj, rotationZObj);
	attributeAffects(rotateOrderObj, rotationZObj);

	return(MS::kSuccess);
}

MTypeId RollingNode::GetTypeId()
{
	return(TYPE_ID);
}

MString RollingNode::GetTypeName()
{
	return(TYPE_NAME);
}
