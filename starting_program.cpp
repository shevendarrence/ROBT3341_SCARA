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
double roundToThousandths(double n);

typedef struct SCARA_POS {
	double x, y, theta1, theta2; // TCP coordinate and joint variables
	int armSol;  // right or left arm solution
}SCARA_POS;

// Global Variables
long encoderCount[2];
SCARA_POS scaraState;
SCARA_POS targetPos;
bool isLimp;

// This function is executed every 100 ms
void update(void) {
	// Update the Encoder Counts
	vitalEncoderRead(1, &encoderCount[1]); // read the encoder for joint one
	vitalEncoderRead(2, &encoderCount[2]);
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
	// Update the Position
	while (!_kbhit()) {
		// Calculate Degrees
		scaraState.theta1 = pulsesToDeg(encoderCount[1]);  // calculate thetas
		scaraState.theta2 = pulsesToDeg(encoderCount[2]);  // calculate thetas
		degToRad(scaraState.theta1); // make 'em rads
		degToRad(scaraState.theta2);
		// Calculate Coordinates
		scaraState.x = L * (cos(scaraState.theta1) + cos(scaraState.theta2));
		scaraState.y = L * (sin(scaraState.theta1) + sin(scaraState.theta2));
		// Display Results
		printf_s(" x = %.2lf \n y = %.2lf \n theta1 = %.2lf \n theta2 = %.2lf \n encoder1 = %d \n encoder2 = %d \n", scaraState.x, scaraState.y, radToDeg(scaraState.theta1), radToDeg(scaraState.theta2), encoderCount[1], encoderCount[2]);
		Sleep(100);
		system("CLS");
	}
}

void moveJoint() {
	printf("Move a Joint (J1 J2):");
	// Initialize Variables
	double arg1, arg2;
	int option;
	// Read User Input
	printf("Select a input type:\n  1) Angles\n  2) Encoder counts\n");
	scanf("%d", &option);
	printf("Enter J1 arg:\n");
	scanf("%lf", &arg1);
	printf("Enter J2 arg:\n");
	scanf("%lf", &arg2);
	// Update Target Position for Closed-Loop Control
	if (option == 1) {
		targetPos.theta1 = arg1;
		targetPos.theta2 = arg2;
	}							 // lets keep it in degrees in both cases
	else if (option == 2) {
		targetPos.theta1 = pulsesToDeg(arg1);
		targetPos.theta2 = pulsesToDeg(arg2);
	}
	else {
		printf("error");
	}

	// Unlimp

	// not sure yet
}

void moveTCP() {
	printf("Move TCP to Position (X Y):");
	// Initialize Variables
	double lenght, alpha, beta, theta2den, theta2num;
	// Read User Input
	printf("Enter X: ");
	scanf("%lf", &targetPos.x);
	printf("\nEnter Y: ");									// saving args to global var targetPos
	scanf("%lf", &targetPos.y);
	printf("\nEnter armSol: (0 is Right Arm, 1 is Left Arm");
	scanf("%d", &targetPos.armSol);
	// Inverse Kinematics
	alpha = atan2(targetPos.y, targetPos.x);
	lenght = sqrt(pow(targetPos.x, 2) + pow(targetPos.y, 2));
	lenght = roundToThousandths(lenght);
	beta = acos((pow(lenght, 2)) / (2 * L * lenght));  // if L1 == L2 then L1^2 - L2^2 = 0, denominator then only lenght^2
	// Update Target Position for Closed-Loop Control
	// if armsol == right arm, beta - alpha, else, beta + alpha
	targetPos.theta1 = (targetPos.armSol == 0) ? targetPos.theta1 = beta - alpha : beta + alpha;
	theta2num = targetPos.y - (L * sin(targetPos.theta1));		// numerator calc
	theta2den = targetPos.x - (L * cos(targetPos.theta1));		// denominator calc
	targetPos.theta2 = atan2(theta2num, theta2den);
	// Unlimp
}

void limpUnlimp() {
	 char limpSelect = 'd';
    while ((limpSelect != 'U') & (limpSelect != 'L') & (limpSelect != 'Q') & (limpSelect != 'u') & (limpSelect != 'l') & (limpSelect != 'q'))
    {
        printf("Limp[L] or Unlimp [U] the Robot, or Quit [Q]\n");
        scanf_s(" %c", &limpSelect);
        if ((limpSelect == 'U') || (limpSelect =='u'))
        {
            isLimp = false; 
            printf("Arm activated. Press enter.\n");
        }
        else if ((limpSelect == 'L') || (limpSelect == 'l'))
        {
            joint1volt = 0; //vitalDacWrite(0, 0.0);
            
            joint2volt = 0; //vitalDacWrite(1, 0.0);

            isLimp = true;

            printf("Arm limped. Press enter.\n");
        }
        else if ((limpSelect == 'Q') || (limpSelect == 'q'))
        {
            printf("Limp/Unlimp Status Unchanged. Returning to main menu.\n");
        }
        else
        {
            printf("Invalid Selection.\n");
        }

    }
    getchar();
    getchar();
    return;
}

void moveRobot() {
	// Initialize Variables
	double theta1Error, theta2Error;
	// Calculate the Current Position
	scaraState.theta1 = pulsesToDeg(encoderCount[1]);  // calculate current thetas
	scaraState.theta2 = pulsesToDeg(encoderCount[2]);
	// Calculate Error
	theta1Error = scaraState.theta1 - targetPos.theta1;
	theta2Error = scaraState.theta2 - targetPos.theta2;
	// Turn Motors Based on Error

	/* if scaraState theta1 < targetPos theta1
			turn motor 1 with pos voltage and vice versa (?)
			still unsure how to do this and make it
			work with the update function		*/
}

double pulsesToDeg(long pCount) {
	return pCount / (GEARRATIO * PULSESPERDEG);
}

double degToPulses(long desDeg) {
	return GEARRATIO * PULSESPERDEG * desDeg;
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
/************************************************************************************
 * Function: roundToThousandths
 * - rounds a float to the third decimal place
 *
 * Arguments:
 * n - double
 *
 * return: rounded double
 *
 * Author: Marcus Kuhn
 * Date: 29/04/2020
 * Modified: 29/04/2020
 ***********************************************************************************/
double roundToThousandths(double n) {
	return floor(n * 10000 + 0.5) / 10000;
}
