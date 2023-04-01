#include "MultiplyNode.h"

#include <maya/MFnNumericAttribute.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7F8);
static const MString TYPE_NAME = "multiplynode";

// static var
MObject MultiplyNode::multiplierObj;
MObject MultiplyNode::multiplicandObj;
MObject MultiplyNode::productObj;

//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
MultiplyNode::MultiplyNode() :
    MPxNode()
{
}

MultiplyNode::~MultiplyNode()
{
}

MStatus MultiplyNode::compute(const MPlug& plug, MDataBlock& data)
{
    if (plug == productObj)
    {
        int multiplier = data.inputValue(multiplierObj).asInt();
        double multiplicand = data.inputValue(multiplicandObj).asDouble();

        double product = multiplier * multiplicand;

        MDataHandle productDataHandle = data.outputValue(productObj);
        productDataHandle.setDouble(product);

        data.setClean(plug);
    }

    return MStatus::kSuccess;
}

//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* MultiplyNode::Creator()
{
    return(new MultiplyNode());
}

MStatus MultiplyNode::Initialize()
{
    MFnNumericAttribute numericAttr;

    multiplierObj = numericAttr.create("multiplier", "mul", MFnNumericData::kInt, 2);
    numericAttr.setKeyable(true);
    numericAttr.setReadable(false);

	multiplicandObj = numericAttr.create("multiplicand", "mulc", MFnNumericData::kDouble, 0.0);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

    productObj = numericAttr.create("product", "prod", MFnNumericData::kDouble, 0.0);
    numericAttr.setWritable(false);

	addAttribute(multiplierObj);
	addAttribute(multiplicandObj);
	addAttribute(productObj);

    attributeAffects(multiplierObj, productObj);
    attributeAffects(multiplicandObj, productObj);

    return(MS::kSuccess);
}

MTypeId MultiplyNode::GetTypeId()
{
    return(TYPE_ID);
}

MString MultiplyNode::GetTypeName()
{
    return(TYPE_NAME);
}
