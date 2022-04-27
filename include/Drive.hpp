#pragma once
#include "main.h"

/**
26 * @brief Turns the robot to the desired global angle (using closed-loop control)
27 *
28 * @param targetAngle - the target odometry global angle to turn to, normalized to [-pi,
,→ pi]
29 */
void turnToAngle(okapi::QAngle targetAngle);