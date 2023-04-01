#pragma once

#include <maya/MPxNode.h>
#include <maya/MPoint.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>


class RollingNode : public MPxNode
{
public:
	RollingNode();
	virtual ~RollingNode() override;

	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

	// Static Methods
	static void* Creator();
	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();

private:
	// input attr
	static MObject positionObj;
	static MObject radiusObj;
	static MObject rotateOrderObj;

	// output attr
	static MObject rotationObj;
	static MObject rotationXObj;
	static MObject rotationYObj;
	static MObject rotationZObj;

	// private variables
	int count = 0;
	MPoint lastPos = MPoint();
	MVector upVector = MVector(0, 1, 0);
	MQuaternion quat = MQuaternion();
};
