#include "ExpandedSkidSteerModel.hpp"

namespace ryan{

void ExpandedSkidSteerModel::curvature(double iThrottle, double iCurvature, double iThreshold){
    if(abs(iThrottle) <= iThreshold){
        arcade(0, iCurvature, iThreshold);
        return;
    }

    iThrottle = abs(iThrottle) > iThreshold ? iThrottle : 0;
    iCurvature = abs(iCurvature) > iThreshold ? iCurvature : 0;

    double left = iThrottle + abs(iThrottle) * iCurvature;
    double right = iThrottle - abs(iThrottle) * iCurvature;

    double mag = std::max(abs(left), abs(right));
    if(mag > 1.0){
        left /= mag;
        right /= mag;
    }

    leftSideMotor->moveVoltage(left * maxVoltage);
    rightSideMotor->moveVoltage(right * maxVoltage);
}

}