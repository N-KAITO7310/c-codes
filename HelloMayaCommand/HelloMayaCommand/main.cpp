//#include <maya/MSimple.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
//DeclareSimpleCommand(getMeshVertexCount, "TECH ART ONLINE", "2020");

// GetMeshVertexCount
class GetMeshVertexCount : public MPxCommand
{
public:
	MStatus doIt(const MArgList&);
	static void* creator();
};

MStatus GetMeshVertexCount::doIt(const MArgList & args)
{
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);
	MItSelectionList iter(selection, MFn::kMesh);
	MDagPath dagPath;
	MFnMesh fnMesh;

	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath);
		fnMesh.setObject(dagPath);
		int numVertices = fnMesh.numVertices();
		char text[100];
		sprintf_s(text, "%s vertex count = %d", dagPath.partialPathName().asChar(), numVertices);
		MGlobal::displayInfo(text);
	}

	return MS::kSuccess;
}

void* GetMeshVertexCount::creator()
{
	return new GetMeshVertexCount();
}


// PrintHello
class PrintHello : public MPxCommand
{
public:
	MStatus doIt(const MArgList&);
	static void* creator();
};

MStatus PrintHello::doIt(const MArgList& args)
{
	MGlobal::displayInfo("Hello MPxCommand");
	return MS::kSuccess;
}

void* PrintHello::creator()
{
	return new GetMeshVertexCount();
}


MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "TTECH ART ONLINE", "1.0", "Any");
	plugin.registerCommand("getMeshVertexCount", GetMeshVertexCount::creator);
	plugin.registerCommand("printHello", PrintHello::creator);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("getMeshVertexCount");
	plugin.deregisterCommand("printHello");
	return MS::kSuccess;
}