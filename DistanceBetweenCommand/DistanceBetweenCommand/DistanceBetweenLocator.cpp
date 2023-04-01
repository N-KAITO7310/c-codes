#include "DistanceBetweenLocator.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MRampAttribute.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7FD);
static const MString TYPE_NAME = "distanceBetweenLocator";

static const MString DRAW_DB_CLASSIFICATION = "drawdb/geometry/distancebetweenlocator";
static const MString DRAW_REGISTRATION_ID = "DistanceBetweenLocator";

static const float DEFAULT_POSITIONS[] = { 0.0, 1.0 };
static const MColor DEFAULT_COLORS[] = { MColor(0.0, 0.0, 0.0), MColor(1.0, 1.0, 1.0) };
static const int DEFAULT_INTERPS[] = { MRampAttribute::kLinear, MRampAttribute::kLinear };


//-----------------------------------------------------------------------------
// STATIC VARIABLES
//-----------------------------------------------------------------------------
MObject DistanceBetweenLocator::point1Obj;
MObject DistanceBetweenLocator::point2Obj;

MObject DistanceBetweenLocator::distanceObj;
MObject DistanceBetweenLocator::minDistanceObj;
MObject DistanceBetweenLocator::maxDistanceObj;

MObject DistanceBetweenLocator::colorRampObj;


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
DistanceBetweenLocator::DistanceBetweenLocator() :
    MPxLocatorNode()
{
}

DistanceBetweenLocator::~DistanceBetweenLocator()
{
}

void DistanceBetweenLocator::postConstructor()
{
    MFloatArray positions(DEFAULT_POSITIONS, 2);
    MColorArray colors(DEFAULT_COLORS, 2);
    MIntArray interps(DEFAULT_INTERPS, 2);

    MRampAttribute colorRampAttr(thisMObject(), colorRampObj);
    colorRampAttr.addEntries(positions, colors, interps);
}

MStatus DistanceBetweenLocator::compute(const MPlug& plug, MDataBlock& data)
{
    MPoint point1 = MPoint(data.inputValue(point1Obj).asFloatVector());
    MPoint point2 = MPoint(data.inputValue(point2Obj).asFloatVector());

    double distance = point1.distanceTo(point2);

    data.outputValue(distanceObj).setDouble(distance);

    data.setClean(plug);

    return(MS::kSuccess);
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* DistanceBetweenLocator::Creator()
{
    return(new DistanceBetweenLocator());
}

MStatus DistanceBetweenLocator::Initialize()
{
    MFnNumericAttribute numericAttr;

    point1Obj = numericAttr.createPoint("point1", "p1");
    numericAttr.setReadable(false);
    numericAttr.setKeyable(true);

    point2Obj = numericAttr.createPoint("point2", "p2");
    numericAttr.setReadable(false);
    numericAttr.setKeyable(true);

    distanceObj = numericAttr.create("distance", "dist", MFnNumericData::kDouble, 0.0);
    numericAttr.setWritable(false);

    minDistanceObj = numericAttr.create("minDistance", "min", MFnNumericData::kDouble, 0.0);
    numericAttr.setReadable(false);
    numericAttr.setKeyable(true);
    numericAttr.setMin(0.0);

    maxDistanceObj = numericAttr.create("maxDistance", "max", MFnNumericData::kDouble, 10.0);
    numericAttr.setReadable(false);
    numericAttr.setKeyable(true);
    numericAttr.setMin(0.0);

    colorRampObj = MRampAttribute::createColorRamp("colorRamp", "col");

    addAttribute(point1Obj);
    addAttribute(point2Obj);
    addAttribute(distanceObj);
    addAttribute(minDistanceObj);
    addAttribute(maxDistanceObj);
    addAttribute(colorRampObj);

    attributeAffects(point1Obj, distanceObj);
    attributeAffects(point2Obj, distanceObj);

    return(MS::kSuccess);
}

MTypeId DistanceBetweenLocator::GetTypeId()
{
    return(TYPE_ID);
}

MString DistanceBetweenLocator::GetTypeName()
{
    return(TYPE_NAME);
}

MString DistanceBetweenLocator::GetDrawDbClassification()
{
    return(DRAW_DB_CLASSIFICATION);
}

MString DistanceBetweenLocator::GetDrawRegistrationId()
{
    return(DRAW_REGISTRATION_ID);
}

