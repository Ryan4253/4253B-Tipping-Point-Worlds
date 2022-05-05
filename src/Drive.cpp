#include "main.h"

void turnToAngle(okapi::QAngle targetAngle, bool agro){
    turnPID->reset();
    turnPID->setTarget(0);
    agroTurnPID->reset();
    agroTurnPID->setTarget(0);

    do {
        if(!agro){
            (chassis->getModel())->arcade(0, turnPID->step(-Math::rescale180(targetAngle.convert(degree)-imu.get())));
        } else {
            (chassis->getModel())->arcade(0, agroTurnPID->step(-Math::rescale180(targetAngle.convert(degree)-imu.get())));
        }
        pros::delay(10);
    } while(agro ? !agroTurnPID->isSettled() : !turnPID->isSettled());

    (chassis->getModel())->stop();
}