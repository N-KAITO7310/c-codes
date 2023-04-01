#include "BlendDeformerNode.h"

#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MItGeometry.h>
#include <maya/MPointArray.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7FD);
static const MString TYPE_NAME = "blenddeformernode";


//-----------------------------------------------------------------------------
// STATIC VARIABLES
//-----------------------------------------------------------------------------
MObject BlendDeformerNode::blendMeshObj;
MObject BlendDeformerNode::blendWeightObj;


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
BlendDeformerNode::BlendDeformerNode() :
    MPxDeformerNode()
{
}

BlendDeformerNode::~BlendDeformerNode()
{
}

MStatus BlendDeformerNode::deform(MDataBlock& dataBlock, MItGeometry& geoIter, const MMatrix& matrix, unsigned int multiIndex)
{
    float envelopeValue = dataBlock.inputValue(envelope).asFloat();
    float blendWeightValue = dataBlock.inputValue(blendWeightObj).asFloat();
    MObject targetMesh = dataBlock.inputValue(blendMeshObj).asMesh();

    if(envelopeValue == 0 || blendWeightValue == 0 || targetMesh.isNull())
    {
        return(MS::kSuccess);
    }

    MPointArray targetPoints;

    MFnMesh targetMeshFn(targetMesh);
    targetMeshFn.getPoints(targetPoints);

    float globalWeight = blendWeightValue * envelopeValue;

    geoIter.reset();
    while(!geoIter.isDone())
    {
        float sourceWeight = weightValue(dataBlock, multiIndex, geoIter.index());

        MPoint sourcePt = geoIter.position();
        MPoint targetPt = targetPoints[geoIter.index()];
        MPoint finalPt = sourcePt + ((targetPt - sourcePt) * globalWeight * sourceWeight);

        geoIter.setPosition(finalPt);

        geoIter.next();
    }

    return(MS::kSuccess);
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* BlendDeformerNode::Creator()
{
    return(new BlendDeformerNode());
}

MStatus BlendDeformerNode::Initialize()
{
    MFnTypedAttribute typeAttr;
    blendMeshObj = typeAttr.create("blendMesh", "bMesh", MFnData::kMesh);

    MFnNumericAttribute numericAttr;
    blendWeightObj = numericAttr.create("blendWeight", "bWeight", MFnNumericData::kFloat, 0.0f);
    numericAttr.setKeyable(true);
    numericAttr.setMin(0.0f);
    numericAttr.setMax(1.0f);

    addAttribute(blendMeshObj);
    addAttribute(blendWeightObj);

    attributeAffects(blendMeshObj, outputGeom);
    attributeAffects(blendWeightObj, outputGeom);

    return(MS::kSuccess);
}

MTypeId BlendDeformerNode::GetTypeId()
{
    return(TYPE_ID);
}

MString BlendDeformerNode::GetTypeName()
{
    return(TYPE_NAME);
}