#pragma once

#include <maya/MPxLocatorNode.h>


class DistanceBetweenLocator : public MPxLocatorNode
{
public:
    DistanceBetweenLocator();
    virtual ~DistanceBetweenLocator() override;

    virtual void postConstructor() override;

    virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;


    // Static methods
    static void* Creator();
    static MStatus Initialize();

    static MTypeId GetTypeId();
    static MString GetTypeName();

    static MString GetDrawDbClassification();
    static MString GetDrawRegistrationId();


private:
    static MObject point1Obj;
    static MObject point2Obj;
    static MObject distanceObj;

    static MObject minDistanceObj;
    static MObject maxDistanceObj;
    static MObject colorRampObj;
};
