#ifndef AC259769_B206_4BA5_94F3_8BFDC4CD000D
#define AC259769_B206_4BA5_94F3_8BFDC4CD000D

#include "config.h"

#include <vector>

using std::vector;

int binom(int a, int b);			// calculate a choose b
int invBinomU(int a, int b);	// calculate the least int c st. a >= binom(c,b)
int invBinomD(int a, int b); 	// calculate the most int c st. a <= binom(c,b)

int factorial(int a);					// calculate factorial

template<typename T>
bool nextNonIncSeq(vector<T>& seq, T maxVal) {		// iterate through non-inreasing sequences with maximal value at most maxVal. If it was the last appropriate one, it returns to the 0,0,0,...,0 and returns false
	usint idx = seq.size()-1;
	while(idx>0 && seq[idx]==seq[idx-1])
		idx--;
	if(idx == 0) {
		if(seq[0] == maxVal-1) {
			fill(seq.begin(), seq.end(), 0);
			return false;
		}
		seq[seq.size()-1]++;
		fill(seq.begin(), seq.end()-1, 0);
    return true;
	}
	seq[idx-1]++;
	fill(seq.begin()+idx,seq.end()-1,seq[idx-1]);
	return true;
}

template<typename T>
usint countOccurrences(const vector<T>& vec, const T& val) { return count(vec.begin(), vec.end(), val); }

void sortAndUnique(vector<auto>& vec) {
	sort(vec.begin(), vec.end());
	auto it = unique(vec.begin(), vec.end());
	vec.resize(distance(vec.begin(), it));
}

template<typename T, typename Comp1, typename Comp2>
void sortAndUnique(vector<T>& vec, Comp1 less, Comp2 equal) {
	sort(vec.begin(), vec.end(), less);
	auto it = unique(vec.begin(), vec.end(), equal);
	vec.resize(distance(vec.begin(), it));
}

#endif /* AC259769_B206_4BA5_94F3_8BFDC4CD000D */
