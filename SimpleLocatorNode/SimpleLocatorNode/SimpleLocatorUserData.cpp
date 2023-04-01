#include "SimpleLocatorUserData.h"

SimpleLocatorUserData::SimpleLocatorUserData(bool deleteAfterData) :
	MUserData(deleteAfterData),
	shapeIndex(0),
	wireframeColor(MColor(1.0f, 1.0f, 1.0f))
{
}

SimpleLocatorUserData::~SimpleLocatorUserData()
{
}
