#include "utils.h"

int combineRegSigned(int highbyte, int lowbyte)
{
	if (highbyte>>7 == 1)				//If the top bit of highbyte is 1, number is negative
	{
		highbyte = highbyte | ~(0xFF);	//So make all the bits after 8 high as well
	}

	return (highbyte<<8) | lowbyte;		//Shift the highbyte over, append the lowbyte and return
}

int combineRegUnsigned(int highbyte, int lowbyte)
{
	return (highbyte<<8) | lowbyte;
}

int bitFlip(int bit, int value)
{
	return value ^ (1<<bit);
}

int bitHigh(int bit, int value)
{
	return value | (1<<bit);
}

int bitLow(int bit, int value)
{
	return value & ~(1<<bit);
}