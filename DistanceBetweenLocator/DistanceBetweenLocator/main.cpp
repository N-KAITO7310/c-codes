#include <maya/MDrawRegistry.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "DistanceBetweenDrawOverride.h"
#include "DistanceBetweenLocator.h"


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

    MString drawDbClassification = DistanceBetweenLocator::GetDrawDbClassification();

    status = pluginFn.registerNode(
        DistanceBetweenLocator::GetTypeName(),
        DistanceBetweenLocator::GetTypeId(),
        DistanceBetweenLocator::Creator,
        DistanceBetweenLocator::Initialize,
        DistanceBetweenLocator::kLocatorNode,
        &drawDbClassification
    );

    if(!status)
    {
        MGlobal::displayError("Failed to register distance between locator node.");
        return(status);
    }

    status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(drawDbClassification, DistanceBetweenLocator::GetDrawRegistrationId(), DistanceBetweenDrawOverride::Creator);
    if(!status)
    {
        MGlobal::displayError("Failed to register distance between draw override.");
        return(status);
    }

    return(status);
}

MStatus uninitializePlugin(MObject pluginObj)
{
    MStatus status;

    MFnPlugin pluginFn(pluginObj);

    status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(DistanceBetweenLocator::GetDrawDbClassification(), DistanceBetweenLocator::GetDrawRegistrationId());
    if(!status)
    {
        MGlobal::displayError("Failed to deregister distance between draw override.");
        return(status);
    }

    status = pluginFn.deregisterNode(DistanceBetweenLocator::GetTypeId());
    if(!status)
    {
        MGlobal::displayError("Failed to deregister distance between locator node.");
        return(status);
    }

    return(status);
}