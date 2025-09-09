#include "star.h"

#include <numeric>
#include <iostream>
#include <map>
#include "cust_utils.h"

using std::pair;
using std::map;
using std::cout;

static usshort computeMultiplicity(const uschar center, const vector<uschar>& leafOutTypes) {
    map<uschar, uschar> c;
    for(auto i: leafOutTypes)
        c[i]++;
    usshort res = factorial(leafOutTypes.size());
    for(auto p: c)
        res /= factorial(p.second);
    if(center>0) res *= 2;
		return res;
}

static usint computeStarIndex(const uschar center, const vector<uschar>& leafOutTypes) {
		auto T = leafOutTypes;
		sort(T.begin(), T.end());
		usint idx = center * binom(DEGREE+MAX_LEAF-1,DEGREE);
		idx += binom(DEGREE+T[DEGREE-1], DEGREE)-1;
		for(uschar i=1; i<DEGREE; i++) 
        idx -= binom(DEGREE-i-1+T[DEGREE-1]-T[i-1], DEGREE-i);
		return idx;
}

SimpleStar::SimpleStar(const uschar center, const vector<uschar>& leafOutTypes) :
    m_center{center, (uschar) countOccurrences(leafOutTypes, uschar(0)), (uschar) countOccurrences(leafOutTypes, uschar(1))},
		m_leafOutTypes{leafOutTypes},
    m_multiplicity{computeMultiplicity(center, leafOutTypes)},
		m_starIndex{computeStarIndex(center, leafOutTypes)}
{
    for(uschar i=0; i<DEGREE; i++) {
        m_leafs.push_back(Point::calculateLeafPoint(m_center, m_leafOutTypes[i]));
    }
}


SimpleStar::SimpleStar() : SimpleStar{0, vector<uschar>()} {cout << "Something went wrong with SimpleStars...: tried to create simple star without parameters";}		// this should not be used anywhere but the compiler drops an error if there is no default constructor...

vector<usint> SimpleStar::firstNeighsInt(const SimpleStar& s) {
    vector<usint> res;
    auto c = s.getCenter().getColour();
    auto leafOutTypes = s.getLeafOutTypes();
		vector<uschar> leafOutTypesReversed;
		for(auto a: leafOutTypes)
			leafOutTypesReversed.push_back(Point::reverseLeafOutType(a));
    if(c == 0) {
        res.push_back(computeStarIndex(1,leafOutTypes));
 				res.push_back(computeStarIndex(1,leafOutTypesReversed));
    }
    for(uschar i=0; i<DEGREE; i++) {
        if(leafOutTypes[i] > 1) {
            auto l = leafOutTypes;
            l[i] = 0;
            res.push_back(computeStarIndex(c,l));
            l[i] = 1;
            res.push_back(computeStarIndex(c,l));
						// check if the i-th neighbour has an uncoloured neighbour apart from the center
            if(s.getLeafs()[i].numOfUncol()>(s.getCenter().getColour() == 0)) {
                l[i] = Point::calculateLeafOutType(s.getCenter(), s.getLeafs()[i])
                    + DEGREE - (s.getLeafs()[i].numOfNeig()[1]-(s.getCenter().getColour()==1));
                res.push_back(computeStarIndex(c,l));
                l[i] = Point::calculateLeafOutType(s.getCenter(), s.getLeafs()[i])
                    + 1;
                res.push_back(computeStarIndex(c,l));
            }
        }
    }
    sort(res.begin(), res.end());
    auto it = unique(res.begin(), res.end());
    res.resize(distance(res.begin(), it));
    return res;
}

void Star::updateMeas() {
    m_diffs.push_back(m_meas);
    sort(m_diffs.begin(), m_diffs.end(), 
        [](udouble a, udouble b){return abs(a)<abs(b);});
    m_meas = accumulate(m_diffs.begin(), m_diffs.end(), udouble(0));
    m_diffs.clear();
}

SimpleStar SimpleStar::reverseSimpleStar(const SimpleStar& orig) {
	auto o_center = orig.getCenter().getColour();
	auto o_leafOutTypes = orig.getLeafOutTypes();
	uschar res_center = (o_center == 0) ? 0 : 3-o_center;
	vector<uschar> res_leafOutTypes;
	for(auto lot: o_leafOutTypes)
		res_leafOutTypes.push_back(Point::reverseLeafOutType(lot));
	sort(res_leafOutTypes.begin(), res_leafOutTypes.end());
	return SimpleStar(res_center, res_leafOutTypes);
}
