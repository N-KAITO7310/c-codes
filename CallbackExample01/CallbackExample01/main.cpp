#include <maya/MCallbackIdArray.h>
#include <maya/MConditionMessage.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MEventMessage.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MSceneMessage.h>
#include <maya/MTimerMessage.h>
#include <maya/MUiMessage.h>


static MCallbackIdArray sCallbackIds;


static void OnNewScene(void* clientData)
{
    MGlobal::displayInfo("[Callback] New scene opened");
}

static void OnTimeChanged(void* clientData)
{
    MGlobal::displayInfo("[Callback] Time changed");
}

static void OnSelectionChanged(void* clientData)
{
    MGlobal::displayInfo("[Callback] Selection changed");
}

static void BeforeImport(void* clientData)
{
    MGlobal::displayInfo("Import pre-processing");
}

static void AfterImport(void* clientData)
{
    MGlobal::displayInfo("Import post-processing");
}

static void OnViewportCameraChanged(const MString& panelName, MObject& node, void* clientData)
{
    MFnDependencyNode nodeFn(node);

    MGlobal::displayInfo(MString("Camera changed in ") + panelName + " to \"" + nodeFn.name() + "\"");
}

static void OnPlayingBackStateChanged(bool isPlaying, void* clientData)
{
    MGlobal::displayInfo(MString("Playing state changed: ") + static_cast<int>(isPlaying));
}

static void OnTimerFired(float elapsedTime, float previousExecutionTime, void* clientData)
{
    MGlobal::displayInfo(MString("Timer fired - Elapsed Time: ") + elapsedTime);
}

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

    //sCallbackIds.append(MEventMessage::addEventCallback("NewSceneOpened", OnNewScene));
    //sCallbackIds.append(MEventMessage::addEventCallback("timeChanged", OnTimeChanged));
    //sCallbackIds.append(MEventMessage::addEventCallback("SelectionChanged", OnSelectionChanged));

    sCallbackIds.append(MSceneMessage::addCallback(MSceneMessage::kBeforeImport, BeforeImport));
    sCallbackIds.append(MSceneMessage::addCallback(MSceneMessage::kAfterImport, AfterImport));

    sCallbackIds.append(MUiMessage::addCameraChangedCallback("modelPanel4", OnViewportCameraChanged));

    sCallbackIds.append(MConditionMessage::addConditionCallback("playingBack", OnPlayingBackStateChanged));

    sCallbackIds.append(MTimerMessage::addTimerCallback(2.5, OnTimerFired));

    return(MS::kSuccess);
}

MStatus uninitializePlugin(MObject pluginObj)
{
    MStatus status;

    MMessage::removeCallbacks(sCallbackIds);
    sCallbackIds.clear();

    return(status);
}
