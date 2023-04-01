#include "HelloWorldNode.h"

static const MTypeId TYPE_ID = MTypeId(0x0007F7F7);
static const MString TYPE_NAME = "helloworld";

static const MString DRAW_DB_CLASSIFICATION = "drawdb/geometry/helloworld";
static const MString DRAW_REGISTRATION_ID = "HelloWorldNode";

HelloWorldNode::HelloWorldNode() :
	MPxLocatorNode()
{

}

HelloWorldNode::~HelloWorldNode()
{

}

void* HelloWorldNode::Creator()
{
	return(new HelloWorldNode());
}

MStatus HelloWorldNode::Initialize()
{
	return(MStatus::kSuccess);
}

MTypeId HelloWorldNode::GetTypeId()
{
	return(TYPE_ID);
}

MString HelloWorldNode::GetTypeName()
{
	return(TYPE_NAME);
}

MString HelloWorldNode::GetDrawDbClassification()
{
	return(DRAW_DB_CLASSIFICATION);
}

MString HelloWorldNode::GetDrawRegistrationId()
{
	return(DRAW_REGISTRATION_ID);
}