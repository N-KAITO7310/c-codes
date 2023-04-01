#include "SwitchableShapeUserData.h"

SwitchableShapeUserData::SwitchableShapeUserData(bool deleteAfterData) :
	MUserData(deleteAfterData),
	shapeIndex(0),
	radius(1),
	fontSize(14),
	fontWeight(100),
	displayString(""),
	wireframeColor(MColor(1.0f, 1.0f, 1.0f))
{
}

SwitchableShapeUserData::~SwitchableShapeUserData()
{
}
