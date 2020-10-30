#include <stdio.h>
#include "winmotenc.h"		//for Motenc card
#include <windows.h>		//for Sleep() function
#include <conio.h>		//for _kbhit() function
#include "homing.h"
#include "tic_generator.h"
#include <math.h>

#define M_PI       3.14159265358979323846

// Function Declarations
void homeRobot();
void displayPosition();
void moveJoint();
void moveTCP();
void limpUnlimp();
void moveRobot();

// Global Variables

// This function is executed every 100 ms
void update(void){
	// Update the Encoder Counts

	// Execute the Closed Loop Control
	moveRobot();
}

int main(){
	// Menu Variables
	// test commit
	int input;		// Menu Select Variable
	bool quit = 0;		// Exit Flag

	// Initialize the System 
	vitalInit();			//initialize Motenc card
	printf("\n\nWelcome to the SCARA Lab!\nPress any key to START...");
	getchar( );
	tic_setup(update, 100000); 	//Setup a function to execute every 100ms.
	tic_enable(1); 			//Start the timer, update is now repeatedly called

	// Main Menu
	while(!quit){
		// Prompt the User with the Menu
		
		// Read the User Input
		scanf("%d", &input);
		system("CLS");

		//
		switch(input){
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
	getchar( );	
	vitalQuit();			//unitialize Motenc card
	tic_enable(0); //stop the timer
	return(0);
}

void homeRobot(){
	printf("Homing the Robot");
	// Ensure motors are off (Do this after Limp is implemented)

	// Ask user to move arm manually

	// Press any key when ready

	// Reset Encoder Counts
}

void displayPosition(){
	printf("Display the Position");
	
	// Initialize Variables
	
	// Update the Position
	while(!_kbhit()){
		// Calculate Degrees

		// Calculate Coordinates
		
		// Display Results

		Sleep(100);
		system("CLS");
	}
}

void moveJoint(){
	printf("Move a Joint (J1 J2):");
	// Initialize Variables

	// Read User Input

	// Update Target Position for Closed-Loop Control

	// Unlimp
}

void moveTCP(){
	printf("Move TCP to Position (X Y):");
	// Initialize Variables
	
	// Read User Input

	// Inverse Kinematics

	// Update Target Position for Closed-Loop Control
	
	// Unlimp
}

void limpUnlimp(){
	printf("Limp[L] or Unlimp [U] the Robot");
	// Initialize Variables

	// Read User Input
	
	// Limp or Unlimp the Robot
}

void moveRobot(){
	// Initialize Variables

	// Calculate the Current Position

	// Calculate Error

	// Turn Motors Based on Error

}