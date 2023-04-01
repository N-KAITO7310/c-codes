#include <maya/MDrawRegistry.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "HelloWorldDrawOverride.h"
#include "HelloWorldNode.h"

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
		return (status);
	}

	MString drawDbClassification = HelloWorldNode::GetDrawDbClassification();

	status = pluginFn.registerNode(HelloWorldNode::GetTypeName(),
		HelloWorldNode::GetTypeId(),
		HelloWorldNode::Creator,
		HelloWorldNode::Initialize,
		HelloWorldNode::kLocatorNode,
		&drawDbClassification);

	if (!status)
	{
		MGlobal::displayError("Failed to register hello world node.");
		return(status);
	}

	status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(HelloWorldNode::GetDrawDbClassification(), HelloWorldNode::GetDrawRegistrationId(), HelloWorldDrawOverride::Creator);

	if (!status)
	{
		MGlobal::displayError("Failed to register hello world draw override");
		return(status);
	}

	return(status);
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin pluginFn(obj);

	status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(HelloWorldNode::GetDrawDbClassification(), HelloWorldNode::GetDrawRegistrationId());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister hello world draw override");
		return(status);
	}

	status = pluginFn.deregisterNode(HelloWorldNode::GetTypeId());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister hello world node");
		return(status);
	}

	return(status);
}