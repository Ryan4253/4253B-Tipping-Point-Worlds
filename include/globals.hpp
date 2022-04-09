#pragma once
#include "main.h"

#define LVGL_SCREEN_WIDTH 480
#define LVGL_SCREEN_HEIGHT 240

// CONSTANTS
const double DEADBAND = 0.0500;

namespace LiftPosition{
    const int MINHEIGHT = 0;
    const int LOWBRANCH = 0; // TODO TUNE READING
    const int TOPBRANCH = 0; // TODO TUNE READING
    const int MAXHEIGHT = 750;
};

namespace TopBranchPosition{
    const int REST = 0; // TODO TUNE READING
    const int LOADING = 0; // TODO TUNE READING
    const int LOWBRANCH = 0; // TODO TUNE READING
    const int TOPBRANCH = 0; // TODO TUNE READING   
}
// CONTROLLER
extern Controller master;
extern Controller partner;

// MOTORS
extern MotorGroup leftDrive;
extern MotorGroup rightDrive;
extern MotorGroup lift;
extern Motor topBranch;
extern Motor roller;

// SENSORS
extern RotationSensor left;
extern RotationSensor right;
extern RotationSensor mid;
extern IMU imu; 
extern pros::Vision vision_sensor;

// PNEUMATICS
extern Pneumatics clamp;
extern Pneumatics backClamp;
extern Pneumatics tilter;
extern Pneumatics needle;

// MOTION PROFILE CONSTANTS
extern ProfileConstraint moveLimit;
extern ProfileConstraint turnLimit;
extern FFVelocityController leftController;
extern FFVelocityController rightController;

// SUBSYSTEM CONTROLLERS
extern std::shared_ptr<OdomChassisController> chassis;
extern std::shared_ptr<AsyncMotionProfiler> profiler;
extern std::shared_ptr<AsyncMotionProfiler> turnProfiler;
extern std::shared_ptr<AsyncPositionController<double, double>> liftController;
extern std::shared_ptr<AsyncPositionController<double, double>> topBranchController;

/**
 * @brief Create a Blank Background using LVGL
 * 
 */
void createBlankBackground();