#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>

#include "RollingNode.h"
#include "RollingNodeCmd.h"


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

	status = pluginFn.registerCommand(RollingNodeCmd::CommandName(), RollingNodeCmd::Creator, RollingNodeCmd::CreateSyntax);
	if (!status)
	{
		MGlobal::displayError("Failed to register command: " + status.errorString());
		return(status);
	}

	status = pluginFn.registerNode(RollingNode::GetTypeName(),
		RollingNode::GetTypeId(),
		RollingNode::Creator,
		RollingNode::Initialize,
		RollingNode::kDependNode);
	if (!status)
	{
		MGlobal::displayError("Failed to register node: " + RollingNode::GetTypeName());
		return(status);
	}

	return(status);
}

MStatus uninitializePlugin(MObject pluginObj)
{
	MStatus status;

	MFnPlugin pluginFn(pluginObj);

	status = pluginFn.deregisterNode(RollingNode::GetTypeId());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister node: " + RollingNode::GetTypeName());
		return(status);
	}

	status = pluginFn.deregisterCommand(RollingNodeCmd::CommandName());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister command: " + status.errorString());
		return(status);
	}

	return(status);
}
