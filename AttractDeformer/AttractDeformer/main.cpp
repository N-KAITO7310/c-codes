#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "AttractDeformerNode.h"


MStatus initializePlugin(MObject pluginObj)
{
    const char* vendor = "Kaito Nakamura";
    const char* version = "1.0.1";
    const char* requiredApiVersion = "Any";

    MStatus status;

    MFnPlugin pluginFn(pluginObj, vendor, version, requiredApiVersion, &status);
    if(!status)
    {
        MGlobal::displayError("Failed to initialize plugin: " + status.errorString());
        return(status);
    }

    status = pluginFn.registerNode(AttractDeformerNode::GetTypeName(),
                                   AttractDeformerNode::GetTypeId(),
                                   AttractDeformerNode::Creator,
                                   AttractDeformerNode::Initialize,
                                   MPxNode::kDeformerNode);
    if(!status)
    {
        MGlobal::displayError("Failed to register node: " + AttractDeformerNode::GetTypeName());
        return(status);
    }

    return(MS::kSuccess);
}

MStatus uninitializePlugin(MObject pluginObj)
{
    MStatus status;

    MFnPlugin pluginFn(pluginObj);

    status = pluginFn.deregisterNode(AttractDeformerNode::GetTypeId());
    if(!status)
    {
        MGlobal::displayError("Failed to deregister node: " + AttractDeformerNode::GetTypeName());
        return(status);
    }

    return(status);
}
