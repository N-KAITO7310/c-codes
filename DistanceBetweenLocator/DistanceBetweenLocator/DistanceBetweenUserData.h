#include <maya/MColor.h>
#include <maya/MPoint.h>
#include <maya/MUserData.h>


class DistanceBetweenUserData : public MUserData
{
public:
    DistanceBetweenUserData(bool deleteAfterUse);
    virtual ~DistanceBetweenUserData() override;

    double distance;

    MPoint point1;
    MPoint point2;

    MColor color;
};
