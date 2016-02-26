#ifndef UTILS_H
#define UTILS_H

#define M_PI 			3.14159265359

int combineRegSigned(int highbyte, int lowbyte);

int combineRegUnsigned(int highbyte, int lowbyte);

int bitFlip(int bit, int value);

int bitHigh(int bit, int value);

int bitLow(int bit, int value);

#endif