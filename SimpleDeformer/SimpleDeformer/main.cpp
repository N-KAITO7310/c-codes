#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "SimpleDeformerNode.h"


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

    status = pluginFn.registerNode(SimpleDeformerNode::GetTypeName(),
                                   SimpleDeformerNode::GetTypeId(),
                                   SimpleDeformerNode::Creator,
                                   SimpleDeformerNode::Initialize,
                                   MPxNode::kDeformerNode);
    if(!status)
    {
        MGlobal::displayError("Failed to register node: " + SimpleDeformerNode::GetTypeName());
        return(status);
    }

    return(MS::kSuccess);
}

MStatus uninitializePlugin(MObject pluginObj)
{
    MStatus status;

    MFnPlugin pluginFn(pluginObj);

    status = pluginFn.deregisterNode(SimpleDeformerNode::GetTypeId());
    if(!status)
    {
        MGlobal::displayError("Failed to deregister node: " + SimpleDeformerNode::GetTypeName());
        return(status);
    }

    return(status);
}
