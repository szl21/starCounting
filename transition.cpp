#include "transition.h"
#include "cust_utils.h"

#include <iostream>
#include <numeric>
#include <cmath>

using std::cout;
using std::endl;
using std::next_permutation;


void Transition::initTransitionTable(const vector<Star>& stars) {
    m_transitionTable.resize(stars.size());
 			for(int from = stars.size()-1; from >= 0; from--) {
 				// progress bar
				if((stars.size() - from) % 1000 == 0)
				  cout << "Working on " << stars.size() - from << "/" << stars.size() << "..." << endl;

				auto fn = SimpleStar::firstNeighsInt(stars[from]);
				vector<usint> encountered;
				while(true) {
					sortAndUnique(encountered);
					sortAndUnique(fn);
					vector<usint> setdif;
					std::set_difference(fn.begin(), fn.end(), encountered.begin(), encountered.end(), std::back_inserter(setdif));
				  
					vector<usint> newEncountered;
					vector<usint> lastNeighbours;
					for(auto to: setdif) {
					  newEncountered.push_back(to);
						auto tData = getTransitionData(stars[from], stars[to]);
						if(!isNegligible(tData)) {
							lastNeighbours.push_back(to);
              m_transitionTable[from].push_back(pair<usint, tDataType>(to, tData));
						} 
					}
					if(lastNeighbours.size()==0)
						break;
					fn.clear();
				  vector<usint> prevCalc;
					for(auto a: lastNeighbours) {
						if(m_transitionTable[a].size() == 0) {
							auto f = SimpleStar::firstNeighsInt(stars[a]);
							fn.insert(fn.end(), f.begin(), f.end());
						} else {
							std::transform(m_transitionTable[a].begin(), m_transitionTable[a].end(), std::back_inserter(prevCalc), [](const auto& p){return p.first; });
						}
					}
					if(prevCalc.size()>0) {
						sortAndUnique(prevCalc);
						setdif.clear();
						std::set_difference(prevCalc.begin(), prevCalc.end(), encountered.begin(), encountered.end(), std::back_inserter(setdif));
					
						for(auto to: setdif) {
						  encountered.push_back(to);
							auto tData = getTransitionData(stars[from], stars[to]);
							if(!isNegligible(tData)) {
          	    m_transitionTable[from].push_back(pair<int, tDataType>(to, tData));
							} 
						}
					}

					encountered.insert(encountered.end(), newEncountered.begin(), newEncountered.end());
				}

				sortAndUnique(m_transitionTable[from], [](auto a1, auto a2){return a1.first < a2.first;}, [](auto a1, auto a2){return a1.first == a2.first;});
	
    }
}

tDataType Transition::getTransitionData(const SimpleStar& from, const SimpleStar& to) const {
	// handling of center node
	tDataType tData;
  // basic checks: if transition is not possible, return tData with a multiplicity of zero
	if( from.getCenter().getColour() > 0 and
    from.getCenter().getColour() != to.getCenter().getColour())
    return tData;
  else if(from.getCenter().getColour() == 0) {
    if(from.getCenter().isSameType(m_coloured)) {
		  // nothing happens here
    } else if(to.getCenter().getColour() > 0) {
      return tData;
    }
	}
	if(from.getCenter().getColour() == to.getCenter().getColour()) {
		if(from.getCenter().numOfNeig()[1] > to.getCenter().numOfNeig()[1] or
			 from.getCenter().numOfNeig()[2] > to.getCenter().numOfNeig()[2])
			return tData;
	}

  // setting up transition probabilities for the center
  if(from.getCenter().getColour() == 0) {
    if(from.getCenter().isSameType(m_coloured)) {
      if(to.getCenter().getColour() == 0)
        tData.exp[1]++;
      else
        tData.exp[0]++;
    }
  }
	
  auto l1 = from.getLeafs();
  auto l2 = to.getLeafs();
	sort(l2.begin(), l2.end());

	
	if((from.getCenter().getColour()==0 and to.getCenter().getColour()>0 and
	from.getCenter().isSameType(m_coloured) and 
        ( from.getCenter().numOfNeig()[to.getCenter().getColour()] >= 
            from.getCenter().numOfNeig()[3-to.getCenter().getColour()] )) or from.getCenter().getColour()>0 or to.getCenter().getColour()==0 ){

	// check transitions for all possible permutations
	do {

		bool succ = true;
		for(uschar i=0; i<DEGREE; i++) {
   	  if(l1[i].getColour() != 0) {    // it is coloured
        if(l1[i].getColour() != l2[i].getColour()) {succ = false; break;}
	    } else {    // it was not coloured
  	    if(l2[i].getColour() != 0) {    // it get coloured
    	    if(not(l1[i].isSameType(m_coloured)))    // it couldnt get coloured
            {succ = false; break;}
          if( l1[i].numOfNeig()[l2[i].getColour()] <
              l1[i].numOfNeig()[3-l2[i].getColour()])
            {succ = false; break;}
 	      } else {
  	      char dr = (l2[i].numOfNeig()[1]-(to.getCenter().getColour()==1))
    	            -(l1[i].numOfNeig()[1]-(from.getCenter().getColour()==1));
          char db = (l2[i].numOfNeig()[2]-(to.getCenter().getColour()==2))
                  -(l1[i].numOfNeig()[2]-(from.getCenter().getColour()==2));
          if(dr<0 or db<0)
            {succ = false; break;}
 	      }
  	  }
   	}
		if(succ) { // transition is possible with this permutation
			if(tData.multipl == 0) {	// transition probabilities are not calculated yet
				tData.multipl = 1;
    		for(uschar i=0; i<DEGREE; i++) {
        	if(l1[i].getColour() == 0) {    // it was not coloured
            if(l2[i].getColour() != 0) {    // it get coloured
                tData.exp[0]++;
            } else {
                if(l1[i].isSameType(m_coloured)) // could get coloured but it didnt
                    tData.exp[1]++;
                char dr = (l2[i].numOfNeig()[1]-(to.getCenter().getColour()==1))
                        -(l1[i].numOfNeig()[1]-(from.getCenter().getColour()==1));
                char db = (l2[i].numOfNeig()[2]-(to.getCenter().getColour()==2))
                        -(l1[i].numOfNeig()[2]-(from.getCenter().getColour()==2));
                char tu = (l1[i].numOfNeig()[0]-(from.getCenter().getColour()==0));
                tData.multipl *= binom(tu, tu-dr-db)*binom(dr+db,db);	
                tData.exp[2] += dr+db;
                tData.exp[3] += tu-dr-db;
            }
        	}
    		}
			} else {
				usshort mult = 1;
    		for(uschar i=0; i<DEGREE; i++) {
        	if(l1[i].getColour() == 0 and l2[i].getColour() == 0) {    // it stayed uncoloured
            char dr = (l2[i].numOfNeig()[1]-(to.getCenter().getColour()==1))
                    -(l1[i].numOfNeig()[1]-(from.getCenter().getColour()==1));
            char db = (l2[i].numOfNeig()[2]-(to.getCenter().getColour()==2))
                    -(l1[i].numOfNeig()[2]-(from.getCenter().getColour()==2));
            char tu = (l1[i].numOfNeig()[0]-(from.getCenter().getColour()==0));
            mult *= binom(tu, tu-dr-db)*binom(dr+db,db);	
        	}
    		}
				tData.multipl += mult;
			}
		}

	} while (next_permutation(l2.begin(), l2.end()));
}

	auto to_reversed = SimpleStar::reverseSimpleStar(to);
	
	if(from.getCenter().getColour()==0 and to_reversed.getCenter().getColour()>0 and
	from.getCenter().isSameType(m_coloured) and 
        ( from.getCenter().numOfNeig()[to_reversed.getCenter().getColour()] >= 
            from.getCenter().numOfNeig()[3-to_reversed.getCenter().getColour()] ) ){
	
  l2 = to_reversed.getLeafs();
	sort(l2.begin(), l2.end());

	// check transitions for all possible permutations
	do {

		bool succ = true;
		for(uschar i=0; i<DEGREE; i++) {
   	  if(l1[i].getColour() != 0) {    // it is coloured
        if(l1[i].getColour() != l2[i].getColour()) {succ = false; break;}
	    } else {    // it was not coloured
  	    if(l2[i].getColour() != 0) {    // it get coloured
    	    if(not(l1[i].isSameType(m_coloured)))    // it couldnt get coloured
            {succ = false; break;}
          if( l1[i].numOfNeig()[l2[i].getColour()] <
              l1[i].numOfNeig()[3-l2[i].getColour()])
            {succ = false; break;}
 	      } else {
  	      char dr = (l2[i].numOfNeig()[1]-(to_reversed.getCenter().getColour()==1))
    	            -(l1[i].numOfNeig()[1]-(from.getCenter().getColour()==1));
          char db = (l2[i].numOfNeig()[2]-(to_reversed.getCenter().getColour()==2))
                  -(l1[i].numOfNeig()[2]-(from.getCenter().getColour()==2));
          if(dr<0 or db<0)
            {succ = false; break;}
 	      }
  	  }
   	}
		if(succ) { // transition is possible with this permutation
			if(tData.multipl == 0) {	// transition probabilities are not calculated yet
				tData.multipl = 1;
    		for(uschar i=0; i<DEGREE; i++) {
        	if(l1[i].getColour() == 0) {    // it was not coloured
            if(l2[i].getColour() != 0) {    // it get coloured
                tData.exp[0]++;
            } else {
                if(l1[i].isSameType(m_coloured)) // could get coloured but it didnt
                    tData.exp[1]++;
                char dr = (l2[i].numOfNeig()[1]-(to_reversed.getCenter().getColour()==1))
                        -(l1[i].numOfNeig()[1]-(from.getCenter().getColour()==1));
                char db = (l2[i].numOfNeig()[2]-(to_reversed.getCenter().getColour()==2))
                        -(l1[i].numOfNeig()[2]-(from.getCenter().getColour()==2));
                char tu = (l1[i].numOfNeig()[0]-(from.getCenter().getColour()==0));
                tData.multipl *= binom(tu, tu-dr-db)*binom(dr+db,db);	
                tData.exp[2] += dr+db;
                tData.exp[3] += tu-dr-db;
            }
        	}
    		}
			} else {
				usshort mult = 1;
    		for(uschar i=0; i<DEGREE; i++) {
        	if(l1[i].getColour() == 0 and l2[i].getColour() == 0) {    // it stayed uncoloured
            char dr = (l2[i].numOfNeig()[1]-(to_reversed.getCenter().getColour()==1))
                    -(l1[i].numOfNeig()[1]-(from.getCenter().getColour()==1));
            char db = (l2[i].numOfNeig()[2]-(to_reversed.getCenter().getColour()==2))
                    -(l1[i].numOfNeig()[2]-(from.getCenter().getColour()==2));
            char tu = (l1[i].numOfNeig()[0]-(from.getCenter().getColour()==0));
            mult *= binom(tu, tu-dr-db)*binom(dr+db,db);	
        	}
    		}
				tData.multipl += mult;
			}
		}

	} while (next_permutation(l2.begin(), l2.end()));

  }

	return tData;
}

bool Transition::isNegligible(const tDataType& tData) const {
    return tData.multipl == 0;
}

udouble Transition::calculateTransitionValue(const tDataType& tData, const udouble probKnown, const udouble probUnknown) const {
    udouble result = tData.multipl;
    for(uschar i=0; i<tData.exp[0]; i++)
        result *= probKnown;
    for(uschar i=0; i<tData.exp[1]; i++)
        result *= 1-(1+m_coloured.isSymmetric())*probKnown;
    for(uschar i=0; i<tData.exp[2]; i++)
        result *= probUnknown;
    for(uschar i=0; i<tData.exp[3]; i++)
        result *= 1-2*probUnknown;
    return result;
}

void Transition::doTheTransition(vector<Star>& stars, const udouble probKnown, const udouble probUnknown) const {
		for(usint from = 0; from < m_transitionTable.size(); from++)
        for(auto t: m_transitionTable[from]) {
            usint to = t.first;
            auto tData = t.second;
            udouble diff = stars[from].getMeas() * calculateTransitionValue(tData, probKnown, probUnknown);
            stars[from].addDiff(-diff);
            stars[to].addDiff(diff*stars[from].getMultiplicity()/stars[to].getMultiplicity());
        }
    for(auto it = stars.begin(); it != stars.end(); ++it)
        it->updateMeas();
}
