#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <maya/MPxNode.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>


class RotationDriverNode : public MPxNode
{
public:
	RotationDriverNode();
	virtual ~RotationDriverNode() override;

	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

	// Static Methods
	static void* Creator();
	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();
	
private:
	// input attr
	static MObject operationAttr;
	static MObject methodAttr;
	static MObject reverseOrderAttr;
	static MObject rotateXAttr;
	static MObject rotateYAttr;
	static MObject rotateZAttr;
	static MObject rotateAttr;
	static MObject rotateOrderAttr;
	static MObject axisOrientXAttr;
	static MObject axisOrientYAttr;
	static MObject axisOrientZAttr;
	static MObject axisOrientAttr;

	// output attr
	static MObject outRotateX;
	static MObject outRotateY;
	static MObject outRotateZ;
	static MObject outRotate;

	double pi2 = 2. * M_PI;
	MVector xVec = MVector::xAxis;
	MVector yVec = MVector::yAxis;
	MVector zVec = MVector::zAxis;
	double returnBoundAngle(double x);
	void toRollBendHV(MQuaternion quat, double roll[]);
	void toRoll(MQuaternion quat, double roll[]);
	void toBendHV(MQuaternion quat, double roll[]);
	MQuaternion fromRollBendHV(double rhv[]);
};