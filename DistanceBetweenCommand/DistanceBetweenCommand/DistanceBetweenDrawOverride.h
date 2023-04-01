#pragma once

#include <maya/MPxDrawOverride.h>


class DistanceBetweenDrawOverride : public MHWRender::MPxDrawOverride
{
public:
    virtual ~DistanceBetweenDrawOverride() override;

    virtual MHWRender::DrawAPI supportedDrawAPIs() const override;
    virtual bool hasUIDrawables() const  override;

    virtual bool refineSelectionPath(const MSelectionInfo& selectInfo, const MRenderItem& hitItem, MDagPath& path, MObject& geomComponents, MSelectionMask& objectMask) override;

    virtual MUserData* prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData) override;
    virtual void addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* userData) override;


    // Static Methods
    static MHWRender::MPxDrawOverride* Creator(const MObject& obj);

private:
    DistanceBetweenDrawOverride(const MObject& obj);
};
