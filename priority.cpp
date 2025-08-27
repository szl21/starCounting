#include "priority.h"

#include <numeric>

using std::endl;

bool Priority::operator<(const Priority& p) const {
    uschar r1 = m_red;
    uschar b1 = m_blue;
    uschar r2 = p.m_red;
    uschar b2 = p.m_blue;
    if(r1+b1 == DEGREE)
        return false;
    if(r2+b2 == DEGREE)
        return true;
    if(abs(r1-b1) < abs(r2-b2))
        return true;
    if(abs(r1-b1) > abs(r2-b2))
        return false;
    if(min(r1, b1) < min(r2, b2))
        return true;
    return false;
}

udouble Priority::calculateMeas(const vector<Star>& stars) {
    vector<udouble> m;
    for(auto s: stars)
        if(s.getCenter().getColour() == 0 and s.getCenter().isSameType(*this))
            m.push_back(s.getMeas()*s.getMultiplicity());
    sort(m.begin(), m.end());
    m_meas = accumulate(m.begin(), m.end(), udouble(0));
		printType(); cout << " " << m_meas << endl;
		return m_meas;
}
