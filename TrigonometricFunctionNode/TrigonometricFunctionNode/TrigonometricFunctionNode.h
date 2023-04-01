#pragma once

#include <maya/MPxNode.h>


class TrigonometricFunctionNode : public MPxNode
{
public:
	TrigonometricFunctionNode();
	virtual ~TrigonometricFunctionNode() override;

	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

	// Static Methods
	static void* Creator();
	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();

private:
	// input attr
	static MObject timingAttr;
	static MObject sizeAttr;
	static MObject speedAttr;
	static MObject offsetAttr;
	static MObject operationAttr;

	// output attr
	static MObject outputAttr;

};
