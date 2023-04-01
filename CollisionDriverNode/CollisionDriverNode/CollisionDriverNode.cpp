#include "CollisionDriverNode.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>
#include <algorithm>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7F7);
static const MString TYPE_NAME = "collisionDriverNode";


//-----------------------------------------------------------------------------
// STATIC VARIABLES
//-----------------------------------------------------------------------------
MObject CollisionDriverNode::inputDriverMatrixAttr;
MObject CollisionDriverNode::inputGuideMatrixAttr;
MObject CollisionDriverNode::weightAttr;

MObject CollisionDriverNode::outputAttr;

//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
CollisionDriverNode::CollisionDriverNode() :
	MPxNode()
{
}

CollisionDriverNode::~CollisionDriverNode()
{
}

MStatus CollisionDriverNode::compute(const MPlug& plug, MDataBlock& data)
{
	if (plug == outputAttr)
	{
		// get incoming data
		MMatrix driverMatrix = data.inputValue(inputDriverMatrixAttr).asMatrix();
		MMatrix guideMatrix = data.inputValue(inputGuideMatrixAttr).asMatrix();
		double weight = data.inputValue(weightAttr).asDouble();

		// culc
		MMatrix childMat = driverMatrix * guideMatrix;
		MVector childToGuideVec = MTransformationMatrix(childMat).getTranslation(MSpace::kWorld);
		double dist = childToGuideVec.length();
		double distClamped = std::max(std::min(dist, 1.0), 0.0);
		double outputValue = 1.0 - distClamped;
		double result = outputValue * weight;

		MDataHandle outputDataHandle = data.outputValue(outputAttr);
		outputDataHandle.setDouble(result);

		data.setClean(plug);
	}

	return(MS::kSuccess);
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* CollisionDriverNode::Creator()
{
	return(new CollisionDriverNode());
}

MStatus CollisionDriverNode::Initialize()
{
	MFnMatrixAttribute matrixAttr;
	inputDriverMatrixAttr = matrixAttr.create("inputDriverMatrix", "inDrMat", MFnMatrixAttribute::kDouble);
	matrixAttr.setKeyable(false);
	matrixAttr.setReadable(false);

	inputGuideMatrixAttr = matrixAttr.create("inputGuideInverseMatrix", "inGuMat", MFnMatrixAttribute::kDouble);
	matrixAttr.setKeyable(false);
	matrixAttr.setReadable(false);

	MFnNumericAttribute numericAttr;
	weightAttr = numericAttr.create("weight", "w", MFnNumericData::kDouble, 0.0);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	outputAttr = numericAttr.create("output", "out", MFnNumericData::kDouble, 0.0);
	numericAttr.setWritable(false);
	numericAttr.setStorable(false);

	addAttribute(inputDriverMatrixAttr);
	addAttribute(inputGuideMatrixAttr);
	addAttribute(weightAttr);
	addAttribute(outputAttr);

	attributeAffects(inputDriverMatrixAttr, outputAttr);
	attributeAffects(inputGuideMatrixAttr, outputAttr);
	attributeAffects(weightAttr, outputAttr);

	return(MS::kSuccess);
}

MTypeId CollisionDriverNode::GetTypeId()
{
	return(TYPE_ID);
}

MString CollisionDriverNode::GetTypeName()
{
	return(TYPE_NAME);
}
