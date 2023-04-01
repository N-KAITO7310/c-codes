#pragma once

#include <maya/MPxNode.h>


class MultiplyNode : public MPxNode
{
public:
    MultiplyNode();
    virtual ~MultiplyNode() override;

    virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

    // Static Methods
    static void* Creator();
    static MStatus Initialize();

    static MTypeId GetTypeId();
    static MString GetTypeName();

private:
    // input attrs
    static MObject multiplierObj;
    static MObject multiplicandObj;

    // output attr
    static MObject productObj;
};
