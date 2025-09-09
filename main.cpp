#include "main.h"
#include "cust_utils.h"

#include "math.h"
#include <numeric>
#include <sstream>
#include <fstream>

using std::vector;
using std::endl;

int main() {
		// setting up vector for stars. apart from the fully uncoloured one, everything have a weight of zero
		auto stars = generateStars();
    
    // setting up priority list
		auto priorities = generatePriorities(true);

		// setting up transitions
		auto transitions = generateTransitions(stars, priorities, true);
	
		// running the first part of the algorithm
		while(update(stars, priorities, transitions));

		// second part of algorithm & writing the report
   report(stars, priorities);
	return 0;
}

vector<Star> generateStars() {
    vector<Star> stars;
		// creating all followed stars
    for(uschar c=0; c<2; c++) {
				// leaf out type vector
        vector<uschar> lot(DEGREE, 0);
        do {
            stars.push_back(Star(c,lot));
        } while(nextNonIncSeq(lot, MAX_LEAF));
    }
		// setting up starting star to have weight 1
    stars[SimpleStar(0, vector<uschar>(DEGREE, 2)).getStarIndex()].setMeas(1);
    return stars;
}

vector<Priority> generatePriorities(const bool status) {
    vector<Priority> p;
    for(uschar r=0; r<=DEGREE; r++)
        for(uschar b=r; b+r<=DEGREE; b++)
            p.push_back(Priority(PointType(r,b)));
    sort(p.rbegin(), p.rend());
		
		// writing out priority list
		if(status) {
		  cout << "Order of priorities:\n";
      for(auto it=p.begin(); it!=p.end(); ++it) {
        it->printType();
				cout << ",\t";
			}
      cout << "\n----------\n";
		}
    return p;
}

vector<Transition> generateTransitions(const vector<Star>& stars, const vector<Priority>& priorities, const bool status) {
	vector<Transition> t;
	
	  if(status)
	    cout << "Generating transitions for type...\n";
	  #pragma omp parallel for
		for(auto p: priorities) {
		  if(status) {
		    p.printType();
			  cout << "\n";
		  }
		  t.push_back(Transition(stars, p));
		}
	  if(status)
      cout << "----------\n";
	return t;
}

bool update(vector<Star>& stars, vector<Priority>& priorities, vector<Transition>& transitions) {
    // calculate the weight of the different types before the current step
		udouble sumOfMargs = calculateMargs(stars, priorities);
		
		// finding the type with the highest priority which have weight at least eps
    auto colouredIt = find_if(priorities.begin(), priorities.end(),
        [](auto p) {return p.isColourable();});
		
		// if all type have weight less than eps the first part of the algorithm finishes
    if(colouredIt == priorities.end())
        return false;
    
		// writing out the total weight/eps of uncoloured vertices and the current leading type
    cout << sumOfMargs/EPS << " ";
		colouredIt->printType();
		cout << endl;
    
		// searching for transition scheme corresponding to the leading type; if it is not found then create a new one
		auto transitionIt = find_if(transitions.begin(), transitions.end(),
        [colouredIt](auto t) {return t.getColoured().isSameType(*colouredIt);});
		
		// calculate the basic transition probabilities
    udouble probKnown = EPS*(2-colouredIt->isSymmetric())/(2* (colouredIt->getMeas()));
		udouble pukMeas;
		{
			vector<udouble> m;
			for(auto s: stars)
				if(s.getCenter().getColour() == 0 and s.getCenter().numOfUncol() > 0)
					m.push_back(s.getMeas()*s.getMultiplicity()*s.getCenter().numOfUncol()/DEGREE);
			sort(m.begin(), m.end());
			pukMeas = accumulate(m.begin(), m.end(), udouble(0));
		}
		udouble probUnknown = EPS*(colouredIt->numOfUncol())/(2*DEGREE*pukMeas);
		
		transitionIt->doTheTransition(stars, probKnown, probUnknown);
		// continue with the first part of the algorithm
		return true;
}

udouble calcCut(const vector<Star>& stars) {
    vector<udouble> c;
    for(auto s: stars) {
        if(s.getCenter().getColour() == 1)
            c.push_back(s.getMeas()*s.getMultiplicity()*s.getCenter().numOfNeig()[2]);
        else if(s.getCenter().getColour() == 2)
            c.push_back(s.getMeas()*s.getMultiplicity()*s.getCenter().numOfNeig()[1]);
    }
    sort(c.begin(), c.end());
    return accumulate(c.begin(), c.end(), udouble(0))/2;
}

udouble calculateMargs(const vector<Star>& s, vector<Priority>& p) {
    vector<udouble> m;
    for(auto it = p.begin(); it!=p.end(); ++it)
        m.push_back(it->calculateMeas(s));
    sort(m.begin(), m.end());
    udouble res = accumulate(m.begin(), m.end(), udouble(0));
    return res;
}

void report(const vector<Star>& s, vector<Priority>& p) {
    cout << "\n----- REPORT -----\n";

		cout << "DEGREE: " << DEGREE << endl;
    cout << "EPS: " << EPS << endl;
    
		udouble cut = calcCut(s);
    cout << "Size of cut: " << cut << endl;
    cout << "Remaining uncoloured vertices: \n" << calculateMargs(s, p) << endl;
    cout << "Maximal contribution to cut: ";
    udouble cont = 0;
    for(usint i=0; i<s.size(); i++) {
        if(s[i].getCenter().getColour() != 0)
            continue;
        if(s[i].getCenter().numOfNeig()[1]<s[i].getCenter().numOfNeig()[2]) {
            cont += (DEGREE-s[i].getCenter().numOfNeig()[2])*s[i].getMeas()*s[i].getMultiplicity();
        } else if(s[i].getCenter().numOfNeig()[1]>s[i].getCenter().numOfNeig()[2]) {
            cont += (DEGREE-s[i].getCenter().numOfNeig()[1])*s[i].getMeas()*s[i].getMultiplicity();
        } else {
            cont += (DEGREE-s[i].getCenter().numOfNeig()[1])*s[i].getMeas()*s[i].getMultiplicity()/2;
        }
    }
    cout << cont << "\n";
    cout << "So the total cut is " << cut+cont << "\n";
    cout << "Distribution of fully coloured stars:\n";
    cout << "Center\tRed\tBlue\tMeas\n";
    udouble lost = 0;
    for(usint i=0; i<s.size(); i++) {
        if(s[i].getCenter().getColour()==0 or s[i].getCenter().numOfUncol()>0)
            continue;
        if(s[i].getCenter().getColour()==1)
            cout << "Red\t";
        else
            cout << "Blue\t";
        cout << (int) s[i].getCenter().numOfNeig()[1] << "\t"
             << (int) s[i].getCenter().numOfNeig()[2] << "\t"
             << s[i].getMeas()*s[i].getMultiplicity() << "\n";
        if(s[i].getCenter().numOfNeig()[s[i].getCenter().getColour()] < s[i].getCenter().numOfNeig()[3-s[i].getCenter().getColour()])
            lost += s[i].getMeas()*s[i].getMultiplicity();
    }
    cout << "Measure of lost vertices: " << lost << "\n";
    cout << endl;
    cout << "So the improved cut size is at most " << cut+cont-lost*impConst << endl;
}
