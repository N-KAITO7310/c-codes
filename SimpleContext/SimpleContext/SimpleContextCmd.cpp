#include "SimpleContextCmd.h"

#include "SimpleContext.h"


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MString COMMAND_NAME = "czSimpleCtx";


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
SimpleContextCmd::SimpleContextCmd() :
    MPxContextCommand()
{
}

SimpleContextCmd::~SimpleContextCmd()
{
}

MPxContext* SimpleContextCmd::makeObj()
{
    return(new SimpleContext());
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* SimpleContextCmd::Creator()
{
    return(new SimpleContextCmd());
}

MString SimpleContextCmd::GetCommandName()
{
    return(COMMAND_NAME);
}