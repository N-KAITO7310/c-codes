#pragma once

#include <maya/MColor.h>
#include <maya/MUserData.h>
#include <maya/MString.h>

class SwitchableShapeUserData : public MUserData
{
public:
	SwitchableShapeUserData(bool deleteAfterData);
	virtual ~SwitchableShapeUserData() override;

	int shapeIndex;
	int radius;
	int fontSize;
	int fontWeight;
	MString displayString;
	
	MColor wireframeColor;
};