#include "DistanceBetweenCmd.h"

#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>

#include "DistanceBetweenLocator.h"


//-----------------------------------------------------------------------------
// STATIC CONSTANTS
//-----------------------------------------------------------------------------
static const MString MEL_COMMAND = "czDistanceBetween";


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
DistanceBetweenCmd::DistanceBetweenCmd() :
    MPxCommand()
{
}

DistanceBetweenCmd::~DistanceBetweenCmd()
{
}

MStatus DistanceBetweenCmd::doIt(const MArgList& args)
{
    MStatus status;

    MArgDatabase argData(syntax(), args, &status);
    if (!status)
    {
        MGlobal::displayError("Error parsing command arguments");
        return(status);
    }

    MSelectionList selectionList;
    status = argData.getObjects(selectionList);
    if (!status)
    {
        MGlobal::displayError("Failed to get selection list");
        return(status);
    }

    if (selectionList.length() != 2)
    {
        MGlobal::displayError("Command requires two transform nodes");
        return(MS::kInvalidParameter);
    }

    MObject selectedObjs[2];
	selectionList.getDependNode(0, selectedObjs[0]);
	selectionList.getDependNode(1, selectedObjs[1]);

    for (int i = 0; i < 2; i++)
    {
        if (selectedObjs[i].apiType() != MFn::kTransform)
        {
            MGlobal::displayError("One or more nodes is not a transform");
            return(MS::kInvalidParameter);
        }
    }

    mTransformObj = mDagModifier.createNode("transform", MObject::kNullObj);

    MString transformName = DistanceBetweenLocator::GetTypeName() + 1;
    mDagModifier.renameNode(mTransformObj, transformName);

    mShapeObj = mDagModifier.createNode(DistanceBetweenLocator::GetTypeId(), mTransformObj);

    MString shapeName = DistanceBetweenLocator::GetTypeName() + "shape1";
    mDagModifier.renameNode(mShapeObj, shapeName);

    MFnDependencyNode destFn(mShapeObj);
    MString coords[] = { "X", "Y", "Z" };

    for (int i = 0; i < 2; i++)
    {
        MFnDependencyNode srcFn(selectedObjs[i]);

        for (int j = 0; j < 3; j++)
        {
            MPlug srcPlug = srcFn.findPlug(MString("translate" + coords[j]), false);
            MPlug destPlug = destFn.findPlug(MString("point") + (i + 1) + coords[j], false);

            status = mDagModifier.connect(srcPlug, destPlug);
            if (!status)
            {
                MGlobal::displayError("Failed to create connection");
                return(status);
            }
        }
    }

    return(redoIt());
}

MStatus DistanceBetweenCmd::undoIt()
{
    return(mDagModifier.undoIt());
}

MStatus DistanceBetweenCmd::redoIt()
{
    mDagModifier.doIt();

    MFnDependencyNode shapeFn(mShapeObj);
    shapeFn.setName(GetShapeName(MFnDependencyNode(mTransformObj).name()));

    return(MS::kSuccess);
}

bool DistanceBetweenCmd::isUndoable() const
{
    return(true);
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* DistanceBetweenCmd::Creator()
{
    return(new DistanceBetweenCmd());
}

MString DistanceBetweenCmd::CommandName()
{
    return(MEL_COMMAND);
}

MSyntax DistanceBetweenCmd::CreateSyntax()
{
    MSyntax syntax;

    syntax.setObjectType(MSyntax::kSelectionList);
    syntax.useSelectionAsDefault(true);

    return(syntax);
}

MString DistanceBetweenCmd::GetShapeName(const MString& transformName)
{
    const int substringStart = DistanceBetweenLocator::GetTypeName().length();
    const int substringEnd = transformName.length() - 1;

    MString number = transformName.substring(substringStart, substringEnd);

    return(DistanceBetweenLocator::GetTypeName() + "Shape" + number);
}
