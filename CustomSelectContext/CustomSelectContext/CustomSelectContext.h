#include <maya/MPxContext.h>


class CustomSelectContext : public MPxContext
{
public:
    CustomSelectContext();
    virtual ~CustomSelectContext() override;

    virtual MStatus helpStateHasChanged(MEvent& event) override;

    virtual MStatus doPress(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;
    virtual MStatus doRelease(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;
    virtual MStatus doDrag(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;

private:
    void DrawSelectionRectangle(MHWRender::MUIDrawManager& drawMgr, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);

    short mViewportStartX, mViewportStartY;
    short mViewportEndX, mViewportEndY;

    bool mLightsOnly;
    bool mMeshesOnly;

};
