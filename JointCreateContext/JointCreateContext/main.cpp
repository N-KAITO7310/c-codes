#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "JointCreateContextCmd.h"


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

    status = pluginFn.registerContextCommand(JointCreateContextCmd::GetCommandName(), JointCreateContextCmd::Creator);
    if(!status)
    {
        MGlobal::displayError("Failed to register context command: " + JointCreateContextCmd::GetCommandName());
        return(status);
    }

    return(MS::kSuccess);
}

MStatus uninitializePlugin(MObject pluginObj)
{
    MStatus status;

    MFnPlugin pluginFn(pluginObj);

    status = pluginFn.deregisterContextCommand(JointCreateContextCmd::GetCommandName());
    if(!status)
    {
        MGlobal::displayError("Failed to deregister context command: " + JointCreateContextCmd::GetCommandName());
        return(status);
    }

    return(status);
}
