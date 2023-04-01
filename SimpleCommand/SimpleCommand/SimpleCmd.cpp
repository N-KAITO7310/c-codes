#include "SimpleCmd.h"

#include <maya/MArgDatabase.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnTransform.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>


//-----------------------------------------------------------------------------
// STATIC CONSTANTS
//-----------------------------------------------------------------------------
static const MString MEL_COMMAND = "SimpleCmd";

static const char* TRANSLATE_FLAG[2] = { "-t", "-translate" };
static const char* VERSION_FLAG[2] = { "-v", "-version" };

//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
SimpleCmd::SimpleCmd() :
    MPxCommand(),
    mUndoable(false),
    mEdit(false),
    mQuery(false),
    mTranslate(false),
    mVersion(false)
{
}

SimpleCmd::~SimpleCmd()
{
}

MStatus SimpleCmd::doIt(const MArgList& args)
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
    selectionList.getDependNode(0, mSelectedObj);
    if (mSelectedObj.apiType() != MFn::kTransform)
    {
        MGlobal::displayError("This command requires a transform node.");
        return(MS::kFailure);
    }

    mEdit = argData.isEdit();
    mQuery = argData.isQuery();

    mVersion = argData.isFlagSet(VERSION_FLAG[0]);

    mTranslate = argData.isFlagSet(TRANSLATE_FLAG[0]);
    if (mTranslate)
    {
        MFnTransform transformFn(mSelectedObj);
        mOrigTranslation = transformFn.getTranslation(MSpace::kTransform);

        if (mEdit)
        {
            mNewTranslation = MVector(argData.flagArgumentDouble(TRANSLATE_FLAG[0], 0),
                argData.flagArgumentDouble(TRANSLATE_FLAG[0], 1),
                argData.flagArgumentDouble(TRANSLATE_FLAG[0], 2));

            mUndoable = true;
        }
    }
    
    return(redoIt());
}

MStatus SimpleCmd::undoIt()
{
    MFnTransform transformFn(mSelectedObj);

    transformFn.setTranslation(mOrigTranslation, MSpace::Space::kTransform);

    return(MS::kSuccess);
}

MStatus SimpleCmd::redoIt()
{
    MFnTransform transformFn(mSelectedObj);

    if (mQuery)
    {
        if (mTranslate)
        {
            double result[3];
            mOrigTranslation.get(result);

            setResult(MDoubleArray(result, 3));
        }
        else
        {
            MGlobal::displayError("Flag does not support query");
        }

    }
    else if (mEdit)
    {
        if (mTranslate) {
            transformFn.setTranslation(mNewTranslation, MSpace::kTransform);
        }
        else
        {
            MGlobal::displayError("Flag does not support edits");
            return(MS::kFailure);
        }
    }
    else if (mVersion)
    {
        setResult("1.0.0");
    }
    else
    {
        setResult(transformFn.name());
    }

    return MStatus();
}

bool SimpleCmd::isUndoable() const
{
    return(mUndoable);
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* SimpleCmd::Creator()
{
    return(new SimpleCmd());
}

MString SimpleCmd::CommandName()
{
    return(MEL_COMMAND);
}

MSyntax SimpleCmd::CreateSyntax()
{
    MSyntax syntax;

    syntax.enableEdit(true);
    syntax.enableQuery(true);

    syntax.addFlag(TRANSLATE_FLAG[0], TRANSLATE_FLAG[1], MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble);
    syntax.addFlag(VERSION_FLAG[0], VERSION_FLAG[1]);

    syntax.setObjectType(MSyntax::kSelectionList, 1, 1);
    syntax.useSelectionAsDefault(true);

    return(syntax);
}
