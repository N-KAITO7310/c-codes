#include "CustomSelectContextCmd.h"

#include "CustomSelectContext.h"


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MString COMMAND_NAME = "czCustomSelectCtx";


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
CustomSelectContextCmd::CustomSelectContextCmd() :
    MPxContextCommand()
{
}

CustomSelectContextCmd::~CustomSelectContextCmd()
{
}

MPxContext* CustomSelectContextCmd::makeObj()
{
    return(new CustomSelectContext());
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* CustomSelectContextCmd::Creator()
{
    return(new CustomSelectContextCmd());
}

MString CustomSelectContextCmd::GetCommandName()
{
    return(COMMAND_NAME);
}
