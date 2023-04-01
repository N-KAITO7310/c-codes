#include <maya/MDrawRegistry.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "DistanceBetweenCmd.h"
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

    status = pluginFn.registerCommand(DistanceBetweenCmd::CommandName(), DistanceBetweenCmd::Creator, DistanceBetweenCmd::CreateSyntax);
    if(!status)
    {
        MGlobal::displayError("Failed to register command: " + DistanceBetweenCmd::CommandName());
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

    status = pluginFn.deregisterCommand(DistanceBetweenCmd::CommandName());
    if(!status)
    {
        MGlobal::displayError("Failed to deregister command: " + DistanceBetweenCmd::CommandName());
        return(status);
    }

    return(status);
}