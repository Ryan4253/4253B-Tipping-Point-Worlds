#include "main.h"
using namespace ryan;

// CONTROLLER
Controller master(ControllerId::master);
Controller parter(ControllerId::partner);

// MOTOR
Motor leftFront(3, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); // TODO CHECK REVERSE
Motor leftBack(2, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); // TODO CHECK REVERSE
Motor rightFront(17, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); // TODO CHECK REVERSE
Motor rightBack(20, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); // TODO CHECK REVERSE
Motor liftTop(4, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); // TODO CHECK REVERSE
Motor liftBottom(0, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); // TODO CHANGE PORT, CHECK REVERSE
Motor roller(10, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees); // TODO CHECK REVERSE
Motor topBranch(0, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees); // TODO CHANGE PORT, CHECK REVERSE

MotorGroup leftDrive({leftFront, leftBack});
MotorGroup rightDrive({rightFront, rightBack});
MotorGroup lift({liftTop, liftBottom});

// SENSOR
RotationSensor left(1); // TODO CHECK REVERSE
RotationSensor right(18); // TODO CHECK REVERSE
RotationSensor mid(0); // TODO CHANGE PORT, CHECK REVERSE
IMU imu(6);
pros::Vision vision(0); // TODO CHANGE PORT

// PNEUMATICS
Pneumatics clamp('A', true); // TODO CHANGE PORT, CHECK REVERSE
Pneumatics backClamp('B'); // TODO CHANGE PORT, CHECK REVERSE
Pneumatics tilter('C', true); // TODO CHANGE PORT, CHECK REVERSE
Pneumatics needle('D'); // TODO CHANGE PORT, CHECK REVERSE

// MOTION PROFILE CONSTANTS
ProfileConstraint moveLimit({4.8_ftps, 17.5_ftps2, 17.5_ftps2, 25_ftps3}); // TODO TUNE GAINS
ProfileConstraint turnLimit({4.8_ftps, 17.5_ftps2, 17.5_ftps2, 25_ftps3}); // TODO TUNE GAINS
FFVelocityController leftController(0.187, 0.04, 0.025, 2.5, 0); // TODO TUNE GAINS
FFVelocityController rightController(0.187, 0.04, 0.025, 2.5, 0); // TODO TUNE GAINS

// SUBSYSTEM CONTROLLER
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
    .withGains({0.007, 0.0, 0.000075}) // TODO TUNE GAINS
    .build();

std::shared_ptr<AsyncPositionController<double, double>> topBranchController = AsyncPosControllerBuilder()
    .withMotor(topBranch)
    .withGains({0.007, 0.0, 0.000075}) // TODO TUNE GAINS
    .build();

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