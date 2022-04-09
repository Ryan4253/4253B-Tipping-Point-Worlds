#include "main.h"
using namespace ryan;

// CONTROLLER
Controller master(ControllerId::master);
Controller parter(ControllerId::partner);

// MOTOR
Motor leftFront(0);
Motor leftBack(0);
Motor rightFront(0);
Motor rightBack(0);
Motor liftTop(0);
Motor liftBottom(0);

MotorGroup leftDrive({leftFront, leftBack});
MotorGroup rightDrive({rightFront, rightBack});
MotorGroup lift({liftTop, liftBottom});
Motor roller(0);
Motor topBranch(0);

// SENSOR
RotationSensor left(0);
RotationSensor right(0);
RotationSensor mid(0);
IMU imu(0);
pros::Vision vision(0);

// PNEUMATICS
Pneumatics clamp('A', true);
Pneumatics backClamp('B');
Pneumatics tilter('C', true);
Pneumatics needle('D');

// MOTION PROFILE CONSTANTS


// SUBSYSTEM CONTROLLER
std::shared_ptr<OdomChassisController> chassis;
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