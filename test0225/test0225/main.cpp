#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include "HelloWorldCommand.h"

MStatus initializePlugin(MObject obj)
{
	const char* vendor = "Kaito Nakamura";
	const char* version = "1.0.0";
	const char* requiredApiVersion = "Any";

	MStatus status;

	MFnPlugin pluginFn(obj, vendor, version, requiredApiVersion, &status);
	if (!status)
	{
		MGlobal::displayError("Failed to initialize plugin: " + status.errorString());
		return(status);
	}

	status = pluginFn.registerCommand(HelloWorldCommand::CommandName(), HelloWorldCommand::Creator);
	if (!status)
	{
		MGlobal::displayError("Failed to register HelloWorldCommand");
		return(status);
	}

	return(status);
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin pluginFn(obj);

	status = pluginFn.deregisterCommand(HelloWorldCommand::CommandName());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister HelloWorldCommand");
		return(status);
	}

	return(status);
}