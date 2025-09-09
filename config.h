#ifndef B3557EA6_5454_4982_9A96_EC06A43B1F59
#define B3557EA6_5454_4982_9A96_EC06A43B1F59

typedef long double udouble;
typedef unsigned char uschar;		// used for leaf out type
typedef unsigned short usshort; // used for multiplicities
typedef unsigned int usint;			// used for star indices

const udouble EPS = 1.0/50000;
const usint DEGREE = 5;
const uschar MAX_LEAF = 2+DEGREE*(DEGREE+1)/2;
const udouble impConst = 0.34116;				// the guaranteed ratio of wrongly coloured vertices which can be effectively swapped.
const uschar BINOM_BIG_NUMBER = 50;			// technical constant
const uschar FACTORIAL_BIG_NUMBER = 10;	// technical constant
const udouble IS_COLOURABLE_MULTIPLIER = 1;	// treshold for accepting dominant type

#endif /* B3557EA6_5454_4982_9A96_EC06A43B1F59 */
