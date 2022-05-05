#include "main.h"
using namespace ryan;

// CONTROLLERS
Controller master(ControllerId::master);
Controller partner(ControllerId::partner);

// MOTORS
Motor leftFront(3, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); 
Motor leftBack(2, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); 
Motor rightFront(17, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); 
Motor rightBack(20, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); 
Motor liftTop(4, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); 
Motor liftBottom(6, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor roller(10, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees); 
Motor topBranch(15, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees); 

MotorGroup leftDrive({leftFront, leftBack});
MotorGroup rightDrive({rightFront, rightBack});
MotorGroup lift({liftTop, liftBottom});

// SENSORS
RotationSensor left(1, true); // todo check port
RotationSensor right(18); 
RotationSensor mid(9);
RotationSensor topBranchSensor(14, true);
IMU imu(8);
pros::Vision vision(16); 

// PNEUMATICS
Pneumatics clamp('F'); 
Pneumatics backClamp('C'); 
Pneumatics tilter('E'); 
Pneumatics leftNeedle('H'); 
Pneumatics rightNeedle('G'); 

// MOTION PROFILE CONSTANTS
ProfileConstraint moveLimit({3_ftps, 5_ftps2, 5_ftps2, 25_ftps3}); // TODO TUNE GAINS
ProfileConstraint turnLimit({4.8_ftps, 17.5_ftps2, 17.5_ftps2, 25_ftps3}); // TODO TUNE GAINS
FFVelocityController leftController(0.187, 0.04, 0.025, 2.5, 0); // TODO TUNE GAINS
FFVelocityController rightController(0.187, 0.04, 0.025, 2.5, 0); // TODO TUNE GAINS

// SUBSYSTEM CONTROLLERS
std::shared_ptr<OdomChassisController> chassis = ChassisControllerBuilder()
    .withMotors(leftDrive, rightDrive)
    .withDimensions({AbstractMotor::gearset::green, 5.0/7.0}, {{3.25_in, 1.294_ft}, imev5BlueTPR}) // TODO CALCULATE CORRECT TRACK WIDTH
    .withSensors(left, right, mid)
    .withOdometry({{2.75_in, 7_in, 2_in, 2.75_in}, 360}, StateMode::CARTESIAN) // TODO CALCULATE CORRECT TRACK WIDTH
    .buildOdometry();

std::shared_ptr<AsyncMotionProfiler> profiler = AsyncMotionProfilerBuilder()
    .withOutput(chassis)
    .withProfiler(std::make_unique<SCurveMotionProfile>(moveLimit))
    .build();

std::shared_ptr<AsyncMotionProfiler> turnProfiler = AsyncMotionProfilerBuilder()
    .withOutput(chassis)
    .withProfiler(std::make_unique<SCurveMotionProfile>(turnLimit))
    .build();

std::shared_ptr<AsyncPositionController<double, double>> liftController = AsyncPosControllerBuilder()
    .withMotor(lift)
    // .withGearset({AbstractMotor::gearset::green, 7.0/1.0})
    .withGains({0.007, 0.0, 0.000075}) // TODO TUNE GAINS
    .build();

std::shared_ptr<AsyncPositionController<double, double>> topBranchController = AsyncPosControllerBuilder()
    .withMotor(topBranch)
    // .withGearset({AbstractMotor::gearset::red, 3.0/1.0})
    .withMaxVelocity(0.5)
    // .withSensor(std::make_shared<RotationSensor>(topBranchSensor))
    .withGains({0.00685, 0.0, 0.000082}) // TODO TUNE GAINS 0.000075
    // .withMaxVelocity(0.5)
    .build();

std::shared_ptr<IterativePosPIDController> turnPID = std::make_shared<IterativePosPIDController>(0.037, 0.0, 0.00065, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)); // #TODO - Tune Constant
std::shared_ptr<IterativePosPIDController> agroTurnPID = std::make_shared<IterativePosPIDController>(0.05, 0.00025, 0.00065, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)); // #TODO - Tune Constant

void createBlankBackground(){
    lv_obj_t *background;
    lv_style_t backgroundStyle;
    lv_style_copy(&backgroundStyle, &lv_style_plain);
    backgroundStyle.body.main_color = LV_COLOR_BLACK;
    backgroundStyle.body.grad_color = LV_COLOR_BLACK;
    backgroundStyle.body.radius = 0;
    backgroundStyle.text.color = LV_COLOR_WHITE;
    background = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_free_num(background, 0);
    lv_obj_set_style(background, &backgroundStyle);
    lv_obj_set_size(background, LVGL_SCREEN_WIDTH, LVGL_SCREEN_HEIGHT);
    lv_obj_align(background, NULL, LV_ALIGN_CENTER, 0, 0);
}