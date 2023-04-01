#include "JointCreateContextCmd.h"

#include "JointCreateContext.h"


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MString COMMAND_NAME = "czJointCreateCtx";


//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
JointCreateContextCmd::JointCreateContextCmd() :
    MPxContextCommand()
{
}

JointCreateContextCmd::~JointCreateContextCmd()
{
}

MPxContext* JointCreateContextCmd::makeObj()
{
    return(new JointCreateContext());
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* JointCreateContextCmd::Creator()
{
    return(new JointCreateContextCmd());
}

MString JointCreateContextCmd::GetCommandName()
{
    return(COMMAND_NAME);
}
