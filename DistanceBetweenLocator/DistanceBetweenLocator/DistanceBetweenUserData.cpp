#include "DistanceBetweenUserData.h"

DistanceBetweenUserData::DistanceBetweenUserData(bool deleteAfterUse) :
    MUserData(deleteAfterUse),
    distance(0)
{
}

DistanceBetweenUserData::~DistanceBetweenUserData()
{
}
