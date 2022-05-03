#include "ExpandedSkidSteerModel.hpp"

namespace ryan{

void ExpandedSkidSteerModel::curvature(double iThrottle, double iCurvature, double iThreshold){
    if(std::fabs(iThrottle) <= iThreshold){
        arcade(0, iCurvature, iThreshold);
        return;
    }

    iThrottle = std::fabs(iThrottle) > iThreshold ? iThrottle : 0;
    iCurvature = std::fabs(iCurvature) > iThreshold ? iCurvature : 0;

    double left = iThrottle + std::fabs(iThrottle) * iCurvature;
    double right = iThrottle - std::fabs(iThrottle) * iCurvature;

    double mag = std::max(std::fabs(left), std::fabs(right));
    if(mag > 1.0){
        left /= mag;
        right /= mag;
    }

    leftSideMotor->moveVoltage(left * maxVoltage);
    rightSideMotor->moveVoltage(right * maxVoltage);
}

}