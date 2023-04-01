#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>

#include "MultiplyNode.h"


MStatus initializePlugin(MObject pluginObj)
{
	const char* vendor = "Kaito Nakamura";
#ifndef _DEBUG
	const char* version = "1.0.0";
#else
	const char* version = "1.0.0 DEBUG";
#endif
	const char* requiredApiVersion = "Any";

	MStatus status;

	MFnPlugin pluginFn(pluginObj, vendor, version, requiredApiVersion, &status);
	if (!status)
	{
		MGlobal::displayError("Failed to initialize plugin: " + status.errorString());
		return(status);
	}

	status = pluginFn.registerNode(MultiplyNode::GetTypeName(),
		MultiplyNode::GetTypeId(),
		MultiplyNode::Creator,
		MultiplyNode::Initialize,
		MultiplyNode::kDependNode);
	if (!status)
	{
		MGlobal::displayError("Failed to register node: " + MultiplyNode::GetTypeName());
		return(status);
	}

	return(status);
}

MStatus uninitializePlugin(MObject pluginObj)
{
	MStatus status;

	MFnPlugin pluginFn(pluginObj);

	status = pluginFn.deregisterNode(MultiplyNode::GetTypeId());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister node: MultiplyNode");
		return(status);
	}

	return(status);
}
