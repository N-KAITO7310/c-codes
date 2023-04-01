#pragma once

#include <maya/MObject.h>
#include <maya/MPxCommand.h>
#include <maya/MVector.h>


class SimpleCmd : public MPxCommand
{
public:
    SimpleCmd();
    virtual ~SimpleCmd() override;

    virtual MStatus doIt(const MArgList& args) override;

    virtual MStatus undoIt() override;
    virtual MStatus redoIt() override;
    virtual bool isUndoable() const override;

    // Static methods
    static void* Creator();
    static MString CommandName();

    static MSyntax CreateSyntax();

private:
    MObject mSelectedObj;

    bool mUndoable;

    bool mEdit;
    bool mQuery;

    bool mTranslate;
    bool mVersion;

    MVector mOrigTranslation;
    MVector mNewTranslation;
};
