#include "main.h"

void turnToAngle(okapi::QAngle targetAngle){
    turnPID->reset();
    turnPID->setTarget(0);

    do {
        (chassis->getModel())->arcade(0, turnPID->step(-Math::rescale180(targetAngle.convert(degree)-imu.get())));
        pros::delay(10);
    } while(!turnPID->isSettled());

    (chassis->getModel())->stop();
}