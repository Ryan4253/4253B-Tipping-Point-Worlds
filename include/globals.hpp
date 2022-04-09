#pragma once
#include "main.h"

#define LVGL_SCREEN_WIDTH 480
#define LVGL_SCREEN_HEIGHT 240

// CONSTANTS
const double DEADBAND = 0.0500;
const int MAX_LIFT_HEIGHT = 750; 
const int LIFT_STEP = 11; 

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
extern ProfileConstraint constraint;
extern ProfileConstraint trapLimit;
extern FFVelocityController leftLinear;
extern FFVelocityController rightLinear;
extern FFVelocityController leftTrajectory;
extern FFVelocityController rightTrajectory;

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