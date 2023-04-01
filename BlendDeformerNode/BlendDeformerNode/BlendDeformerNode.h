#pragma once

#include <maya/MPxDeformerNode.h>


class BlendDeformerNode : public MPxDeformerNode
{
public:
    BlendDeformerNode();
    virtual ~BlendDeformerNode() override;

    virtual MStatus deform(MDataBlock& dataBlock, MItGeometry& geoIter, const MMatrix& matrix, unsigned int multiIndex) override;

    // Static Methods
    static void* Creator();
    static MStatus Initialize();

    static MTypeId GetTypeId();
    static MString GetTypeName();

private:
    static MObject blendMeshObj;
    static MObject blendWeightObj;

};