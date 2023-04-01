#pragma once

#include <maya/MDagModifier.h>
#include <maya/MDGModifier.h>
#include<maya/MPxCommand.h>

class RollingNodeCmd : public MPxCommand
{
public:
	RollingNodeCmd();
	virtual ~RollingNodeCmd() override;

	virtual MStatus doIt(const MArgList& args) override;

	virtual MStatus undoIt() override;
	virtual MStatus redoIt() override;
	virtual bool isUndoable() const override;

	// static methods
	static void* Creator();
	static MString CommandName();

	static MSyntax CreateSyntax();

private:
	MString GetShapeName(const MString& transformName);

	MDagModifier mDagModifier;
	MDGModifier mDgModifier;

	MObject mTransformObj;
	MObject mShapeObj;

	bool mCreateLocator;
	int mVertexMethod;
};