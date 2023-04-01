#pragma once

#include <maya/MPxNode.h>
#include <maya/MVector.h>


class FloorContactNode : public MPxNode
{
public:
	FloorContactNode();
	virtual ~FloorContactNode() override;

	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

	// Static Methods
	static void* Creator();
	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();

private:
	// input attr
	static MObject radiusAttr;
	static MObject source1MatrixAttr;
	static MObject source2MatrixAttr;
	static MObject ctrlMatrixAttr;
	static MObject normalAttr;

	// output attr
	static MObject outputTranslateAttr;
};
