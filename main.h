#ifndef D6B804B8_685B_4DB1_B451_1032DC28B8FB
#define D6B804B8_685B_4DB1_B451_1032DC28B8FB

#include "transition.h"
#include "priority.h"

vector<Star> generateStars();
vector<Priority> generatePriorities(const bool);
vector<Transition> generateTransitions(const vector<Star>&, const vector<Priority>&, const bool);
vector<Transition> loadTransitions(const vector<Star>&, const vector<Priority>&, const bool);
bool update(vector<Star>&, vector<Priority>&, vector<Transition>&);
udouble calcCut(const vector<Star>&);
udouble calculateMargs(const vector<Star>&, vector<Priority>&);
void report(const vector<Star>&, vector<Priority>&);

#endif /* D6B804B8_685B_4DB1_B451_1032DC28B8FB */
