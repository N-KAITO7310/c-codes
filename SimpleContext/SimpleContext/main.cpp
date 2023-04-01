#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "SimpleContextCmd.h"


MStatus initializePlugin(MObject pluginObj)
{
    const char* vendor = "Chris Zurbrigg";
    const char* version = "1.0.0";
    const char* requiredApiVersion = "Any";

    MStatus status;

    MFnPlugin pluginFn(pluginObj, vendor, version, requiredApiVersion, &status);
    if(!status)
    {
        MGlobal::displayError("Failed to initialize plugin: " + status.errorString());
        return(status);
    }

    status = pluginFn.registerContextCommand(SimpleContextCmd::GetCommandName(), SimpleContextCmd::Creator);
    if(!status)
    {
        MGlobal::displayError("Failed to register context command: " + SimpleContextCmd::GetCommandName());
        return(status);
    }

    return(MS::kSuccess);
}

MStatus uninitializePlugin(MObject pluginObj)
{
    MStatus status;

    MFnPlugin pluginFn(pluginObj);

    status = pluginFn.deregisterContextCommand(SimpleContextCmd::GetCommandName());
    if(!status)
    {
        MGlobal::displayError("Failed to deregister context command: " + SimpleContextCmd::GetCommandName());
        return(status);
    }

    return(status);
}
