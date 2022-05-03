#include "main.h"

void initialize() {
	// LCD initialization
	pros::lcd::initialize();

	// Vision initialization
    auto RED_SIG = pros::Vision::signature_from_utility(1, 5685, 8135, 6910, -1963, -1705, -1834, 4.000, 0);
    vision.set_signature(1, &RED_SIG);

	// imu initialization
	pros::lcd::set_text(2, "imu calibrating..............");
	imu.calibrate();
	pros::delay(2000);
	pros::lcd::set_text(2, "imu calibrated");

    // topBranchSensor.reset();
    topBranch.tarePosition();
    topBranchController->tarePosition();
    lift.tarePosition();
    liftController->tarePosition();
    // topBranchController->setMaxVelocity(0.5);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol(){
	// sets brake mode
	leftDrive.setBrakeMode(AbstractMotor::brakeMode::coast);
	rightDrive.setBrakeMode(AbstractMotor::brakeMode::coast);
    lift.setBrakeMode(AbstractMotor::brakeMode::hold);
    topBranch.setBrakeMode(AbstractMotor::brakeMode::hold);

	// initializes subsystems
	auto model = std::static_pointer_cast<ExpandedSkidSteerModel>(chassis->getModel());
    // liftController->flipDisable(true);

	// initializes constants
	bool rollerState = false;

	// initializes gif
	createBlankBackground();
	auto gif = std::make_unique<Gif>("/usd/gif/crab-rave.gif", lv_scr_act());

    auto swingerState = ControllerDigital::down;
    
    // topBranchController->setTarget(-260);
    // backClamp.set(true);
    // pros::delay(500);
    // liftController->setTarget(750);
    topBranchController->setTarget(TopBranchPosition::REST);

	while(true){
        if(master[ControllerDigital::A].changedToPressed()) turnToAngle(0_deg);

		model->curvature(master.getAnalog(ControllerAnalog::leftY), master.getAnalog(ControllerAnalog::rightX), 0.05);

        /** PID Lift Control */
        if(partner[ControllerDigital::up].changedToPressed()) {
            liftController->setTarget(LiftPosition::TOPBRANCH);
        } else if(partner[ControllerDigital::left].changedToPressed()) {
            liftController->setTarget(LiftPosition::LOADINGHEIGHT);
        } else if(partner[ControllerDigital::right].changedToPressed()) {
            liftController->setTarget(LiftPosition::LOWBRANCH);
        } else if(partner[ControllerDigital::down].changedToPressed()) {
            liftController->setTarget(LiftPosition::MINHEIGHT);
        }

        /** PID TopBranch Control */
        if(partner[ControllerDigital::X].changedToPressed()) {
            topBranchController->setTarget(TopBranchPosition::TOPBRANCH);
        } else if(partner[ControllerDigital::Y].changedToPressed()) {
            topBranchController->setTarget(TopBranchPosition::LOADINGHEIGHT);
        } else if(partner[ControllerDigital::B].changedToPressed()) {
            topBranchController->setTarget(TopBranchPosition::REST);
        }

		/**
         * @brief lift control using an async PID controller
         *        L1 pressed: increments target angle by LIFT_STEP
         *        L2 pressed: decrements target angle by LIFT_STEP
         *        note: target angle is capped to [0, MAX_LIFT_HEIGHT] to protect the lift
         */
		// lift.moveVelocity(200*(master.getDigital(ControllerDigital::L1) - master.getDigital(ControllerDigital::L2)));
        
        /**
         * @brief Manual swinger control
         *        UP pressed: moves up
         *        DOWN pressed: moves down 
         */
        // topBranch.moveVoltage(10000 * (master.getDigital(ControllerDigital::up) - master.getDigital(ControllerDigital::down)));
        if(master.getDigital(ControllerDigital::up)) {
            topBranchController->setTarget(-260);
            // topBranchController->setTarget(190);
        } else if (master.getDigital(ControllerDigital::down)) {
            topBranchController->setTarget(-825);
        }

        /**
         * @brief Needle control
         *        LEFT pressed: moves left
         *        RIGHT pressed: moves right
         * 
         */
        if(master[ControllerDigital::left].changedToPressed()) {
            leftNeedle.toggle();
        }
        if(master[ControllerDigital::right].changedToPressed()) {
            rightNeedle.toggle();
        }

        /**
         * @brief controls the claw depending on the button value 
         *        When R1 is pressed: claw solenoid is set to true
         *        When R2 is unpressed: claw solenoid is set to false
         */
        clamp.set(!master.getDigital(ControllerDigital::R1));

		/**
         * @brief controls the claw depending on the button value 
         *        When R1 is pressed: claw solenoid is set to true
         *        When R2 is unpressed: claw solenoid is set to false
         */
        if(master[ControllerDigital::X].changedToPressed()){
            roller.moveVelocity(rollerState * 600);
            rollerState = !rollerState;
        }

		/**
         * @brief controls the mogo holder pistons using rising edge detection (上升緣觸發)
         *        When X is pressed: toggles the state of the mogo solenoid
         */
        if(master[ControllerDigital::Y].changedToPressed()){
            backClamp.toggle();
        }


        /**
         * @brief controls the mogo clamp pistons using rising edge detection (上升緣觸發)
         *        When R2 is pressed: toggles the state of the mogo clamp
         */
		if(master[ControllerDigital::R2].changedToPressed()){
            if(backClamp.getState() == false){
                backClamp.toggle();
                pros::delay(250);
                tilter.toggle();
            }
            else{
                tilter.toggle();
                pros::delay(250);
                backClamp.toggle();
            }
        }

		pros::delay(10);
	}
}
