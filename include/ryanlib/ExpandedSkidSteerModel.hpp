#pragma once
#include "okapi/api/chassis/model/skidSteerModel.hpp"

namespace ryan{

class ExpandedSkidSteerModel : public okapi::SkidSteerModel{
    public:
    void curvature(double iThrottle, double iCurvature, double iThreshold = 0);

};

}
