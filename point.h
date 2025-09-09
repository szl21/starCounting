#ifndef BAFB8DEE_B271_430A_85B8_97EDC98C3590
#define BAFB8DEE_B271_430A_85B8_97EDC98C3590

#include "config.h"

#include <vector>

using std::min;
using std::max;
using std::vector;

class PointType {
    protected:
        uschar m_red;	// number of red neighbours
        uschar m_blue;	// number of blue neighbours
    public:
        PointType(const uschar red, const uschar blue): m_red{red}, m_blue{blue} {}
				bool isSameType(const PointType& p) const {
					return	min(m_red, m_blue) == min(p.m_red, p.m_blue) and
									max(m_red, m_blue) == max(p.m_red, p.m_blue); }	// {m_red, m_blue} are the same as an unordered list
        bool isSymmetric() const {return m_red == m_blue; }	// m_red is the same as m_blue
        uschar numOfUncol() const {return DEGREE - m_red - m_blue; }	// number of uncoloured neighbours
};

class Point : public PointType {
    private:
        uschar m_center;	// colour of center: 0: uncol, 1: red, 2: blue
    public:
        Point(const uschar center, const uschar red, const uschar blue): PointType{red, blue}, m_center{center} {}
        uschar getColour() const {return m_center; }  // 0: uncol, 1: red, 2: blue
        vector<uschar> numOfNeig() const {return vector<uschar>{numOfUncol(), m_red, m_blue}; }  // (#uncol, #red, #blue)
				
				bool operator<(const Point&);
        
				static Point calculateLeafPoint(const Point&, const uschar);			// 1st arg is the center, 2nd arg is the leafOutType
        static uschar calculateLeafOutType(const Point&, const Point&);	// 1st arg is the center, 2nd arg is the leaf point
				static uschar reverseLeafOutType(const uschar);
};

#endif /* BAFB8DEE_B271_430A_85B8_97EDC98C3590 */
