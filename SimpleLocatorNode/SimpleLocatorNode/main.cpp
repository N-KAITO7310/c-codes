#include <maya/MDrawRegistry.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "SimpleLocatorDrawOverride.h"
#include "SimpleLocatorNode.h"


MStatus initializePlugin(MObject pluginObj)
{
    const char* vendor = "Kaito Nakamura";
    const char* version = "1.0.0";
    const char* requiredApiVersion = "Any";

    MStatus status;

    MFnPlugin pluginFn(pluginObj, vendor, version, requiredApiVersion, &status);
    if(!status)
    {
        MGlobal::displayError("Failed to initialize plugin: " + status.errorString());
        return(status);
    }

    MString drawDbClassification = SimpleLocatorNode::GetDrawDbClassification();

    status = pluginFn.registerNode(
        SimpleLocatorNode::GetTypeName(),
        SimpleLocatorNode::GetTypeId(),
        SimpleLocatorNode::Creator,
        SimpleLocatorNode::Initialize,
        SimpleLocatorNode::kLocatorNode,
        &drawDbClassification
    );

	if (!status)
	{
		MGlobal::displayError("Failed to register simple locator node.");
		return(status);
	}

    status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(drawDbClassification, SimpleLocatorNode::GetDrawRegistrationId(), SimpleLocatorDrawOverride::Creator);
	if (!status)
	{
		MGlobal::displayError("Failed to register simple locator draw override.");
		return(status);
	}

    return(status);
}

MStatus uninitializePlugin(MObject pluginObj)
{
    MStatus status;

    MFnPlugin pluginFn(pluginObj);

    status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(SimpleLocatorNode::GetDrawDbClassification(), SimpleLocatorNode::GetDrawRegistrationId());

    status = pluginFn.deregisterNode(SimpleLocatorNode::GetTypeId());
	if (!status)
	{
		MGlobal::displayError("Failed to deregister simple locator node.");
		return(status);
	}

    return(status);
}