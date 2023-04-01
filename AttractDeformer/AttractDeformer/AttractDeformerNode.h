#pragma once

#include <maya/MPxDeformerNode.h>


class AttractDeformerNode : public MPxDeformerNode
{
public:
    AttractDeformerNode();
    virtual ~AttractDeformerNode() override;

    virtual MStatus deform(MDataBlock& dataBlock, MItGeometry& geoIter, const MMatrix& matrix, unsigned int multiIndex) override;

    // Static Methods
    static void* Creator();
    static MStatus Initialize();

    static MTypeId GetTypeId();
    static MString GetTypeName();


private:
    static MObject maxDistanceObj;
    static MObject targetPositionObj;
};
