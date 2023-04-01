#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "SimpleCmd.h"


MStatus initializePlugin(MObject pluginObj)
{
	const char* vendor = "Kaito Nakamura";
	const char* version = "1.0.0";
	const char* requiredApiVersion = "Any";

	MStatus status;

	MFnPlugin pluginFn(pluginObj, vendor, version, requiredApiVersion, &status);
	if (!status)
	{
		MGlobal::displayError("Failed to initialize plugin: " + status.errorString());
		return(status);
	}

	status = pluginFn.registerCommand(SimpleCmd::CommandName(), SimpleCmd::Creator, SimpleCmd::CreateSyntax);
	if (!status)
	{
		MGlobal::displayError("Failed to register command: " + SimpleCmd::CommandName());
		return(status);
	}

	return(status);
}

MStatus uninitializePlugin(MObject pluginObj)
{
	MStatus status;

	MFnPlugin pluginFn(pluginObj);

	status = pluginFn.deregisterCommand(SimpleCmd::CommandName());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister command: " + SimpleCmd::CommandName());
		return(status);
	}

	return(status);
}
