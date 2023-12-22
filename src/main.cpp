#include "main.h"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include <math.h>
#include "okapi/api.hpp"
#include "okapi/api/chassis/controller/chassisControllerPid.hpp"
using namespace okapi;

const int FRONT_LEFT_PORT = 1;
const int FRONT_RIGHT_PORT = 11;
const int BACK_LEFT_PORT = 10;
const int BACK_RIGHT_PORT = 20;


// pros::Motor topLeft(1, false);
// pros::Motor topRight(19, true);
// pros::Motor bottRight(10, false);
// pros::Motor bottLeft(20, true);

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
	
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */




void autonomous() {

	std::shared_ptr<ChassisController> bot = ChassisControllerBuilder()     
			.withMotors(FRONT_LEFT_PORT, -FRONT_RIGHT_PORT, -BACK_RIGHT_PORT, BACK_LEFT_PORT)  //reverse front right and back right in order to go forward 
			.withGains(
				{0.001, 0, 0.0001}, // Distance controller gains
				{0.001, 0, 0.0001}, // turn controller gains
				{0.000, 0, 0.0000}	// Angle controller (helps bot drive straight)
				)
			.withMaxVelocity(50)
			// Green gearset, 3 inch wheel diam, 9 inch wheel track

			.withDimensions(AbstractMotor::gearset::green, {{3_in, 9_in}, imev5GreenTPR})
			.build();

	pros::lcd::set_text(1, "THIS IS AUTON!");
	bot->moveDistance(10_in); //drive forward 10 inches

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */






void opcontrol() {

	pros::Motor topLeft(1, false);
	pros::Motor topRight(11, true);
	pros::Motor bottRight(20, false);
	pros::Motor bottLeft(10, true);

	int yMotion;
	int xMotion;

	while (true)
	{

		pros::lcd::set_text(1, std::to_string(topLeft.get_position()));
		pros::lcd::set_text(2, std::to_string(topRight.get_position()));
		pros::lcd::set_text(3, std::to_string(bottLeft.get_position()));
		pros::lcd::set_text(4, std::to_string(bottRight.get_position()));

		pros::Controller master(pros::E_CONTROLLER_MASTER);
		// driving control code


		yMotion = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // ik this looks wrong, but it works
		xMotion = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);


		int right = -xMotion + yMotion; //-power + turn
		int left = xMotion + yMotion;	// power + turn

		topLeft.move(left); // Swap negatives if you want the bot to drive in the other direction
		bottLeft.move(-left);
		bottRight.move(right);
		topRight.move(-right);
		
		//centerWheel.move(rotate);
		pros::delay(1);

		pros::delay(20);
	}

}