#include "RotationDriver.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7F1);
static const MString TYPE_NAME = "rotationDriverNode";


//-----------------------------------------------------------------------------
// STATIC VARIABLES
//-----------------------------------------------------------------------------
MObject RotationDriverNode::operationAttr;
MObject RotationDriverNode::methodAttr;
MObject RotationDriverNode::reverseOrderAttr;
MObject RotationDriverNode::rotateXAttr;
MObject RotationDriverNode::rotateYAttr;
MObject RotationDriverNode::rotateZAttr;
MObject RotationDriverNode::rotateAttr;
MObject RotationDriverNode::rotateOrderAttr;
MObject RotationDriverNode::axisOrientAttr;
MObject RotationDriverNode::axisOrientXAttr;
MObject RotationDriverNode::axisOrientYAttr;
MObject RotationDriverNode::axisOrientZAttr;

MObject RotationDriverNode::outRotateX;
MObject RotationDriverNode::outRotateY;
MObject RotationDriverNode::outRotateZ;
MObject RotationDriverNode::outRotate;

//-----------------------------------------------------------------------------
// PRIVATE METHODS
//-----------------------------------------------------------------------------
double RotationDriverNode::returnBoundAngle(double x)
{
	if (x > M_PI)
	{
		return x - RotationDriverNode::pi2;
	}
	else
	{
		if (x < -M_PI)
		{
			return x + RotationDriverNode::pi2;
		}
		else
		{
			return x;
		}
	}
}

void RotationDriverNode::toRollBendHV(MQuaternion quat, double roll[])
{
	MVector rotatedVec = RotationDriverNode::xVec.rotateBy(quat);
	MQuaternion bendQ = MQuaternion(RotationDriverNode::xVec, rotatedVec);
	double bend = (RotationDriverNode::xVec * rotatedVec) + 1.;

	MQuaternion rollQ = quat * bendQ.inverse();
	roll[0] = returnBoundAngle(atan2(rollQ[0], rollQ[3]) * 2.);
	roll[1] = atan2(zVec * rotatedVec, bend) * -2.;
	roll[2] = atan2(yVec * rotatedVec, bend) * 2.;

	return;
}

void RotationDriverNode::toRoll(MQuaternion quat, double rot[])
{
	MVector rotatedVec = RotationDriverNode::xVec.rotateBy(quat);
	MQuaternion bendQ = MQuaternion(RotationDriverNode::xVec, rotatedVec);
	double bend = (RotationDriverNode::xVec * rotatedVec) + 1.;

	MQuaternion rollQ = quat * bendQ.inverse();
	rot[0] = returnBoundAngle(atan2(rollQ[0], rollQ[3]) * 2.);
	rot[1] = returnBoundAngle(atan2(rollQ[1], rollQ[3]) * 2.);
	rot[2] = returnBoundAngle(atan2(rollQ[2], rollQ[3]) * 2.);

	return;
}

void RotationDriverNode::toBendHV(MQuaternion quat, double roll[])
{
	MVector rotatedVec = RotationDriverNode::xVec.rotateBy(quat);
	MQuaternion bendQ = MQuaternion(RotationDriverNode::xVec, rotatedVec);
	double bend = (RotationDriverNode::xVec * rotatedVec) + 1.;

	MQuaternion rollQ = quat * bendQ.inverse();
	roll[0] = returnBoundAngle(atan2(bendQ[0], bendQ[3]) * 2.);
	roll[1] = atan2(zVec * rotatedVec, bend) * -2.;
	roll[2] = atan2(yVec * rotatedVec, bend) * 2.;

	return;
}

MQuaternion RotationDriverNode::fromRollBendHV(double rhv[])
{
	double half = .5 * rhv[0];
	double sinHalf = sin(half);
	MQuaternion quat(RotationDriverNode::xVec[0] * sinHalf, RotationDriverNode::xVec[1] * sinHalf, RotationDriverNode::xVec[2] * sinHalf, cos(half));

	double h = tan(-.5 * rhv[1]);
	double v = tan(.5 * rhv[2]);
	double f = 2. / (h * h + v * v + 1.);
	MQuaternion tempQuat = MQuaternion(RotationDriverNode::xVec, RotationDriverNode::xVec * (f - 1.) + RotationDriverNode::yVec * (v * f) + RotationDriverNode::zVec * (h * f));
	MQuaternion resultQuat = quat * tempQuat;
	return resultQuat;
}


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
RotationDriverNode::RotationDriverNode() :
	MPxNode()
{
}

RotationDriverNode::~RotationDriverNode()
{
}

MStatus RotationDriverNode::compute(const MPlug& plug, MDataBlock& data)
{
	if (plug == outRotate || plug == outRotateX || plug == outRotateY || plug == outRotateZ)
	{
		// get incoming data
		MEulerRotation::RotationOrder rotateOrder;
		switch (data.inputValue(rotateOrderAttr).asInt())
		{
		case 0:
			rotateOrder = MEulerRotation::RotationOrder::kXYZ;
			break;
		case 1:
			rotateOrder = MEulerRotation::RotationOrder::kYZX;
			break;
		case 2:
			rotateOrder = MEulerRotation::RotationOrder::kZXY;
			break;
		case 3:
			rotateOrder = MEulerRotation::RotationOrder::kXZY;
			break;
		case 4:
			rotateOrder = MEulerRotation::RotationOrder::kYXZ;
			break;
		case 5:
			rotateOrder = MEulerRotation::RotationOrder::kZYX;
			break;
		default:
			rotateOrder = MEulerRotation::RotationOrder::kXYZ;
			break;
		}
		MQuaternion quat = MEulerRotation(data.inputValue(axisOrientAttr).asDouble3()).asQuaternion();
		MQuaternion oriQ = quat.inverse();
		quat *= MEulerRotation(data.inputValue(rotateAttr).asDouble3(), rotateOrder).asQuaternion();
		quat *= oriQ;

		// rotate to rhv
		double rhv[] = {0.0, 0.0, 0.0};

		int operation = data.inputValue(operationAttr).asInt();

		bool reverse = data.inputValue(reverseOrderAttr).asBool();
		if (reverse)
		{
			quat = quat.inverse();
		}
		short method = data.inputValue(methodAttr).asShort();
		if (method)
		{
			MQuaternion log = quat.log();
			rhv[0] = log[0] * 2.;
			rhv[1] = log[1] * 2.;
			rhv[2] = log[2] * 2.;
		}
		else
		{
			switch (operation)
			{
			case 0:
				toRollBendHV(quat, rhv);
				break;
			case 1:
				toRoll(quat, rhv);
				break;
			case 2:
				toBendHV(quat, rhv);
				break;
			default:
				toRollBendHV(quat, rhv);
				break;
			}
		}
		if (reverse)
		{
			rhv[0] = -rhv[0];
			rhv[1] = -rhv[1];
			rhv[2] = -rhv[2];
		}

		// rhv to rotate
		if (method)
		{
			quat = MQuaternion(rhv[0] * .5, rhv[1] * .5, rhv[2] * .5, 0.).exp();
		}
		else
		{
			quat = fromRollBendHV(rhv);
		}

		MEulerRotation rotFromQuat = quat.asEulerRotation();
		MEulerRotation rot = MEulerRotation(rotFromQuat.x, rotFromQuat.y, rotFromQuat.z, rotateOrder);

		data.outputValue(outRotate).set3Double(rot.x, rot.y, rot.z);
		data.setClean(plug);
	}

	return(MS::kSuccess);
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* RotationDriverNode::Creator()
{
	return(new RotationDriverNode());
}

MStatus RotationDriverNode::Initialize()
{
	// input
	MFnEnumAttribute enumAttr;
	operationAttr = enumAttr.create("operation", "op");
	enumAttr.addField("RollBendHV", 0);
	enumAttr.addField("Roll", 1);
	enumAttr.addField("BendHV", 2);
	enumAttr.setWritable(true);
	enumAttr.setKeyable(false);

	methodAttr = enumAttr.create("method", "met");
	enumAttr.addField("Stereographic Projection", 0);
	enumAttr.addField("Exponential Map", 1);
	enumAttr.setWritable(true);
	enumAttr.setKeyable(false);

	MFnNumericAttribute numericAttr;
	reverseOrderAttr = numericAttr.create("reverseOrder", "ror", MFnNumericData::kBoolean, false);

	MFnUnitAttribute unitAttr;
	rotateXAttr = unitAttr.create("rotateX", "rx", MFnUnitAttribute::kAngle, 0.);
	rotateYAttr = unitAttr.create("rotateY", "ry", MFnUnitAttribute::kAngle, 0.);
	rotateZAttr = unitAttr.create("rotateZ", "rz", MFnUnitAttribute::kAngle, 0.);
	rotateAttr = numericAttr.create("rotate", "r", rotateXAttr, rotateYAttr, rotateZAttr);
	numericAttr.setWritable(true);
	numericAttr.setKeyable(false);

	rotateOrderAttr = enumAttr.create("rotateOrder", "ro");
	enumAttr.addField("xyz", 0);
	enumAttr.addField("yzx", 1);
	enumAttr.addField("zxy", 2);
	enumAttr.addField("xzy", 3);
	enumAttr.addField("yxz", 4);
	enumAttr.addField("zyx", 5);

	axisOrientXAttr = unitAttr.create("axisOrientX", "aox", MFnUnitAttribute::kAngle, 0.);
	axisOrientYAttr = unitAttr.create("axisOrientY", "aoy", MFnUnitAttribute::kAngle, 0.);
	axisOrientZAttr = unitAttr.create("axisOrientZ", "aoz", MFnUnitAttribute::kAngle, 0.);
	axisOrientAttr = numericAttr.create("axisOrient", "ao", axisOrientXAttr, axisOrientYAttr, axisOrientZAttr);

	// output
	outRotateX = unitAttr.create("outRatateX", "orx", MFnUnitAttribute::kAngle, 0.);
	outRotateY = unitAttr.create("outRatateY", "ory", MFnUnitAttribute::kAngle, 0.);
	outRotateZ = unitAttr.create("outRatateZ", "orz", MFnUnitAttribute::kAngle, 0.);
	outRotate = numericAttr.create("outRotate", "or", outRotateX, outRotateY, outRotateZ);
	numericAttr.setWritable(false);
	numericAttr.setStorable(false);

	addAttribute(operationAttr);
	addAttribute(methodAttr);
	addAttribute(reverseOrderAttr);
	addAttribute(rotateAttr);
	addAttribute(rotateOrderAttr);
	addAttribute(axisOrientAttr);
	addAttribute(outRotate);

	attributeAffects(operationAttr, outRotate);
	attributeAffects(methodAttr, outRotate);
	attributeAffects(reverseOrderAttr, outRotate);
	attributeAffects(rotateAttr, outRotate);
	attributeAffects(rotateOrderAttr, outRotate);
	attributeAffects(axisOrientAttr, outRotate);

	return(MS::kSuccess);
}

MTypeId RotationDriverNode::GetTypeId()
{
	return(TYPE_ID);
}

MString RotationDriverNode::GetTypeName()
{
	return(TYPE_NAME);
}