#include "HelloWorldDrawOverride.h"

// public
HelloWorldDrawOverride::~HelloWorldDrawOverride()
{

}

MHWRender::DrawAPI HelloWorldDrawOverride::supportedDrawAPIs() const
{
	return(MHWRender::kAllDevices);
}

bool HelloWorldDrawOverride::hasUIDrawables() const
{
	return(true);
}

void HelloWorldDrawOverride::addUIDrawables(const MDagPath& objPath, MUIDrawManager& drawManager, const MFrameContext& frameContext, const MUserData* data)
{
	drawManager.beginDrawable();

	drawManager.text2d(MPoint(100, 100), "Hello World");

	drawManager.endDrawable();
}

MUserData* HelloWorldDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData)
{
	return(nullptr);
}

// static
MHWRender::MPxDrawOverride* HelloWorldDrawOverride::Creator(const MObject& obj)
{
	return(new HelloWorldDrawOverride(obj));
}

// private
HelloWorldDrawOverride::HelloWorldDrawOverride(const MObject& obj) :
	MHWRender::MPxDrawOverride(obj, nullptr)
{

}