#include "HelloWorldCommand.h"

#include <maya/MGlobal.h>
#include <maya/MIOStream.h>

// Static constants
static const MString MEL_COMMAND = "HelloWorld";

// public methods
HelloWorldCommand::HelloWorldCommand()
{

}

HelloWorldCommand::~HelloWorldCommand()
{

}

MStatus HelloWorldCommand::doIt(const MArgList& args)
{
	MGlobal::displayInfo("Hello World");
	cout << "Hello World" << endl;

	return(MStatus::kSuccess);
}

// static methods
void* HelloWorldCommand::Creator()
{
	return(new HelloWorldCommand());
}

MString HelloWorldCommand::CommandName()
{
	return(MEL_COMMAND);
}