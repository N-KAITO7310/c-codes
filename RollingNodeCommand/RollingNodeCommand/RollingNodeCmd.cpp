#include "RollingNodeCmd.h"

#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MFnTransform.h>
#include <maya/MDagPath.h>
#include <maya/MItGeometry.h>

#include "RollingNode.h"

//-----------------------------------------------------------------------------
// STATIC CONSTANTS
//-----------------------------------------------------------------------------
static const MString MEL_COMMAND = "rollingNodeCmd";

static const char* CREATE_LOCATOR_FLAG[2] = { "-l", "-locator" };
static const char* VERTEX_GET_METHOD_FLAG[2] = { "-m", "-method" };

//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
RollingNodeCmd::RollingNodeCmd() :
	MPxCommand(),
	mCreateLocator(false)
{
}

RollingNodeCmd::~RollingNodeCmd()
{
}

MStatus RollingNodeCmd::doIt(const MArgList& args)
{
	MStatus status;
	MArgDatabase argData(syntax(), args, &status);
	if (!status)
	{
		MGlobal::displayError("Error parsing command arguments");
		return(status);
	}

	MSelectionList selectionList;
	status = argData.getObjects(selectionList);
	if (!status)
	{
		MGlobal::displayError("Failed to get selection list");
		return(status);
	}

	MObject selectedObjs[3];
	selectionList.getDependNode(0, selectedObjs[0]);
	selectionList.getDependNode(1, selectedObjs[1]);
	selectionList.getDependNode(2, selectedObjs[2]);

	//for (int i = 0; i < 2; i++)
	//{
	//	if (selectedObjs[i].apiType() != MFn::kTransform)
	//	{
	//		MGlobal::displayError("This command requires a transform node.");
	//		return(MS::kFailure);
	//	}
	//}
	
	// rollingNode setup translate to rotate
	MObject rollingNode = mDgModifier.createNode("rollingNode");
	MString rollingNodeName = RollingNode::GetTypeName() + 1;
	mDgModifier.renameNode(rollingNode, rollingNodeName);

	MFnDependencyNode rollingNodeFn(rollingNode);
	MString coords[] = { "X", "Y", "Z" };

	MFnDependencyNode srcFn(selectedObjs[0]);
	MFnDependencyNode dstFn(selectedObjs[1]);
	MPlug srcPlug = srcFn.findPlug(MString("translate"), false);
	MPlug rollingNodePosPlug = rollingNodeFn.findPlug(MString("position"), false);

	status = mDagModifier.connect(srcPlug, rollingNodePosPlug);
	if (!status)
	{
		MGlobal::displayError("Failed to create connection: " + status.errorString());
		return(status);
	}

	for (int i = 0; i < 3; i++)
	{
		MPlug rollingNodeRotPlug = rollingNodeFn.findPlug(MString("rotation" + coords[i]), false);
		MPlug destPlug = dstFn.findPlug(MString("rotate" + coords[i]), false);

		status = mDagModifier.connect(rollingNodeRotPlug, destPlug);
		if (!status)
		{
			MGlobal::displayError("Failed to create connection: " + status.errorString());
			return(status);
		}
	}

	// rollingNode setup radius
	MFnTransform meshObjTransformFn(selectedObjs[2]);
	MVector meshObjVec = meshObjTransformFn.getTranslation(MSpace::kWorld);

	// get all vertex
	MDagPath meshDagPath;
	selectionList.getDagPath(2, meshDagPath);
	meshDagPath.extendToShape();
	if (meshDagPath.apiType() != MFn::kMesh)
	{
		MGlobal::displayError("This source shape is not a shape.");
		return(MS::kFailure);
	}

	MItGeometry geoIter(meshDagPath, &status);
	if (!status)
	{
		MGlobal::displayError("Error: MItGeometry " + status.errorString());
		return(MS::kFailure);
	}

	mVertexMethod = argData.flagArgumentInt(VERTEX_GET_METHOD_FLAG[0], 0);
	MGlobal::displayInfo(MString() + mVertexMethod);

	geoIter.reset();
	MPoint vtxPos;
	MVector pivotToVtxVec;
	double distance = 0.0;
	double pivotToVtxDist = 0.0;
	MPoint bestDistancePoint;
	int count = 0;
	while (!geoIter.isDone())
	{
		vtxPos = geoIter.position();
		pivotToVtxVec = meshObjVec - MVector(vtxPos);
		pivotToVtxDist = pivotToVtxVec.length();

		switch (mVertexMethod)
		{
		case 0:
			// pattern max
			if (pivotToVtxDist > distance)
			{
				distance = pivotToVtxDist;
				bestDistancePoint = vtxPos;
			}
			break;
		case 1:
			// pattern min
			if (count == 0 || pivotToVtxDist < distance)
			{
				distance = pivotToVtxDist;
				bestDistancePoint = vtxPos;
			}
			break;
		case 2:
			// pattern average
			distance += pivotToVtxDist;
			bestDistancePoint = vtxPos;
			break;
		default:
			// pattern max
			if (pivotToVtxDist > distance)
			{
				distance = pivotToVtxDist;
				bestDistancePoint = vtxPos;
			}
			break;
		}

		count++;
		geoIter.next();
	}
	if (mVertexMethod == 2)
	{
		distance = distance / count;
	}
	
	// locator setup
	mCreateLocator = argData.isFlagSet(CREATE_LOCATOR_FLAG[0]);
	if (mCreateLocator)
	{
		mTransformObj = mDagModifier.createNode("transform", MObject::kNullObj);
		MString transformName = RollingNode::GetTypeName() + "locator" + 1;
		mDagModifier.renameNode(mTransformObj, transformName);
		MFnDependencyNode locFn(mTransformObj);

		mDagModifier.commandToExecute("setAttr " + transformName + ".translate" + " " + bestDistancePoint.x + " " + bestDistancePoint.y + " " + bestDistancePoint.z);

		mShapeObj = mDagModifier.createNode("locator", mTransformObj);
		MString shapeName = RollingNode::GetTypeName() + "locator" + "shape1";
		mDagModifier.renameNode(mShapeObj, shapeName);

		MObject distanceBetween = mDgModifier.createNode("distanceBetween");
		MFnDependencyNode distFn(distanceBetween);

		MPlug dstTransPlug = dstFn.findPlug(MString("translate"), false);
		MPlug locTransPlug = locFn.findPlug(MString("translate"), false);
		MPlug distPos1Plug = distFn.findPlug(MString("point1"), false);
		MPlug distPos2Plug = distFn.findPlug(MString("point2"), false);

		status = mDagModifier.connect(dstTransPlug, distPos1Plug);
		if (!status)
		{
			MGlobal::displayError("Failed to create connection: " + status.errorString());
			return(status);
		}
		status = mDagModifier.connect(locTransPlug, distPos2Plug);
		if (!status)
		{
			MGlobal::displayError("Failed to create connection: " + status.errorString());
			return(status);
		}

		MPlug distPlug = distFn.findPlug(MString("distance"), false);
		MPlug radPlug = rollingNodeFn.findPlug(MString("radius"), false);
		status = mDagModifier.connect(distPlug, radPlug);
		if (!status)
		{
			MGlobal::displayError("Failed to create connection: " + status.errorString());
			return(status);
		}

		mDagModifier.commandToExecute("parentConstraint -mo " + srcFn.name() + " " + transformName);
	}
	else
	{
		mDgModifier.commandToExecute("setAttr " + rollingNodeName + ".radius" + " " + distance);
	}


	return(redoIt());
}

MStatus RollingNodeCmd::undoIt()
{
	MStatus mDagModifierResult = mDagModifier.undoIt();
	MStatus mDgModifierResult = mDgModifier.undoIt();

	MStatus resultStatus;

	if (mDagModifierResult == MS::kSuccess && mDgModifierResult == MS::kSuccess)
	{
		resultStatus = MS::kSuccess;
	}
	else
	{
		resultStatus = MS::kFailure;
	}

	return(resultStatus);
}

MStatus RollingNodeCmd::redoIt()
{
	mDgModifier.doIt();
	mDagModifier.doIt();

	MFnDependencyNode shapeFn(mShapeObj);
	shapeFn.setName(GetShapeName(MFnDependencyNode(mTransformObj).name()));

	return(MS::kSuccess);
}

bool RollingNodeCmd::isUndoable() const
{
	return(true);
}

//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* RollingNodeCmd::Creator()
{
	return(new RollingNodeCmd());
}

MString RollingNodeCmd::CommandName()
{
	return(MEL_COMMAND);
}

MSyntax RollingNodeCmd::CreateSyntax()
{
	MSyntax syntax;

	syntax.addFlag(CREATE_LOCATOR_FLAG[0], CREATE_LOCATOR_FLAG[1], MSyntax::kBoolean);
	syntax.addFlag(VERTEX_GET_METHOD_FLAG[0], VERTEX_GET_METHOD_FLAG[1], MSyntax::kUnsigned);

	syntax.setObjectType(MSyntax::kSelectionList, 3, 3);
	syntax.useSelectionAsDefault(true);

	return(syntax);
}

MString RollingNodeCmd::GetShapeName(const MString& transformName)
{
	const int substringStart = RollingNode::GetTypeName().length();
	const int substringEnd = transformName.length() - 1;

	MString number = transformName.substring(substringStart, substringEnd);

	return(RollingNode::GetTypeName() + "Shape" + number);
}
