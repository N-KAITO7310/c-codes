#pragma once

#include <maya/MPxDrawOverride.h>


class SwitchableShapeDrawOverride : public MHWRender::MPxDrawOverride
{
public:
	virtual ~SwitchableShapeDrawOverride() override;

	virtual MHWRender::DrawAPI supportedDrawAPIs() const override;
	virtual bool hasUIDrawables() const override;

	virtual MUserData* prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData);
	virtual void addUIDrawables(const MDagPath& objPath, MUIDrawManager& drawManager, const MFrameContext& frameContext, const MUserData* data);

	// Static Methods
	static MHWRender::MPxDrawOverride* Creator(const MObject& obj);

private:
	SwitchableShapeDrawOverride(const MObject& obj);
};
