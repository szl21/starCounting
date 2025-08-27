#include "point.h"

using std::vector;

bool Point::operator<(const Point& p) {
	if(m_red  != p.m_red)  return m_red < p.m_red;
	if(m_blue != p.m_blue) return m_blue < p.m_blue;
	return m_center < p.m_center;
}

Point Point::calculateLeafPoint(const Point& center, const uschar leafOutType) {
    if(leafOutType < 2)
        return Point(leafOutType+1,-1,-1);
    uschar red = center.getColour()==1;
    uschar blue = center.getColour()==2;
    uschar m = DEGREE-1;
    uschar lt = leafOutType-2;
    while(lt>m) {
        lt -= (m+1);
        m--;
        red++;
    }
    blue += lt;
    return Point(0, red, blue);
}

uschar Point::calculateLeafOutType(const Point& center, const Point& leafPoint) {
    if(leafPoint.getColour() > 0)
        return leafPoint.getColour() - 1;
    uschar red  = leafPoint.numOfNeig()[1] - (center.getColour() == 1);
    uschar blue = leafPoint.numOfNeig()[2] - (center.getColour() == 2);
    return 2 + red * DEGREE - (red * (red - 1)) / 2 + blue;
}

uschar Point::reverseLeafOutType(const uschar lot) {
	if(lot < 2)
		return 1-lot;
	uschar red = 0;
	uschar m = DEGREE-1;
	uschar lt = lot-2;
	while(lt>m) {
		lt -= (m+1);
		m--;
		red++;
	}
	uschar blue = lt;
	return 2 + blue * DEGREE - (blue * (blue -1)) / 2 + red;	
}
