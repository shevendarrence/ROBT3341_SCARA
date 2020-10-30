#include <stdio.h>
#include "winmotenc.h"		//for Motenc card
#include <windows.h>		//for Sleep() function
#include <conio.h>		//for _kbhit() function
#include "homing.h"
#include "tic_generator.h"
#include <math.h>
#pragma warning(suppress : 4996)

#define M_PI       3.14159265358979323846
// robot constants
#define L 254.0 
#define GEARRATIO 72
#define PULSESPERDEG 11.11

// Function Declarations
void homeRobot();
void displayPosition();
void moveJoint();
void moveTCP();
void limpUnlimp();
void moveRobot();

double degToRad(double angDeg);
double radToDeg(double angRad);

double pulsesToDeg(long pCount);

// Global Variables

// This function is executed every 100 ms
void update(void) {
	// Update the Encoder Counts

	// Execute the Closed Loop Control
	moveRobot();
}

int main() {
	// Menu Variables
	int input;		// Menu Select Variable
	bool quit = 0;		// Exit Flag

	// Initialize the System 
	vitalInit();			//initialize Motenc card
	printf("\n\nWelcome to the SCARA Lab!\nPress any key to START...");
	getchar();
	tic_setup(update, 100000); 	//Setup a function to execute every 100ms.
	tic_enable(1); 			//Start the timer, update is now repeatedly called

	// Main Menu
	while (!quit) {
		// Prompt the User with the Menu
		printf("Please select a SCARA function\n  1) Home SCARA\n  2) Display Position\n  3) Move Joint\n  4) Move TCP\n  5) Limp\n  Q) Quit\n");
		// Read the User Input
		scanf("%d", &input);
		system("CLS");

		//
		switch (input) {
		case 1: // Home
			homeRobot();
			break;
		case 2: // Display Position
			displayPosition();
			break;
		case 3: // Move Joint
			moveJoint();
			break;
		case 4: // Move TCP
			moveTCP();
			break;
		case 5: // Limp
			limpUnlimp();
			break;
		default:
			// Quit
			quit = 1;
		}
		printf("\n Press any key to return to menu.");
		getchar();
		getchar();
	}
	getchar();
	vitalQuit();			//unitialize Motenc card
	tic_enable(0); //stop the timer
	return(0);
}

void homeRobot() {
	printf("Homing the Robot");
	// Ensure motors are off (Do this after Limp is implemented)

	// Ask user to move arm manually

	// Press any key when ready

	// Reset Encoder Counts
}

void displayPosition() {
	printf("Display the Position");

	// Initialize Variables
	double theta1, theta2, x, y;
	long encoderCount[4];
	vitalEncoderRead(1, &encoderCount[1]); // read the encoder for joint one
	vitalEncoderRead(2, &encoderCount[2]);
	// Update the Position
	while (!_kbhit()) {
		// Calculate Degrees
		theta1 = pulsesToDeg(encoderCount[1]);  // calculate thetas
		theta2 = pulsesToDeg(encoderCount[2]);  // calculate thetas
		degToRad(theta1); // make 'em rads
		degToRad(theta2);
		// Calculate Coordinates
		x = L * (cos(theta1) + cos(theta2));
		y = L * (sin(theta1) + sin(theta2));
		// Display Results
		printf_s(" x = %.2lf \n y = %.2lf \n theta1 = %.2lf \n theta2 = %.2lf \n encoder1 = %d \n encoder2 = %d \n", x, y, radToDeg(theta1), radToDeg(theta2), encoderCount[1], encoderCount[2]);
		Sleep(100);
		system("CLS");
	}
}

void moveJoint() {
	printf("Move a Joint (J1 J2):");
	// Initialize Variables

	// Read User Input

	// Update Target Position for Closed-Loop Control

	// Unlimp
}

void moveTCP() {
	printf("Move TCP to Position (X Y):");
	// Initialize Variables

	// Read User Input

	// Inverse Kinematics

	// Update Target Position for Closed-Loop Control

	// Unlimp
}

void limpUnlimp() {
	printf("Limp[L] or Unlimp [U] the Robot");
	// Initialize Variables

	// Read User Input

	// Limp or Unlimp the Robot
}

void moveRobot() {
	// Initialize Variables

	// Calculate the Current Position

	// Calculate Error

	// Turn Motors Based on Error

}

double pulsesToDeg(long pCount) {
	return pCount / (GEARRATIO * PULSESPERDEG);
}

//---------------------------------------------------------------------------------------
// Returns angle in radians from input angle in degrees
double degToRad(double angDeg) {
	return (M_PI / 180.0) * angDeg;
}

//---------------------------------------------------------------------------------------
// Returns angle in radians from input angle in degrees
double radToDeg(double angRad) {
	return (180.0 / M_PI) * angRad;
}