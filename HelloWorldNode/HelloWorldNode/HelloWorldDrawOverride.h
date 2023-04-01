#pragma once

#include <maya/MPxDrawOverride.h>

class HelloWorldDrawOverride : public MHWRender::MPxDrawOverride
{
public:
	virtual ~HelloWorldDrawOverride() override;

	virtual MHWRender::DrawAPI supportedDrawAPIs() const override;

	virtual bool hasUIDrawables() const override;
	virtual void addUIDrawables(const MDagPath& objPath, MUIDrawManager& drawManager, const MFrameContext& frameContext, const MUserData* data) override;

	virtual MUserData* prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData) override;

	// static method
	static MHWRender::MPxDrawOverride* Creator(const MObject& obj);

private:
	HelloWorldDrawOverride(const MObject& obj);

};