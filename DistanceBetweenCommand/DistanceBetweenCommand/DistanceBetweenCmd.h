#pragma once

#include <maya/MDagModifier.h>
#include <maya/MPxCommand.h>


class DistanceBetweenCmd : public MPxCommand
{
public:
    DistanceBetweenCmd();
    virtual ~DistanceBetweenCmd() override;

    virtual MStatus doIt(const MArgList& args) override;

    virtual MStatus undoIt() override;
    virtual MStatus redoIt() override;
    virtual bool isUndoable() const override;

    // Static methods
    static void* Creator();
    static MString CommandName();

    static MSyntax CreateSyntax();

private:
    MString GetShapeName(const MString& transformName);

    MDagModifier mDagModifier;

    MObject mTransformObj;
    MObject mShapeObj;
};
