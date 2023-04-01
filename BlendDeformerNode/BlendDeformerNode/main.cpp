#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include "BlendDeformerNode.h"


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

    status = pluginFn.registerNode(BlendDeformerNode::GetTypeName(),
                                   BlendDeformerNode::GetTypeId(),
                                   BlendDeformerNode::Creator,
                                   BlendDeformerNode::Initialize,
                                   MPxNode::kDeformerNode);
    if(!status)
    {
        MGlobal::displayError("Failed to register node: " + BlendDeformerNode::GetTypeName());
        return(status);
    }

    MString cmd = MString("import maya.cmds as cmds\n");
    cmd += MString("cmds.makePaintable(\"") + BlendDeformerNode::GetTypeName() + MString("\", \"weights\", attrType=\"multiFloat\", shapeMode=\"deformer\")");
    MGlobal::executePythonCommand(cmd);

    return(MS::kSuccess);
}

MStatus uninitializePlugin(MObject pluginObj)
{
    MStatus status;

    MString cmd = MString("import maya.cmds as cmds\n");
    cmd += MString("cmds.makePaintable(\"") + BlendDeformerNode::GetTypeName() + MString("\", \"weights\", remove=True)");
    MGlobal::executePythonCommand(cmd);

    MFnPlugin pluginFn(pluginObj);

    status = pluginFn.deregisterNode(BlendDeformerNode::GetTypeId());
    if(!status)
    {
        MGlobal::displayError("Failed to deregister node: " + BlendDeformerNode::GetTypeName());
        return(status);
    }

    return(status);
}
