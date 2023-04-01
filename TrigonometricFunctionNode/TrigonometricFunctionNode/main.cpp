#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>

#include "TrigonometricFunctionNode.h"


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

	status = pluginFn.registerNode(TrigonometricFunctionNode::GetTypeName(),
		TrigonometricFunctionNode::GetTypeId(),
		TrigonometricFunctionNode::Creator,
		TrigonometricFunctionNode::Initialize,
		TrigonometricFunctionNode::kDependNode);
	if (!status)
	{
		MGlobal::displayError("Failed to register node: " + TrigonometricFunctionNode::GetTypeName());
		return(status);
	}

	return(status);
}

MStatus uninitializePlugin(MObject pluginObj)
{
	MStatus status;

	MFnPlugin pluginFn(pluginObj);

	status = pluginFn.deregisterNode(TrigonometricFunctionNode::GetTypeId());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister node: " + TrigonometricFunctionNode::GetTypeName());
		return(status);
	}

	return(status);
}
