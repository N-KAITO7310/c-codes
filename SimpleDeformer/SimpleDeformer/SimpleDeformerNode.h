#pragma once

#include <maya/MPxDeformerNode.h>


class SimpleDeformerNode : public MPxDeformerNode
{
public:
    SimpleDeformerNode();
    virtual ~SimpleDeformerNode() override;

    virtual MStatus deform(MDataBlock& dataBlock, MItGeometry& geoIter, const MMatrix& matrix, unsigned int multiIndex) override;

    // Static Methods
    static void* Creator();
    static MStatus Initialize();

    static MTypeId GetTypeId();
    static MString GetTypeName();
};