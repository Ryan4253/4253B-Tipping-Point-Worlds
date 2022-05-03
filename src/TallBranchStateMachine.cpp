#include "main.h"

void TallBranchStateMachine::move() {
    switch (getState()) {
        case TallBranchState::IDLE:
            liftController->flipDisable(true);
            topBranchController->setTarget(0);
            leftNeedle.set(false);
            rightNeedle.set(false);
            break;
        case TallBranchState::LOADING:
            liftController->flipDisable(false);
            liftController->setTarget(0);
            topBranchController->setTarget(-810);
            leftNeedle.set(false);
            rightNeedle.set(false);
            break;
        case TallBranchState::LOADED:
            liftController->flipDisable(false);
            liftController->setTarget(0);
            topBranchController->setTarget(-810);
            leftNeedle.set(true);
            rightNeedle.set(true);
            break;
        case TallBranchState::UNLOADING:
            liftController->flipDisable(false);
            liftController->setTarget(700);
            topBranchController->setTarget(-240);
            leftNeedle.set(true);
            rightNeedle.set(true);

            tilter.set(false);
            backClamp.set(false);
            break;
        case TallBranchState::CLAMPED:
            liftController->flipDisable(false);
            liftController->setTarget(700);
            topBranchController->setTarget(-240);
            leftNeedle.set(true);
            rightNeedle.set(true);

            tilter.set(false);
            backClamp.set(true);
            break;
        case TallBranchState::UNLOADED:
            liftController->flipDisable(false);
            liftController->setTarget(700);
            topBranchController->setTarget(-240);
            leftNeedle.set(false);
            rightNeedle.set(false);
            break;
    }
}
