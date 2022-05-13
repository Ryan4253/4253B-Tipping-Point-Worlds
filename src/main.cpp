#include "main.h"

void disabled() {}

void competition_initialize() {}

void initialize() {
	// LCD initialization
	pros::lcd::initialize();

	// Vision initialization
    auto RED_SIG = pros::Vision::signature_from_utility(1, 5685, 8135, 6910, -1963, -1705, -1834, 4.000, 0);
    vision.set_signature(1, &RED_SIG); 

	// IMU Initialization
	pros::lcd::set_text(2, "imu calibrating..............");
    std::cout << "imu calibrating.............." << std::endl;
	imu.calibrate();
    pros::delay(2000);
	pros::lcd::set_text(2, "imu calibrated");
    std::cout << "imu calibrated, initialization finished!!!\n" << std::endl;

    // Subsystem initialization
    topBranch.tarePosition();
    topBranchController->tarePosition();
    lift.tarePosition();
    liftController->tarePosition();

    // Auton Initialization
    auto&& selector = AutonSelector::getInstance();
    selector.addRoute(AWP, "AWP");
    selector.addRoute(highBranch, "High Branch");
}

void autonomous() {
    // Initialization
    leftDrive.setBrakeMode(AbstractMotor::brakeMode::brake);
    rightDrive.setBrakeMode(AbstractMotor::brakeMode::brake);
    topBranchController->setTarget(TopBranchPosition::REST);
    liftController->setTarget(0);
    clamp.set(true);

    // Execute auton
    AutonSelector::getInstance().execute();    
}

void opcontrol(){
	// Sets brake mode
	leftDrive.setBrakeMode(AbstractMotor::brakeMode::coast);
	rightDrive.setBrakeMode(AbstractMotor::brakeMode::coast);
    lift.setBrakeMode(AbstractMotor::brakeMode::hold);
    topBranch.setBrakeMode(AbstractMotor::brakeMode::hold);

	// Initializes subsystems
	auto model = std::static_pointer_cast<ExpandedSkidSteerModel>(chassis->getModel());

	// Initializes constants
	bool rollerState = false;
    auto swingerState = ControllerDigital::down;

	// Initializes brasin background
	createBlankBackground();
	auto gif = std::make_unique<Gif>("/usd/gif/crab-rave.gif", lv_scr_act());

	while(true){
        // Chassis control using curvature drives
		model->curvature(master.getAnalog(ControllerAnalog::leftY), master.getAnalog(ControllerAnalog::rightX), DEADBAND);

        /**
         * @brief PID Lift Control
         */
        if(partner[ControllerDigital::up].changedToPressed()){
            liftController->setTarget(LiftPosition::MAXHEIGHT);
        }
        else if(partner[ControllerDigital::left].changedToPressed()){
            liftController->setTarget(LiftPosition::LOADINGHEIGHT);
        }
        else if(partner[ControllerDigital::right].changedToPressed()){
            liftController->setTarget(LiftPosition::TOPBRANCH);
        }
        else if(partner[ControllerDigital::down].changedToPressed()){
            liftController->setTarget(LiftPosition::MINHEIGHT);
        }

        /**
         * @brief PID Top Branch Control
         */
        if(partner[ControllerDigital::X].changedToPressed()){
            topBranchController->setTarget(TopBranchPosition::TOPBRANCH);
        } 
        else if(partner[ControllerDigital::Y].changedToPressed()){
            topBranchController->setTarget(TopBranchPosition::LOADINGHEIGHT);
        } 
        else if(partner[ControllerDigital::B].changedToPressed()){
            topBranchController->setTarget(TopBranchPosition::REST);
        }
        
        /**
         * @brief Manual Top Branch Control
         *        UP pressed: moves up
         *        DOWN pressed: moves down 
         */
        if(master.getDigital(ControllerDigital::up)){
            topBranchController->setTarget(-260);
        } else if (master.getDigital(ControllerDigital::down)){
            topBranchController->setTarget(-825);
        }

        /**
         * @brief Needle Control
         *        LEFT pressed: moves left
         *        RIGHT pressed: moves right
         */
        if(master[ControllerDigital::left].changedToPressed()){
            leftNeedle.toggle();
        }
        if(master[ControllerDigital::right].changedToPressed()){
            rightNeedle.toggle();
        }

        /**
         * @brief Controls the claw depending on the button value 
         *        When R1 is pressed: claw solenoid is set to true
         *        When R2 is unpressed: claw solenoid is set to false
         */
        clamp.set(!master.getDigital(ControllerDigital::R1));

        /**
         * @brief Controls the roller depending on the button value
         *        When L1 is pressed: Roller intakes
         *        When L2 is pressed: ROller outtakes
         * 
         */
        roller.moveVoltage(12000 * (master.getDigital(ControllerDigital::L1) - master.getDigital(ControllerDigital::L2)));

		/**
         * @brief Controls the mogo clamp using rising edge detection
         *        When X is pressed: toggles the state of the mogo solenoid
         */
        if(master[ControllerDigital::Y].changedToPressed()){
            backClamp.toggle();
        }


        /**
         * @brief Controls the mogo clamp pistons using rising edge detection
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

        // Rotates the robot to face the field when A is pressed
        if(master[ControllerDigital::A].changedToPressed()){
            turnToAngle(0_deg);
        }

		pros::delay(10);
	}
}
