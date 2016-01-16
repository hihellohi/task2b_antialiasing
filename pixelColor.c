#include "pixelcolor.h"
#include <math.h>

#define MAX 256
#define TOP 255
#define MID 180
#define LOW 105
#define FACT 0.2

/*unsigned char stepsToGreen(int value){
	return ((double)1/(double)(1+(double)(((double)value*FACT)-TOP)*(double)(((double)value*FACT)-TOP)))*255;
}

unsigned char stepsToBlue(int value){
    return ((double)1/(double)(1+(double)(((double)value*FACT)-MID)*(double)(((double)value*FACT)-MID)))*255;
}

unsigned char stepsToRed(int value){
	return ((double)1/(double)(1+(double)(((double)value*FACT)-LOW)*(double)(((double)value*FACT)-LOW)))*255;
}*/

unsigned char stepsToRed(int value){
	return 0;
}

unsigned char stepsToGreen(int value){
	return value;
}

unsigned char stepsToBlue(int value){
	return 0;
}
