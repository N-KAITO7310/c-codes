#pragma once

#include <maya/MPxNode.h>


class CollisionDriverNode : public MPxNode
{
public:
	CollisionDriverNode();
	virtual ~CollisionDriverNode() override;

	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

	// Static Methods
	static void* Creator();
	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();

private:
	// input attr
	static MObject inputDriverMatrixAttr;
	static MObject inputGuideMatrixAttr;
	static MObject weightAttr;

	// output attr
	static MObject outputAttr;

};
