#pragma once

#include <maya/MPxNode.h>
#include <maya/MPoint.h>


class SoftIkNode : public MPxNode
{
public:
	SoftIkNode();
	virtual ~SoftIkNode() override;

	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

	// Static Methods
	static void* Creator();
	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();

private:
	// input attr
	static MObject point1Attr;
	static MObject point2Attr;
	static MObject ctrlPointAttr;
	static MObject softAttr;

	// output attr
	static MObject outTranslateAttr;
};
