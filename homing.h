#include <stdio.h>
#include "winmotenc.h"		//for Motenc card

//#include <windows.h>		//for Sleep() function

//****************************
// Move to index (used when homing a motor).  Move the motor until an index pulse is 
// detected and then clear the encoder count to zero at that location.
//
// 1. enable the index latch for the encoder
// 2. move the motor slowly until the index latch is set.
//    When the index latch is set, the encoder count will automatically be cleared to zero
// 3. stop the motor and disable the index latch
//
//****************************
void move_to_index(int motor_number);

/****************************
offset_move - completes a move of a single motor by a specified number of pulses.

inputs:
	motor_number
	offset - distance to move in pulses (-4000 >= offset <= +4000)
	
outputs:
	none
****************************/
void offset_move(int motor_number,long offset);
