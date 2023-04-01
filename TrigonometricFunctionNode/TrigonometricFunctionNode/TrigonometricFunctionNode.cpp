#include "TrigonometricFunctionNode.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MGlobal.h>
#include <math.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7F8);
static const MString TYPE_NAME = "trigonometricFunctionNode";


//-----------------------------------------------------------------------------
// STATIC VARIABLES
//-----------------------------------------------------------------------------
MObject TrigonometricFunctionNode::timingAttr;
MObject TrigonometricFunctionNode::sizeAttr;
MObject TrigonometricFunctionNode::speedAttr;
MObject TrigonometricFunctionNode::offsetAttr;
MObject TrigonometricFunctionNode::operationAttr;

MObject TrigonometricFunctionNode::outputAttr;


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
TrigonometricFunctionNode::TrigonometricFunctionNode() :
	MPxNode()
{
}

TrigonometricFunctionNode::~TrigonometricFunctionNode()
{
}

MStatus TrigonometricFunctionNode::compute(const MPlug& plug, MDataBlock& data)
{
	if (plug == outputAttr)
	{
		// get incoming data
		double timing = data.inputValue(timingAttr).asDouble();
		double size = data.inputValue(sizeAttr).asDouble();
		double speed = data.inputValue(speedAttr).asDouble();
		double offset = data.inputValue(offsetAttr).asDouble();
		int operation = data.inputValue(operationAttr).asInt();

		// compute
		double result = 0.0;
		switch (operation)
		{
		case 0:
			result = sin(timing * speed - offset) * size;
			break;
		case 1:
			result = cos(timing * speed - offset) * size;
			break;
		case 2:
			result = tan(timing * speed - offset) * size;
			break;
		default:
			MGlobal::displayError("Error: operation is invalid value.");
			break;
		}

		MDataHandle outputDataHandle = data.outputValue(outputAttr);
		outputDataHandle.setDouble(result);

		data.setClean(plug);
	}

	return(MS::kSuccess);
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* TrigonometricFunctionNode::Creator()
{
	return(new TrigonometricFunctionNode());
}

MStatus TrigonometricFunctionNode::Initialize()
{
	// input
	MFnNumericAttribute numericAttr;
	timingAttr = numericAttr.create("timing", "tim", MFnNumericData::kDouble, 0.0);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	sizeAttr = numericAttr.create("size", "si", MFnNumericData::kDouble, 1.0);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	speedAttr = numericAttr.create("speed", "sp", MFnNumericData::kDouble, 1.0);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	offsetAttr = numericAttr.create("offset", "off", MFnNumericData::kDouble, 0.0);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	MFnEnumAttribute enumAttr;
	operationAttr = enumAttr.create("operation", "op");
	enumAttr.addField("sin", 0);
	enumAttr.addField("cos", 1);
	enumAttr.addField("tan", 2);
	enumAttr.setWritable(true);
	enumAttr.setKeyable(true);

	// output
	outputAttr = numericAttr.create("output", "out", MFnNumericData::kDouble, 0.0);
	numericAttr.setWritable(false);
	numericAttr.setStorable(false);

	addAttribute(timingAttr);
	addAttribute(sizeAttr);
	addAttribute(speedAttr);
	addAttribute(offsetAttr);
	addAttribute(operationAttr);
	addAttribute(outputAttr);

	attributeAffects(timingAttr, outputAttr);
	attributeAffects(sizeAttr, outputAttr);
	attributeAffects(speedAttr, outputAttr);
	attributeAffects(offsetAttr, outputAttr);
	attributeAffects(operationAttr, outputAttr);

	return(MS::kSuccess);
}

MTypeId TrigonometricFunctionNode::GetTypeId()
{
	return(TYPE_ID);
}

MString TrigonometricFunctionNode::GetTypeName()
{
	return(TYPE_NAME);
}
