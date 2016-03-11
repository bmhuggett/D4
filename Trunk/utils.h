/*	utils.h
*	Author: George
*	Description: Provides utility functions for register manipulation
*/

#ifndef UTILS_H
#define UTILS_H

#include <fstream>

#define M_PI 			3.14159265359

#define LOGGING_FULL		// Uncomment to log debug info to file


#ifdef LOGGING_FULL
extern std::ofstream logfile;	// Global file to log debug info to
#endif

int combineRegSigned(int highbyte, int lowbyte);

int combineRegUnsigned(int highbyte, int lowbyte);

//Change the bit in location "bit" of the value "value" and return

int bitFlip(int bit, int value);

int bitHigh(int bit, int value);

int bitLow(int bit, int value);

#endif
