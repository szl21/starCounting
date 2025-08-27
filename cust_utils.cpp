#include "cust_utils.h"
#include "config.h"
#include <iostream>

int LOOKUP_TABLE_FOR_BINOMIAL_COEFFICIENTS[BINOM_BIG_NUMBER][BINOM_BIG_NUMBER];
int LOOKUP_TABLE_FOR_FACTORIAL[FACTORIAL_BIG_NUMBER];


int binom(int a, int b) {
	// calculate a choose b
	if(a>=BINOM_BIG_NUMBER or b>=BINOM_BIG_NUMBER) {
		std::cout << "TOO BIG NUMBER IN FUNCTION binom\n";
		return -1;
	}
	if(a < b)
		return 0;
	if(a == b)
		return 1;
	if(b == 0)
		return 1;
	if(LOOKUP_TABLE_FOR_BINOMIAL_COEFFICIENTS[a][b] == 0) {
		LOOKUP_TABLE_FOR_BINOMIAL_COEFFICIENTS[a][b] = binom(a-1,b)+binom(a-1,b-1);
	}
	return LOOKUP_TABLE_FOR_BINOMIAL_COEFFICIENTS[a][b];
}

int invBinomU(int a, int b) {
	int c = 0;
	for(;binom(c,b)<a;c++) {}
	return c;
}

int invBinomD(int a, int b) {
	int c = 0;
	for(;binom(c,b)<=a; c++) {}
	return c;
}

int factorial(int a) {
	if(a>=FACTORIAL_BIG_NUMBER) {
		std::cout << "TOO BIG NUMBER IN FUNCTION factorial.\n";
		return -1;
	}
	if(LOOKUP_TABLE_FOR_FACTORIAL[a] == 0) {
		if(a==0) {
			LOOKUP_TABLE_FOR_FACTORIAL[a] = 1;
		} else {
			LOOKUP_TABLE_FOR_FACTORIAL[a] = a * factorial(a-1);
		}
	}
	return LOOKUP_TABLE_FOR_FACTORIAL[a];
}

