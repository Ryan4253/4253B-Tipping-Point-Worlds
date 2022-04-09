#include "main.h"
using namespace ryan;

// CONTROLLERS
Controller master(ControllerId::master);
Controller parter(ControllerId::partner);

// MOTORS
Motor leftFront(3);
Motor leftBack(2);
Motor rightFront(17);
Motor rightBack(20);
Motor liftTop(4);
Motor liftBottom(0); // todo - check bottom lift port

MotorGroup leftDrive({leftFront, leftBack});
MotorGroup rightDrive({rightFront, rightBack});
MotorGroup lift({liftTop, liftBottom});
Motor roller(10);
Motor topBranch(0); // todo - check motor port

// SENSORS
RotationSensor leftEncoder(1);
RotationSensor rightEncoder(18);
RotationSensor midEncoder(0); // check port
IMU imu(6);
pros::Vision vision(0);

// PNEUMATICS
// TODO - check wiring
Pneumatics clamp('A', true);
Pneumatics backClamp('B');
Pneumatics tilter('C', true);
Pneumatics needle('D');

// MOTION PROFILE CONSTANTS


// SUBSYSTEM CONTROLLERS
auto chassis = ChassisControllerBuilder()
	.withMotors(leftDrive, rightDrive)
    // todo - check gear ratio
	.withDimensions({AbstractMotor::gearset::green, 5.0/3.0}, {{3.25_in, 1.294_ft}, imev5GreenTPR})
    .withSensors(leftEncoder, rightEncoder, midEncoder)
    // todo - check tracking wheel track
    .withOdometry({{2.75_in, 7_in}, 360}, StateMode::FRAME_TRANSFORMATION)
	.build();
std::shared_ptr<AsyncMotionProfiler> profiler;
std::shared_ptr<AsyncMotionProfiler> turnProfiler;
extern std::shared_ptr<AsyncPositionController<double, double>> liftController;
extern std::shared_ptr<AsyncPositionController<double, double>> topBranchController;

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