/* Kevin Ni
 * Date: 16/4/2014
 * contains functions called by bmpServer.c
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mandelbrot.h"
#include "pixelcolor.h"

//#define NOAA
#define SSAAX4
#define MODIFIER_A 0.1
#define MODIFIER_B 0.3
#define SAMPLING_RATE 8 

typedef struct _complexNo{
    long double real;
    long double imagine;
} complexNo;


void testEscapeSteps(void); 
static long double findInit(long double centre, long double zoom);
static void colorPixel(int value, FILE *file);
static int escapeSteps(long double x, long double y);
static int escape(complexNo num);
static complexNo square(complexNo num);
static complexNo plus(complexNo numA, complexNo numB);
static int getPixel(long double x, long double y, long double zoom);

void draw(long double centreX, long double centreY, long double zoom, FILE *file){
    // draws the mandelbrot set to a file given a centre and a zoom level
    srand(time(NULL));
    long double inc = 1/pow(2, zoom); 
    long double y = findInit(centreY, zoom), endY = y + (inc * SIZE);
    long double x = findInit(centreX, zoom), endX = x + (inc * SIZE);
	int county = 0;
    printf("%Lf\n", inc);
    //if(inc > 0) printf("not zero\n");
    while(county < SIZE){
		int countx = 0;
        while(countx < SIZE){
            colorPixel(getPixel(x, y, inc), file);
            x += inc;
			countx++;
        }
		county++;
		printf("%lf\n",(double)county/(double)SIZE);
        y += inc;
        x = findInit(centreX, zoom);
    }
	printf("%d\n",county);
    testEscapeSteps();
    return;
}

int zero(int a){
    if(a == 256){
        return 0;
    }else{
        return a;
    }
}

static int getPixel(long double x, long double y, long double zoom){
    
    #ifdef NOAA
        return zero(escapeSteps(x, y));
    #else

    #ifdef SSAAX4
        int mean = 0;
        mean += zero(escapeSteps(x - (zoom * MODIFIER_B), y + (zoom * MODIFIER_A)));
        mean += zero(escapeSteps(x + (zoom * MODIFIER_A), y + (zoom * MODIFIER_B)));
        mean += zero(escapeSteps(x + (zoom * MODIFIER_B), y - (zoom * MODIFIER_A)));
        mean += zero(escapeSteps(x - (zoom * MODIFIER_A), y - (zoom * MODIFIER_B)));
        //printf("%Lf\n",x);
        //printf("%Lf\n",y);
        //printf("%d\n",(int)(mean/4));
        return mean/4;
    #else
        int mean = 0;
        int count = 0;
        while(count < SAMPLING_RATE){
            double xMod = (double)((rand() % 100) - 50) / 100;
            double yMod = (double)((rand() % 100) - 50) / 100;
            mean += zero(escapeSteps(x + (zoom * xMod), y + (zoom * yMod)));
            count++;
        }
        return mean/count;
    #endif
    #endif
}

static long double findInit(long double centre, long double zoom){
    // finds the coordinates of the first pixel to be drawn 
    // given a centre and a zoom level
    
    long double init;
    
    init = centre - (SIZE / 2) / pow(2, zoom);

    return init;
}

static void colorPixel(int value, FILE *file){
    // needs to be updated
    
    unsigned char pixel[3] = {stepsToBlue(value), stepsToGreen(value), stepsToRed(value)};
    fwrite(&pixel, sizeof(unsigned char), 3, file);

    return;
}

static int escapeSteps(long double x, long double y){
    // finds the mandelbrot value of a pixel
    
    //printf("%Lf\n",x);
    //printf("%Lf\n",y);
    complexNo num;
    num.real = x;
    num.imagine = y;
    complexNo original = num;
    int count = 1;
   
    while(count < 256 && !escape(num)){
        num = square(num);
        num = plus(num,original);
        count++;
    }
    return count;
}

static int escape(complexNo num){
    // determines if a value has escaped the set

    return RADIUS_SQUARED <= pow(num.real, 2) + pow(num.imagine, 2);
}

static complexNo plus(complexNo numA, complexNo numB){
    // adds two complex numbers

    complexNo new;
    new.real = numA.real + numB.real;
    new.imagine = numA.imagine + numB.imagine;
    return new;
}

static complexNo square(complexNo num){
    // squares a complex number

    complexNo new;
    new.real = (num.real * num.real) - (num.imagine * num.imagine);
    new.imagine = (2 * num.real * num.imagine);
    return new;
}

void testEscapeSteps(void) {
   assert (escapeSteps (100.0, 100.0) == 1);
   assert (escapeSteps (0.0, 0.0)     == 256);

   assert (escapeSteps (-1.5000000000000, -1.5000000000000) == 1);
   assert (escapeSteps (-1.4250000000000, -1.4250000000000) == 1);
   assert (escapeSteps (-1.3500000000000, -1.3500000000000) == 2);
   assert (escapeSteps (-1.2750000000000, -1.2750000000000) == 2);
   assert (escapeSteps (-1.2000000000000, -1.2000000000000) == 2);
   assert (escapeSteps (-1.1250000000000, -1.1250000000000) == 3);
   assert (escapeSteps (-1.0500000000000, -1.0500000000000) == 3);
   assert (escapeSteps (-0.9750000000000, -0.9750000000000) == 3);
   assert (escapeSteps (-0.9000000000000, -0.9000000000000) == 3);
   assert (escapeSteps (-0.8250000000000, -0.8250000000000) == 4);
   assert (escapeSteps (-0.7500000000000, -0.7500000000000) == 4);
   assert (escapeSteps (-0.6750000000000, -0.6750000000000) == 6);
   assert (escapeSteps (-0.6000000000000, -0.6000000000000) == 12);
   assert (escapeSteps (-0.5250000000000, -0.5250000000000) == 157);
   assert (escapeSteps (-0.4500000000000, -0.4500000000000) == 256);
   assert (escapeSteps (-0.3750000000000, -0.3750000000000) == 256);
   assert (escapeSteps (-0.3000000000000, -0.3000000000000) == 256);
   assert (escapeSteps (-0.0750000000000, -0.0750000000000) == 256);
   assert (escapeSteps (-0.0000000000000, -0.0000000000000) == 256);

   assert (escapeSteps (-0.5400000000000, 0.5600000000000) == 256);
   assert (escapeSteps (-0.5475000000000, 0.5650000000000) == 58);
   assert (escapeSteps (-0.5550000000000, 0.5700000000000) == 28);
   assert (escapeSteps (-0.5625000000000, 0.5750000000000) == 22);
   assert (escapeSteps (-0.5700000000000, 0.5800000000000) == 20);
   assert (escapeSteps (-0.5775000000000, 0.5850000000000) == 15);
   assert (escapeSteps (-0.5850000000000, 0.5900000000000) == 13);
   assert (escapeSteps (-0.5925000000000, 0.5950000000000) == 12);
   assert (escapeSteps (-0.6000000000000, 0.6000000000000) == 12);

   assert (escapeSteps (0.2283000000000, -0.5566000000000) == 20);
   assert (escapeSteps (0.2272500000000, -0.5545000000000) == 19);
   assert (escapeSteps (0.2262000000000, -0.5524000000000) == 19);
   assert (escapeSteps (0.2251500000000, -0.5503000000000) == 20);
   assert (escapeSteps (0.2241000000000, -0.5482000000000) == 20);
   assert (escapeSteps (0.2230500000000, -0.5461000000000) == 21);
   assert (escapeSteps (0.2220000000000, -0.5440000000000) == 22);
   return;
}
