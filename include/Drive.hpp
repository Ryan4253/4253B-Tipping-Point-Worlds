#pragma once
#include "main.h"

/**
 * @brief Turns the robot to the desired global angle (using closed-loop control)
 *
 * @param targetAngle - the target odometry global angle to turn to
 */
void turnToAngle(QAngle targetAngle);
