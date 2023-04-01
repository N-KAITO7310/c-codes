#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>

#include "CollisionDriverNode.h"


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

	status = pluginFn.registerNode(CollisionDriverNode::GetTypeName(),
		CollisionDriverNode::GetTypeId(),
		CollisionDriverNode::Creator,
		CollisionDriverNode::Initialize,
		CollisionDriverNode::kDependNode);
	if (!status)
	{
		MGlobal::displayError("Failed to register node: " + CollisionDriverNode::GetTypeName());
		return(status);
	}

	return(status);
}

MStatus uninitializePlugin(MObject pluginObj)
{
	MStatus status;

	MFnPlugin pluginFn(pluginObj);

	status = pluginFn.deregisterNode(CollisionDriverNode::GetTypeId());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister node: " + CollisionDriverNode::GetTypeName());
		return(status);
	}

	return(status);
}
