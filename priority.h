#ifndef DF7E1027_E601_4CE3_A59B_C0CEE53E3E97
#define DF7E1027_E601_4CE3_A59B_C0CEE53E3E97

#include "star.h"

#include <iostream>

using std::cout;

class Priority : public PointType{
    private:
        udouble m_meas;
    public: 
        Priority(const PointType& center): PointType{center}, m_meas{0} {}
        udouble getMeas() const {return m_meas; }
        bool isColourable() const {return m_meas >= IS_COLOURABLE_MULTIPLIER*EPS; }
        void printType() const {cout << "(" << (int) m_red <<", " << (int) m_blue <<")"; }

				bool operator<(const Priority&) const;
        udouble calculateMeas(const vector<Star>&);
};

#endif /* DF7E1027_E601_4CE3_A59B_C0CEE53E3E97 */
