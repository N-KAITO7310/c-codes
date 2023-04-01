#include "SimpleLocatorNode.h"

#include <maya/MFnNumericAttribute.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7FE);
static const MString TYPE_NAME = "simplelocator";

static const MString DRAW_DB_CLASSIFICATION = "drawdb/geometry/simplelocator";
static const MString DRAW_REGISTRATION_ID = "SimpleLocatorNode";

//-----------------------------------------------------------------------------
// STATIC VARIABLES
//-----------------------------------------------------------------------------
MObject SimpleLocatorNode::shapeIndexObj;

//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
SimpleLocatorNode::SimpleLocatorNode() :
    MPxLocatorNode()
{
}

SimpleLocatorNode::~SimpleLocatorNode()
{
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* SimpleLocatorNode::Creator()
{
    return(new SimpleLocatorNode());
}

MStatus SimpleLocatorNode::Initialize()
{
    MFnNumericAttribute numericAttr;
    shapeIndexObj = numericAttr.create("shapeIndex", "si", MFnNumericData::kInt, 0);
    numericAttr.setMin(0);
    numericAttr.setMax(2);

    addAttribute(shapeIndexObj);

    return(MS::kSuccess);
}

MTypeId SimpleLocatorNode::GetTypeId()
{
    return(TYPE_ID);
}

MString SimpleLocatorNode::GetTypeName()
{
    return(TYPE_NAME);
}

MString SimpleLocatorNode::GetDrawDbClassification()
{
    return(DRAW_DB_CLASSIFICATION);
}

MString SimpleLocatorNode::GetDrawRegistrationId()
{
    return(DRAW_REGISTRATION_ID);
}
