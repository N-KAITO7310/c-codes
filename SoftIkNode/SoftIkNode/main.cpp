#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "SoftIkNode.h"


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

	status = pluginFn.registerNode(SoftIkNode::GetTypeName(),
		SoftIkNode::GetTypeId(),
		SoftIkNode::Creator,
		SoftIkNode::Initialize,
		SoftIkNode::kDependNode);
	if (!status)
	{
		MGlobal::displayError("Failed to register node: " + SoftIkNode::GetTypeName());
		return(status);
	}

	return(status);
}

MStatus uninitializePlugin(MObject pluginObj)
{
	MStatus status;

	MFnPlugin pluginFn(pluginObj);

	status = pluginFn.deregisterNode(SoftIkNode::GetTypeId());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister node: " + SoftIkNode::GetTypeName());
		return(status);
	}

	return(status);
}
