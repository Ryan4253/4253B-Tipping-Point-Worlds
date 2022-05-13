#pragma once
#include "okapi/api/chassis/model/skidSteerModel.hpp"

namespace ryan{

/**
 * @brief A subclass to the okapi SkidSteerModel class which adds extra functionality
 *        To use, simply static point cast the SkidSteerModel into this class and use the functions
 * 
 */
class ExpandedSkidSteerModel : public okapi::SkidSteerModel{
    public:
    /**
     * @brief Computes the desired drive speed to control the chassis using curvature control
     *    The turn power is scaled with the linear power in order to allow finer control
     *    of the chassis. The easiest way to imagine this control is by consider driving
     *    a car: the left stick is the gas pedal (throttle), the right stick is the steering
     *    wheel, which controls the curvature the car drives in by manipulating the wheel location
     * 
     * @param iThrottle Desired power, normalized to [-1, 1]
     * @param iCurvature Desired curvature to drive, normalized to [-1, 1]
     * @param iThreshold Deadband of the function, any value between -iThreshold and iThreshold will be treated as 0
     * 
     */
    void curvature(double iThrottle, double iCurvature, double iThreshold = 0);

};

}
