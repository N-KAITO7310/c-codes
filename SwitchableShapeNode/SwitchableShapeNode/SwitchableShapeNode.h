#pragma once

#include <maya/MPxLocatorNode.h>


class SwitchableShapeNode : public MPxLocatorNode
{
public:
	SwitchableShapeNode();
	virtual ~SwitchableShapeNode() override;

	// Static methods
	static void* Creator();
	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();

	static MString GetDrawDbClassification();
	static MString GetDrawRegistrationId();

private:
	static MObject shapeIndexObj;
	static MObject fontSizeObj;
	static MObject fontWeightObj;
	static MObject radiusObj;

};
