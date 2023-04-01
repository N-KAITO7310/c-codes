#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "FloorContactNode.h"


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

	status = pluginFn.registerNode(FloorContactNode::GetTypeName(),
		FloorContactNode::GetTypeId(),
		FloorContactNode::Creator,
		FloorContactNode::Initialize,
		FloorContactNode::kDependNode);
	if (!status)
	{
		MGlobal::displayError("Failed to register node: " + FloorContactNode::GetTypeName());
		return(status);
	}

	return(status);
}

MStatus uninitializePlugin(MObject pluginObj)
{
	MStatus status;

	MFnPlugin pluginFn(pluginObj);

	status = pluginFn.deregisterNode(FloorContactNode::GetTypeId());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister node: " + FloorContactNode::GetTypeName());
		return(status);
	}

	return(status);
}
